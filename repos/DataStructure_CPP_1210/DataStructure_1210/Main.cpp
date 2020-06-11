#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

int main()
{
    vector<string> participant, completion;

    participant.push_back("korean");
    completion.push_back("korean2");

    unordered_map<string, int> partMap;

    for (auto iter = participant.begin(); iter != participant.end(); ++iter)
    {
        ++partMap[*iter];
    }

    string inCom;
    for (auto iter = completion.begin(); iter != completion.end(); ++iter)
    {
        if (partMap[*iter] > 1)
        {
            --partMap[*iter];
        }
        else
        {
            inCom = *iter;
            break;
        }
    }

    cout << inCom << endl;
}
