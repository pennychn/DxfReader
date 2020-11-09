#include "Graph.h"

// ======================= VERTEX section ===========================
Vertex::Vertex(void) {
	kernel = NULL;
	nEdges = new NeighborEdgeList();
	mergedShapes = new list<Shape*>(); 
	weight = 0;
}
Vertex::Vertex(MyRectangle* _val) {
	kernel = _val;
	nEdges = new NeighborEdgeList();
	mergedShapes = new list<Shape*>(); 
	weight = 0;
}
Vertex::Vertex(MyRectangle* _kernel, MyRectangle* _val) {
	kernel = _kernel;
	if (kernel == NULL)
		*pLog->errFile() << "Vertex construction failed due to null kernel\n";
	nEdges = new NeighborEdgeList();
	mergedShapes = new list<Shape*>(); 
	weight = 0;
}
void Vertex::addEdge(Edge* e) {
	nEdges->push_back(e);
}
void Vertex::printIDs() {
	list<Shape*>::const_iterator i = mergedShapes->begin();
	while (i != mergedShapes->end()) {
		*pLog->logFile() << (*i)->print() << " ";
		++i;
	}
	*pLog->logFile() << endl;
}
void Vertex::print(ofstream &file) {
#ifdef DEBUG
	int counter = 0;
	file << "v(" << getID() << ")" << " degree = " 
		   << nEdges->size() << endl;
//	file << "Frame: \n"; 
//	this->getFrame()->print(file); 
	file << "Kernel: \n"; 
	this->getKernel()->print(file); 
	for (NeighborEdgeListItr i = nEdges->begin(); i != nEdges->end(); ++i) {
		++counter;
		file << "e" << counter << "  connected to: " 
			   << ((*i)->getOppVert(this))->getID() << std::endl;
	}
	file << "======================================\n";
	file << "Shapes are represented by this v\n"
		   << "======================================\n";
	list<Shape*>::const_iterator mcItr = mergedShapes->begin();
	while (mcItr != mergedShapes->end()) {
		file << "\t" << (*mcItr)->print() << std::endl;
		++mcItr;
	}
	file << "======================================\n";
#endif
}
bool Vertex::IsNeighborNode(Vertex *v) {
	NeighborEdgeListItr i = nEdges->begin();
	while (i != nEdges->end()) {
		Vertex *neighbor = (*i)->getOppVert(this);
		if (neighbor == v) {
			return true;
		}
		++i;
	}
	return false;
}
// ======================= EDGE section ===========================
void Edge::addVertex(Vertex* v) {
	cNodes->push_back(v);
}
void Edge::delVertex(Vertex* v) {
	for (ConnectedNodeListItr i = cNodes->begin(); i != cNodes->end(); ++i) {
		if ((*i) == v) {
			cNodes->erase(i);
			break;
		}
	}
}
bool Edge::existVertex(Vertex* v) {
	for (ConnectedNodeListItr i = cNodes->begin(); i != cNodes->end(); ++i) {
		if ((*i) == v)
			return true;
	}
	return false;
}
Vertex* Edge::getOppVert(Vertex* v) {
	// 假設net為two-pin net
	if (cNodes->size() > 2) {
		*pLog->errFile() << "\tNet " << getID() 
			                 << " is not two-pin nets, net nums = " 
											 << cNodes->size() << endl;
	}
	else {
		// 找到此edge上相連之verteices(假設最多為2)中與v不相同者
		for (ConnectedNodeListItr i = cNodes->begin(); 
				 i != cNodes->end(); ++i) {
			if ((*i) != v) {
				return (*i);
			}
		}
	}
	pLog->ErrLog(pLog->LOG_LEVEL_FATAL, "getOppVert() failed!!");
	return NULL;
}
void Edge::print(ofstream &file) {
#ifdef DEBUG
	file << this->getID() << " has connecting nodes =  " << cNodes->size() << endl;
#endif
}
// ======================= GRAPH section ===========================
Graph::Graph() {
	vertices = new GraphVertexVect();
	edges = new GraphEdgeList();
	isolatedVertices = new GraphVertexVect();
}

// edge operations
void Graph::addEdgeBetween(Vertex *v1, Vertex *v2) {
  Edge *e = new Edge();
  edges->push_back(e);

  e->addVertex(v1);
  e->addVertex(v2);

  v1->addEdge(e);
  v2->addEdge(e);
}
void Graph::addDirectedEdgeBetween(Vertex *v1, Vertex *v2)
{
  Edge *e = new Edge();
  edges->push_front(e);

  e->addVertex(v1);
  e->addVertex(v2);

  v1->addEdge(e);
}
// vertex operations
void Graph::addVertex(Vertex *v) {
  vertices->push_back(v);
}
int Graph::getVertexPosition(Vertex *vert) {
	int position = 0;
	for(GraphVertexVectItr i = vertices->begin(); i != vertices->end(); i++) {
		if((*i) == vert)
			return position;
		position++;
	}
	*pLog->errFile() << "Vertex (" << vert->getID() << ") is not found in graph" << endl;
	return -1;
}
Vertex* Graph::getVertexWithRID(int _id) {
	for(GraphVertexVectItr i = vertices->begin(); i != vertices->end(); i++) {
		if((*i)->getID() == _id)
			return *i;
	}
	*pLog->errFile() << "RID (" << _id << ") is not found in graph" << endl;
	return NULL;
}
// misc
void Graph::print(ofstream &file) {
#ifdef DEBUG
	for(GraphVertexVectItr i = vertices->begin(); 
			i != vertices->end(); i++)
	{
		(*i)->print(file);
		file << endl;
	}
#endif
}
bool Graph::EdgeDelete(Vertex *v1, Vertex *v2, Edge *e) {
	NeighborEdgeListItr edgeInV1 = find(v1->nEdges->begin(), v1->nEdges->end(), e);
	NeighborEdgeListItr edgeInV2 = find(v2->nEdges->begin(), v2->nEdges->end(), e);
	if (edgeInV1 != v1->nEdges->end() && edgeInV2 != v2->nEdges->end())
	{
		v1->nEdges->remove(e);
		v2->nEdges->remove(e);
		edges->remove(e);
		e->cNodes->clear();
		//delete e;
	}
	else {
#ifdef DEBUG
		*pLog->errFile() << "Edge "<< e->getID() 
			                 << " delete failed. Between " << v1->getID()
			                 << " and " << v2->getID() << endl;
#endif
		return false;
	}
//#ifdef DEBUG
	//pLog->Log(pLog->LOG_LEVEL_INFO_HEAD, "EdgeDelete Success!!");
//#endif
	return true;
}

void Graph::AllEdgeDelete(Vertex *v1) {
	for (NeighborEdgeListcItr eItr = v1->nEdges->begin(); eItr != v1->nEdges->end(); ++eItr) {
		Vertex *v = (*eItr)->getOppVert(v1);
		v->nEdges->remove(*eItr);
		// this->edges->remove(*eItr);
		delete *eItr;
	}
	v1->nEdges->clear();
}
bool Graph::VertexDelete(Vertex *v) {
	// 移除v1, v2對e之關係
  GraphVertexVectItr delV = find(vertices->begin(), vertices->end(), v);
	if (delV != vertices->end()) {
		vertices->erase(delV);
	}
	else
		return false;
	return true;
}
Edge * Graph::getEdgeBetween(Vertex* v1, Vertex* v2) {
	for(NeighborEdgeListItr i = v1->nEdges->begin(); i != v1->nEdges->end(); i++)
	{
		Vertex *v = (*i)->getOppVert(v1);
		if (v == v2)
			return (*i);
		//Edge* e = (*i);
		//if (e->existVertex(v2))
			//return e;
	}
	*pLog->errFile() << "v1 = " << v1->getID() << " v2 = " << v2->getID() << endl;
	*pLog->errFile() << " Not found edge between v1 and v2 " << endl;
	return NULL;
}
list<Vertex*> *Graph::getCommonList(Vertex* v1,Vertex* v2) {
	list<Vertex*> *comList = new list<Vertex*>();
	
	if (true == this->EdgeDelete(v1, v2, this->getEdgeBetween(v1, v2))) {
		NeighborEdgeListItr i = v1->getEdgeBegin();
		while (i != v1->getEdgeEnd()) {
			Vertex *neighbor = (*i)->getOppVert(v1);
			//neighbor->print(*pLog->logFile());
			if (true == v2->IsNeighborNode(neighbor)) {
				comList->push_back(neighbor);
			}
			++i;
		}
		this->AllEdgeDelete(v1);
		this->AllEdgeDelete(v2);
	}
	return comList;
}
bool Graph::_IsMergedable(Vertex* v1, Vertex* v2) {
	return v1->getKernel()->IsCovering( v2->getKernel() );
}
Vertex *Graph::_NewMergedVertex(Vertex* v1, int vid) {
	*pLog->logFile() << "Start merge any vertex into v(" << v1->getID() << ")" << std::endl;
	Vertex *ret = NULL;
	//double maxW = v1->getFrame()->getWidth(),
				 //maxH = v1->getFrame()->getHeight();
	double maxW = v1->getKernel()->getSlackX(),
				 maxH = v1->getKernel()->getSlackY();
	*pLog->logFile() << "maxW = " << maxW <<  ", "
									 << "maxH = " << maxH <<  endl;
	// merge list: 先挑出可以合併的vertices(即與v1相鄰者)
	GraphVertexList candidateList, selectedList;
	for(NeighborEdgeListItr i = v1->nEdges->begin(); i != v1->nEdges->end(); ++i) {
		Vertex *v = (*i)->getOppVert(v1);
		if (NULL == v) {
			*pLog->errFile() << "Shit! You have a null neighboring...Node v\n";
			v1->print(*pLog->errFile());
		}
		else {
			candidateList.push_back(v);
			v->weight = fabs(v1->getKernel()->getCenterX() - v->getKernel()->getCenterX()) +
				          fabs(v1->getKernel()->getCenterY() - v->getKernel()->getCenterY()); 
		}
	}
	// 先合併距離較近者
	candidateList.sort(SortVertexByWeightInc());
	double curr_blX = v1->getKernel()->getBLX(), curr_blY = v1->getKernel()->getBLY(),
				 curr_trX = v1->getKernel()->getTRX(), curr_trY = v1->getKernel()->getTRY();
	*pLog->logFile() << "Kernel boundary" << endl;
	*pLog->logFile() << "bl = (" << curr_blX <<  ", " << curr_blY << ") "
	                 << "tr = (" << curr_trX <<  ", " << curr_trY << ")" << endl;
									 
	int mergedVertexNum = (int)candidateList.size();
	while (0 != candidateList.size() && 0 != mergedVertexNum) {
		mergedVertexNum = 0;
		// Once, the mergedVertexNum is not chaged, the merging process will be stopped.
		for (GraphVertexListItr i = candidateList.begin(); i != candidateList.end(); ) {
			*pLog->logFile() << "\tSelecting vertex " << (*i)->getID() << endl;
			double _minX = (curr_blX < (*i)->getKernel()->getBLX())?curr_blX:(*i)->getKernel()->getBLX();
			double _minY = (curr_blY < (*i)->getKernel()->getBLY())?curr_blY:(*i)->getKernel()->getBLY();
			double _maxX = (curr_trX > (*i)->getKernel()->getTRX())?curr_trX:(*i)->getKernel()->getTRX();
			double _maxY = (curr_trY > (*i)->getKernel()->getTRY())?curr_trY:(*i)->getKernel()->getTRY();
			*pLog->logFile() << "\t\tmin = (" << _minX <<  ", " << _minY << ") "
											 << "max = (" << _maxX <<  ", " << _maxY << ")" << endl;
			if ((_maxX - _minX) <= maxW && (_maxY - _minY) <= maxH
					&& (_maxX - _minX) >= 0 && (_maxY - _minY) >= 0) {
				curr_blX = _minX; curr_blY = _minY; curr_trX = _maxX; curr_trY = _maxY;
#ifdef DEBUG
				*pLog->logFile() << "\tAfter selecting vertex " << (*i)->getID() << endl;
				*pLog->logFile() << "\t\tbottom left = (" << (*i)->getKernel()->getBLX() <<  ", " << (*i)->getKernel()->getBLY() << ") "
												 << "top right= (" << (*i)->getKernel()->getTRX() <<  ", " << (*i)->getKernel()->getTRY() << ")" << endl;
				*pLog->logFile() << "\t\tbottom left = (" << curr_blX <<  ", " << curr_blY << ") "
												 << "top right= (" << curr_trX <<  ", " << curr_trY << ")" << endl;
#endif
				selectedList.push_back(*i);
				i = candidateList.erase(i); 
				++mergedVertexNum;
			}
			else {
				++i;
			}
		}
	}
	*pLog->logFile() << "Selection done" << endl;
	// 建立以final大小為基準所形成的矩形
	//  + 必須有挑選到時才會執行
	Pin *f_bl = NULL, *f_tr = NULL;
	if (0 != selectedList.size()) {
		MyRectangle *_kernel = new MyRectangle(new Pin(curr_blX, curr_blY), new Pin(curr_trX, curr_trY));
		_kernel->setID(vid);
		//double slack_x = maxW - _kernel->getWidth(),
					 //slack_y = maxH - _kernel->getHeight();
		//f_bl = new Pin(_kernel->getBLX() - slack_x, _kernel->getBLY() - slack_y); 
		//f_tr = new Pin(_kernel->getTRX() + slack_x, _kernel->getTRY() + slack_y); 
		//MyRectangle *_frame = new MyRectangle(f_bl, f_tr);
		//ret = new Vertex(_kernel, _frame);
		ret = new Vertex(_kernel);
		for (GraphVertexListItr i = selectedList.begin(); i != selectedList.end(); ++i) {
			Vertex *v = *i;
			this->AllEdgeDelete(v);
			this->VertexDelete(v);
			ret->mergedShapes->merge(*(v->mergedShapes));
		}
		this->AllEdgeDelete(v1);
		this->VertexDelete(v1); 
		ret->mergedShapes->merge(*(v1->mergedShapes));
		//this->addVertex(ret); // 將新合併之節點加入並沒有用
#ifdef DEBUG
		*pLog->logFile() << "==== Final merged rectangle ====" << endl;
		ret->print(*pLog->logFile());
		*pLog->logFile() << endl;
#endif
	}
	*pLog->logFile() << "End merging process for v(" << v1->getID() << ")" << std::endl;
	return ret;
}
void Graph::FeasibleShapeSolution(BinList &binList) {
	pLog->Log("START -- FeasibleShapeSolution");
	// 建立相鄰連線(可合併)
	std::sort(vertices->begin(),vertices->end(),SortVertexByXInc());
	GraphVertexVectItr vcItr = vertices->begin(); 
	while (vcItr != vertices->end()) {
		Vertex* v1 = *vcItr;
		++vcItr;
		for(GraphVertexVectItr j = vcItr; j != vertices->end(); ++j) {
			Vertex *v2 = *j;
			if (true == this->_IsMergedable(v1, v2)) {
				this->addEdgeBetween(v1, v2);
			}
		}
	}
#ifdef DEBUG
	*pLog->logFile() << "Start -- Adjacent relation constr.\n";
	this->print(*pLog->logFile()); // 驗證連線是否正常
	*pLog->logFile() << "End -- Adjacent relation constr.\n";
#endif
	/**
	 * @todo 2010/3/22
	 *  + 從目前的連線關係中，找出degree最大者先取，並存至isolated list中
	 */
	std::sort(vertices->begin(),vertices->end(),SortVertexByDegDes());
	Vertex *currV = NULL, *newV = NULL;
	int lastVNum = this->getNumVertices(); 
	while (0 != this->getNumVertices()) {
		// 0. pick curr node with max. degree
		vcItr = this->getVertices()->begin(); 
		currV = *vcItr;
#ifdef DEBUG
		*pLog->logFile() << "==========currV(" << currV->getID() << ")============\n";
		//currV->print(*pLog->logFile());
#endif
		// 1. currV picks the neighborV with min. degree
		//    * 與相鄰的vertex均合併在一起
		//    	+ 檢查合併後的位置是滿足MIRROR Size的要求
		if (0 != currV->getNumEdges()) {
			// In following function, currV must be remove from vertices
			newV = this->_NewMergedVertex(currV, lastVNum);
			if (NULL != newV) {
				++lastVNum; 
				isolatedVertices->push_back(newV);
			}
			else {
				// 有相鄰卻不能合併的情況
				// 	* 合併後大小超過MIRROR SIZE
				pLog->Log("Merging failed due to the over-sized construction.");
				currV->print(*pLog->logFile());
				///@bug 發生無法合併的情況，暫定將此點直接移至isolatedVertices
				/// 理論上，應該再討論其它點對此點的情況(因為在合併時只有directed edge)
				this->AllEdgeDelete(currV);
				this->VertexDelete(currV); // 從graph中移至isolatedVertices
				isolatedVertices->push_back(currV);
			}
		}
		else {
			// while currV w/o any neighbor
			this->VertexDelete(currV); // 從graph中移至isolatedVertices
			isolatedVertices->push_back(currV);
			*pLog->logFile() << "Without merging processing\n";
		}
	} // O(n) processing
	*pLog->logFile() << "===========================\n";
	pLog->Log("END -- FeasibleShapeSolution");
	for(GraphVertexVectItr i = isolatedVertices->begin(); i != isolatedVertices->end(); ++i) {
		Vertex *v = *i;
		MyRectangle *rect = v->getKernel();
		Bin *b = new Bin(rect->getWidth(), rect->getHeight(), rect->getBLX(), rect->getBLY(), 0);
		for (list<Shape*>::iterator sItr = v->mergedShapes->begin(); 
				 sItr != v->mergedShapes->end(); ++sItr) {
			b->AddShape(*sItr);
		}
		binList.push_back(b);
		delete *i;
	}
	isolatedVertices->clear();
	// this->printIsolatedVect();	
}
/*
void Graph::addEdgeBetween(int v1, int v2) {
  Edge *e = new Edge();
  edges->push_back(e);

  e->addVertex((*vertices)[v1]);
  e->addVertex((*vertices)[v2]);

  (*vertices)[v1]->addEdge(e);
  (*vertices)[v2]->addEdge(e);
}
Vertex * Graph::getVertexAt(int position) {
	if(position < getNumVertices() && position >= 0)
		return (*vertices)[position];
	else
	{
#ifdef DEBUG
		*pLog->errFile() << "Invalid position number.  Returning NULL" << endl;
#endif
		return NULL;
	}
}
Edge * Graph::getEdgeBetween(int p1, int p2) {
	Vertex *v1 = getVertexAt(p1);
	Vertex *v2 = getVertexAt(p2);
	for(NeighborEdgeListItr i = v1->nEdges->begin(); i != v1->nEdges->end(); i++)
	{
		Vertex *v = (*i)->getOppVert(v1);
		if (v == v2)
			return (*i);
		//Edge* e = (*i);
		//if (e->existVertex(v2))
			//return e;
	}
	*pLog->errFile() << "v1 = " << v1->getID() << " v2 = " << v2->getID() << endl;
	*pLog->errFile() << " Not found edge between v1 and v2 " << endl;
	return NULL;
}
Edge * Graph::getEdgeAt(int position) {
	int counter = 0;
	Edge * returnEdge = NULL;
	for(GraphEdgeListItr i = edges->begin(); i != edges->end(); i++)
	{
		if(counter == position)
			returnEdge = *i;
		counter++;
	}
	return returnEdge;
}
*/
