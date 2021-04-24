#include <iostream>

using namespace std;

class Base{
public:
    virtual void mf1() = 0;
    virtual void mf1(int x){
        cout << "Base mf1(int x)" << endl;
    }
    virtual void mf2()
    {
        cout << "Base mf2()" << endl;
    }
    void mf3(){
        cout << "Base mf3()" << endl;
    }
    void mf3(double y){
        cout << "Base mf3(double y)" << endl;
    }
    
    void show1()
    {
        cout << "Base show1()" << endl;
    }

    void show1(int)
    {
        cout << "Base show1(int)" << endl;
    }
};

class Derived: public Base{
public:
    using Base::mf1; //让Base class内名为mf1和mf3的所有东西在
    using Base::mf3; //Derived作用域内都可见并且public
    using Base::show1;
    virtual void mf1(){
        cout << "Derived mf1()" <<endl;
    }
    void mf3(){
        cout << "Derived mf3()" <<endl;
    }
    void mf4(){
        cout << "Derived mf4()" <<endl;

    }
    void show1()
    {
        cout << "Derived show1()" << endl;
    }
};


void test01()
{
    Derived d;
    d.mf1();
    d.mf1(3);
    d.mf3();
    d.mf3(3.2);
    d.mf4();
    d.mf2();
    d.show1();
    d.show1(2);
    cout <<"test of polymorphically" <<endl;
    Base* bptr = new Derived(); //可以调用派生类中virtual版本的函数
    bptr->mf1();
    bptr->mf1(3);
    bptr->mf2();
    bptr->mf3(3.1); 
    bptr->show1();
    bptr->show1(2);
    // bptr->mf4(); //不行，不是多态
}

int main()
{
    test01();

    return 0;
}