#pragma once

#ifndef SEGMENT_H
#define SEGMENT_H

#include "Pin.h"

class Segment
{
	public:
		// constructor
		Segment(); 
		Segment(Pin* a, Pin* b); // <-- prefered constructor
		~Segment(); 

		// doesn't new a Point, just modify the value
		void SetStrPin(double _x, double _y); 
		void SetEndPin(double _x, double _y); 
		void SetPins(double _x1, double _y1,
				         double _x2, double _y2);  

		// accessor
		Pin* GetStrPin() { return m_StrPin; }
		Pin* GetEndPin() { return m_EndPin; }
		double GetXdist() const { return m_EndPin->getXDist(m_StrPin); }
		double GetYdist() const { return m_EndPin->getYDist(m_StrPin); }

		// misc
		void print();
		// gemetry condition: assumed m_StrPin and m_EndPin at same layer
		bool IsHorizontal() { return m_StrPin->getY() == m_EndPin->getY(); }
		bool IsVertical()   { return m_StrPin->getX() == m_EndPin->getX(); }
		bool IsSamePin() { return (IsHorizontal() && IsVertical()); }

		// Segment vs. Segment
		bool IsOverlap(Segment *); // if this->segment is overlap with seg
		bool IsIntersect(Segment *);
	private:
		int _ccw(Pin*, Pin*, Pin*);

		Pin* m_StrPin;
		Pin* m_EndPin;

		// interior function
		double _min(double, double);
		double _max(double, double);
};

typedef list<Segment* > segmentList;
typedef segmentList::iterator segmentListItr;
typedef segmentList::const_iterator segmentListcItr;

#endif


