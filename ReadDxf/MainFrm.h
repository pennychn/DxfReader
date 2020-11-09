#pragma once
#ifndef WINVER  // Specifies that the minimum required platform is Windows Vista.
#define WINVER 0x0501 // Change this to the appropriate value to target other versions of Windows.
#endif

#include <afxwin.h>
#include "resource.h"
#include <afxext.h>   // CToolBar & CStatusBar, ���J�u��C�ɨϥ�
#include <afxtempl.h> // container header file
#include <fstream>
#include <cstdlib>
#include <list>
#include "DxfReader.h"
#include "Shape.h"
#include "Graph.h"
#include "Logger.h" // Debug-used logger

Logger *pLog = new Logger();
// =====================================
// ���ε{�����O class MyApp_C
// �����{�����O class MyFrame_C
// ������O     class MyDoc_C
//               �w�q�x�s�����c�θ��
// ������O     class MyView_C
//               ��ܤ�����O�������
// =====================================
class MyDoc_C : public CDocument {
	public:
		ShapeList m_ShapeList;
		/**
		 * 1. 
		 */
		void printReadData();
		void mergeGraphConstr();

		DECLARE_DYNCREATE(MyDoc_C)
		DECLARE_MESSAGE_MAP(); // �ŧi�����O��Ĳ�o�ƥ�
};

// �ŧiMyDoc_C��run-time���O
IMPLEMENT_DYNCREATE(MyDoc_C, CDocument)
BEGIN_MESSAGE_MAP(MyDoc_C, CDocument)
	// USER DEFINE
END_MESSAGE_MAP()

class MyFrame_C : public CFrameWnd {
protected:
	CMenu *m_pMainMenu; // IDR_MENU1
public:
	CToolBar m_ToolBar;
	MyFrame_C() {};
	~MyFrame_C() {};
	// ======================
	// �ƥ�Ĳ�o���
	// ======================
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_DYNCREATE(MyFrame_C)
	DECLARE_MESSAGE_MAP(); // �ŧi�����O��Ĳ�o�ƥ�
};
IMPLEMENT_DYNCREATE(MyFrame_C, CFrameWnd)

// �ƥ�Ĳ�o��ƻPID������
BEGIN_MESSAGE_MAP(MyFrame_C, CFrameWnd)
	// WINS BUILD-IN
	ON_WM_CREATE()
END_MESSAGE_MAP()

class MyView_C : public CView {
public:
	void OnDraw(CDC *aDC) { }
	// ======================
	// �ƥ�Ĳ�o���
	// ======================
	afx_msg void OnOpen();
	afx_msg void OnExit();

	DECLARE_DYNCREATE(MyView_C)
	DECLARE_MESSAGE_MAP(); // �ŧi�����O��Ĳ�o�ƥ�
};

IMPLEMENT_DYNCREATE(MyView_C, CView)
// �ƥ�Ĳ�o��ƻPID������
BEGIN_MESSAGE_MAP(MyView_C, CView)
	ON_COMMAND(ID_FILE_OPEN, OnOpen)
	ON_COMMAND(ID_FILE_EXIT, OnExit)

END_MESSAGE_MAP()

class MyApp_C : public CWinApp {
public:
	BOOL InitInstance(); // entry point
};
