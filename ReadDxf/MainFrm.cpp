#include "MainFrm.h"
// =========================================
// ���U�O���OMyFrame_C
// =========================================

// =========================================
// ���U�O���OMyFrame_C���ƥ�Ĳ�o���
// =========================================
afx_msg int MyFrame_C::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWnd::OnCreate(lpCreateStruct))
		return 1;

	m_ToolBar.Create(this);
	m_ToolBar.LoadToolBar(IDR_MENU1);
	m_ToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_ToolBar.SetBarStyle( m_ToolBar.GetBarStyle()
		| CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	EnableDocking(CBRS_ALIGN_ANY);
	// �[�J�u��C
	// DockControlBar(&m_ToolBar);
	// �[�J���A�C

	return 0;
}
// =========================================
// ���U�O���OMyDoc_C
// =========================================
void MyDoc_C::printReadData() {
	ShapeListcItr i = m_ShapeList.begin();
	while (i != m_ShapeList.end()) {
		*pLog->logFile() << (*i)->print() << std::endl;
		++i;
	}
}
/**
 * �w��C��shape�إ̤߳j�i�۾F���ϰ�A�z�L�G�Ӭ۾F�ϰ�O�_
 * ��u�X�֡v�t�@�ϰ�
 */
float _min(float x, float y) {
	return (x>y)?y:x;
}
float _max(float x, float y) {
	return (x>y)?x:y;
}
void MyDoc_C::mergeGraphConstr() {
	int objCnt = 1, failObjCnt = 0;
	Graph* adj_graph = new Graph();
	Vertex* new_v = NULL; // each vertex represents the kernel of the shape

  pLog->Log("START Merge Graph Constr.");
	/// @bug Line(300,750)->(550,750)�W�LMIRROW_WIDTH�]���ثeparser�|�����D
	for (ShapeListcItr i = m_ShapeList.begin(); i != m_ShapeList.end(); ++i) {
		new_v = NULL;
		if (typeid(**i) == typeid(MyDot)) {
			MyDot *dot = (MyDot*)(*i);
			*pLog->logFile() << "Dot's Kernal: " << dot->print() << std::endl;
			//Pin *bl = NULL, *tr = NULL;
			/// ���I�զ���kernel
			MyRectangle *_kernel = dot->getKernel();
			_kernel->setID(objCnt++);
			double slack_x = MIRROR_WIDTH  - _kernel->getWidth(),
						 slack_y = MIRROR_HEIGHT - _kernel->getHeight();
			_kernel->setSlackX(slack_x);
			_kernel->setSlackY(slack_y);
			//bl = new Pin(_kernel->getBLX() - slack_x, _kernel->getBLY() - slack_y); 
			//tr = new Pin(_kernel->getTRX() + slack_x, _kernel->getTRY() + slack_y); 
			/// �z�Lkernel�����X���x��frame
			//MyRectangle *_frame  = new MyRectangle(bl, tr);
			/// �[�J�ܹϧθ�
			new_v = new Vertex(_kernel);
			new_v->mergedShapes->push_back(*i);
			adj_graph->addVertex(new_v);
		}
		else if (typeid(**i) == typeid(MyLine)) {
			MyLine *line = (MyLine*)(*i);
			*pLog->logFile() << "Line's Kernel: " << line->print() << std::endl;
			// ���ˬd��seg�O�_�i�ֳQ�@��mirror�]��
			// if true  ==> �إߤ@�ӯx��; if flase ==> ����P���u�q
			//Pin *bl = NULL, *tr = NULL;
			MyRectangle *_kernel = line->getKernel();
			_kernel->setID(objCnt++);
			if (MIRROR_WIDTH  >= _kernel->getWidth() && 
					MIRROR_HEIGHT >= _kernel->getHeight()) {
				// segment is inner a mirror range
				double slack_x = MIRROR_WIDTH  - _kernel->getWidth(),
							 slack_y = MIRROR_HEIGHT - _kernel->getHeight();
				_kernel->setSlackX(slack_x);
				_kernel->setSlackY(slack_y);
				//bl = new Pin(_kernel->getBLX() - slack_x, _kernel->getBLY() - slack_y); 
				//tr = new Pin(_kernel->getTRX() + slack_x, _kernel->getTRY() + slack_y); 
				/// �z�Lkernel�����X���x��frame
				//MyRectangle *_frame  = new MyRectangle(bl, tr);
				/// �[�J�ܹϧθ�
				// new_v = new Vertex(_kernel, _frame);
				new_v = new Vertex(_kernel);
				new_v->mergedShapes->push_back(*i);
				adj_graph->addVertex(new_v);
			} 
			else {
				// segment is larger than a mirror range
				*pLog->errFile() << "Currently unconsidering splitting LINE cases" << std::endl
					               << line->print() << endl;
				++failObjCnt;
			}
		}
		else if (typeid(**i) == typeid(MyArc)) {
			MyArc *arc = (MyArc*)(*i);
			*pLog->logFile() << "Arc's Kernel: " << arc->print();
			//Pin *bl = NULL, *tr = NULL;
			MyRectangle *_kernel = arc->getKernel();
			_kernel->setID(objCnt++);
			if (MIRROR_WIDTH  >= _kernel->getWidth() && 
					MIRROR_HEIGHT >= _kernel->getHeight()) {
				// segment is inner a mirror range
				double slack_x = MIRROR_WIDTH  - _kernel->getWidth(),
							 slack_y = MIRROR_HEIGHT - _kernel->getHeight();
				_kernel->setSlackX(slack_x);
				_kernel->setSlackY(slack_y);
				//bl = new Pin(_kernel->getBLX() - slack_x, _kernel->getBLY() - slack_y); 
				//tr = new Pin(_kernel->getTRX() + slack_x, _kernel->getTRY() + slack_y); 
				/// �z�Lkernel�����X���x��frame
				//MyRectangle *_frame  = new MyRectangle(bl, tr);
				/// �[�J�ܹϧθ�
				// new_v = new Vertex(_kernel, _frame);
				new_v = new Vertex(_kernel);
				new_v->mergedShapes->push_back(*i);
				adj_graph->addVertex(new_v);
			} 
			else {
				// segment is larger than a mirror range
				*pLog->errFile() << "Currently unconsidering splitting Arc cases" << std::endl;
				++failObjCnt;
			}
		}
		else if (typeid(**i) == typeid(MyPolygon)) {
			MyPolygon *poly = (MyPolygon*)(*i);
			*pLog->logFile() << "Polygon's Kernel: " << poly->print() << std::endl;
			//Pin *bl = NULL, *tr = NULL;
			MyRectangle *_kernel = poly->getKernel();
			_kernel->setID(objCnt++);
			if (MIRROR_WIDTH  >= _kernel->getWidth() && 
					MIRROR_HEIGHT >= _kernel->getHeight()) {
				// segment is inner a mirror range
				double slack_x = MIRROR_WIDTH  - _kernel->getWidth(),
							 slack_y = MIRROR_HEIGHT - _kernel->getHeight();
				_kernel->setSlackX(slack_x);
				_kernel->setSlackY(slack_y);
				//bl = new Pin(_kernel->getBLX() - slack_x, _kernel->getBLY() - slack_y); 
				//tr = new Pin(_kernel->getTRX() + slack_x, _kernel->getTRY() + slack_y); 
				/// �z�Lkernel�����X���x��frame
				//MyRectangle *_frame  = new MyRectangle(bl, tr);
				/// �[�J�ܹϧθ�
				// new_v = new Vertex(_kernel, _frame);
				new_v = new Vertex(_kernel);
				new_v->mergedShapes->push_back(*i);
				adj_graph->addVertex(new_v);
			} 
			else {
				// segment is larger than a mirror range
				*pLog->errFile() << "Currently unconsidering splitting Polygon cases" << std::endl;
				++failObjCnt;
			}
		}
		else {
			*pLog->errFile() << "Shape Type is unexcepted type\n";
			++failObjCnt;
		}
	}
  pLog->Log("End of Merge Graph Constr.");
	BinList binList;
	pLog->Log("Starting construct adjacent relation");
	adj_graph->FeasibleShapeSolution(binList);
	*pLog->logFile() << "Total objects are " << adj_graph->getFinalDivisionNumber() << endl;
	*pLog->logFile() << "Fail to recognized objects are " << failObjCnt << endl;
	//adj_graph->print(*pLog->logFile());
	pLog->Log("End of construct adjacent relation");
	for (binListcItr i = binList.begin(); i != binList.end(); ++i) {
		Bin *b = *i;
		b->print();
	}

	// pLog->Log("Starting merging");
}
// =========================================
// ���U�O���OMyView_C���ƥ�Ĳ�o���
// =========================================
afx_msg void MyView_C::OnOpen() {
	MyDoc_C *pDoc = (MyDoc_C*)GetDocument();
	DxfReader reader(&(pDoc->m_ShapeList));
	std::string _file = "example01.dxf";
	if (false == reader.readFile(_file)) {
		MessageBox("Open File failed");
	}
	// Verification the read data
	pDoc->printReadData();
	pDoc->mergeGraphConstr();

}
afx_msg void MyView_C::OnExit() {
	MessageBox("Exit");
}
// =========================================
// ���U�O���OMyApp_C
// =========================================
// �бN�ҥH�D�{���X�g��InitInstance()
BOOL MyApp_C::InitInstance() // entry point
{
	// 1. ����˪��إ�
	CSingleDocTemplate *pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MENU1, // ����������Ω�Main Menu
		RUNTIME_CLASS(MyDoc_C), 
		RUNTIME_CLASS(MyFrame_C),
		RUNTIME_CLASS(MyView_C));
	AddDocTemplate(pDocTemplate); // �N�˪��[�JMyApp_C
	// 2. �إߤ��
	CDocument *pDoc = pDocTemplate->CreateNewDocument();
	// 3. �إ߮ج[
	//    m_pMainWnd�~�Ӧ�CWinApp
	m_pMainWnd = pDocTemplate->CreateNewFrame(pDoc, NULL);
	// 4. ��l�Ʈج[�ós��MyView_C
	pDocTemplate->InitialUpdateFrame(
		(CFrameWnd*)m_pMainWnd, pDoc);
	// 5. ��ܵ���
	//    �N���ε{�����V����������
	m_pMainWnd->ShowWindow(SW_SHOW);
	return true;
}

// ================================
// ����{�����a��
// ================================
MyApp_C run_app;
