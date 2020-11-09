#pragma once
#include "shape.h"
class MyLine : public Shape
{
	public:
		MyLine(void);
		MyLine(const Point3D& sPnt, const Point3D& ePnt); 
		MyLine(const MyLine& l);
		~MyLine(void);
		MyLine& operator=(const MyLine& l);
		/**
		 * Following virtual function must be "reimplemented" due to the inheritance of Shape
		 */
		virtual Shape* clone(void) const;
		virtual bool operator==(const Shape &s) const; 
		virtual bool operator!=(const Shape &s) const; 
		virtual std::string print(void); 
		virtual Point3D getStartPoint(void);
		virtual Point3D getEndPoint(void);
		virtual double getAngle(void);
		/*virtual MyRectangle* getKernel(void) const;*/

		virtual LaserProperty *const getProperty() const;
		virtual void setProperty(LaserProperty *const lp);
		virtual void draw(CDC &aDC, COLORREF color, COLORREF fcolor , 
				              int width, bool Filled);
		/**
		 * self member function
		 */
		MyRectangle* getKernel(void);
	protected:
		Point3D m_startPnt,
						m_endPnt;
};
