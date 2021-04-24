#include <iostream>
#include <vector>

using namespace std;

void test01()
{
    vector<int> v{1,2,3,45,6};
    int i = 0;
    while(i < v.size()) cout << "i:" << i, i++;

    return;
}

int main()
{
    test01();

    return 0;
}