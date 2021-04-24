#include <string>
#include <iostream>
#include <iomanip>
#include <utility>
#include <vector>

using namespace std;

struct A
{
    std::string s;
    int k;
    A() : s("test"), k(-1) { }
    A(string str, int num) : s(str), k(num) { }
    A(const A& o) : s(o.s), k(o.k) { std::cout << "move failed!\n"; }
    A(A&& o) noexcept :
           s(std::move(o.s)),       // explicit move of a member of class type
           k(std::exchange(o.k, 0)) // explicit move of a member of non-class type
    { }

    void showA()
    {
        cout << "s:" << s << "k:" << k << endl;
    }

    A& operator=(const A& rhs)
    {
        if(this != &rhs)
        {
            s = rhs.s;
            k = rhs.k;
        }

        return *this;
    }
};
 
A f(A a)
{
    return a;
}
 
struct B : A
{
    std::string s2;
    int n;
    // implicit move constructor B::(B&&)
    // calls A's move constructor
    // calls s2's move constructor
    // and makes a bitwise copy of n
};
 
struct C : B
{
    ~C() { } // destructor prevents implicit move constructor C::(C&&)
};
 
struct D : B
{
    D() { }
    ~D() { }          // destructor would prevent implicit move constructor D::(D&&)
    D(D&&) = default; // forces a move constructor anyway
};

void test()
{
    vector<A> v1{A(string("xiaozhu"), 22), A(string("xiaohu"), 23)}; //必须加string("字符串")
    v1.insert(v1.begin(), A());
    vector<A> v2(v1);
    cout << "before move" << endl;
    for(auto item: v1)
    {
        item.showA();
    }

    vector<A> v3(move(v1));
    cout << "after move" << endl;
    for(auto item: v1)
    {
        item.showA();
    }
}

int main()
{
    test();

    return 0;
}


