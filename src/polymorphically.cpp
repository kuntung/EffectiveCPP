#include <iostream>
#include <string>

using namespace std;

class Base{
public:
    virtual void printInfo()
    {
        cout << "I am your father" << endl;
    }

    void testSon(){
        cout << "I am a grandfather" << endl;
    }
};

class Son: public Base{
public:
    void printInfo(){
        cout << "I am a son of Base" << endl;
    }

    virtual void testSon()
    {
        cout << "I am a father of my son" << endl;
    }
};

class GrandSon: public Son{
public:
    void printInfo(){
        cout << "I am a grandSon of Base" << endl;
    }

    void testSon()
    {
        cout << "I am a son of my father" << endl;
    }

};

void test01()
{ 
    Base* bptr = new Base(); //"I am your father"
    bptr->printInfo();
    bptr = new Son();//"I am a son of Base" 
    bptr->testSon(); //I am a grandfather
    bptr->printInfo();
    bptr = new GrandSon();
    bptr->printInfo(); // "I am a grandSon of Base" 

    Son* ptrs = new Son();
    ptrs->testSon(); ////"I am a father of my son"
    ptrs = new GrandSon();
    ptrs->testSon(); ////"I am a father of my son" (添加virtual前)
    //"I am a son of my father" (添加virtual后)
    
}

int main()
{
    test01();

    return 0;
}