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
		/** �Ҽ{�N�ӷ|����Bin�����e�A�ҥH
		 *  ��Hdouble�Ӥ��ξ�ƫ��A�h�x�s
		 */
		double m_width,
					 m_height;
};

typedef list<Bin *> BinList;
typedef list<Bin *>::iterator binListItr;
typedef list<Bin *>::const_iterator binListcItr;

