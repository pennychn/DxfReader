#pragma once

#ifndef GRAPH_H
#define GRAPH_H

#include "Rectangle.h"
#include "Bin.h"

class Edge;
class Graph;

typedef list<Edge *> NeighborEdgeList;
typedef NeighborEdgeList::iterator NeighborEdgeListItr;
typedef NeighborEdgeList::const_iterator NeighborEdgeListcItr;


class Vertex
{
	public:
		// constructor & destructor
		Vertex(void);
		Vertex(MyRectangle* _val);
		Vertex(MyRectangle* _kernel, MyRectangle* _val);
		~Vertex(void) { delete mergedShapes; }
		// accessor
		int getNumEdges() const { return nEdges->size(); }
		void setKernel(MyRectangle* _r) { kernel = _r; }
		MyRectangle* getKernel() const { return kernel; }
		MyRectangle* getKernel() { return kernel; }

		int getID() { return kernel->getID(); }
		// misc
		void addEdge(Edge* );
		void printIDs(); // print all the frame in mergedShapes
		void print(ofstream &);
		bool IsNeighborNode(Vertex *v);
		int  getShapeNum() { return static_cast<int>(mergedShapes->size()); }

		NeighborEdgeListItr getEdgeBegin() { return nEdges->begin(); }
		NeighborEdgeListItr getEdgeEnd() { return nEdges->end(); }

		Edge *getFirstEdge() { return nEdges->front(); }
		list<Shape*> *mergedShapes;
		double weight;
	private:
		/**
		 *  kernel is the minimum rectangle to cover the shape
		 */
		MyRectangle* kernel; // 目前形狀所圍出來的最小矩形
		NeighborEdgeList* nEdges;
	
	friend class Graph;
};

class SortVertexByXInc {
	public:
		bool operator()(const Vertex *v1, const Vertex *v2) const {
			return (v1->getKernel()->getBLX() < v2->getKernel()->getBLX());
		}
};
class SortVertexByWeightInc {
	public:
		bool operator()(const Vertex *v1, const Vertex *v2) const {
			return (v1->weight < v2->weight);
		}
};
class SortVertexByDegDes {
	public:
		bool operator()(const Vertex *v1, const Vertex *v2) const {
			return (v1->getNumEdges() > v2->getNumEdges());
		}
};


typedef list<Vertex* > ConnectedNodeList;
typedef ConnectedNodeList::iterator ConnectedNodeListItr;
typedef vector< list<Vertex *> > RelationShip;
class Edge
{
	public:
		Edge() : m_id(-1) { cNodes = new ConnectedNodeList(); }
		Edge(int _id) : m_id(_id) { cNodes = new ConnectedNodeList(); } 
		~Edge() {} 

		int getID() { return m_id; }
		int getNumberVertices() const { return cNodes->size(); }
		Vertex* getFirstNode() { return cNodes->front(); }

		void addVertex(Vertex *);
		void delVertex(Vertex *);
		bool existVertex(Vertex *);
		Vertex* getOppVert(Vertex* );
		void print(ofstream &);

	private:
		int m_id;
		ConnectedNodeList* cNodes;

	friend class Graph;
};

// graph
typedef vector<Vertex *> GraphVertexVect;
typedef GraphVertexVect::iterator GraphVertexVectItr;
typedef GraphVertexVect::const_iterator GraphVertexVectcItr;

typedef list<Vertex *> GraphVertexList;
typedef GraphVertexList::iterator GraphVertexListItr;
typedef GraphVertexList::const_iterator GraphVertexListcItr;

typedef list<Edge *> GraphEdgeList;
typedef GraphEdgeList::iterator GraphEdgeListItr;
typedef GraphEdgeList::const_iterator GraphEdgeListcItr;

class Graph
{
	public:
		// constructor & copy constructor & destructor
		Graph();
		~Graph() {}
		/** 
		 * 由於對vertices會進行sorting，因此底下的函數
		 * 會不能使用
		 */
		// void addEdgeBetween(int v1, int v2);
		// Edge* getEdgeBetween(int ,int );
		// Edge* getEdgeAt(int position);
		// Vertex* getVertexAt(int);

		// edge operations
		void addEdgeBetween(Vertex *v1, Vertex *v2);
		void addDirectedEdgeBetween(Vertex *v1, Vertex *v2);
		// vertex operations
		void addVertex(Vertex* );
		// searching vertex
		Vertex* getVertexWithRID(int _id);
		int getVertexPosition(Vertex* );

		// misc
		int getNumVertices() { return static_cast<int>(vertices->size()); }
		int getNumEdges()    { return static_cast<int>(edges->size()); }
		void print(ofstream &);
		/*void printIsolatedVect();*/
		bool EdgeDelete(Vertex* ,Vertex* ,Edge* );
		/**
		 * 此函數會移除與v1相鄰的所有連線, 包含相鄰點至v1的連線(bi-direction edge)
		 * 並真正的釋放edge佔用的memory
		 */
		void AllEdgeDelete(Vertex *v1); 
		/**
		 * 僅從vertices容器中移除vertex，但不會delete此點及移除其相鄰的連線
		 * ==> 請確認在執行此函數時，會先執行AllEdgeDelete()
		 * vertices->erase()
		 */
		bool VertexDelete(Vertex *);
		Edge* getEdgeBetween(Vertex* ,Vertex* );
		list<Vertex*> *getCommonList(Vertex* ,Vertex* );

		/**
		 * 找出適當的合併結果
		 * Constraints
		 * 	* 每個vertex代表的frame大小不能超過MIRROR大小(MAX_W  & MAX_H)
		 * Objective
		 * 	* 合併後，vertex總數越少越好
		 * Policy
		 * 	*1 Kernel併Kernel的優先合併
		 * 	*2 Frame併Kernel情況
		 * 	Return
		 * 		* 回傳合併後kernel並轉換成Bin類別
		 */
		void FeasibleShapeSolution(BinList &binList);

		int getFinalDivisionNumber() { return (int)isolatedVertices->size(); }


		GraphVertexVect* getVertices() { return vertices; }
		GraphEdgeList* getEdges() { return edges; }

		GraphVertexVect* isolatedVertices;
	private:
		///@todo 
		/// 測試是否可合併 v1->v2
		bool _IsMergedable(Vertex* v1, Vertex* v2);
		/**
		 * @param v1 找尋合併的節點
		 * @param _id 新節點的編號
		 * @return 合併後新的節點
		 */
		Vertex *_NewMergedVertex(Vertex* v1, int vid);

		GraphVertexVect* vertices;
		GraphEdgeList* edges;
};

#endif
