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
		MyRectangle* kernel; // �ثe�Ϊ��ҳ�X�Ӫ��̤p�x��
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
		 * �ѩ��vertices�|�i��sorting�A�]�����U�����
		 * �|����ϥ�
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
		 * ����Ʒ|�����Pv1�۾F���Ҧ��s�u, �]�t�۾F�I��v1���s�u(bi-direction edge)
		 * �ïu��������edge���Ϊ�memory
		 */
		void AllEdgeDelete(Vertex *v1); 
		/**
		 * �ȱqvertices�e��������vertex�A�����|delete���I�β�����۾F���s�u
		 * ==> �нT�{�b���榹��ƮɡA�|������AllEdgeDelete()
		 * vertices->erase()
		 */
		bool VertexDelete(Vertex *);
		Edge* getEdgeBetween(Vertex* ,Vertex* );
		list<Vertex*> *getCommonList(Vertex* ,Vertex* );

		/**
		 * ��X�A���X�ֵ��G
		 * Constraints
		 * 	* �C��vertex�N��frame�j�p����W�LMIRROR�j�p(MAX_W  & MAX_H)
		 * Objective
		 * 	* �X�֫�Avertex�`�ƶV�ֶV�n
		 * Policy
		 * 	*1 Kernel��Kernel���u���X��
		 * 	*2 Frame��Kernel���p
		 * 	Return
		 * 		* �^�ǦX�֫�kernel���ഫ��Bin���O
		 */
		void FeasibleShapeSolution(BinList &binList);

		int getFinalDivisionNumber() { return (int)isolatedVertices->size(); }


		GraphVertexVect* getVertices() { return vertices; }
		GraphEdgeList* getEdges() { return edges; }

		GraphVertexVect* isolatedVertices;
	private:
		///@todo 
		/// ���լO�_�i�X�� v1->v2
		bool _IsMergedable(Vertex* v1, Vertex* v2);
		/**
		 * @param v1 ��M�X�֪��`�I
		 * @param _id �s�`�I���s��
		 * @return �X�֫�s���`�I
		 */
		Vertex *_NewMergedVertex(Vertex* v1, int vid);

		GraphVertexVect* vertices;
		GraphEdgeList* edges;
};

#endif
