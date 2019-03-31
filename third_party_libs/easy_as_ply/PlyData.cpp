#include "PlyData.h"

namespace EasyAsPLY
{
	PlyData::PlyData()
	{
	}

	PlyData::~PlyData()
	{
		for (auto& list : elements)
		{
			delete list;
		}
		elements.clear();
	}
}