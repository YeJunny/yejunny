#include <iostream>
#include "ConvertInToPost.h"
#include "ExpTree.h"
using namespace std;

int main()
{
	char* postExp = ConvertInToPost::CovertInfixToPostfix("1/3");

	ExpTree expTree(postExp);
	expTree.ShowExpInfixOrder();
	std::cout << std::endl;
	expTree.ShowExpPostfixOrder();
	cout << endl;

	cout << expTree.CalulateExp() << endl;

	delete postExp;
}
