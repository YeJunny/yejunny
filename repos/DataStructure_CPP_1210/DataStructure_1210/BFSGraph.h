#pragma once

#include "SimpleGraph.h"
#include "SimpleQueue.h"

template <typename TVertex>
class BFSGraph : public SimpleGraph<TVertex>
{
public:
	BFSGraph(int numVertex, TVertex* vertexList)
		: SimpleGraph<TVertex>(numVertex, vertexList)
	{
		mbIsVisit = new bool[numVertex];
		for (int i = 0; i < numVertex; ++i)
		{
			mbIsVisit[i] = false;
		}
	}

	~BFSGraph()
	{
		delete[] mbIsVisit;
	}

	void SearchAllNode(const TVertex vertex)
	{
		ShowBFSGraphInfoRecursive(vertex);

		for (int i = 0; i < this->mNumVertex; ++i)
		{
			mbIsVisit[i] = false;
		}
	}

private:
	void ShowBFSGraphInfoRecursive(const TVertex vertex)
	{
		int vertexIndex = this->GetIndexByVertex(vertex);

		if (!mbIsVisit[vertexIndex])
		{
			mbIsVisit[vertexIndex] = true;
			std::cout << this->mVertexList[vertexIndex] << std::endl;
		}

		SimpleQueue<TVertex> queue;
		TVertex outVertex;
		if (this->mVertices[vertexIndex].First(&outVertex))
		{
			if (!mbIsVisit[this->GetIndexByVertex(outVertex)])
			{
				queue.Push(outVertex);
			}

			while (this->mVertices[vertexIndex].Next(&outVertex))
			{
				if (!mbIsVisit[this->GetIndexByVertex(outVertex)])
				{
					queue.Push(outVertex);
				}
			}
		}

		while (queue.Pop(&outVertex))
		{
			ShowBFSGraphInfoRecursive(outVertex);
		}
	}

	bool* mbIsVisit;
};

