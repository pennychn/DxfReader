#pragma once
#include "shape.h"

class MyArc : public Shape
{
	public:
		MyArc(void);
		MyArc(const Point3D& center, double sAngle, double eAngle, double radius);
		MyArc(const MyArc& a);
		~MyArc(void);

		MyArc& operator=(const MyArc& a);
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
		virtual bool isCircle();
		virtual void draw(CDC &aDC, COLORREF color, COLORREF fcolor, 
				              int width, bool Filled);
		/**
		 * self member function
		 */
		double getStartAngle(void);
		double getEndAngle(void);
		double getRadius(void);
		Point3D getCenterPoint(void);
		bool isCounterClockWise(void);
		MyRectangle* getKernel(void);
		bool _isAngleInQ1(double angle);
		bool _isAngleInQ2(double angle);
		bool _isAngleInQ3(double angle);
		bool _isAngleInQ4(double angle);
		double _min(double x, double y);
		double _max(double x, double y);
	protected:
		Point3D m_centerPnt;
		double m_startAngle,
					 m_endAngle;
		double m_radius;
		bool m_dirCCW;
};
