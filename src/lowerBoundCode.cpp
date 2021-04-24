#include <algorithm>
#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include <typeinfo>

using namespace std;

int main()
{
    int target = 9;
    vector<int> testV = {0, 1, 2, 5, 7, 8, 10, 10, 20, 30, 49};

    {
        auto it = lower_bound(testV.begin(), testV.end(), 10); 
    cout << "before insert:" << *it << endl;
    it = testV.insert(it, target);
    cout << "after insert:" << *it << endl;
    // cout << it - testV.begin() << endl;
    for(int i: testV)
    {
        cout << i << ",";
    }
    }

    {
        pair<vector<int>::iterator, vector<int>::iterator> bounds; //equal_range的返回值类型
        bounds = equal_range(testV.begin(), testV.end(), 10);
        cout << "the number of 10 in testV is " << bounds.second - bounds.first << endl;
        for_each(bounds.first, bounds.second, [](int x){ cout << x << ", ";});

    }
    return 0;
    

}