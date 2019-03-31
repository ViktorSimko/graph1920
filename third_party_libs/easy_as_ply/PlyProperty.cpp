#include "PlyProperty.h"

namespace EasyAsPLY
{
	PlyProperty::PlyProperty()
	{

	}

	std::string PlyProperty::PropertyTypeToString(PlyPropertyType type)
	{
		switch (type)
		{
		case CHAR:
			return "char";
		case UCHAR:
			return "uchar";
		case SHORT:
			return "short";
		case USHORT:
			return "ushort";
		case INT:
			return "int";
		case UINT:
			return "uint";
		case FLOAT:
			return "float";
		case DOUBLE:
			return "double";
		case LIST:
			return "list";
		}

		return "";
	}

	PlyPropertyType PlyProperty::StringToPropertyType(std::string typeString)
	{
		if (typeString.compare("char") == 0)
		{
			return PlyPropertyType::CHAR;
		}
		else if (typeString.compare("uchar") == 0)
		{
			return PlyPropertyType::UCHAR;
		}
		else if (typeString.compare("short") == 0)
		{
			return PlyPropertyType::SHORT;
		}
		else if (typeString.compare("ushort") == 0)
		{
			return PlyPropertyType::USHORT;
		}
		else if (typeString.compare("int") == 0)
		{
			return PlyPropertyType::INT;
		}
		else if (typeString.compare("uint") == 0)
		{
			return PlyPropertyType::UINT;
		}
		else if (typeString.compare("float") == 0)
		{
			return PlyPropertyType::FLOAT;
		}
		else if (typeString.compare("double") == 0)
		{
			return PlyPropertyType::DOUBLE;
		}
		else if (typeString.compare("list") == 0)
		{
			return PlyPropertyType::LIST;
		}
		else
		{
			return PlyPropertyType::NONE;
		}
	}

	PlyProperty* PlyProperty::CreateProperty(PlyPropertyType type, std::string value)
	{
		PlyProperty* property = new PlyProperty();

		if (type == PlyPropertyType::CHAR)
		{
			int val = std::stoi(value);

			if (val >= std::numeric_limits<char>::min() && val <= std::numeric_limits<char>::max())
			{
				property->SetValue((char)val);
			}
			else
			{
				return nullptr;
			}
		}
		else if (type == PlyPropertyType::UCHAR)
		{
			int val = std::stoi(value);

			if (val >= std::numeric_limits<unsigned char>::min() && val <= std::numeric_limits<unsigned char>::max())
			{
				property->SetValue((unsigned char)val);
			}
			else
			{
				return nullptr;
			}
		}
		else if (type == PlyPropertyType::SHORT)
		{
			int val = std::stoi(value);

			if (val >= std::numeric_limits<short>::min() && val <= std::numeric_limits<short>::max())
			{
				property->SetValue((short)val);
			}
			else
			{
				return nullptr;
			}
		}
		else if (type == PlyPropertyType::USHORT)
		{
			int val = std::stoi(value);

			if (val >= std::numeric_limits<unsigned short>::min() && val <= std::numeric_limits<unsigned short>::max())
			{
				property->SetValue((unsigned short)val);
			}
			else
			{
				return nullptr;
			}
		}
		else if (type == PlyPropertyType::INT)
		{
			long long val = std::stoll(value);

			if (val >= std::numeric_limits<int>::min() && val <= std::numeric_limits<int>::max())
			{
				property->SetValue((int)val);
			}
			else
			{
				return nullptr;
			}
		}
		else if (type == PlyPropertyType::UINT)
		{
			long long val = std::stoll(value);

			if (val >= std::numeric_limits<unsigned int>::min() && val <= std::numeric_limits<unsigned int>::max())
			{
				property->SetValue((unsigned int)val);
			}
			else
			{
				return nullptr;
			}
		}
		else if (type == PlyPropertyType::FLOAT)
		{
			property->SetValue(std::stof(value));
		}
		else if (type == PlyPropertyType::DOUBLE)
		{
			property->SetValue(std::stod(value));
		}
		else if (type == PlyPropertyType::LIST)
		{
			property->SetValue(new std::vector<PlyProperty*>());
		}
		return property;
	}

	std::string PlyProperty::ValueAsString()
	{
		if (value.type == PlyPropertyType::CHAR)
		{
			return std::to_string(value.primitive.Char);
		}
		else if (value.type == PlyPropertyType::UCHAR)
		{
			return std::to_string(value.primitive.Uchar);
		}
		else if (value.type == PlyPropertyType::SHORT)
		{
			return std::to_string(value.primitive.Short);
		}
		else if (value.type == PlyPropertyType::USHORT)
		{
			return std::to_string(value.primitive.Ushort);
		}
		else if (value.type == PlyPropertyType::INT)
		{
			return std::to_string(value.primitive.Int);
		}
		else if (value.type == PlyPropertyType::UINT)
		{
			return std::to_string(value.primitive.Uint);
		}
		else if (value.type == PlyPropertyType::FLOAT)
		{
			return std::to_string(value.primitive.Float);
		}
		else if (value.type == PlyPropertyType::DOUBLE)
		{
			return std::to_string(value.primitive.Double);
		}
		else if (value.type == PlyPropertyType::LIST)
		{
			std::string result;
			for (int i = 0; i < value.List->size(); ++i)
			{
				PlyProperty* prop = value.List->at(i);
				result += prop->GetValue<std::string>();
				if (i != value.List->size() - 1)
				{
					result += " ";
				}
			}
			return result;
		}

		return "";
	}
}