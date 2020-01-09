#include <iostream>
#include "SimpleGraph.h"
using namespace std;

int main()
{
	const char* vertices[6] = { "A", "B", "C", "D", "E", "F" };

	SimpleGraph<char*> graph((char**)vertices, 6, true);
	graph.AddEdge("A", "B", 9);
	graph.AddEdge("B", "C", 2);
	graph.AddEdge("A", "C", 12);
	graph.AddEdge("A", "D", 8);
	graph.AddEdge("D", "C", 6);
	graph.AddEdge("A", "F", 11);
	graph.AddEdge("F", "D", 4);
	graph.AddEdge("D", "E", 3);
	graph.AddEdge("E", "C", 7);
	graph.AddEdge("F", "E", 13);

	graph.ChangeKruskalMST();

	graph.ShowGraphInfo();
	graph.ShowEdgeInfo();
}
