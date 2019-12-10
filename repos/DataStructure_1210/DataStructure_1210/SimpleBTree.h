#pragma once
using FunPtr = void (*)(int);
typedef int DataType;

class SimpleBTree
{
public:
	SimpleBTree();
	SimpleBTree(DataType data);
	~SimpleBTree();
	void MakeLeftSubTree(SimpleBTree* node);
	void MakeRightSubTree(SimpleBTree* node);
	void SearchPreOrderBTree(FunPtr) const;
	void SearchInOrderBTree(FunPtr) const;
	void SearchPostOrderBTree(FunPtr) const;
	void DeleteBTree() const;

private:
	SimpleBTree* mLeftNode;
	SimpleBTree* mRightNode;
	DataType mData;
};

