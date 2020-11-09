#pragma once

#include "Point3D.h"
#include "LaserProperty.h"
#include "Rectangle.h"
#include <list>
#include <cmath>
#include <afxwin.h> // 使用CDC Object

#define M_PIl 3.1415926535897932384626433832795029L /* pi */
#define M_PI 3.14159265358979323846
class Shape
{
	public:
		Shape(void);
		virtual ~Shape(void);
		virtual bool operator==(const Shape &s) const=0; // ( =0 ) pure virtual function
		virtual bool operator!=(const Shape &s) const=0; 
		virtual Shape* clone(void) const=0;
		virtual std::string print(void)=0; 
		virtual Point3D getStartPoint(void)=0;
		virtual Point3D getEndPoint(void)=0;
		virtual double getAngle(void)=0;
		/// @bug  宣告底下方式，會使得繼承者無法宣告成功，原因不明
		/*virtual MyRectangle* getKernel(void)=0;*/
		// virtual std::string getGcode(void)=0;
		virtual LaserProperty *const getProperty() const=0;
		virtual void setProperty(LaserProperty *const lp)=0;
		virtual bool isCircle();
		/**
		 * @param aDC device context
		 * @param color the color setting for GDI object, pen, brush...
		 * @param fcolor the color seeting for ??
		 * @param width the width setting for GDI object, pen, brush...
		 * @param Filled the value to determine fill with object or not
		 */
		virtual void draw(CDC &aDC, COLORREF color, COLORREF fcolor , 
				              int width, bool Filled = false)=0;
	protected:
		LaserProperty* m_laserParam;
};

typedef std::list<Shape*> ShapeList;
typedef std::list<Shape*>::iterator ShapeListItr;
typedef std::list<Shape*>::const_iterator ShapeListcItr;
