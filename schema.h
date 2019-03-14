#pragma once
#include "object.h"

class Schema {
	virtual Object apply(Object object);
};