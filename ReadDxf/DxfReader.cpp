#include "DxfReader.h"

DxfReader::DxfReader(void):m_endEntitySym("0") {
}
DxfReader::DxfReader(ShapeList *shapelist) {
	m_shapes = shapelist;
	m_endEntitySym = "0";
}
DxfReader::~DxfReader(void) {
}
bool DxfReader::readFile(std::string inFileName) {
	bool ret = true;
	const int cnMaxBuf = 1000;
	this->m_dxf_stream.open(inFileName.c_str(), std::ios::in);
	if (!this->m_dxf_stream) {
		ret = false;
	}
	else {
		*pLog->logFile() << "Start Parser DXF file: " << inFileName << std::endl;
		char buffer[cnMaxBuf];
		char *pcToken = NULL;
		char delims[] = " ,\n";
		int nNumOfPoint = 0;
		// 菠DXFㄤウ场だENTITIES SECTION
		do {
			this->m_dxf_stream.getline(buffer, cnMaxBuf, '\n');
		} while ( strncmp(buffer, "ENTITIES", 8) != 0);
		// 秈ENTITIES SECTION
		while (this->m_dxf_stream.getline(buffer, cnMaxBuf, '\n')) {
			pcToken = strtok(buffer, delims);
			if (strncmp(pcToken, "ENDSEC", 6) == 0) {
				break; // 挡parser
			}
			else if (strncmp(pcToken, "AcDbPoint", 9) == 0) {
				this->_ReadDot();
				*pLog->logFile() << __LINE__ << ": get point = " << pcToken << std::endl;
				++nNumOfPoint;
			}
			else if (strncmp(pcToken, "AcDbLine", 8) == 0) {
				this->_ReadLine();
				*pLog->logFile() << __LINE__ << ": get line = " << pcToken << std::endl;
			}
			else if (strncmp(pcToken, "AcDbPolyline", 12) == 0) {
				this->_ReadPolyLine();
				*pLog->logFile() << __LINE__ << ": get poly = " << pcToken << std::endl;

			}
			else if (strncmp(pcToken, "AcDbCircle", 10) == 0) {
				this->_ReadCircle();
				*pLog->logFile() << __LINE__ << ": get circle = " << pcToken << std::endl;

			}
		}
		this->m_dxf_stream.close();
	}
	return ret;
}
// =========================================
// _StripWhitespace: 埃フ
// =========================================
std::string DxfReader::_StripWhitespace(const std::string& in) {
	std::string out(in);
	//trim	leading whitespace
	std::string::size_type notwhite=out.find_first_not_of(' ');
	out.erase(0, notwhite);
	//trim trailing whitespace
	notwhite=out.find_last_not_of(' ');
	out.erase(notwhite+1);
	return out;
}
std::string DxfReader::_ReadNextLine(void) {
	std::string line;
	std::getline(m_dxf_stream, line);
	return _StripWhitespace(line);
}
void DxfReader::_ReadNext2Lines(std::string& s1, std::string& s2) {
	s1 = _ReadNextLine();
	s2 = _ReadNextLine();
}
void DxfReader::_ReadDot() {
	std::string s1, s2;
	Point3D point;
	do {
		_ReadNext2Lines(s1, s2);
		if (s1.compare("10") == 0) {
			// point's xValue
			point.x = atof(s2.data());
		}
		if (s1.compare("20") == 0) {
			// point's yValue
			point.y = atof(s2.data());
		}
		if (s1.compare("30") == 0) {
			// point's zValue
			point.z = atof(s2.data());
		}
	} while (s1.compare(m_endEntitySym) != 0);
	MyDot dot(point);
	m_shapes->push_back(dot.clone());
}
void DxfReader::_ReadLine() {
	std::string s1, s2;
	Point3D p1, p2;
	do {
		_ReadNext2Lines(s1, s2);
		if (s1.compare("8") == 0) {
			// Layer Name
		}
		if (s1.compare("10") == 0) {
			// start point's xValue
			p1.x = atof(s2.data());
		}
		if (s1.compare("20") == 0) {
			// start point's yValue
			p1.y = atof(s2.data());
		}
		if (s1.compare("30") == 0) {
			// start point's zValue
			p1.z = atof(s2.data());
		}
		if (s1.compare("11") == 0) {
			// start point's xValue
			 p2.x = atof(s2.data());
		}
		if (s1.compare("21") == 0) {
			// start point's yValue
			p2.y = atof(s2.data());
		}
		if (s1.compare("31") == 0) {
			// start point's zValue
			p2.z = atof(s2.data());
		}
	} while (s1.compare(m_endEntitySym) != 0);
	MyLine line(p1, p2);
	m_shapes->push_back(line.clone());
}
void DxfReader::_ReadPolyLine() {
	std::string s1, s2;
	int nVerticesCnt = 1, // 娩翴计
	    nFetchCnt = 0,    // 璸计ヘ玡ъ翴计
	    nPolyLineFlg = 0;
	Point3D startPnt, endPnt;
	MyPolygon poly;
	do {
		_ReadNext2Lines(s1, s2);
		if (s1.compare("8") == 0) {
			// Layer Name
		}
		if (s1.compare("90") == 0) {
			// Vertices number
			nVerticesCnt = atoi(s2.data());
		}
		if (s1.compare("70") == 0) {
			// polyline flag
			nPolyLineFlg = atoi(s2.data());
			poly.setFlag(nPolyLineFlg);
		}
		if (s1.compare("10") == 0) {
			// New start point is old end point
			endPnt.x = atof(s2.data());
		}
		if (s1.compare("20") == 0) {
			endPnt.y = atof(s2.data());
			///@note AcDbPolyline has no "30" code <== 痻
			++nFetchCnt;
			// more than two point is set
			if (nFetchCnt > 1) {
				// add line (start_point, end_point)
				poly.addVertex(startPnt);
			}
			startPnt = endPnt;
		}
	} while (nFetchCnt < nVerticesCnt); 
	if (nPolyLineFlg == 1) // This is a closed polyline
		poly.addVertex(startPnt);

	m_shapes->push_back(poly.clone());
}
void DxfReader::_ReadCircle() {
	std::string s1, s2;
	Point3D centerPnt;
	double dxRadius = 0.0f,
				 dxStrAngle = 0.0f,
				 dxEndAngle = 360.0f;
	do {
		_ReadNext2Lines(s1, s2);
		if (s1.compare("8") == 0) {
			// Layer Name
		}
		if (s1.compare("10") == 0) {
			// center point's xValue
			centerPnt.x = atof(s2.data());
		}
		if (s1.compare("20") == 0) {
			// center point's yValue
			centerPnt.y = atof(s2.data());
		}
		if (s1.compare("30") == 0) {
			// center point's zValue
			centerPnt.z = atof(s2.data());
		}
		if (s1.compare("40") == 0) {
			// radius
			dxRadius = atof(s2.data());
		}
		if (s1.compare("50") == 0) {
			// Start Angle
			dxStrAngle = atof(s2.data());
		}
		if (s1.compare("51") == 0) {
			// End Angle
			dxEndAngle = atof(s2.data());
		}
	} while (s1.compare(m_endEntitySym) != 0);
	if (dxStrAngle == 0 && dxEndAngle == 360) {
		MyArc circle(centerPnt, 0.0f, 360.0f, dxRadius);
		m_shapes->push_back(circle.clone());
	}
	else {
		MyArc arc(centerPnt, dxStrAngle, dxEndAngle, dxRadius);
		m_shapes->push_back(arc.clone());
	}
}
