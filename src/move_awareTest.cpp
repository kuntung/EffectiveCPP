#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <utility>
#include <ctime>
#include <random>
#include <cstring>

using namespace std;

class MyString{
public:
    static size_t DCtor;  //累计default-ctor的调用次数
    static size_t Ctor;  //累计ctor的调用次数
    static size_t CCtor; //累计copy-ctor的调用次数
    static size_t CAsgn; //累计copy-assignment的调用次数
    static size_t MCtor; //累计move-ctor的调用次数
    static size_t MAsgn; //累计move-asgn的调用次数
    static size_t Dtor; //累计dtor的调用次数

private:
    char* _data; 
    size_t _len;
    void _init_data(const char *s)
    {
        _data = new char[_len + 1];
        memcpy(_data, s, _len);
        _data[_len] = '\0';
    }

public:
    //default constructor
    MyString() : _data(nullptr), _len(0) { ++DCtor;}

    //constructor
    MyString(const char* p) : _len(strlen(p)) {
        ++Ctor;
        _init_data(p); 
    }

    //copy constructor
    explicit MyString(const MyString& msObj) : _len(msObj._len){
        ++CCtor;
        _init_data(msObj._data);
    }

    //move constructor with "noexcept"
    MyString(MyString&& rhs) noexcept : 
        _data(rhs._data), _len(rhs._len){
        ++MCtor;
        if(_data) delete _data;
        rhs._len = 0;
        rhs._data = nullptr;
    }

    //copy assignment: 
    MyString& operator=(const MyString& rhs){
        ++CAsgn;
        //自我赋值检测
        if(this != &rhs)
        {
            if(_data) delete _data;
            _len = rhs._len;
            _init_data(rhs._data); //deep copy
        }
        
        return *this; //return type(reference to *this)
    }

    //move assignment
    MyString& operator=(MyString& rhs){ //不能传入const &
        ++MAsgn;
        //自我赋值检测
        if(this != &rhs)
        {
            if(_data) delete _data;
            _len = rhs._len;
           _data = rhs._data; //copy
           rhs._len = 0;
           rhs._data = nullptr;
        }
        
        return *this; //return type(reference to *this)
    }

    //dtor
    ~MyString() {
        ++Dtor;
        if(_data) delete _data;
    }

    bool operator<(const MyString& rhs) const //为了set
    {
        return string(this->_data) < string(rhs._data); //通过std::string进行数据大小比较
    }

    bool operator==(const MyString& rhs) const //为了set
    {
        return string(this->_data) == string(rhs._data); 
    }

    char* get() const { return _data;} //get the address
};

class MyStrNoMove{
public:
    static size_t DCtor;  //累计default-ctor的调用次数
    static size_t Ctor;  //累计ctor的调用次数
    static size_t CCtor; //累计copy-ctor的调用次数
    static size_t CAsgn; //累计copy-assignment的调用次数
    static size_t MCtor; //累计move-ctor的调用次数
    static size_t MAsgn; //累计move-asgn的调用次数
    static size_t Dtor; //累计dtor的调用次数

private:
    char* _data; 
    size_t _len;
    void _init_data(const char *s)
    {
        _data = new char[_len + 1];
        memcpy(_data, s, _len);
        _data[_len] = '\0';
    }

public:
    //default constructor
    MyStrNoMove() : _data(nullptr), _len(0) { ++DCtor;}

    //constructor
    MyStrNoMove(const char* p) : _len(strlen(p)) {
        ++Ctor;
        _init_data(p); 
    }

    //copy constructor
    explicit MyStrNoMove(const MyStrNoMove& msObj) : _len(msObj._len){
        ++CCtor;
        _init_data(msObj._data);
    }

    //copy assignment: 
    MyStrNoMove& operator=(const MyStrNoMove& rhs){
        ++CAsgn;
        //自我赋值检测
        if(this != &rhs)
        {
            if(_data) delete _data;
            _len = rhs._len;
            _init_data(rhs._data); //deep copy
        }
        
        return *this; //return type(reference to *this)
    }

    //dtor
    ~MyStrNoMove() {
        ++Dtor;
        if(_data) delete _data;
    }

    bool operator<(const MyStrNoMove& rhs) const //为了set
    {
        return string(this->_data) < string(rhs._data); //通过std::string进行数据大小比较
    }

    bool operator==(const MyStrNoMove& rhs) const //为了set
    {
        return string(this->_data) == string(rhs._data); 
    }

    char* get() const { return _data;} //get the address
};

//output static_data
#include<typeinfo>
template<typename T>
void output_static_data(const T& myStr)
{
    cout << typeid(myStr).name() << "--" <<endl;
    cout << "CCtor = " << T::CCtor
         << "MCtor = " << T::MCtor
         << "CAsgn = " << T::MCtor
         << "MAsgn = " << T::MCtor
         << "Dtor = " << T::MCtor
         << "Ctor = " << T::MCtor
         << "DCtor = " << T::MCtor;
}

//测试程序
template<typename M, typename NM>
void test_moveable(M c1, NM c2, long& value)
{
char buf[10];

//测试moveable
typedef typename iterator_traits<typename M::iterator>::value_type Vltype; //定义Vltype为M容器中的元素类型
clock_t timeStart = clock();
    cout << "test, with moveable elements" << endl;
    for(long i = 0; i < value; ++i)
    {
        snprintf(buf, 10, "%d", rand()); //随机数，放进转换为字符串
        auto ite = c1.end();
        c1.insert(ite, Vltype(buf));
    }

    cout << "constructions, milli-seconds:" << (clock() - timeStart) << endl;
    cout << "size() = " << c1.size() << endl;
    output_static_data(*c1.begin());

M c11(c1);
M c12(std::move(c1)); //必须确保加下来不会再用到c1
    c11.swap(c12);

//测试non-moveable
typedef typename iterator_traits<typename NM::iterator>::value_type VltypeNM; //定义Vltype为M容器中的元素类型
    timeStart = clock();
    cout << "test, with non-moveable elements" << endl;
    for(long i = 0; i < value; ++i)
    {
        snprintf(buf, 10, "%d", rand()); //随机数，放进转换为字符串
        auto ite = c2.end();
        c2.insert(ite, VltypeNM(buf));
    }

    cout << "constructions, milli-seconds:" << (clock() - timeStart) << endl;
    cout << "size() = " << c2.size() << endl;
    output_static_data(*c2.begin());

NM c21(c2);
NM c22(std::move(c2)); //必须确保加下来不会再用到c1
    c21.swap(c22);
}

//begin test
int main()
{
long value = 3e6;
    test_moveable(vector<MyString>(), vector<MyStrNoMove>(), value);
    return 0;
}

