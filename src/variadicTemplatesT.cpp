#include <iostream>
#include <bitset>
#include <tuple>
using namespace std;

void printX() //printX的偏特化版本，递归的终止,必须定义在泛化版本之前
{
}
template <typename T, typename... Types>
void printX(const T &firstArg, const Types &...args)
{
    cout << firstArg << endl;                                     //print first argument
    cout << "current number of args:" << sizeof...(args) << endl; //返回剩余参数个数
    //如果不定义printX()的空实现，通过sizeof...(args)来进行判断递归终止
    printX(args...);
}

//PrintX测试接口
void testPrintX()
{
    printX(7.5, "hello", bitset<16>(377), 42);
}
//通过variadicTemplates实现operator<<重载
//泛化版本
template <int IDX, int MAX, typename... Args>
struct PRINT_TUPLE
{
    static void print(ostream &os, const tuple <Args...> &t)
    {
        os << get<IDX>(t) << (IDX + 1 == MAX ? "" : ",");
        PRINT_TUPLE<IDX + 1, MAX, Args...>::print(os, t);
    }
};
//偏特化版本
template <int MAX, typename... Args>
struct PRINT_TUPLE<MAX, MAX, Args...>
{
    static void print(std::ostream &os, const tuple<Args...> &t)
    {
    }
};

//operator<<重载
template <typename... Args>
ostream& operator<<(ostream &os, const tuple<Args...> &t)
{
    os << "[";
    PRINT_TUPLE<0, sizeof...(Args), Args...>::print(os, t);

    return os << "]";
}

void testPrintTuple()
{
    cout <<make_tuple(7.5, string("hello"), bitset<16>(377), 42);
}
int main()
{
    testPrintX();
    cout << "testPrintTuple: ";
    testPrintTuple();
    return 0;
}