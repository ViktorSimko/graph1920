#ifndef EASY_AS_PLY_FORMAT_H
#define EASY_AS_PLY_FORMAT_H

#include <vector>
#include <map>
#include <iterator>
#include "PlyProperty.h"

namespace EasyAsPLY
{
	class PlyFormatProperty
	{
	public:
		PlyFormatProperty()
			: type(PlyPropertyType::NONE), name(""), listType(PlyPropertyType::NONE), listLengthType(PlyPropertyType::NONE) {};

		PlyFormatProperty(std::string name, PlyPropertyType listType, PlyPropertyType listLengthType) : name(name)
		{
			this->type = PlyPropertyType::LIST;
			this->listType = listType;
			this->listLengthType = listLengthType;
		}

		PlyFormatProperty(std::string name, PlyPropertyType type)
			: name(name), type(type), listType(PlyPropertyType::NONE), listLengthType(PlyPropertyType::NONE) {}
		~PlyFormatProperty() {}

		PlyPropertyType GetType() { return type; }
		std::string GetName() { return name; }

		PlyPropertyType GetListType() { return listType; }
		PlyPropertyType GetListLengthType() { return listLengthType; }

	private:
		PlyPropertyType type;
		std::string name;

		PlyPropertyType listType;
		PlyPropertyType listLengthType;
	};

	class PlyFormatElement
	{
	public:
		PlyFormatElement() {}
		PlyFormatElement(std::string name, int count) : name(name), count(count) {}
		~PlyFormatElement() 
		{
			for (auto& property : properties)
			{
				delete property;
			}

			properties.clear();
		}

		void AddProperty(PlyFormatProperty* property) { properties.push_back(property); }
		std::string GetName() { return name; }
		int GetElementCount() { return count; }
		int GetPropertyCount() { return properties.size(); }

		int GetPropertyIndex(std::string name)
		{
			for (int i = 0; i < properties.size(); ++i)
			{
				if (properties.at(i)->GetName().compare(name) == 0)
				{
					return i;
				}
			}

			return -1;
		}

		PlyFormatProperty* GetProperty(std::string name)
		{
			for (auto& property : properties)
			{
				if (property->GetName().compare(name) == 0)
				{
					return property;
				}
			}

			return nullptr;
		}

		typedef std::vector<PlyFormatProperty*>::iterator iterator;
		typedef std::vector<PlyFormatProperty*>::const_iterator const_iterator;

		iterator begin() { return properties.begin(); }
		const_iterator begin() const { return properties.begin(); }
		iterator end() { return properties.end(); }
		const_iterator end() const { return properties.end(); }

	private:
		std::string name;
		int count;
		std::vector<PlyFormatProperty*> properties;
	};

	class PlyFormat
	{
	public:
		PlyFormat() {}
		~PlyFormat()
		{
			for (auto& element : elements)
			{
				delete element;
			}

			elements.clear();
		}

		void AddElement(PlyFormatElement* element) { elements.push_back(element); }
		int getElementTypeCount() { return elements.size(); }

		int GetElementIndex(std::string name)
		{
			for (int i = 0; i < elements.size(); ++i)
			{
				if (elements.at(i)->GetName().compare(name) == 0)
				{
					return i;
				}
			}

			return -1;
		}

		PlyFormatElement* GetElement(std::string index)
		{
			for (auto& element : elements)
			{
				if (element->GetName().compare(index) == 0)
				{
					return element;
				}
			}

			return nullptr;
		}


		typedef std::vector<PlyFormatElement*>::iterator iterator;
		typedef std::vector<PlyFormatElement*>::const_iterator const_iterator;

		iterator begin() { return elements.begin(); }
		const_iterator begin() const { return elements.begin(); }
		iterator end() { return elements.end(); }
		const_iterator end() const { return elements.end(); }

	private:
		std::vector<PlyFormatElement*> elements;
	};	
}
#endif