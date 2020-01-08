#include <iostream>
#include "SimpleGraph.h"
using namespace std;

int main()
{
	SimpleGraph graph(5);
	graph.AddEdge(EVertex::A, EVertex::B);
	graph.AddEdge(EVertex::A, EVertex::C);
	graph.AddEdge(EVertex::A, EVertex::D);
	graph.AddEdge(EVertex::B, EVertex::D);
	graph.AddEdge(EVertex::C, EVertex::B);
	graph.AddEdge(EVertex::E, EVertex::C);

	graph.ShowGraphInfo();

}
