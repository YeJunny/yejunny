#pragma once

#include "LinkedList.h"

enum EVertex { A, B, C, D, E, F, G };

class SimpleGraph
{
public:
	SimpleGraph(int vertex);
	~SimpleGraph();

	void AddEdge(EVertex fromV, EVertex toV);
	void ShowGraphInfo();

private:
	LinkedList<EVertex>* mVertices;
};

