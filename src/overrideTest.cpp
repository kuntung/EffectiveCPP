#include <iostream>

using namespace std;

class Base{
public:
    virtual int func(int a, int b)
    {
       cout << "int Base::func(int a, int b)" << endl; 
        return a+b;
    }

    virtual int func(int a, int b, int c)
    {
        cout << "int Base::func(int a, int b, int c)" << endl;
        return a+b+c;
    }
    // float func(float a, float b)
    // {
    //     cout << "float Base::func(float a, float b)" << endl;
    //     return a+b;
    // }

    int func(int a)
    {
        cout << "int Base::func(int a) non-virtual" << endl;
        return 2*a;
    }

};

class Derived1 : public Base{
public:
    int func(int a, int b) override{
        cout << "int Derived::func(int a, int b)" << endl;
        return a*b;
    }

    int func(int a, int b, int c) override //override必须同父类的函数signature一样
    {
        cout << "int Derived::func(int a, int b, int c)" << endl;
        return a*b*c;
    }

    float func(string str)
    {
        cout << str << endl;
        return 0;
    }
    int func(int a)
    {
        cout << "int Derived::func(int a) no-override" << endl;
        return a;
    }
};

void test()
{
    Base* bptr = new Derived1();
    bptr->func(1,2); //
    bptr->func(3.0);
    bptr->func(1,2,3);
    // bptr->func("sad"); //无法指针通过父类指针调用派生类中的重载版本
    Derived1 temp;
    temp.func("no override func");
}

int main()
{
    test();

    return 0;
}