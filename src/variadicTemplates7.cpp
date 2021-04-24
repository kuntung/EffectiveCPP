#include <iostream>
//tuple的递归复合实现

using namespace std;

template<typename...Values> class tup;
template<> class tup<>{}; //偏特化版本

template<typename Head, typename... Tail>
class tup<Head, Tail...>
{
    typedef tup<Tail...> composited;
protected:
    composited m_tail;
    Head m_head;
public:
    tup() {}
    tup(Head v, Tail... vtail):m_tail(vtail...), m_head(v){}

    Head head() {return m_head;}
    composited& tail() {return m_tail;}
};

void testTup()
{
    tup<int, float, string> it1(41, 6.3, "nico");
    cout << it1.head() << endl; //41
    cout << it1.tail().head() << endl; //6.3
    cout << it1.tail().tail().head() << endl; //"nico"
}

int main()
{
    testTup();

    return 0;
}