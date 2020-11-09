#include "Pin.h"

Pin::Pin() { 
	m_axisX = m_axisY = 0.0f;
	m_id = -1;
	m_type = UK;
#ifdef DEBUG
	*pLog->errFile() << "Warning -- default constructor of PIN() is called." << endl;
#endif
}
Pin::Pin(double _x, double _y) {
	m_id    = -1;
	m_type  = UK;
	m_axisX = _x; m_axisY = _y;
}
Pin::Pin(int _id, PinType _type, double _x, double _y) {
	m_id    = _id;
	m_type  = _type;
	m_axisX = _x; m_axisY = _y;
}
Pin::Pin(Pin *_p) {
	m_id    = _p->getID();
	m_type  = _p->getType();
	m_axisX = _p->getX(); 
	m_axisY = _p->getY();
}
Pin::~Pin() {
	// do nothing
}
void Pin::print() {
#ifdef DEBUG
	string pinStr;
	switch (m_type) {
		case IN:
			pinStr = "INPUT"; 
			*pLog->logFile() << pinStr << " PIN" << m_id 
												 << " (" << m_axisX << ", " << m_axisY << ")"
												 << endl;
			break;
		case OUT:
			pinStr = "OUTPUT"; 
			*pLog->logFile() << pinStr << " PIN" << m_id 
												 << " (" << m_axisX << ", " << m_axisY << ")"
												 << endl;
			break;
		case FF:
			pinStr = "FLIP_FLOP"; 
			*pLog->logFile() << pinStr << " PIN" << m_id 
												 << " (" << m_axisX << ", " << m_axisY << ")"
												 << endl;
			break;
		case UK:
			pinStr = "UnKnown"; 
			*pLog->logFile() <<  " (" << m_axisX << ", " << m_axisY << ")"
												 << endl;
			break;
		default:
			pinStr = "Error"; 
			*pLog->errFile() << "Warning -- out of exception" << endl;
	}
#endif
}
bool Pin::operator==(Pin *p) {
	return FLOAT_EQ(this->getX(), p->getX()) &&
				 FLOAT_EQ(this->getY(), p->getY());
}
bool Pin::operator!=(Pin *p) {
	return !(*this == p);
}
DIRECTION Pin::getRelativeDir(Pin* aPin) {
	if (this->getX() == aPin->getX() && this->getY() == aPin->getY()) {
		return SAME;
	}
	else {
		if (this->getY() == aPin->getY())
			return (this->getX() > aPin->getX())?E:W;
		else if (this->getX() == aPin->getX())
			return (this->getY() > aPin->getY())?S:N;
		else if (this->getX() > aPin->getX()) {
			return (this->getY() > aPin->getY())?SE:NE;
		}
		else if (this->getX() < aPin->getX()) {
			return (this->getY() > aPin->getY())?SW:NW;
		}
	}
	return UNKNOWN;
}
double Pin::getXDist(Pin *p) {
	return fabs(this->getX() - p->getX());
}
double Pin::getYDist(Pin *p) {
	return fabs(this->getY() - p->getY());
}
double Pin::getXYDist(Pin *p) {
	return ( this->getXDist(p) + this->getYDist(p) );
}
