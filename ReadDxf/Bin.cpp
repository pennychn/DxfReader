#include "Bin.h"

Bin::Bin(void) {
	m_bl.setX(0);
	m_bl.setY(0);
	m_bl.setZ(0);
	m_width = m_height = 0;
}
Bin::Bin(double _binW, double _binH, double x, double y, double z) {
	m_bl.setX(x);
	m_bl.setY(y);
	m_bl.setZ(z);
	m_width = _binW;
	m_height = _binH;
}
Bin::~Bin(void) { 
}
void Bin::AddShape(Shape *_s) {
	m_shapes.push_back(_s);
}
void Bin::print() {
	*pLog->logFile() << "============ Bin info.============" << std::endl;
	*pLog->logFile() << "BL point ";
	*pLog->logFile() << m_bl.print() << std::endl;
	*pLog->logFile() << "Shapes in this bin" << std::endl;
	ShapeListcItr i = m_shapes.begin();
	while (i != m_shapes.end()) {
		*pLog->logFile() << (*i)->print() << std::endl;
		++i;
	}
}
