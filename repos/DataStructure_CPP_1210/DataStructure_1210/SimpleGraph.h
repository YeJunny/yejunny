#pragma once

#include "LinkedList.h"
#include "SimpleHeap.h"
#include "SimpleQueue.h"
#include <cstring>
#include <vector>

template <typename TVertex>
struct Edge
{
	TVertex VStart;
	TVertex VEnd;
	int Weight;
};

template <typename TVertex>
int GetPriority(Edge<TVertex> e1, Edge<TVertex> e2)
{
	return e1.Weight - e2.Weight;
}

template <typename TVertex>
class SimpleGraph
{
public:
	SimpleGraph(TVertex* vertexList, const int numVertex, const bool bHasWeight = false)
		: mNumVertex(numVertex)
		, mVertexList(vertexList)
		, mWeights(nullptr)
	{
		mVertices = new LinkedList<TVertex>[numVertex];
		mEdges.reserve(numVertex * 2);
		if (bHasWeight)
		{
			mWeights = new SimpleHeap<Edge<TVertex>>();
			mWeights->Init(GetPriority, true);
		}
	}

	~SimpleGraph()
	{
		delete[] mVertices;
		if (mWeights)
		{
			delete mWeights;
		}
	}

	void AddEdge(const TVertex fromV, const TVertex toV)
	{
		mVertices[GetIndexByVertex(fromV)].Insert(toV);
		mVertices[GetIndexByVertex(toV)].Insert(fromV);
		mNumEdge++;
	}

	void AddEdge(const TVertex fromV, const TVertex toV, int weight)
	{
		mVertices[GetIndexByVertex(fromV)].Insert(toV);
		mVertices[GetIndexByVertex(toV)].Insert(fromV);
		mNumEdge++;

		Edge<TVertex> edge = { fromV, toV, weight };
		mWeights->Insert(edge);
		mEdges.push_back(edge);
	}

	void ChangeKruskalMST()
	{
		mEdges.clear();
		mbIsVisit = new bool[mNumVertex];
		Edge<TVertex> edge;
		while (mWeights->Remove(&edge))
		{
			if (IsArriveNode(edge.VStart, edge.VEnd))
			{
				RemoveEdge(edge.VStart, edge.VEnd);
				mNumEdge--;
			}
			else
			{
				mEdges.push_back(edge);
			}

			if (mNumEdge + 1 == mNumVertex)
			{
				while (mWeights->Remove(&edge))
				{
					mEdges.push_back(edge);
				}
				break;
			}
		}
		delete[] mbIsVisit;
	}

	void ShowGraphInfo() const
	{
		TVertex vertex;

		for (int i = 0; i < mNumVertex; ++i)
		{
			std::cout << mVertexList[i] << " : ";
			if (mVertices[i].First(&vertex))
			{
				std::cout << vertex << ' ';
			}
			while (mVertices[i].Next(&vertex))
			{
				std::cout << vertex << ' ';
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	void ShowEdgeInfo() const
	{
		for (int i = 0; i < mNumEdge; ++i)
		{
			std::cout << "(" << mEdges[i].VStart << "-" << mEdges[i].VEnd
				<< "), w: " << mEdges[i].Weight << std::endl;
		}
	}

protected:
	int GetIndexByVertex(const TVertex vertex) const
	{
		for (int i = 0; i < mNumVertex; ++i)
		{
			if (vertex == mVertexList[i])
			{
				return i;
			}
		}

		return -1;
	}

	void RemoveEdge(const TVertex v1, const TVertex v2)
	{
		int index = GetIndexByVertex(v1);
		TVertex vertex;
		if (mVertices[index].First(&vertex))
		{
			if (vertex == v2)
			{
				mVertices[index].Remove();
			}
			else
			{
				while (mVertices[index].Next(&vertex))
				{
					if (vertex == v2)
					{
						mVertices[index].Remove();
						break;
					}
				}
			}

			index = GetIndexByVertex(v2);
			if (mVertices[index].First(&vertex))
			{
				if (vertex == v1)
				{
					mVertices[index].Remove();
				}
				else
				{
					while (mVertices[index].Next(&vertex))
					{
						if (vertex == v1)
						{
							mVertices[index].Remove();
							break;
						}
					}
				}
			}
		}
	}

	bool IsArriveNode(const TVertex start, const TVertex end)
	{
		for (int i = 0; i < mNumVertex; ++i)
		{
			mbIsVisit[i] = false;
		}

		int vertexIndex = GetIndexByVertex(start);

		mbIsVisit[vertexIndex] = true;

		SimpleQueue<TVertex> queue;
		TVertex outVertex;
		if (mVertices[vertexIndex].First(&outVertex))
		{
			if (!mbIsVisit[GetIndexByVertex(outVertex)] && outVertex != end)
			{
				queue.Push(outVertex);
			}

			while (mVertices[vertexIndex].Next(&outVertex))
			{
				if (!mbIsVisit[GetIndexByVertex(outVertex)] && outVertex != end)
				{
					queue.Push(outVertex);
				}
			}
		}

		while (queue.Pop(&outVertex))
		{
			if (SearchTargetNodeBFSRecursive(outVertex, end))
			{
				return true;
			}
		}

		return false;
	}

	bool SearchTargetNodeBFSRecursive(const TVertex start, const TVertex end)
	{
		if (end == start)
		{
			return true;
		}

		int vertexIndex = GetIndexByVertex(start);

		if (!mbIsVisit[vertexIndex])
		{
			mbIsVisit[vertexIndex] = true;
		}

		SimpleQueue<TVertex> queue;
		TVertex outVertex;
		if (mVertices[vertexIndex].First(&outVertex))
		{
			if (!mbIsVisit[GetIndexByVertex(outVertex)])
			{
				queue.Push(outVertex);
			}

			while (mVertices[vertexIndex].Next(&outVertex))
			{
				if (!mbIsVisit[GetIndexByVertex(outVertex)])
				{
					queue.Push(outVertex);
				}
			}
		}

		while (queue.Pop(&outVertex))
		{
			if (SearchTargetNodeBFSRecursive(outVertex, end))
			{
				return true;
			}
		}

		return false;
	}

	TVertex* mVertexList;
	LinkedList<TVertex>* mVertices;
	SimpleHeap<Edge<TVertex>>* mWeights;
	std::vector<Edge<TVertex>> mEdges;
	bool* mbIsVisit;
	int mNumVertex;
	int mNumEdge;
};


template <>
class SimpleGraph<char*>
{
public:
	SimpleGraph(char** vertexList, const int numVertex, const bool bHasWeight = false)
		: mNumVertex(numVertex)
		, mVertexList(vertexList)
		, mWeights(nullptr)
	{
		mVertices = new LinkedList<char*>[numVertex];
		mEdges.reserve(numVertex * 2);
		if (bHasWeight)
		{
			mWeights = new SimpleHeap<Edge<char*>>();
			mWeights->Init(GetPriority, true);
		}
	}

	~SimpleGraph()
	{
		delete[] mVertices;
		if (mWeights)
		{
			delete mWeights;
		}
	}

	void AddEdge(const char* fromV, const char* toV)
	{
		mVertices[GetIndexByVertex(fromV)].Insert(const_cast<char*>(toV));
		mVertices[GetIndexByVertex(toV)].Insert(const_cast<char*>(fromV));
		mNumEdge++;
	}

	void AddEdge(const char* fromV, const char* toV, int weight)
	{
		mVertices[GetIndexByVertex(fromV)].Insert(const_cast<char*>(toV));
		mVertices[GetIndexByVertex(toV)].Insert(const_cast<char*>(fromV));
		mNumEdge++;

		Edge<char*> edge = { const_cast<char*>(fromV), const_cast<char*>(toV), weight };
		mWeights->Insert(edge);
		mEdges.push_back(edge);
	}

	void ChangeKruskalMST()
	{
		mEdges.clear();
		mbIsVisit = new bool[mNumVertex];
		Edge<char*> edge;
		while (mWeights->Remove(&edge))
		{
			if (IsArriveNode(edge.VStart, edge.VEnd))
			{
				RemoveEdge(edge.VStart, edge.VEnd);
				mNumEdge--;
			}
			else
			{
				mEdges.push_back(edge);
			}

			if (mNumEdge + 1 == mNumVertex)
			{
				while (mWeights->Remove(&edge))
				{
					mEdges.push_back(edge);
				}
				break;
			}
		}
		delete[] mbIsVisit;
	}

	void ShowGraphInfo() const
	{
		char* vertex;

		for (int i = 0; i < mNumVertex; ++i)
		{
			std::cout << mVertexList[i] << " : ";
			if (mVertices[i].First(&vertex))
			{
				std::cout << vertex << ' ';
			}
			while (mVertices[i].Next(&vertex))
			{
				std::cout << vertex << ' ';
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	void ShowEdgeInfo() const
	{
		for (int i = 0; i < mNumEdge; ++i)
		{
			std::cout << "(" << mEdges[i].VStart << "-" << mEdges[i].VEnd
				<< "), w: " << mEdges[i].Weight << std::endl;
		}
	}

protected:
	int GetIndexByVertex(const char* vertex)
	{
		for (int i = 0; i < mNumVertex; ++i)
		{
			if (!strcmp(vertex, mVertexList[i]))
			{
				return i;
			}
		}

		return -1;
	}

	void RemoveEdge(const char* v1, const char* v2)
	{
		int index = GetIndexByVertex(v1);
		char* vertex;
		if (mVertices[index].First(&vertex))
		{
			if (vertex == v2)
			{
				mVertices[index].Remove();
			}
			else
			{
				while (mVertices[index].Next(&vertex))
				{
					if (vertex == v2)
					{
						mVertices[index].Remove();
						break;
					}
				}
			}

			index = GetIndexByVertex(v2);
			if (mVertices[index].First(&vertex))
			{
				if (vertex == v1)
				{
					mVertices[index].Remove();
				}
				else
				{
					while (mVertices[index].Next(&vertex))
					{
						if (vertex == v1)
						{
							mVertices[index].Remove();
							break;
						}
					}
				}
			}
		}
	}

	bool IsArriveNode(const char* start, const char* end)
	{
		for (int i = 0; i < mNumVertex; ++i)
		{
			mbIsVisit[i] = false;
		}

		int vertexIndex = GetIndexByVertex(start);

		mbIsVisit[vertexIndex] = true;

		SimpleQueue<char*> queue;
		char* outVertex;
		if (mVertices[vertexIndex].First(&outVertex))
		{
			if (!mbIsVisit[GetIndexByVertex(outVertex)] && outVertex != end)
			{
				queue.Push(outVertex);
			}

			while (mVertices[vertexIndex].Next(&outVertex))
			{
				if (!mbIsVisit[GetIndexByVertex(outVertex)] && outVertex != end)
				{
					queue.Push(outVertex);
				}
			}
		}

		while (queue.Pop(&outVertex))
		{
			if (SearchTargetNodeBFSRecursive(outVertex, end))
			{
				return true;
			}
		}

		return false;
	}

	bool SearchTargetNodeBFSRecursive(const char* start, const char* end)
	{
		if (end == start)
		{
			return true;
		}

		int vertexIndex = GetIndexByVertex(start);

		if (!mbIsVisit[vertexIndex])
		{
			mbIsVisit[vertexIndex] = true;
		}

		SimpleQueue<char*> queue;
		char* outVertex;
		if (mVertices[vertexIndex].First(&outVertex))
		{
			if (!mbIsVisit[GetIndexByVertex(outVertex)])
			{
				queue.Push(outVertex);
			}

			while (mVertices[vertexIndex].Next(&outVertex))
			{
				if (!mbIsVisit[GetIndexByVertex(outVertex)])
				{
					queue.Push(outVertex);
				}
			}
		}

		while (queue.Pop(&outVertex))
		{
			if (SearchTargetNodeBFSRecursive(outVertex, end))
			{
				return true;
			}
		}

		return false;
	}

	char** mVertexList;
	LinkedList<char*>* mVertices;
	SimpleHeap<Edge<char*>>* mWeights;
	std::vector<Edge<char*>> mEdges;
	bool* mbIsVisit;
	int mNumVertex;
	int mNumEdge;
};