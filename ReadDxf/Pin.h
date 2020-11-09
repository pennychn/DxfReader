#pragma once

#ifndef PIN_H
#define PIN_H

#include "def.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <vector>
#include <list>
#include <algorithm>
#include <stack>
#include <queue>
#include <cstdlib>
using namespace std;

class Pin;
typedef list<Pin* > pinList;
typedef pinList::iterator pinListItr; // linking list
typedef pinList::const_iterator pinListcItr;

class Pin
{
public:
	Pin();
	Pin(double _x, double _y); 
	Pin(int _id, PinType _type, double _x, double _y); 
	Pin(Pin *_p); // copy constructor
	~Pin(void); 
	void print();
	// accessor
	inline int getID() { return m_id; }
	inline PinType getType() { return m_type; }
	inline double getX() { return m_axisX; }
	inline double getY() { return m_axisY; }
	double getXDist(Pin *p);
	double getYDist(Pin *p);
	double getXYDist(Pin *p);
	DIRECTION getRelativeDir(Pin* aPin);
	// modifier
	inline void setX(double _x) { m_axisX = _x; }
	inline void setY(double _y) { m_axisY = _y; }
	inline void setXY(double _x, double _y) { setX(_x); setY(_y); }
	// relation operators
	bool operator==(Pin *p);
	bool operator!=(Pin *p);
protected:
	double m_axisX;
	double m_axisY;

	int  m_id;      // pin id in my own vector
	PinType m_type; // 
};

#endif

