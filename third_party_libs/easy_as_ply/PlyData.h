#ifndef EASY_AS_PLY_DATA_H
#define EASY_AS_PLY_DATA_H

#include <map>
#include <vector>
#include "PlyElement.h"

namespace EasyAsPLY
{
	class PlyData
	{
	public:
		PlyData();
		~PlyData();

		std::vector<PlyElement*>* GetElements(int index)
		{
			if (index >= elements.size())
			{
				elements.push_back(new std::vector<PlyElement*>());
			}
			return elements.at(index);
		}

		typedef std::vector<std::vector<PlyElement*>*>::iterator iterator;
		typedef std::vector<std::vector<PlyElement*>*>::const_iterator const_iterator;

		iterator begin() { return elements.begin(); }
		const_iterator begin() const { return elements.begin(); }
		iterator end() { return elements.end(); }
		const_iterator end() const { return elements.end(); }

	private:
		std::vector<std::vector<PlyElement*>*> elements;
	};
}
#endif