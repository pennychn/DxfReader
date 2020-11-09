#pragma once
#include "shape.h"

class MyDot : public Shape
{
	public:
		MyDot(void);
		MyDot(const Point3D& point);
		MyDot(const MyDot& d);
		~MyDot(void);

		MyDot& operator=(const MyDot& d);
		/**
		 * Following virtual function must be "reimplemented" due to the inheritance of Shape
		 */
		virtual Shape* clone(void) const;
		virtual bool operator==(const Shape &s) const; 
		virtual bool operator!=(const Shape &s) const; 
		virtual std::string print(void); 
		// virtual MyRectangle* getKernel(void) const;
		virtual void draw(CDC &aDC, COLORREF color, COLORREF fcolor , 
				              int width, bool Filled);
		/// \name Unused Function
		//@{
		/// getStartPoint => return m_Point
		virtual Point3D getStartPoint(void);
		/// getEndPoint => return m_Point
		virtual Point3D getEndPoint(void);
		/// getAngle => return 0.0
		virtual double getAngle(void);
		//@}

		virtual LaserProperty *const getProperty() const;
		virtual void setProperty(LaserProperty *const lp);
		/**
		 * self member function
		 */
		MyRectangle* getKernel(void);
		Point3D getPoint(void);
	protected:
		Point3D m_Point;
};
