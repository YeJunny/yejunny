#include<iostream>
#include <fstream>
using namespace std;

int main(int argc, char** argv)
{
	std::ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	int test_case;
	int T;

	ifstream io;
	io.open("input.txt", ios_base::in);
	io >> T;
	/*
	   여러 개의 테스트 케이스가 주어지므로, 각각을 처리합니다.
	*/
	for (test_case = 1; test_case <= T; ++test_case)
	{
		int n;
		io >> n;
		int q;
		io >> q;

		bool bIsSuccess = true;
		/*if (q > n)
		{
			bIsSuccess = false;
			goto Result;
		}*/

		int* nArr;
		nArr = new int[n];
		int* queue;
		queue = new int[n];
		for (int i = 0; i < n; ++i)
		{
			io >> nArr[i];
			queue[i] = 0;
		}

		int pos;
		pos = 0;
		
		for (int i = 0; i < q; ++i)
		{
			int number;
			io >> number;

			if (nArr[0] >= number && !queue[0])
			{
				queue[0] = number;
			}
			else
			{
				for (int j = 0; j < q - i - 1; ++j)
				{
					io >> number;
				}
				bIsSuccess = false;
				goto Result;
			}


			for (int j = pos; j >= 0; --j)
			{
				if (nArr[j + 1] >= queue[j] && !queue[j + 1])
				{
					queue[j + 1] = queue[j];
					queue[j] = 0;
					if (j == pos && j < n - 1)
					{
						++pos;
					}
				}
				else
				{
					pos = j;
				}
			}
		}


	Result:
		cout << '#' << test_case << ' ';
		if (bIsSuccess)
		{
			int index = 0;
			while (!queue[index])
			{
				++index;
			}
			cout << ++index << endl;
		}
		else
		{
			cout << 0 << endl;
		}

		delete[] queue;
		delete[] nArr;
	}
	return 0;//정상종료시 반드시 0을 리턴해야합니다.
}