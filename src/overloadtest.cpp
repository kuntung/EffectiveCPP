#include <iostream>

using namespace std;

int func(int a, int b)
{
    return a + b;
}

float func(int a)
{
    return a;
}

int func(int a, int b, int c)
{
    return a + b + c;
}

// float func(float a, float b)
// {
//     return a+b;
// }

void test()
{
    cout << func(1,2) << endl; //int func(int a, int b)
    cout << func(1,2,3) << endl; //int func(int a, int b, int c)
    cout << func(3) << endl; //float func(int a)
    cout << func(1.2, 3.2) << endl; //float func(float a, float b)
}

int main()
{
    test();

    return 0;
}