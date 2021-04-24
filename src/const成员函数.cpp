#include <string>
#include <iostream>

using namespace std;

class TextBlock{
public:
    TextBlock(string str)
    {   text = str; }
    const char& operator[](size_t position) const //operator[] for const对象
    {   return text[position]; }
    // char& operator[](size_t position) //operator[] for non-const对象
    // {   return text[position]; }
    char& operator[](size_t position)
    {
        return const_cast<char&>(static_cast<const TextBlock&>(*this)[position]);
    } //令non-const operator[]调用其const兄弟，来避免代码重复的安全代码，涉及到转型动作

private:
    string text;
};

int main()
{
    TextBlock tb("hello");
    cout << tb[0];
    tb[0] = 'x'; //这是可以的
    const TextBlock ctb("hello"); //声明一个const对象
    // ctb[0] = 'x'; //错误，不能修改const对象

}