#include "Point3D.h"
#include <cmath>

Point3D::Point3D(void) {
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

Point3D::~Point3D(void)
{
}

Point3D::Point3D(const double _x, const double _y, const double _z) {
	x = _x;
	y = _y;
	z = _z;
}
Point3D::Point3D(const Point3D &p) {
	x = p.x;
	y = p.y;
	z = p.z;
}
bool Point3D::operator==(const Point3D &p) const {
	// return x==p.x && y==p.y && z==p.z
	double dx = fabs(x - p.x);
	double dy = fabs(y - p.y);
	double dz = fabs(z - p.z);
	bool ret = false;
	if (dx < cMinDeltaToBeEqal && dy < cMinDeltaToBeEqal && dz < cMinDeltaToBeEqal) {
		ret = true;
	}
	return ret;
}
bool Point3D::operator!=(const Point3D &p) const {
	return ! operator==(p);
}

Point3D Point3D::operator+(const Point3D p) const {
	Point3D retPnt;
	retPnt.x = x + p.x;
	retPnt.y = y + p.y;
	retPnt.z = z + p.z;
	return retPnt;
}
Point3D Point3D::operator-(const Point3D p) const {
	Point3D retPnt;
	retPnt.x = x - p.x;
	retPnt.y = y - p.y;
	retPnt.z = z - p.z;
	return retPnt;
}
std::string Point3D::print(void) const {
	std::stringstream stream;
	stream << "(" << x << ", " << y << ", " << z << ")";
	return stream.str();
}
