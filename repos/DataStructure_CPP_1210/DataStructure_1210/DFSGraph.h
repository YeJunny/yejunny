#pragma once

#include "SimpleGraph.h"

template <typename TVertex>
class DFSGraph : public SimpleGraph<TVertex>
{
public:
	DFSGraph(int numVertex, TVertex* vertexList)
		: SimpleGraph<TVertex>(numVertex, vertexList)
	{
		mbIsVisit = new bool[numVertex];
		for (int i = 0; i < numVertex; ++i)
		{
			mbIsVisit[i] = false;
		}
	}

	~DFSGraph()
	{
		delete[] mbIsVisit;
	}

	void SearchAllNode(const TVertex vertex)
	{
		ShowDFSGraphInfoRecursive(vertex);

		for (int i = 0; i < this->mNumVertex; ++i)
		{
			mbIsVisit[i] = false;
		}
	}

private:
	void ShowDFSGraphInfoRecursive(const TVertex vertex)
	{
		int vertexIndex = this->GetIndexByVertex(vertex);

		mbIsVisit[vertexIndex] = true;
		std::cout << this->mVertexList[vertexIndex] << std::endl;

		TVertex outVertex;
		if (this->mVertices[vertexIndex].First(&outVertex))
		{
			if (!mbIsVisit[this->GetIndexByVertex(outVertex)])
			{
				ShowDFSGraphInfoRecursive(outVertex);
			}

			while (this->mVertices[vertexIndex].Next(&outVertex))
			{
				if (!mbIsVisit[this->GetIndexByVertex(outVertex)])
				{
					ShowDFSGraphInfoRecursive(outVertex);
				}
			}
		}
	}

	bool* mbIsVisit;
};

