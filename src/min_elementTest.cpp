#include <algorithm>
#include <vector>
#include <iostream>
#include <deque>
#include <array>
#include <set>
#include <map>
#include <unordered_map>
#include <functional>   // std::bind

using namespace std;

struct myclass:public binary_function<pair<int,int>, pair<int,int>, bool> {
  bool operator() (const pair<int, int>& x, const pair<int, int>& y)
   { if(x.first < y.first || (x.first == y.first && x.second > y.second)) return true;
    else return false;
    }
}; //自定义抽象二元谓词

int main()
{   
    using namespace std::placeholders;    // adds visibility of _1, _2, _3,...
    vector<int> v{1, 3, 4, 5, 7};
    cout << "the minimum element in vector<int> is: " << *min_element(v.begin(), v.end()) << endl;
    deque<int> d = {4,1,23,123,0,12,-1};
    cout << "the minimum element in deque<int> is: " << *min_element(d.begin(), d.end()) << endl;
    array<int, 8> a = {1,3,4,5,67,2,1,0};
    cout << "the minimum element in array<int, size> is: " << *min_element(a.begin(), a.end()) << endl;
    set<int> s = {1,2,3,4,7};
    cout << "the minimum element in set<int> is: " << *min_element(s.begin(), s.end()) << endl;
    multiset<int> multis {1,2,2,1,1,3,5};
    cout << "the minimum element in multiset<int> is: " << *min_element(multis.begin(), multis.end()) << endl;
    map<int, int> mp = {{1,3}, {2,4}, {5,5}};
    cout << "the minimum element in map<int, int> is: " << min_element(mp.begin(), mp.end())->second << endl;
    unordered_multimap<int, int> multihash = {{1,3}, {1,2},{2,2},{3,3}};
    auto fn = bind(myclass(), _1, _2);
    pair<int, int> min_index = *min_element(multihash.begin(), multihash.end(), fn);
    cout << "the minimum element in unordered_multimap<int, int> is: " 
        << "first:" << min_index.first <<",second" << min_index.second << endl;
                            
    return 0;
}