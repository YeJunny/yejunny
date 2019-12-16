#include "ConvertInToPost.h"
#include "SimpleStack.h"

char* ConvertInToPost::CovertInfixToPostfix(const char* exp)
{
	SimpleStack<int>* stack = new SimpleStack<int>();
	char* postExp = new char[128];
	int index = 0;

	for (int i = 0; exp[i] != 0; i++)
	{
		char elem = exp[i];
		switch (elem)
		{
		case '(':
		{
			stack->Push('(');
			break;
		}
		case ')':
		{
			int data;
			while (stack->Pop(&data))
			{
				if (data == '(')
				{
					break;
				}
				postExp[index++] = data;
			}
			break;
		}
		case '+':
		case '-':
		{
			char vData = static_cast<char>(stack->View());
			if (vData == '*' || vData == '/')
			{
				int data;
				while (stack->Pop(&data))
				{
					postExp[index++] = data;
				}
			}
			stack->Push(elem);
			break;
		}
		case '*':
		case '/':
		{
			stack->Push(elem);
			break;
		}
		default:
		{
			postExp[index++] = elem;
			break;
		}
		}
	}
	int data;
	while (stack->Pop(&data))
	{
		postExp[index++] = data;
	}
	postExp[index] = 0;
	delete stack;
	return postExp;
}