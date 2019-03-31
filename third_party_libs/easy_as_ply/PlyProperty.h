#ifndef EASY_AS_PLY_PROPERTY_H
#define EASY_AS_PLY_PROPERTY_H

#include <vector>
#include <string>

namespace EasyAsPLY
{
	enum PlyPropertyType
	{
		NONE = -1,
		CHAR,
		UCHAR,
		SHORT,
		USHORT,
		INT,
		UINT,
		FLOAT,
		DOUBLE,
		LIST,
		COUNT
	};

	class PlyProperty
	{
	public:
		PlyProperty();
		~PlyProperty();

		static PlyProperty* CreateProperty(PlyPropertyType type, std::string value);
		static std::string PropertyTypeToString(PlyPropertyType type);
		static PlyPropertyType StringToPropertyType(std::string type);

		template <typename T>
		T GetValue();

		template <> char GetValue()							{ return value.primitive.Char; }
		template <> unsigned char GetValue()				{ return value.primitive.Uchar; }
		template <> short GetValue()						{ return value.primitive.Short; }
		template <> unsigned short GetValue()				{ return value.primitive.Ushort; }
		template <> int GetValue()							{ return value.primitive.Int; }
		template <> unsigned int GetValue()					{ return value.primitive.Uint; }
		template <> float GetValue()						{ return value.primitive.Float; }
		template <> double GetValue()						{ return value.primitive.Double; }
		template <> std::vector<PlyProperty*>* GetValue()	{ return value.List; }
		template <> std::string GetValue()					{ return ValueAsString(); }
		

		void SetValue(char val)							{ value.primitive.Char = val; value.type = PlyPropertyType::CHAR; }
		void SetValue(unsigned char val)				{ value.primitive.Uchar = val; value.type = PlyPropertyType::UCHAR; }
		void SetValue(short val)						{ value.primitive.Short = val; value.type = PlyPropertyType::SHORT; }
		void SetValue(unsigned short val)				{ value.primitive.Ushort = val; value.type = PlyPropertyType::USHORT; }
		void SetValue(int val)							{ value.primitive.Int = val; value.type = PlyPropertyType::INT; }
		void SetValue(unsigned int val)					{ value.primitive.Uint = val; value.type = PlyPropertyType::UINT; }
		void SetValue(float val)						{ value.primitive.Float = val; value.type = PlyPropertyType::FLOAT; }
		void SetValue(double val)						{ value.primitive.Double = val; value.type = PlyPropertyType::DOUBLE; }
		void SetValue(std::vector<PlyProperty*>* val)	{ value.List = val; value.type = PlyPropertyType::LIST; }

	private:

		std::string ValueAsString();

		struct value
		{
			PlyPropertyType type;

			union primitive
			{
				char			Char;
				unsigned char	Uchar;
				short			Short;
				unsigned short	Ushort;
				int				Int;
				unsigned int	Uint;
				float			Float;
				double			Double;
			}primitive;

			std::vector<PlyProperty*>* List;
		}value;
		
	};
}

#endif