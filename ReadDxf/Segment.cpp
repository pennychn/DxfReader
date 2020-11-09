#include "Segment.h"
Segment::Segment() {
	m_StrPin = m_EndPin = NULL;
}
Segment::Segment(Pin* a, Pin* b) {
	m_StrPin = a;
	m_EndPin = b;
}
Segment::~Segment() { 
}
void Segment::SetStrPin(double _x, double _y) {
	m_StrPin->setXY(_x, _y);
}
void Segment::SetEndPin(double _x, double _y) {
	m_EndPin->setXY(_x, _y);
}
void Segment::SetPins(double _x1, double _y1, double _x2, double _y2) {
	SetStrPin(_x1, _y1);
	SetEndPin(_x2, _y2);
}
bool Segment::IsOverlap(Segment* seg) {
	Pin *segOne = seg->GetStrPin();
	Pin *segTwo = seg->GetEndPin();

	if(IsVertical() && seg->IsVertical() && 
		 m_StrPin->getX() == segOne->getX()) {
		double minSegY = _min(segOne->getY(), segTwo->getY());
		double maxSegY = _max(segOne->getY(), segTwo->getY());
		double minY = _min(m_StrPin->getY(), m_EndPin->getY());
		double maxY = _max(m_StrPin->getY(), m_EndPin->getY());

		if(minSegY < minY && minY <= maxSegY)
			return true;
		else if(minY <= minSegY && minSegY <= maxY)
			return true;
	}
	else if(IsHorizontal() && seg->IsHorizontal()&& 
			    m_StrPin->getY() == segOne->getY()) {
		double minSegX = _min(segOne->getX(), segTwo->getX());
		double maxSegX = _max(segOne->getX(), segTwo->getX());
		double minX = _min(m_StrPin->getX(), m_EndPin->getX());
		double maxX = _max(m_StrPin->getX(), m_EndPin->getX());

		if(minSegX < minX && minX <= maxSegX)
			return true;
		else if(minX <= minSegX && minSegX <= maxX)
			return true;
	}
	return false;
}
void Segment::print() {
#ifdef DEBUG
	*pLog->logFile() << "Segment: (" << m_StrPin->getX() << ", " << m_StrPin->getY()
																		<< ") -- (" 
																		<< m_EndPin->getX() << ", " << m_EndPin->getY()
																		<< ")" << endl;
#endif
}
bool Segment::IsIntersect(Segment* s2) {
	if (_ccw(m_StrPin, m_EndPin, s2->GetStrPin()) == 
		_ccw(m_StrPin, m_EndPin, s2->GetEndPin()))
		return false;
	else if (_ccw(s2->GetStrPin(), s2->GetEndPin(), m_StrPin) == 
		_ccw(s2->GetStrPin(), s2->GetEndPin(), m_EndPin))
		return false;
	else
		return true;
}
// ========================================
// Interior Function
// ========================================
int Segment::_ccw(Pin* a, Pin* b, Pin* c) {
	double triArea = (b->getX() - a->getX())*(c->getY() - a->getY()) - 
		(b->getY() - a->getY())*(c->getX() - a->getX());
	
	if (triArea < 0)      return -1; // a-b-c clockwise
	else if (triArea > 0) return  1; // a-b-c counterclockwise
	else				  return  0; // collinear a b c
}
double Segment::_min(double m_StrPin, double m_EndPin) {
	if (m_StrPin < m_EndPin)
		return m_StrPin;
	else
		return m_EndPin;
}
double Segment::_max(double m_StrPin, double m_EndPin) {
	if (m_StrPin > m_EndPin)
		return m_StrPin;
	else
		return m_EndPin;
}
