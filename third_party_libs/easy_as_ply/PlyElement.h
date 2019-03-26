#ifndef EASY_AS_PLY_ELEMENT_H
#define EASY_AS_PLY_ELEMENT_H

#include <string>
#include <vector>
#include "PlyProperty.h"

namespace EasyAsPLY
{
	class PlyElement
	{
	public:
		PlyElement();
		~PlyElement();

		PlyProperty* GetProperty(int index)
		{
			return data[index];
		}

		int GetPropertyCount() { return data.size(); }

		void AddProperty(PlyProperty* property) { data.push_back(property); };

	private:
		std::vector<PlyProperty*> data;
	};
}

#endif