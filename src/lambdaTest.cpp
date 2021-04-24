//测试匿名函数lambda
#include <iostream>

using namespace std;

void test()
{
    int id = 40;
    auto f = [&id](int para)->int{
        cout << "id:" << id << endl;
        ++id;
        ++ para;
        return para;
    }; //这个地方属于一个语句，应该加上;

    int res = f(8);
    cout << "first print res :" << res << endl;
    res = f(8);
    cout << "second print res :" << res << endl;    
}

void test01()
{
    //测试匿名函数中的mutable以及static变量
    int id = 40;
    auto f = [id]()mutable -> int{
        static int age = 0; //静态变量只声明一次，但是可被多次修改
        id++;  //在有mutable的时候是可行的
        //删除掉mutable,报错：表达式必须是可修改的左值
        age ++;
        return age;
    }; //匿名函数单独声明后面需要添加;

    int res = f(); 
    cout << "first print res :" << res << endl; // 1
    res = f();
    cout << "second print res :" << res << endl; // 2
}

int main()
{
    test();
    cout << "test01():mutable and static local var" << endl;
    test01();

    return 0;
}