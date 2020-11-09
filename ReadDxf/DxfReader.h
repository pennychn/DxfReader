#pragma once

#include <fstream>
#include <list>
#include "def.h"
#include "Arc.h"
#include "Dot.h"
#include "Line.h"
#include "Polygon.h"

class DxfReader
{
	public:
		DxfReader(void);
		DxfReader(ShapeList *shapelist);
		~DxfReader(void);
		bool readFile(std::string inFileName);
	private:
		void _ReadNext2Lines(std::string& s1, std::string& s2);
		std::string _ReadNextLine(void);
		std::string _StripWhitespace(const std::string& in);
		void _ReadLine();
		void _ReadPolyLine();
		/// Parsing AcDbCircle and AcDbArc
		void _ReadCircle();
		void _ReadDot();
		/*void _ReadArc();*/
	private:
		std::string m_endEntitySym; // 物件結尾符號
		std::ifstream m_dxf_stream;
		ShapeList* m_shapes;
};
