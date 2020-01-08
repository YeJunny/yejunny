#include "SimpleGraph.h"
#include <iostream>

SimpleGraph::SimpleGraph(int vertex)
	: mNumVertex(vertex)
{
	mVertices = new LinkedList<EVertex>[vertex];
}

SimpleGraph::~SimpleGraph()
{
	delete[] mVertices;
}

void SimpleGraph::AddEdge(EVertex fromV, EVertex toV)
{
	mVertices[fromV].Insert(toV);
	mVertices[toV].Insert(fromV);
}

void SimpleGraph::ShowGraphInfo()
{
	for (int i = 0; i < mNumVertex; ++i)
	{
		std::cout << static_cast<char>(i + 'A') << " : ";
		EVertex vertex;
		if (mVertices[i].First(&vertex))
		{
			std::cout << static_cast<char>(vertex + 'A') << ' ';
		}
		while (mVertices[i].Next(&vertex))
		{
			std::cout << static_cast<char>(vertex + 'A') << ' ';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}