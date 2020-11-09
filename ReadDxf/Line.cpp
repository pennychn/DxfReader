#include "Line.h"

MyLine::MyLine(void) {
}
MyLine::~MyLine(void) {
}
MyLine::MyLine(const Point3D& sPnt, const Point3D& ePnt) { 
	m_startPnt = sPnt;
	m_endPnt = ePnt;
	m_laserParam = NULL;
}
MyLine::MyLine(const MyLine& l) {
	m_startPnt = l.m_startPnt;
	m_endPnt = l.m_endPnt;
	m_laserParam = l.m_laserParam;
}
MyLine& MyLine::operator=(const MyLine& l) {
	m_startPnt = l.m_startPnt;
	m_endPnt = l.m_endPnt;
	m_laserParam = l.m_laserParam;
	return *this;
}
Shape* MyLine::clone(void) const {
	return new MyLine(*this);
}
bool MyLine::operator==(const Shape &s) const { 
	bool ret = false;
	if (typeid(s) == typeid(*this)) {
		MyLine line=*(MyLine*)&s;
		ret = (m_startPnt == line.getStartPoint() 
		      && m_endPnt == line.getEndPoint());
	}
	return ret;
}
bool MyLine::operator!=(const Shape &s) const { 
	return ! operator==(s);
}
std::string MyLine::print(void) { 
	std::stringstream stream;
	stream << "MyLine: start:" << m_startPnt.print();
	stream << " ,end:" << m_endPnt.print();
	return stream.str(); 
}
Point3D MyLine::getStartPoint(void) {
	return m_startPnt;
}
Point3D MyLine::getEndPoint(void) {
	return m_endPnt;
}
double MyLine::getAngle(void) {
	double d_x = m_endPnt.x - m_startPnt.x;
	double d_y = m_endPnt.y - m_startPnt.y;
	double angle = atan2 (d_y, d_x) / M_PI * 180.0 ;
	return angle;
}
LaserProperty *const MyLine::getProperty() const {
	return m_laserParam;
}
void MyLine::setProperty(LaserProperty *const lp) {
	m_laserParam = lp;
}
void MyLine::draw(CDC &aDC, COLORREF color, COLORREF fcolor , 
									int width, bool Filled = false) {
	CPen pen(PS_SOLID, width, color);
	CPen *oldPen = aDC.SelectObject(&pen);

	aDC.MoveTo(m_startPnt.x, m_startPnt.y);
	aDC.LineTo(m_endPnt.x, m_endPnt.y);

	aDC.SelectObject(oldPen);
}
MyRectangle* MyLine::getKernel(void) {
	double min_x = 0.0f, min_y = 0.0f, max_x = 0.0f, max_y = 0.0f;
	double sX = this->getStartPoint().getX(),
				 sY = this->getStartPoint().getY(),
				 eX = this->getEndPoint().getX(),
				 eY = this->getEndPoint().getY();
	if (sX < eX) {
		min_x = sX;
		max_x = eX;
	}
	else {
		min_x = eX;
		max_x = sX;
	}
	if (sY < eY) {
		min_y = sY;
		max_y = eY;
	}
	else {
		min_y = eY;
		max_y = sY;
	}
	Pin *bl = new Pin(min_x, min_y);
	Pin *tr = new Pin(max_x, max_y);
	return new MyRectangle(bl, tr);
}
