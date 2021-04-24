#include <iostream>

using namespace std;

template<unsigned n>
struct Factorial {
    enum { value = n * Factorial<n-1>::value};
};

template<>
struct Factorial<0>{
    enum {value = 1};
};//特化版本

void testFac()
{
    cout << Factorial<10>::value << endl;
}

int main()
{

    testFac();

    return 0;
}