#pragma once
#ifndef WINVER  // Specifies that the minimum required platform is Windows Vista.
#define WINVER 0x0501 // Change this to the appropriate value to target other versions of Windows.
#endif

#include <afxwin.h>
#include "resource.h"
#include <afxext.h>   // CToolBar & CStatusBar, 載入工具列時使用
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
// 應用程式類別 class MyApp_C
// 視窗程式類別 class MyFrame_C
// 文件類別     class MyDoc_C
//               定義儲存的結構及資料
// 顯示類別     class MyView_C
//               顯示文件類別中的資料
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
		DECLARE_MESSAGE_MAP(); // 宣告此類別有觸發事件
};

// 宣告MyDoc_C為run-time類別
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
	// 事件觸發函數
	// ======================
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_DYNCREATE(MyFrame_C)
	DECLARE_MESSAGE_MAP(); // 宣告此類別有觸發事件
};
IMPLEMENT_DYNCREATE(MyFrame_C, CFrameWnd)

// 事件觸發函數與ID對應表
BEGIN_MESSAGE_MAP(MyFrame_C, CFrameWnd)
	// WINS BUILD-IN
	ON_WM_CREATE()
END_MESSAGE_MAP()

class MyView_C : public CView {
public:
	void OnDraw(CDC *aDC) { }
	// ======================
	// 事件觸發函數
	// ======================
	afx_msg void OnOpen();
	afx_msg void OnExit();

	DECLARE_DYNCREATE(MyView_C)
	DECLARE_MESSAGE_MAP(); // 宣告此類別有觸發事件
};

IMPLEMENT_DYNCREATE(MyView_C, CView)
// 事件觸發函數與ID對應表
BEGIN_MESSAGE_MAP(MyView_C, CView)
	ON_COMMAND(ID_FILE_OPEN, OnOpen)
	ON_COMMAND(ID_FILE_EXIT, OnExit)

END_MESSAGE_MAP()

class MyApp_C : public CWinApp {
public:
	BOOL InitInstance(); // entry point
};
