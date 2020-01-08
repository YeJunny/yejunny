#include "SimpleGraph.h"

SimpleGraph::SimpleGraph(int vertex)
{
	mVertices = new LinkedList<EVertex>[vertex];
}

SimpleGraph::~SimpleGraph()
{
	delete[] mVertices;
}

void SimpleGraph::AddEdge(EVertex fromV, EVertex toV)
{

}

void SimpleGraph::ShowGraphInfo()
{

}