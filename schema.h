#pragma once
#include "mesh.h"

class Schema {
	virtual Mesh apply(Mesh mesh);
};