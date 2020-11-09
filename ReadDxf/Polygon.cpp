#include "Polygon.h"
#include <algorithm>

MyPolygon::MyPolygon(void) {
	m_flag = 0;
	m_laserParam = NULL;
}
MyPolygon::~MyPolygon(void) { 
}
MyPolygon::MyPolygon(const MyPolygon& poly) {
	m_vertices.assign(poly.m_vertices.begin(), poly.m_vertices.end());
	//std::copy(poly.m_vertices.begin(), poly.m_vertices.end(),
						//m_vertices.begin());
	m_flag = poly.m_flag;
	m_laserParam = poly.m_laserParam;
}
MyPolygon& MyPolygon::operator=(const MyPolygon& poly) {
	std::copy(poly.m_vertices.begin(), poly.m_vertices.end(),
						m_vertices.begin());
	m_flag = poly.m_flag;
	m_laserParam = poly.m_laserParam;
	return *this;
}
Shape* MyPolygon::clone(void) const {
	return new MyPolygon(*this);
}
bool MyPolygon::operator==(const Shape &s) const { 
	bool ret = false;
	if (typeid(s) == typeid(*this)) { // 先確認是否是同一種的型態
		MyPolygon poly = *(MyPolygon*)&s; // 將s轉成(MyPolygon*)型，再取其值*
		ret = (m_flag == poly.m_flag
				   && std::equal(m_vertices.begin(), m_vertices.end(), 
						             poly.m_vertices.begin())
				);
	}
	return ret;
}
bool MyPolygon::operator!=(const Shape &s) const { 
	return ! operator==(s);
}
std::string MyPolygon::print(void) { 
	std::stringstream stream;
	stream << "Polygon:";
	stream << " flat:" << m_flag;
	stream << " point list: \n";
	std::list<Point3D>::const_iterator vItr = m_vertices.begin();
	while (vItr != m_vertices.end()) {
		stream << " Point:" << (*vItr).print();
		stream << std::endl;
		++vItr;
	}
	return stream.str();
}
Point3D MyPolygon::getStartPoint(void) {
	return m_vertices.back();
}
Point3D MyPolygon::getEndPoint(void) {
	return m_vertices.front();
}
double MyPolygon::getAngle(void) {
	return 0.0;
}
LaserProperty *const MyPolygon::getProperty() const {
	return m_laserParam;
}
void MyPolygon::setProperty(LaserProperty *const lp) {
	m_laserParam = lp;
}
int MyPolygon::getFlag(void) {
	return m_flag;
}
void MyPolygon::setFlag(int flag) {
	m_flag = flag;
}
size_t MyPolygon::getVertexNum(void) {
	return m_vertices.size();
}
void MyPolygon::addVertex(Point3D &p) {
	m_vertices.push_back(p);
}
void MyPolygon::draw(CDC &aDC, COLORREF color, COLORREF fcolor , 
									   int width, bool Filled = false) {


}
MyRectangle* MyPolygon::getKernel(void) {
	double min_x = 0.0f, min_y = 0.0f,
	      max_x = 0.0f, max_y = 0.0f;
	std::list<Point3D>::const_iterator vItr = m_vertices.begin();
	min_x = max_x = (*vItr).getX();
	min_y = max_y = (*vItr).getY();
	++vItr;
	while (vItr != m_vertices.end()) {
		if ((*vItr).getX() < min_x) min_x = (*vItr).getX();
		if ((*vItr).getX() > max_x) max_x = (*vItr).getX();
		if ((*vItr).getY() < min_y) min_y = (*vItr).getY();
		if ((*vItr).getY() > max_y) max_y = (*vItr).getY();
		++vItr;
	}
	Pin *bl = new Pin(min_x, min_y);
	Pin *tr = new Pin(max_x, max_y);
	return new MyRectangle(bl, tr);
}
