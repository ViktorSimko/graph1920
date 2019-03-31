#include "PlyReader.h"
#include <sstream>
#include <fstream>

namespace EasyAsPLY
{
	PlyReader::PlyReader()
	{
		data = nullptr;
		format = nullptr;
	}

	PlyReader::~PlyReader()
	{
		Close();
	}

	bool PlyReader::Read(std::string filepath)
	{
		std::ifstream* inStream = new std::ifstream(filepath);
		if (!Load(inStream)) { delete inStream; return false; }
		delete inStream;

		if (!ParseHeader()) { return false; };
		if (!ParseElements()) { return false; };

		return true;
	}

	bool PlyReader::Read(std::istream* in)
	{
		if (!Load(in)) { return false; }
		if (!ParseHeader()) { return false; };
		if (!ParseElements()) { return false; };

		return true;
	}

	void PlyReader::Open()
	{
		if (data == nullptr) { data = new PlyData(); }
		if (format == nullptr) { format = new PlyFormat(); }
	}

	void PlyReader::Close()
	{
		if (data != nullptr) { delete data; data = nullptr;  }
		if (format != nullptr) { delete format; format = nullptr; }
	}

	bool PlyReader::Load(std::istream* in)
	{
		std::string str((std::istreambuf_iterator<char>(*in)),
			std::istreambuf_iterator<char>());
		lines = tokenise(str, "\n");
		lineTokens = tokenise(lines.at(linePos));
		return true;
	}

	bool PlyReader::ParseHeader()
	{
		if (gobble().compare("ply") != 0) { return false; }
		skipComments();
		if (gobble().compare("format") != 0)
		{
			return false;
		}
		else
		{
			std::string format = gobble();
			if (format.compare("ascii") != 0) { return false; } // doesn't support binary

			// version
			gobble();
		}
		skipComments();

		for (std::string token = peek(); token.compare("end_header") != 0; token = peek())
		{
			skipComments();
			if (!ParseElementFormat()) { return false; }
			skipComments();
		}

		// skip the end_header tag
		gobble();
		nextLine();
		return true;
	}

	bool PlyReader::ParseElementFormat()
	{
		PlyFormatElement* element;
		std::string temp = gobble();
		if (temp.compare("element") != 0)
		{
			return false;
		}
		else
		{
			std::string name = gobble();
			int elementCount = std::atoi(gobble().c_str());
			element = new PlyFormatElement(name, elementCount);
		}

		skipComments();
		// add properties to element
		for (std::string nextTag = peek(); nextTag.compare("element") != 0 && nextTag.compare("end_header") != 0; nextTag = peek())
		{
			gobble();
			if (nextTag.compare("property") == 0)
			{
				PlyPropertyType type = PlyProperty::StringToPropertyType(gobble());
				if (type == PlyPropertyType::NONE)
				{
					return false;
				}
				else if (type == PlyPropertyType::LIST)
				{
					PlyPropertyType listLengthType = PlyProperty::StringToPropertyType(gobble());
					PlyPropertyType listType = PlyProperty::StringToPropertyType(gobble());
					std::string name = gobble();
					PlyFormatProperty* property = new PlyFormatProperty(name, listType, listLengthType);
					element->AddProperty(property);
				}
				else
				{
					std::string name = gobble();
					PlyFormatProperty* property = new PlyFormatProperty(name, type);
					element->AddProperty(property);
				}
			}
			else if (nextTag.compare("comment") == 0)
			{
				nextLine();
			}
			else
			{
				return false;
			}
		}

		format->AddElement(element);
		return true;
	}

	bool PlyReader::ParseElements()
	{
		for (auto& element : *format)
		{
			for (int i = 0; i < element->GetElementCount(); ++i)
			{
				skipComments();
				if (!ParseElement(element)) { return false; }
				skipComments();
			}
		}

		return true;
	}

	bool PlyReader::ParseElement(PlyFormatElement* elemFormat)
	{
		PlyElement* element = new PlyElement();
		for (auto& propertyFormat : *elemFormat)
		{
			if (propertyFormat->GetType() == PlyPropertyType::LIST)
			{
				int index = elemFormat->GetPropertyIndex(propertyFormat->GetName());

				int size = std::stoi(gobble());
				for (int i = 0; i < size; ++i)
				{
					PlyProperty* property = PlyProperty::CreateProperty(propertyFormat->GetListType(), gobble());
					if (index >= element->GetPropertyCount())
					{
						element->AddProperty(PlyProperty::CreateProperty(PlyPropertyType::LIST, ""));
					}
					element->GetProperty(index)->GetValue<std::vector<PlyProperty*>*>()->push_back(property);
				}
			}
			else
			{
				PlyProperty* property = PlyProperty::CreateProperty(propertyFormat->GetType(), gobble());
				element->AddProperty(property);
			}
		}
		int index = format->GetElementIndex(elemFormat->GetName());
		data->GetElements(index)->push_back(element);
		return true;
	}

	std::string PlyReader::gobble()
	{
		if (tokenPos >= lineTokens.size())
		{
			if (linePos + 1 < lines.size())
			{
				lineTokens = tokenise(lines.at(++linePos));
				tokenPos = 0;
			}
			else
			{
				return "";
			}
		}

		std::string token = lineTokens.at(tokenPos);
		++tokenPos;
		return token;
	}

	std::string PlyReader::peek()
	{
		if (tokenPos >= lineTokens.size())
		{
			if (linePos + 1 < lines.size())
			{
				std::string token = tokenise(lines.at(linePos + 1)).at(0);
				return token;
			}
			else
			{
				return "";
			}
		}
		return lineTokens.at(tokenPos);
	}

	void PlyReader::nextLine()
	{
		lineTokens = tokenise(lines.at(++linePos));
		tokenPos = 0;
	}

	std::vector<std::string> PlyReader::tokenise(const std::string& str, const std::string& delimiter)
	{
		std::vector<std::string> tokens;

		std::string::size_type lastPos = str.find_first_not_of(delimiter, 0);
		std::string::size_type pos = str.find_first_of(delimiter, lastPos);

		while (std::string::npos != pos || std::string::npos != lastPos)
		{
			tokens.push_back(str.substr(lastPos, pos - lastPos));
			lastPos = str.find_first_not_of(delimiter, pos);
			pos = str.find_first_of(delimiter, lastPos);
		}

		return tokens;
	}


	// TODO fix this
	void PlyReader::skipComments()
	{
		for (std::string token = peek(); token.compare("comment") == 0; token = peek())
		{
			nextLine();
		}
	}
}