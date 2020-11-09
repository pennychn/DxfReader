#include "Rectangle.h"
MyRectangle::MyRectangle() {
	m_RID = -1;
	m_tr = m_bl = NULL;
	m_slack[0] = m_slack[1] = 0;

#ifdef DEBUG
	*pLog->errFile() << "Warning -- default constructor of MyRectangle() is called." << endl;
#endif
}
MyRectangle::MyRectangle(Pin *_bl, Pin *_tr) {
	m_RID = -1;
	m_tr = _tr;
	m_bl = _bl;
	m_slack[0] = m_slack[1] = 0;
#ifdef DEBUG
	if (m_bl->getX() > m_tr->getX())
		*pLog->errFile() << "ERROR: BL.X > TR.X" << endl;
	if (m_bl->getY() > m_tr->getY())
		*pLog->errFile() << "ERROR: BL.Y > TR.Y" << endl;
#endif
}
MyRectangle::~MyRectangle() { 
}
bool MyRectangle::operator>(const MyRectangle *r) const {
	return (this->getBLX() > r->getTRX());
}
bool MyRectangle::operator<(const MyRectangle *r) const {
	return (this->getTRX() < r->getBLX());
	// return !operator>(r);
}
int MyRectangle::pointPos(Pin* aPin) {
  if (aPin->getX() < m_bl->getX() ||  
			aPin->getY() < m_bl->getY() || 
	    aPin->getX() > m_tr->getX() || 
			aPin->getY() > m_tr->getY())
		return 0; // outer
  else if (aPin->getX() > m_bl->getX() && 
			     aPin->getX() < m_tr->getX() && 
	         aPin->getY() > m_bl->getY() && 
					 aPin->getY() < m_tr->getY())
	  return 1; // inner
  else
		return -1; // in-line
}
int MyRectangle::pointPos(double x, double y) {
	Pin *pnt = new Pin(x, y);
	int rstInter = this->pointPos(pnt);
	delete pnt;
	return rstInter;
}
bool MyRectangle::isPin() {
	return (m_bl == m_tr);
}
bool MyRectangle::isSegment() {
	if (! isPin()) {
		if (getWidth() == 0 || getHeight() == 0)
			return true;
	}
	return false;
}
void MyRectangle::print() {
#ifdef DEBUG
	*pLog->logFile()   << "Bottom Left" << "(" << m_bl->getX() << ", " 
										 << m_bl->getY()  << ")" << endl
	                   << "Top Right"   << "(" << m_tr->getX() << ", " 
										 << m_tr->getY()  << ")" << endl;
#endif
}
void MyRectangle::print(ofstream &file) {
#ifdef DEBUG
	file << "Bottom Left" << "(" << m_bl->getX() << ", " 
			 << m_bl->getY()  << ")" << endl
	     << "Top Right"   << "(" << m_tr->getX() << ", " 
			 << m_tr->getY()  << ")" << endl;
#endif
}
void MyRectangle::printToError() {
#ifdef DEBUG
	*pLog->errFile() << "Net" << this->getID() << endl
		                 << "Bottom Left" << "(" << m_bl->getX() << ", " 
										 << m_bl->getY()  << ")" << endl
	                   << "Top Right"   << "(" << m_tr->getX() << ", " 
										 << m_tr->getY()  << ")" << endl;
#endif
}
// TRUE if this cover _rect
// 1. 找出二矩形之外框的高度(H')及寬度(W')
// 2. 計算目前矩形加上slack後是否會大於H'及W'
bool MyRectangle::IsCovering(MyRectangle *_rect) {
	double _minX = (this->getBLX() < _rect->getBLX())?this->getBLX():_rect->getBLX();
	double _minY = (this->getBLY() < _rect->getBLY())?this->getBLY():_rect->getBLY();
	double _maxX = (this->getTRX() > _rect->getTRX())?this->getTRX():_rect->getTRX();
	double _maxY = (this->getTRY() > _rect->getTRY())?this->getTRY():_rect->getTRY();

	double outerWidth  = _maxX - _minX,
				 outerHeight = _maxY - _minY;
	double availWidth  = this->getWidth() + this->getSlackX(),
				 availHeight = this->getHeight() + this->getSlackY();
	if ((availWidth > outerWidth || FLOAT_EQ(availWidth, outerWidth)) &&
			(availHeight > outerHeight || FLOAT_EQ(availHeight, outerHeight))) {
		// pLog->Log("TRUE");
		return true;
	}

	return false;
}
