#pragma once

#include "helper.h"

class Triangle {
private:
	double a, b, c, d;
public:
	Point3D points[3];
	Color color;
	Color backColor;
	Color actualColor;


	bool valid;
	double distance;

	Triangle() {
		valid = true;
	}

	/*Triangle(const Triangle& t) {
		for (int i = 0; i < 3; i++) {
			points[0] = t.points[0];
		}
		color = t.color;
		backColor = t.backColor;
		actualColor = t.actualColor;
		distance = t.distance;
	}*/

	Triangle(Point3D A, Point3D B, Point3D C, Color color, Color backColor) {
		points[0] = A;
		points[1] = B;
		points[2] = C;
		this->color = color;
		this->backColor = backColor;

		// Irányvektor kiszámítása
		recalculatePlane(A, B, C);

		valid = true;
	}

	void recalculatePlane(Point3D A, Point3D B, Point3D C) {
		Point3D v;
		v.x = B.x - A.x;
		v.y = B.y - A.y;
		v.z = B.z - A.z;

		a = v.y * -1.0;
		b = v.x;
		c = v.z;

		d = -(a * A.x + b * A.y + c * A.z);
	}

	int whichSide(Point3D point) {
		Vector A(points[0]);
		Vector B(points[1]);
		Vector C(points[2]);

		Vector BA;
		BA.x = B.x - A.x;
		BA.y = B.y - A.y;
		BA.z = B.z - A.z;

		Vector CB;
		CB.x = C.x - B.x;
		CB.y = C.y - B.y;
		CB.z = C.z - B.z;

		Vector AC;
		AC.x = A.x - C.x;
		AC.y = A.y - C.y;
		AC.z = A.z - C.z;

		Vector nVector = AC.crossProduct(BA);

		Vector PX;
		PX.x = point.x - A.x;
		PX.y = point.y - A.y;
		PX.z = point.z - A.z;

		Vector nx;
		nx = BA.crossProduct(PX);
		if (nx * nVector < 0)
			return -1;

		PX.x = point.x - B.x;
		PX.y = point.y - B.y;
		PX.z = point.z - B.z;

		nx = CB.crossProduct(PX);
		if (nx * nVector < 0)
			return -1;


		PX.x = point.x - C.x;
		PX.y = point.y - C.y;
		PX.z = point.z - C.z;

		nx = AC.crossProduct(PX);
		if (nx * nVector < 0)
			return -1;

		return 1;

	}

	void operator=(const Triangle& t) {
		for (int i = 0; i < 3; i++) {
			points[i] = t.points[i];
			color = t.color;
			backColor = t.backColor;
			actualColor = t.actualColor;
			valid = t.valid;
			distance = t.distance;
		}
	}
};