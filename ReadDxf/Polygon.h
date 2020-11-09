#pragma once
#include "shape.h"
#include <list>

/// Polygon == Polyline
class MyPolygon : public Shape
{
	public:
		MyPolygon(void);
		MyPolygon(const MyPolygon& poly);
		~MyPolygon(void);
		MyPolygon& operator=(const MyPolygon& poly);
		/**
		 * Following virtual function must be "reimplemented" due to the inheritance of Shape
		 */
		virtual Shape* clone(void) const;
		virtual bool operator==(const Shape &s) const; 
		virtual bool operator!=(const Shape &s) const; 
		virtual std::string print(void); 
		/// \name Unused Function
		//@{
		/// getStartPoint => return m_Point
		virtual Point3D getStartPoint(void);
		/// getEndPoint => return m_Point
		virtual Point3D getEndPoint(void);
		/// getAngle => return 0.0
		///@bug what's the meaning of angle for this type
		virtual double getAngle(void);
		//@}
		/*virtual MyRectangle* getKernel(void) const;*/

		virtual LaserProperty *const getProperty() const;
		virtual void setProperty(LaserProperty *const lp);
		virtual void draw(CDC &aDC, COLORREF color, COLORREF fcolor , 
				              int width, bool Filled);
		/**
		 * self member function
		 */
		MyRectangle* getKernel(void);
		int getFlag(void);
		void setFlag(int flag);
		size_t getVertexNum(void);
		void addVertex(Point3D &p);
	protected:
		std::list<Point3D> m_vertices;
		int m_flag;
};
