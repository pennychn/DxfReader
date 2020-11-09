#include "Arc.h"

MyArc::MyArc(void) {
}
MyArc::~MyArc(void) {
}
MyArc::MyArc(const Point3D& center, double sAngle, double eAngle, double radius) {
	m_centerPnt = center;
	m_startAngle = sAngle;
	m_endAngle = eAngle;
	m_radius = radius;
	m_dirCCW = true;
	m_laserParam = NULL;
}
MyArc::MyArc(const MyArc& a) {
	m_centerPnt = a.m_centerPnt;
	m_startAngle = a.m_startAngle;
	m_endAngle = a.m_endAngle;
	m_radius = a.m_radius;
	m_dirCCW = a.m_dirCCW;
	m_laserParam = a.m_laserParam;
}
MyArc& MyArc::operator=(const MyArc& a) {
	m_centerPnt = a.m_centerPnt;
	m_startAngle = a.m_startAngle;
	m_endAngle = a.m_endAngle;
	m_radius = a.m_radius;
	m_dirCCW = a.m_dirCCW;
	m_laserParam = a.m_laserParam;
	return *this;
}
Shape* MyArc::clone(void) const {
	return new MyArc(*this);
}
bool MyArc::operator==(const Shape &s) const {
	bool ret = false;
	if (typeid(s) == typeid(*this)) { // 先確認是否是同一種的型態
		MyArc arc = *(MyArc*)&s; // 將s轉成(MyArc*)型，再取其值*
		ret = ( m_centerPnt == arc.m_centerPnt
				    && m_startAngle == arc.m_startAngle 
				    && m_endAngle == arc.m_endAngle 
						&& m_radius == arc.m_radius
						&& m_dirCCW == arc.m_dirCCW );
	}
	return ret;
}
bool MyArc::operator!=(const Shape &s) const {
	return ! operator==(s);
}
std::string MyArc::print(void) {
	std::stringstream stream;
	stream << "Arc:" << std::endl;
	stream << " start-point:" << getStartPoint().print() << std::endl;
	stream << " end-point:" << getEndPoint().print() << std::endl;
	stream << " center:"    << m_centerPnt.print() << std::endl;
	stream << " start_angle:" << m_startAngle << std::endl;
	stream << " end_angle:" << m_endAngle << std::endl;
	stream << " r:" << m_radius << std::endl;
	return stream.str();
}
Point3D MyArc::getStartPoint(void) {
	Point3D start_point;
	start_point.x = m_centerPnt.x + cos(m_startAngle*double(M_PIl)/double(180.0))*m_radius;
	start_point.y = m_centerPnt.y + sin(m_startAngle*double(M_PIl)/double(180.0))*m_radius;
	return start_point;
}
Point3D MyArc::getEndPoint(void) {
	Point3D end_point;
	end_point.x = m_centerPnt.x + cos(m_endAngle*double(M_PIl)/double(180.0))*m_radius;
	end_point.y = m_centerPnt.y + sin(m_endAngle*double(M_PIl)/double(180.0))*m_radius;       
	return end_point;
}
double MyArc::getAngle(void) {
	Point3D end_point(getEndPoint());
	Point3D start_point(getStartPoint());
	double d_x = end_point.x - start_point.x;
	double d_y = end_point.y - start_point.y;
	double angle = atan2 (d_y, d_x) / M_PI * 180.0 ;
	return angle;
}
LaserProperty *const MyArc::getProperty() const {
	return m_laserParam;
}
void MyArc::setProperty(LaserProperty *const lp) {
	m_laserParam = lp;
}
bool MyArc::isCircle() {
	bool ret = false;
	double angle = fabs(getEndAngle() - getStartAngle());
	if (angle == 360)
		ret = true;
	return ret;
}
void MyArc::draw(CDC &aDC, COLORREF color, COLORREF fcolor, 
								 int width, bool Filled = false) {


}
double MyArc::getStartAngle(void) { return m_startAngle; }
double MyArc::getEndAngle(void)   { return m_endAngle; }
double MyArc::getRadius(void) { return m_radius; }
Point3D MyArc::getCenterPoint(void) { return m_centerPnt; }
bool MyArc::isCounterClockWise(void) { return m_dirCCW; }
bool MyArc::_isAngleInQ1(double angle) {
	return (0.0f <= angle && angle < 90.0f);
}
bool MyArc::_isAngleInQ2(double angle) {
	return (90.0f <= angle && angle < 180.0f);
}
bool MyArc::_isAngleInQ3(double angle) {
	return (180.0f <= angle && angle < 270.0f);
}
bool MyArc::_isAngleInQ4(double angle) {
	return (270.0f <= angle && angle < 360.0f);
}
double MyArc::_min(double x, double y) {
	return (x>y)?y:x;
}
double MyArc::_max(double x, double y) {
	return (x>y)?x:y;
}
MyRectangle* MyArc::getKernel(void) {
	/**
	 * I) 對於一個弧而言，可能有四種shape
	 *  s1 圓形(circle)
	 *  s2 優扇形(major sector) ==> 大於半圓
	 *  s3 半圓(half-circle)
	 *  s4 劣扇形(minor sector) ==> 小於半圓
	 * II) 對於一個DXF所定義, 弧是從start point逆時針連至end point
	 *
	 * 判斷二點間的夾角 theta 大小
	 * 1 theta > 180
	 * 	+ 代表kernel是包圍一個圓
	 * 2 90 < theta <= 180
	 * 	+ 代表kernel是包圍一個長方形(即半圓)
	 * 3 theta <= 90
	 * 	+ 代表kernel是包圍一個正方形(即1/4圓)
	 */
	// Arc store a circle
	Pin *bl = NULL, *tr = NULL;
	MyRectangle *ret = NULL;
	if (true == this->isCircle()) {
		bl = new Pin(this->getCenterPoint().getX() - this->getRadius(),
				         this->getCenterPoint().getY() - this->getRadius());
		tr = new Pin(this->getCenterPoint().getX() + this->getRadius(),
				         this->getCenterPoint().getY() + this->getRadius());
		ret = new MyRectangle(bl, tr);
		*pLog->logFile() << "\t Construct a complete circle frame\n";
		ret->print();
	} 
	else {
		double sAngle = this->getStartAngle(),
					 eAngle = this->getEndAngle(),
					 radius   = this->getRadius();
		Point3D sPnt = this->getStartPoint(),
						ePnt = this->getEndPoint(),
						cPnt = this->getCenterPoint();
		/**
		 * Comment Explain
		 * r        ==> radius
		 * c_x, c_y ==> cPnt.getX(), cPnt.getY()
		 * s_x, s_y ==> sPnt.getX(), sPnt.getY()
		 * e_x, e_y ==> ePnt.getX(), ePnt.getY()
		 * max_x    ==> _max(sPnt.getX(), ePnt.getX())
		 * max_y    ==> _max(sPnt.getY(), ePnt.getY())
		 * min_x    ==> _min(sPnt.getX(), ePnt.getX())
		 * min_y    ==> _min(sPnt.getY(), ePnt.getY())
		 */
		if (eAngle > sAngle) {
			if (true == _isAngleInQ1(sAngle)) {
				if (true == _isAngleInQ1(eAngle)) {
					/// @note w/o consider sAngle = eAngle = 0
					// BL(c_x, c_y)，TR(s_x, e_y) 
					bl = new Pin(cPnt.getX(), cPnt.getY());
					tr = new Pin(sPnt.getX(), ePnt.getY());
					*pLog->logFile() << "\t Construct a Q1-Q1 frame\n";
				} 
				else if (true == _isAngleInQ2(eAngle)) {
					if (0.0f == sAngle && 90.0f == eAngle) {
						// BL(c_x, c_y)，TR(s_x, e_y) 
						bl = new Pin(cPnt.getX(), cPnt.getY());
						tr = new Pin(sPnt.getX(), ePnt.getY());
						*pLog->logFile() << "\t Construct a Q1 quad-circle frame\n";
					}
					else {
						// BL(e_x, c_y)，TR(s_x, c_y+r) 
						*pLog->logFile() << "\t Construct a Q1-Q2 frame\n";
						bl = new Pin(ePnt.getX(), cPnt.getY());
						tr = new Pin(sPnt.getX(), cPnt.getY() + radius);
					}
				} 
				else if (true == _isAngleInQ3(eAngle)) {
					if (0.0f == sAngle && 180.0f == eAngle) {
						// BL(e_x, e_y)，TR(s_x, c_y+r) 
						*pLog->logFile() << "\t Construct a Q1-Q2 half-circle frame\n";
						bl = new Pin(ePnt.getX(), ePnt.getY());
						tr = new Pin(sPnt.getX(), cPnt.getY() + radius);
					}
					else {
						// BL(c_x-r, e_y)，TR(s_x, c_y+r) 
						*pLog->logFile() << "\t Construct a Q1-Q3 frame\n";
						bl = new Pin(cPnt.getX() - radius, ePnt.getY());
						tr = new Pin(sPnt.getX(), cPnt.getY() + radius);
					}
				} else if (true == _isAngleInQ4(eAngle)) {
					*pLog->logFile() << "\t Construct a Q1-Q4 frame\n";
					// BL(c_x-r, c_y-r)，TR(max_x, c_y+r) 
					bl = new Pin(cPnt.getX() - radius, cPnt.getY() - radius);
					tr = new Pin(_max(sPnt.getX(), ePnt.getX()), cPnt.getY() + radius);
				}
			}
			else if (true == _isAngleInQ2(sAngle)) {
				if (true == _isAngleInQ2(eAngle)) {
					// BL(e_x, c_y)，TR(c_x, s_y) 
					*pLog->logFile() << "\t Construct a Q2-Q2 frame\n";
					bl = new Pin(ePnt.getX(), cPnt.getY());
					tr = new Pin(cPnt.getX(), sPnt.getY());
				} 
				else if (true == _isAngleInQ3(eAngle)) {
					if (90.0f == sAngle && 180.0f == eAngle) {
						// BL(e_x, e_y)，TR(s_x, s_y) 
						*pLog->logFile() << "\t Construct a Q2 quad-circle frame\n";
						bl = new Pin(ePnt.getX(), ePnt.getY());
						tr = new Pin(sPnt.getX(), sPnt.getY());
					}
					else {
						// BL(c_x-r, e_y)，TR(c_x, s_y) 
						*pLog->logFile() << "\t Construct a Q2-Q3 frame\n";
						bl = new Pin(cPnt.getX() - radius, ePnt.getY());
						tr = new Pin(cPnt.getX(), sPnt.getY());
					}
				} 
				else if (true == _isAngleInQ4(eAngle)) {
					if (90.0f == sAngle && 270.0f == eAngle) {
						// BL(c_x-r, e_y)，TR(s_x, s_y) 
						*pLog->logFile() << "\t Construct a Q2-Q3 half-circle frame\n";
						bl = new Pin(cPnt.getX() - radius, ePnt.getY());
						tr = new Pin(sPnt.getX(), sPnt.getY());
					}
					else {
						// BL(c_x-r, c_y-r)，TR(e_x, s_y) 
						*pLog->logFile() << "\t Construct a Q2-Q4 frame\n";
						bl = new Pin(cPnt.getX() - radius, cPnt.getY() - radius);
						tr = new Pin(ePnt.getX(), sPnt.getY());
					}
				}
			}
			else if (true == _isAngleInQ3(sAngle)) {
				if (true == _isAngleInQ3(eAngle)) {
					// BL(s_x, e_y)，TR(c_x, c_y) 
					*pLog->logFile() << "\t Construct a Q3-Q3 frame\n";
					bl = new Pin(sPnt.getX(), ePnt.getY());
					tr = new Pin(cPnt.getX(), cPnt.getY());
				} 
				else if (true == _isAngleInQ4(eAngle)) {
					if (180.0f == sAngle && 270.0f == eAngle) {
						// BL(s_x, e_y)，TR(c_x, c_y) 
						*pLog->logFile() << "\t Construct a Q2 quad-circle frame\n";
						bl = new Pin(sPnt.getX(), ePnt.getY());
						tr = new Pin(cPnt.getX(), cPnt.getY());
					}
					else {
						// BL(s_x, c_y-r)，TR(e_x, c_y) 
						*pLog->logFile() << "\t Construct a Q2-Q4 frame\n";
						bl = new Pin(sPnt.getX(), cPnt.getY() - radius);
						tr = new Pin(ePnt.getX(), cPnt.getY());
					}
				} 
			}
			else if (true == _isAngleInQ4(sAngle)) {
				/// @note eAngle must be in Q4
				// BL(c_x, s_y)，TR(e_x, c_y) 
				*pLog->logFile() << "\t Construct a Q4-Q4 frame\n";
				bl = new Pin(cPnt.getX(), sPnt.getY());
				tr = new Pin(ePnt.getX(), cPnt.getY());
			}
			ret = new MyRectangle(bl, tr);
			ret->print();
		}
		else if (sAngle > eAngle) {
			/// @todo wait to finish
			*pLog->errFile() << "An un-finish zoon\n";


		}
		else {
			// sAngle == eAngle
			*pLog->errFile() << "Unexcepted condition: sAngle == eAngle has problem\n";
		}
	}
	return ret;
}
