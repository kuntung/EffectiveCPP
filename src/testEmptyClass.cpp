#include <iostream>

class Empty{

};

void TestEmpty()
{
    Empty e;
    std::cout << "size of empty class:" << sizeof(e) << std::endl;
    //size of empty class:1
}

int main()
{
    TestEmpty();

    return 0;
}