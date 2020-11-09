#pragma once

#include <string>
#include <iostream>
#include <sstream>

const double cMinDeltaToBeEqal = 1E-10;

class Point3D
{
	public:
		Point3D(void);
		~Point3D(void);
		Point3D(const double _x, const double _y, const double _z);
		Point3D(const Point3D &p);
		bool operator==(const Point3D &p) const;
		bool operator!=(const Point3D &p) const;
		Point3D operator+(const Point3D p) const;
		Point3D operator-(const Point3D p) const;
		std::string print(void) const;
		// Access function
		inline double getX() const { return x; }
		inline double getY() const { return y; }
		inline double getZ() const { return z; }
		inline void   setX(double _x) { x = _x; }
		inline void   setY(double _y) { y = _y; }
		inline void   setZ(double _z) { z = _z; }
	// protected:
		double x, y, z;
};
