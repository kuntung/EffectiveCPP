#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

int main()
{
    vector<int> v1 = {1,2,3,4}; //operator=(initializer_list<int> _l)
    vector<int> v2({2,3,4,5}); //
    vector<int> v3{};
    v3.insert(v3.begin(), {3,2}); //vector& operator=(initializer_list<value_type> _l)

    cout << min({1,2,3,4});

}