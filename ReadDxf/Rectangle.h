#pragma once

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Pin.h"
#include "Segment.h"
#include "Shape.h"

class MyRectangle
{
	public:
		MyRectangle(); 
		MyRectangle(Pin *_bl, Pin *_tr); 
		// based class must be virtual destructor
		~MyRectangle(); 

		// accessor
		inline double getWidth() const  { return m_tr->getXDist(m_bl); }
		inline double getHeight() const { return m_tr->getYDist(m_bl); }
		inline double getBLX() const { return m_bl->getX(); }
		inline double getBLY() const { return m_bl->getY(); }
		inline double getTRX() const { return m_tr->getX(); }
		inline double getTRY() const { return m_tr->getY(); }
		inline double getCenterX() const { return (m_bl->getX() + m_tr->getX()) / 2; } 
		inline double getCenterY() const { return (m_bl->getY() + m_tr->getY()) / 2; }
		inline Pin* getBLP() { return m_bl; }
		inline Pin* getTRP() { return m_tr; }
		inline int getID() { return m_RID; }
		inline void setID(int _id) { m_RID = _id; }
		bool operator>(const MyRectangle *r) const; 
		bool operator<(const MyRectangle *r) const; 
		inline double getSlackX() const { return m_slack[0]; }
		inline double getSlackY() const { return m_slack[1]; }
		inline void setSlackX(double _x) { m_slack[0] = _x; }
		inline void setSlackY(double _y) { m_slack[1] = _y; }

		bool isPin();
		bool isSegment();
		/*bool isLineInterSect(Segment *seg);*/

		// misc
		// o:outer / 1:inner / -1:in-line
		int pointPos(Pin *aPin); 
		int pointPos(double, double); 

		void print();
		void print(ofstream &);
		void printToError(); 
		bool IsCovering(MyRectangle *_rect); 
	private:
		int m_RID;       
		double m_slack[2]; // slack[0] is x-axis, slack[1] is y-axis
		Pin *m_tr, *m_bl;
};
/*
class SortRectByXInc {
	public:
		bool operator()(const MyRectangle *r1, const MyRectangle *r2) const {
			return (r1->getBLX() < r2->getBLX());
		}
};

class SortRectByYInc {
	public:
		bool operator()(const MyRectangle *r1, const MyRectangle *r2) const {
			return (r1->getBLY() < r2->getBLY());
		}
};
*/
#endif 

