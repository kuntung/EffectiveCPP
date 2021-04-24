#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <deque>

using namespace std;

template<typename T>
void printVec(const vector<T>& vec)
{
    for(auto& item : vec)
    {
        cout << item << ",";
    }
    cout << endl;
}
void testErase()
{
    vector<int> v{1,2,3,45,6};
    auto it = v.begin();
    printVec(v);
    cout << "before erase:" << "v.size(): " <<v.size() << ",index(it):" << it - v.begin() << endl;
    v.erase(it);
    printVec(v);
    cout << "after erase but do not assign:" << "v.size(): " <<v.size() << ",index(it):" << it - v.begin() << endl;
    // it = v.erase(it);
    // printVec(v);
    // cout << "after erase with assign:" << "v.size(): " <<v.size() << ",index(it):" << it - v.begin() << endl;

    return;
}
template<typename KEY, typename VALUE>
inline void printMap(const map<KEY, VALUE>& mp)
{
    for(auto& it: mp)
    {
        cout << it.first << ", " << it.second <<endl;
    }
}
void testMapErase()
{
    map<int, int> mp{{1,2},{3,4}, {5,6}};
    auto it = mp.begin();
    cout << "before erase:" << "mp.size(): " <<mp.size() << endl;
    printMap(mp);
    // mp.erase(it); //非赋值型erase
    // cout << "after erase but non-assign: " << "mp.size():" << mp.size() << endl;
    // printMap(mp);
    it = mp.erase(it);
    cout << "after erase with assign: " << "mp.size():" << mp.size() << endl;
    printMap(mp);
    cout << "element of it: " << it->first << ", " << it->second <<endl;
    cout << "test of erase KEY:" << endl;
    cout << "key is not available :" << mp.erase(4) << endl; //0
    cout << "key is exist: " << mp.erase(3) <<endl; //1
}

int main()
{
    // testErase();
    cout << "test Map erase" << endl;
    testMapErase();

    return 0;
}