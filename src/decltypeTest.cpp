#include <iostream>

using namespace std;

class Person{
public:
    void showType()
    {
        cout << "this is a Person Obj" << endl;
    }
};

void test()
{
    Person p;
    decltype(p) p2 = p;
    p2.showType();

    return;
}

int main()
{
    test();

    return 0;
}
