#include "Dot.h"

MyDot::MyDot(void) {
}
MyDot::~MyDot(void) {
}
MyDot::MyDot(const Point3D& point) {
	m_Point = point;
	m_laserParam = NULL;
}
MyDot::MyDot(const MyDot& d) {
	m_Point = d.m_Point;
	m_laserParam = d.m_laserParam;
}
MyDot& MyDot::operator=(const MyDot& d) {
	m_Point = d.m_Point;
	m_laserParam = d.m_laserParam;
	return *this;
}
Shape* MyDot::clone(void) const {
	return new MyDot(*this);
}
bool MyDot::operator==(const Shape &s) const {
	bool ret = false;
	if (typeid(s) == typeid(*this)) { // 先確認是否是同一種的型態
		MyDot d = *(MyDot*)&s; // 將s轉成(MyDot*)型，再取其值*
		ret = (m_Point == d.m_Point);
	}
	return ret;
}
bool MyDot::operator!=(const Shape &s) const {
	return ! operator==(s);
}
std::string MyDot::print(void) {
	std::stringstream stream;
	stream << "Dot:";
	stream << " Axis:" << getPoint().print();
	return stream.str();
}
Point3D MyDot::getStartPoint(void) {
	return m_Point;
}
Point3D MyDot::getEndPoint(void) {
	return m_Point;
}
double MyDot::getAngle(void) {
	return 0.0;
}
LaserProperty *const MyDot::getProperty() const {
	return m_laserParam;
}
void MyDot::setProperty(LaserProperty *const lp) {
	m_laserParam = lp;
}
Point3D MyDot::getPoint(void) { 
	return m_Point; }
void MyDot::draw(CDC &aDC, COLORREF color, COLORREF fcolor , 
								 int width, bool Filled = false) {
}

MyRectangle* MyDot::getKernel(void) {
	Pin *bl = new Pin(this->getPoint().getX(), this->getPoint().getY());
	Pin *tr = new Pin(this->getPoint().getX(), this->getPoint().getY());
	return new MyRectangle(bl, tr);
}
