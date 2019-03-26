#include "PlyWriter.h"

namespace EasyAsPLY
{
	PlyWriter::PlyWriter()
	{
		data = nullptr;
		format = nullptr;
	}

	PlyWriter::~PlyWriter()
	{

	}

	void PlyWriter::Open(PlyFormat* format, PlyData* data)
	{
		this->format = format;
		this->data = data;
	}

	void PlyWriter::Close()
	{
		if (format != nullptr)
		{
			delete format;
			format = nullptr;
		}

		if (data != nullptr)
		{
			delete data;
			data = nullptr;
		}
	}

	void PlyWriter::Write(std::string filepath, std::initializer_list<std::string> comments)
	{
		std::ofstream* outStream = new std::ofstream(filepath);
		WriteHeader(outStream, comments);
		WriteData(outStream);
		delete outStream;
	}

	void PlyWriter::Write(std::ostream* outStream, std::initializer_list<std::string> comments)
	{
		WriteHeader(outStream, comments);
		WriteData(outStream);
	}

	void PlyWriter::WriteHeader(std::ostream* out, std::initializer_list<std::string> comments)
	{
		*out << "ply\n";
		*out << "format ascii 1.0\n";
		*out << "comment Generated by EasyAsPLY\n";
		for (auto& comment : comments)
		{
			*out << "comment " << comment << "\n";
		}

		for (auto& elemFormat : *format)
		{
			*out << "element " << elemFormat->GetName() << " " << elemFormat->GetElementCount() << "\n";
			for (auto& propFormat : *elemFormat)
			{
				if (propFormat->GetType() == PlyPropertyType::LIST)
				{
					*out << "property list " << PlyProperty::PropertyTypeToString(propFormat->GetListLengthType()) << " ";
					*out << PlyProperty::PropertyTypeToString(propFormat->GetListType()) << " " << propFormat->GetName() << "\n";
				}
				else
				{
					*out << "property " << PlyProperty::PropertyTypeToString(propFormat->GetType()) << " " << propFormat->GetName() << "\n";
				}
			}
		}

		*out << "end_header\n";
	}

	void PlyWriter::WriteData(std::ostream* out)
	{
		for (auto& elemFormat : *format)
		{
			int elemIndex = format->GetElementIndex(elemFormat->GetName());
			std::vector<PlyElement*>* elemList = data->GetElements(elemIndex);

			for (PlyElement* elem : *elemList)
			{
				int i = 0;
				for (auto& propFormat : *elemFormat)
				{
					PlyProperty* prop = elem->GetProperty(i);
					++i;
					if (propFormat->GetType() == PlyPropertyType::LIST)
					{
						*out << prop->GetValue<std::vector<PlyProperty*>*>()->size() << " ";
					}

					*out << prop->GetValue<std::string>();
					if (i == elemFormat->GetPropertyCount())
					{
						*out << "\n";
					}
					else
					{
						*out << " ";
					}
				}
			}
		}
	}
}