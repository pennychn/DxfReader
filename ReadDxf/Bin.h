#pragma once

#include "Shape.h"
#include "def.h"
class Bin
{
	public:
		Bin(void);
		Bin(double _binW, double _binH, double x, double y, double z);
		~Bin(void);
		void AddShape(Shape *_s);
		void print();
	private:
		/// Shapes inner this bin
		ShapeList m_shapes;
		Point3D m_bl; // bottom-left point
		/** 考慮將來會改變Bin的長寬，所以
		 *  改以double而不用整數型態去儲存
		 */
		double m_width,
					 m_height;
};

typedef list<Bin *> BinList;
typedef list<Bin *>::iterator binListItr;
typedef list<Bin *>::const_iterator binListcItr;

