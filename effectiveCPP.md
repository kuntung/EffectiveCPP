# 导读

## 命名习惯

1. 指针命名：`指向一个T型对象`

   ```c++
   Widget* pw; //pw means "ptr to Widget"
   ```

2. 引用命名：`rw:reference to Widget`

# 条款

## 让自己习惯C++

### 条款1：视C++为一个语言联邦

1. `Object-Oriented C++`
   - classes
   - 封装
   - 继承
   - 多态
   - virtual函数（动态绑定）
2. `Template C++`
3. STL: 容器、迭代器、算法、函数对象、适配器
4. 内置数据类型（C-like）通过值传递比通过引用传递更高效，而用户自定义数据类型则是因为`构造函数`和`析构函数`的存在，使得通过引用传递的方式更高效。`不然需要在去创建临时对象`

### 尽量以`const,enum,inline`替换#define

1. 对于单纯变量，最好以const对象或者enum替换#defines

2. 对于形似函数的宏，最好改用inline函数来替换#defines

   ```c++
   template<typename T>
   inline void callWithMax(const T& a, const T& b)
   {
       f(a > b ? a : b);
   }
   ```


也就是不要通过`#define ASPECT 1.653`这样的`宏定义`形式去定义一个常量。可以通过`const double AspectRatio  = 1.653;`的形式定义常量

> cpp宏定义：

常量指针的定义

由于常量定义式通常被放在头文件内，因此在定义常量指针的时候，有必要将指针也声明为`const`即

```c++
const char* const authorName = "Junney";
//string对象往往又比`char*`更好用，所以可以定义如下
const std::string const authorName = "Junney";
```

#### class专属常量

为了将常量的作用域限制在class内，需要将其设置为`成员变量`，并且为了确保此变量只有一份实体，需要声明为`static`

```c++
class GamePlayer{
private:
    static const int NumTurns = 5; //常量声明式
    int scores[NumTurns];
};
```

#### enum枚举类型

当上述的`GamePlayer::scores[Numturns];`中，编译器不支持常量在声明中定义的时候，可以考虑使用`the enum hack`的补偿做法

> 一个属于枚举类型(enumerated type)的数值可权充ints来使用。于是

```c++
class GamePlayer{
private:
    enum {NumTurns = 5}; //令NumTurns称为5的一个记号名称
    int scores[NumTurns];
};
```

1. enum hack的行为某方面比较像#define而不像const
2. 对一个const取地址是合法的，**而取一个enum和#define的地址是不合法的**
3. **作用：可以通过enum来限制获取某个整型常量的地址或者引用**

### 尽可能使用`const`

> const可以用来修饰
>
> 1. classes外部global或namespace作用域中的常量
> 2. 修饰文件、函数或区块作用域中被声明为`static`的对象
> 3. classes内部的`static`和`non-static`成员变量

1. 传入参数的过程中修饰参数

   ```c++
   void f1(const Widget* pw); //f1获得一个指针，指向一个不变的widget对象
   ```

2. STL迭代器的两种const方式

   ```c++
   std::vector<int> vec;
   const std::vector<int>::iterator iter = vec.begin(); //iter等价于T* const指针常量，指针指向不可修改，指针指向的内容可以修改
   *iter = 10; //可以
   iter++; //错误，iter是T* const
   
   std::vector<int>::const_iterator cIter = vec.begin(); //cIter等价于cont T*
   ```

3. **const在函数声明时的应用**

   - 令函数返回一个常数值，往往可以降低错误，而不至于放弃安全性和高效性

     ```c++
     class Rational {};
     const Rational operator* (const Rational& lhs, const Rational& rhs);
     //能够避免(a*b = c)这样的情形出现
     ```

#### const成员函数

> 将成员函数声明为const的目的：
>
> 1. 使得该成员函数可以作用于`const对象` **个人理解：对于const Class& obj，提供的专用接口**
> 2. 使得class接口容易被理解，明确哪个函数可以改变对象内容，而哪个函数不行

![image-20210406221449531](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210406221449531.png)

> **存在的问题：以operator[]为例，如果需要实现多种功能，那么分别实现non-const和const版本的接口会使得代码非常冗余**
>
> **解决办法：让non-const方法调用其const兄弟（涉及到转型）**
>
> **原理：** non-const成员函数本来就可以对对象做任何操作，所以调用其中一个成员函数不会带来任何风险
>
> ```c++
> char& operator[](size_t position)
> {
>         return const_cast<char&>(
>             static_cast<const TextBlock&>(*this)[position]);
>     } 
> //1.static_cast 先将*this从原始类型TextBlock& 转换为const TextBlock&
> //2.const_cast 然后将const operator[]的返回值中const移除
> ```

**相关结论：**

1. const成员函数不可以改变对象内任何non-static成员变量
2. 通过`mutable`实现non-static也能被const成员函数修改

### 确定对象被使用前已先被初始化

> **永远在使用对象之前先将它初始化**
>
> 1. 对于无任何成员的内置类型，通过手动完成初始化过程
>
> 2. 对于内置类型外的其他自定义数据类型，通过**构造函数将对象的每一个成员进行初始化**
> 3. 如果成员变量是`const`或者`reference`就一定需要**初始化，而不是赋值**

#### 赋值和初始化的区别

```c++
//执行步骤：先调用default构造函数再调用copy assignment操作符
ABEntry::ABEntry (const std::string& name, const std::string& address) 	
{
    theName = name;
    theAddress = address;
} //非赋值操作

//只执行一次拷贝构造
ABEntry::ABEntry (const std::string& name, const std::string& address)
    	:theName(name), theAddress(address)
{} //这些也都是初始化操作，而非赋值操作
```

**C++规定，对象的成员变量的初始化动作发生在进入构造函数本体之前**

#### 如果classes有多个构造函数或许多成员变量或许多基类

> 这种情况可以选择定义一个`private`成员函数来进行赋值操作，并提供给构造函数使用

## 构造、析构、赋值运算

### 了解C++默认编写并调用哪些函数

> c++默认提供的copy构造函数，default构造函数都是public且inline
>
> 当在类中声明了一个构造函数，那么编译器就不会再提供一个`default构造函数`

![image-20210407212719138](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210407212719138.png)

### 若不想使用编译器自动生成的函数，就该明确拒绝

> 通常情况下，如果不希望class支持某一特定技能，只要不声明对应函数即可。
>
> **但是，**如果不声明`copy构造函数`和`copy assignment操作符`，在尝试调用的时候编译器就会声明他们
>
> **结论：**
>
> **1. 当不想实现copy构造函数和operator=功能的时候，将其声明为`private`并且不实现其定义**
>
> **2.通过private继承一个base class（copy构造函数operator=为private）**
>
> **在C++11以上中，支持=delete的方式拒绝自动生成的函数**

### 为多态基类声明virtual析构函数

> 当父类指针指向子类对象的时候，由于要求子类对象存在在堆区，即通过new的方式开辟的内存块。因此，在适当的时机应该delete掉这部分内存。但是，如果父类的析构函数为`non-virtual`，那么当子类对象的父类指针删除时，可能子类对象并不会被销毁。
>
> **这就是形成资源泄露，影响数据结构，程序进程的主要原因**
>
> **解决办法：为父类提供一个virtual析构函数**

总结：

1. 带多态性质（带有virtual成员函数）的父类应该声明一个虚析构函数
2. 并不是所有的class都应该将其析构函数声明为`虚析构函数`
   - base classes
   - 多态才需要

### 别让异常逃离析构函数

> 因为析构函数吐出异常可能会带来`过早结束程序`或`发生不明确行为`的风险

在析构函数体中，如果要执行一些收尾工作，

![image-20210407222603233](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210407222603233.png)

![image-20210407223002842](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210407223002842.png)

![image-20210407223009342](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210407223009342.png)

![image-20210407223013600](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210407223013600.png)

![image-20210407223310758](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210407223310758.png)

### 绝不在构造和析构过程中调用virtual函数

首先，在创建子类对象的过程中，父类的构造函数会被更早地调用。假如在父类的构造函数中调用`virtual函数`，那么这时候是不会产生多态，即：**在父类构造函数作用期间，所有函数都是父类自身的成员函数实现**

> 因为如果在子类对象构造过程中，父类构造的时候就能通过virtual虚函数调用子类的成员函数，那么就会出现问题。`要求使用对象内部未初始化的成分`

同样地，在析构过程中，子类的析构函数先调用，进而相关成员变量就为`未定义值`，这时候父类析构函数如果再能够通过析构函数中的virtual函数调用子类的相关成员变量，就会出现问题

#### 解决办法

1. 在父类中将该函数声明为`non-virtual`并且要求派生类的构造函数传递必要的信息给基类的构造函数

![image-20210408222953241](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210408222953241.png)

![image-20210408223211605](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210408223211605.png)



### 令operator=返回一个reference to *this

> 为了实现“连锁赋值”，赋值操作符必须返回一个reference指向操作符的左侧实参，

```c++
class Widget{
public:
    Widget& operator=(const Widget& rhs) 
        //返回类型是一个reference，指向当前对象*this
    {
        ...
        return *this;
    }
};
```

### 在operator=中处理自我赋值？？

如果通过对象来管理资源，那么可能会出现`在停止使用资源之前就意外释放了它`

```c++
class Bitmap{...};
class Widget{
 ...
private:
    Bitmap* pb; //指针，指向一个从heap上分配得到的数据
};

//Widget的operator=实现代码
Widget& Widget::operator=(const Widget& rhs)
{
    if(*this == rhs) return *this; //identity test
    delete pb; //停止使用当前的pb;
    pb = new Bimap(*rhs.pb);
    return *this; 
}
//在自我赋值的时候，会出现问题
//如果*this和rhs为同一个对象，即自我赋值，那么delete的时候就删除掉了rhs和*this的
//Bitmap，进而通过pb = new Bimap(*rhs.pb);会使得当前指针指向一个已被删除的对象
```

> 解决办法：添加一个`identity test`

```c++
//Widget的operator=实现代码,添加了identity test
Widget& Widget::operator=(const Widget& rhs)
{
    if(*this == rhs) return *this; //identity test
    delete pb; //停止使用当前的pb;
    pb = new Bimap(*rhs.pb);
    return *this; 
}

//在复制之前不要删除pb
//Widget的operator=实现代码
Widget& Widget::operator=(const Widget& rhs)
{
    Bitmap* pOrig = pb; //原来的pb
    pb = new Bitmap(*rhs.pb); //令pb指向一个*pb的副本
    delete pOrig; //删除掉原来的副本
    return *this; 
}
//这样做的原理是因为：如果new发生了问题，会抛出异常
```

> 通过try, catch来捕获异常

### 复制对象时，勿忘其每一个成分

> 设计良好的面向对象系统会将对象的成员属性封装起来，只留两个函数负责对象拷贝复制操作。
>
> 1. copy构造函数
> 2. copy assignment操作符
>
> 如果所写的copy函数没有做到每个成分的复制，那么一旦发生继承，那么派生类的copy函数需要实现
>
> 1. 复制所有local成员变量
> 2. 调用所有base classes内的适当的copy函数来复制其中的base classes成分

![image-20210408232200142](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210408232200142.png)

`在同一个类中，可能copy构造函数和copy assignment操作符的函数实现有很大程度的相似，但是不要一个copy函数调用另一个copy函数`

- copy assignment操作符调用copy构造函数是不合理的，因为这就像试图构造一个已经存在的对象
- copy构造函数调用copy assignment操作符也毫无意义，因为构造函数是用来初始化新对象，而operator=是作用于已初始化对象上
- 如果copy函数之间有相近的代码，消除重复代码的方法是，建立一个`private成员函数init()`

## 资源管理

### 以对象(智能指针对象)管理资源

#### 常用的资源

> 内存，文件描述器，互斥锁，图形界面中的字型和笔刷，数据库连接，网络sockets

通过new和delete关键字进行管理资源可能会使delete被忽略。因此，单独的依靠delete语句时行不通的

> 需要将资源放进对象中去，利用对象在离开作用域的时候自动调用析构函数的机制去释放资源
>
> **RAII：**Resource Acquisition is initialization
>
> ```c++
> void f()
> {
>     std::auto_ptr<Investment> pInv(createInvestment()); //调用factory函数
> }
> 
> //老版本的智能指针
> std::auto_ptr<std::string> aps(new std::string[10]); 
> //可以通过编译，但是auto_ptr和shared_ptr两者内部是delete而不是delete[]
> std::shared_ptr //auto_ptr赋值会使右值为null
> ```

### 在资源管理类中小心copy行为

> 有时候需要自定义资源 管理类RAII
>
> 但是在应对RAII对象复制的过程中的两种应对策略
>
> 1. **禁止复制**
>
>    - private copy function
>    - =delete
>
> 2. **引用计数法**：有时候我们需要保有资源直到最后一个使用者对象被销毁时。（浅拷贝，但是计数）
>
> 3. 深拷贝
>
> 4. 转移底部资源的拥有权
>
>    > 当只需要一个RAII对象指向一个资源的时候，赋值操作采用这种实现。`auto_ptr`

### 在资源管理类中提供对原始资源的访问

![image-20210413210316937](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210413210316937.png)

![image-20210413212343496](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210413212343496.png)

![image-20210413212440223](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210413212440223.png)

> 隐式转换存在的问题：增加发生错误的机会，
>
> ![image-20210413212756075](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210413212756075.png)

![image-20210413212830151](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210413212830151.png)

![image-20210413212930517](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210413212930517.png)

### 成对使用new和delete时要采取相同形式

> delete的最大问题在于：即将被删除的内存之内究竟存有多少对象，进而决定了有多少个析构函数必须被调用起来

![image-20210413213202107](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210413213202107.png)

![image-20210413213215641](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210413213215641.png)

**对于typedef的考虑：**

> 如果通过`typedef`将数组声明重命名，那么通过new关键字开辟的heap内存，应该通过`delete[]`去释放
>
> **最好不要对数组形式做typedefs**
>
> ```c++
> typedef std:string AddressLines[4]; //每个人的地址有四行，每行是一个string
> std::string* pal = new AddressLines; 
> //new AddressLines返回一个string* 相当于new string[4]
> //因此，需要通过delete[] pal;来释放内存
> ```

### 以独立语句将newed对象置入智能指针

![image-20210413214837410](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210413214837410.png)

![image-20210413220540332](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210413220540332.png)

![image-20210413220701754](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210413220701754.png)

> 解决办法：以独立语句将newed对象置入智能指针

## 设计与声明

### 让接口容器被正确使用，不易被误用

![image-20210413221240003](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210413221240003.png)

![image-20210413221706532](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210413221706532.png)

![image-20210413222038929](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210413222038929.png)

### 设计class犹如设计type

> 定义一个新的class相当于定义了一个新的type

#### 设计class应该考虑的设计规范

![image-20210413222811557](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210413222811557.png)

![image-20210413223011254](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210413223011254.png)

![image-20210413223240830](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210413223240830.png)

![image-20210413223327879](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210413223327879.png)

### 通过pass-by-reference-to-const替换pass-by-value

> 当C++通过值传递的时候，函数参数是以实际参数的副本为初值，这个副本是通过对象的copy构造函数得到。因此，通过值传递的时候，可能会使得pass-by-value成为一个昂贵的操作

![image-20210414153233151](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210414153233151.png)

![image-20210414153334938](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210414153334938.png)

![image-20210414153452625](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210414153452625.png)

![image-20210414153752377](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210414153752377.png)

### 必须返回对象时，别妄想返回其reference

![image-20210414161453183](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210414161453183.png)

![image-20210414161443545](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210414161443545.png)

![image-20210414161704431](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210414161704431.png)

![image-20210414161956607](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210414161956607.png)

![image-20210414162118410](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210414162118410.png)

### 将成员变量声明为private

> 结论：成员变量应该是private

因为在public中的成员变量能够被无数的函数访问，一旦对该变量做了一些改动。就要涉及到许多潜在的改动。

### 宁以non-member、non-friend替换member函数

> C++的特性之一是封装，当定义更多的成员函数的时候，就会使得封装性变得很差。private变量的意义就变得轻微起来
>
> 最好的是将一些操作封装成头文件，放在一个namespace之中去

![image-20210416183908156](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210416183908156.png)

![image-20210416183919868](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210416183919868.png)

### 若所有参数皆需类型转换，请为此采用non-member函数

![image-20210416184022316](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210416184022316.png)

![image-20210416184755281](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210416184755281.png)

![image-20210416184948632](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210416184948632.png)

![image-20210416185054066](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210416185054066.png)

![image-20210416185114655](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210416185114655.png)

### 考虑写出一个不抛出异常的swap函数

![image-20210416190233317](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210416190233317.png)

![image-20210416190454272](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210416190454272.png)

## 实现

### 尽可能延后变量定义式的出现时间

> 原因：
>
> 1. 对于自定义数据类型，在定义的过程中，会有构造和析构的消耗
> 2. 并且，对于开辟在堆区的数据内存，若相应的delete被以某种形式跳过。则可能会造成内存泄漏

![image-20210417100759764](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210417100759764.png)

### 尽量少做转型动作

#### C风格转型

```c++
//旧式转型
(T)expression;  //将expression转型为T
//函数风格的转型看起来像：通过拷贝构造创建了一个新的对象
T(expression);  
```

#### C++提供的四种新式转型

![image-20210417101225233](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210417101225233.png)

> 使用新式转型的好处：
>
> 1. 容易在代码中被辨识出来
> 2. 各转型动作的目标越明确，编译器越可能诊断出错误的运用

![image-20210417101452432](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210417101452432.png)

### 避免返回handles（reference，指针，迭代器）指向对象内部成分

> 对象内部的成分：对象成员中不被公开使用的成员函数，成员变量（都应该是private)

1. 成员变量的封装性最多只等于“返回其reference”的函数的访问级别
2. 如果const成员函数传出一个reference，后者所指数据与对象自身又关联，而它又被存储于对象之外，那么这个函数的调用者可以修改那笔数据
3. 返回一个“代表对象内部数据”的handle，随之而来的是“降低对象封装性”的风险。并且，可能让原先声明为`const`的成员函数却造成对象状态的更改

![image-20210417103822787](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210417103822787.png)

![image-20210417104339315](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210417104339315.png)



![image-20210417104100135](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210417104100135.png)

### 为“异常安全”而努力是值得的

![image-20210417105138223](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210417105138223.png)

```c++
//设计一个class来表现夹带背景图案的GUI菜单，这个class希望用于多线程环境。所以他有个互斥器（mutex）作为并发控制之用
class PrettyMenu{
public:
    void changeBackground(std::istream& imaSrc); //改变背景图像
private:
    Mutex mutex;
    Image* bgImage;
    int imageChanges;
};
//changeBackground()的一种实现
void PrettyMenu::changeBackground(std::istream& imgSrc)
{
	lock(&mutex); //取得互斥器
    delete bgImage; //删除旧的背景图像
    ++imageChanges; 
    bgImage = new Image(imagSrc);
    unlock(&mutex); //释放互斥器
}
```

> 上述代码存在的问题：
>
> 1. 可能存在资源泄露：一旦new失败，那么mutex就不会被释放，造成互斥器占用
> 2. 数据败坏问题：一旦new失败，会导致bgImage指向一个被删除的对象，imageChanges也被累加

```c++
//资源泄露解决方案
void PrettyMenu::changeBackground(std::istream& imgSrc)
{
	Lock ml(&mutex); //条款14：利用资源管理类的构造和析构自动管理资源
    delete bgImage; //删除旧的背景图像
    ++imageChanges; 
    bgImage = new Image(imagSrc);
}
```

#### 异常安全的函数三个保证

1. **基本承诺：**如果异常被抛出，程序内的任何事物仍然保持在有效状态下，没有任何对象或数据结构会因此而败坏

2. **强烈保证**：如果异常被抛出，程序状态不改变。如果函数成功，就是完全成功，如果函数失败，程序会回复到“调用函数之前”的状态

   ![image-20210417111244899](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210417111244899.png)

3. **不抛掷保证（nothorw），**承诺绝不抛出异常，因为他们总是能够完成他们原先承诺的功能

![image-20210417110037532](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210417110037532.png)

![image-20210417111046473](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210417111046473.png)

> copy-and-swap的关键在于：修改对象数据的副本，然后在一个不抛出异常的函数中将修改后的数据和原件置换

![image-20210417111352613](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210417111352613.png)

### 透彻了解inlining的里里外外

> inline函数：可以调用他们却不需蒙受函数调用所招致的额外开销（堆栈区的内存）
>
> inline函数的整体观念是：“对此函数的每一个调用”都以函数本体替换之
>
> 在class中的函数被隐喻声明为`inline`（包括定义在class内部的friend函数）
>
> inline函数通常一定被置于头文件内，因为大多数build environments在编译过程中进行inlining，而为了将一个函数调用替换为`被调用函数的本体`，编译器必须知道那个函数长什么样子

![image-20210419102311774](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210419102311774.png)



![image-20210419102742570](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210419102742570.png)

### 将文件间的编译依存关系降至最低

> 编译依存关系指的是：当前头文件的一些定义，需要引用一些其他的自定义头文件
>
> 如果这些头文件中有一个发生改变，或者这些头脑文件所依赖的其他头文件有任何改变。那么任何一个含入当前class的文件也得重新编译
>
> **利用声明的依存性替换定义的依存性**
>
> 也就是cpp中`.h和.cpp分开编写`的缘由

![image-20210419103126260](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210419103126260.png)

#### 为什么需要前置声明

> 前置声明中的问题：编译器必须在编译期间知道对象的大小
>
> 对于自定义数据类型Person，编译器需要通过`访问Person class的定义式`来确定一个Person对象的大小

![image-20210419105414416](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210419105414416.png)

![image-20210419105439856](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210419105439856.png)

![image-20210419105744003](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210419105744003.png)

#### 结论

1. 如果使用object references或者object pointers可以完成任务，就不要使用objects本身。

   > 一个类型声明式就可以定义出指向该类型的references或者pointers，但是如果定义某个类型的objects，就需要用到该类型的定义式

![image-20210419110349888](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210419110349888.png)

![image-20210419110700475](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210419110700475.png)

## 继承于面向对象设计

![image-20210420100924055](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210420100924055.png)

### 确定你的public继承塑模出is-a关系

> 在C++进行面向对象编程，最重要的一个规则是：public inheritance 意味着 `is - a`的关系
>
> `public继承`意味着`is-a`，适用于base classes身上的每一件事情也一定适用于derived classes身上。因为每一个derived class对象也都是一个base class对象

![image-20210420101402684](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210420101402684.png)![image-20210420101618132](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210420101618132.png)

> 这种情况只针对`public`继承才成立
>
> 在很多时候，A通过public继承并不合理。

![image-20210420102047078](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210420102047078.png)

### 避免掩盖继承而来的名称

> derived class作用域被嵌套在base class作用域内
>
> 1. **derive classes内的名称会遮掩base classes内的名称，在public继承下从来没有人希望如此**
> 2. **为了让被遮掩的名称再见天日，可使用`using`声明式或转交函数**

![image-20210420104353880](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210420104353880.png)

![image-20210420105105854](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210420105105854.png)

![image-20210420105312826](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210420105312826.png)

![image-20210420123707157](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210420123707157.png)

![image-20210420123957752](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210420123957752.png)



### 区分接口继承和实现继承

![image-20210420170822637](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210420170822637.png)

> 如果成员函数是一个`non-virtual`函数，意味着它并不打算在derived classes中有不同的行为。
>
> 声明一个非虚函数的目的是为了让派生类继承函数的接口及一份强制性实现，意味着在derived class中不应该被重新定义

![image-20210420173458643](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210420173458643.png)

![image-20210420173607866](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210420173607866.png)

![image-20210420173623965](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210420173623965.png)

### 考虑virtual函数以外的其他选择

**借由Non-Virtual Interface手法实现Template Method模式（NVI手法）**

> 通过public的non-virtual function调用一个private的virtual函数，实现相关功能

![image-20210420193758793](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210420193758793.png)

### 绝不重新定义继承而来的non-virtual函数

> non-virtual函数是静态绑定的，这意味着，即使父类指针指向的是一个子类对象，但是通过父类指针调用的永远是静态绑定的版本
>
> virtual函数是动态绑定的，所以通过父类指针还是派生类指针都是调用的派生类的对象

### 绝不重新定义继承而来的缺省参数值

![image-20210420203212427](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210420203212427.png)

![image-20210420203317470](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210420203317470.png)

### 通过复合塑模出has-a或根据某物实现出

> 复合是类型之间的一种关系，当某种类型对象内含它种类型对象时，就是这样的关系
>
> 同义词：分层、内含、聚合、内嵌

### 明智而审慎地使用private继承

> public继承意味着is-a的关系，对于base class有效的行为，对于derived class也应该有效
>
> 而private继承并不意味is-a关系。如果class之间的继承关系是private，编译器不会自动将一个derived class对象转换为base class对象
>
> **由private base class继承而来的所有成员，在derived class中都会变成private属性，即使他们在base class中原本是protected或public属性**
>
> private继承意味着只有实现部分被继承，接口部分应略去（`尽可能使用复合，必要时才使用private继承`）
>
> **必要时候：** 当面对“并不存在is-a关系”的两个classes，其中一个需要访问另一个的protected成员，或需要重新定义其中一个或多个virtual函数时，private继承是个很好的选择方式

![image-20210421103321670](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210421103321670.png)

![image-20210421103629497](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210421103629497.png)

### 明智而审慎地使用多重继承

> 多重继承的意思是继承一个以上的base classes，而当这些被继承的base classes又有更高级的相同的base class，就会产生`钻石型多重继承`

```c++
//c++多重继承的语法
class BorrowableItem{
public:
    void checkOut();
};

class ElectronicGadget{
private:
    bool checkOot() const;
};

class MP3Player: public BorrowableItem, public ElectronicGadget{
    
}; //多重继承

MP3Player mp;
mp.checkOut(); //歧义，调用的是哪个checkout？
mp.BorrowableItem::checkOut(); //正确
```

![image-20210421105254932](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210421105254932.png)

![image-20210421110754005](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210421110754005.png)

## 模板与泛型编程（Templates and Generic Programming）

### 了解隐式接口和编译期多态

> 面向对象编程世界总是以`显式接口`和`运行期多态`解决问题

![image-20210421113337102](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210421113337102.png)

### 了解`typename`的双重意义

> 在template声明式中，class和template意义完全相同
>
> 但是在有时候必须得使用typename，但不得在base class lists或member initialization list内以它作为base class修饰符
>
> ```c++
> template<typename C>
> void print2nd(const C& container)
> {
>     if(container.size() >= 2)
>     {
>         C::const_iterator iter(container.begin())；
>         ++iter;
>         cout << *iter;
>     }
> }
> ```
>
> 1. template内出现的名称如果依赖于某个template参数，称为从属名称
> 2. 如果从属名称在class内呈嵌套状，称为嵌套从属名称`C::const_iterator`（实际上还是一个嵌套从属类型名称）
> 3. 嵌套从属名称可能会导致解析困难，因为如果解析器在template中遭遇一个嵌套从属名称，它便假设这个名称不是个类型。*即默认情况下，嵌套从属名称不是类型*
>
> **一般性规则：任何时候当想要在template中指涉一个嵌套从属类型名称，就必须在紧邻它的前一个位置放上关键字`typename`(有另一个例外)**
>
> ![image-20210422100602008](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210422100602008.png)
>
> ![image-20210422100651690](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210422100651690.png)
>
> ```c++
> template<typename IterT>
> void workWithIterator(IterT iter)
> {
>     typename std::iterator_traits<IterT>::value_type temp(*iter);
>     //对traits成员名称进行typedef
>     typedef typename std::iterator_traits<IterT>::value_type value_type;
>     //typedef typename:指涉嵌套丛书类型名称
> }
> ```

### 学习处理模板化基类内的名称

![image-20210422102330192](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210422102330192.png)

### 将与参数无关的代码抽离templates

![image-20210422103842768](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210422103842768.png)

### 运用成员函数模板接受所有兼容类型

![image-20210422104320019](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210422104320019.png)

![image-20210422104714100](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210422104714100.png)

### 需要类型转换时请为模板定义非成员函数

> template在实参推导过程中，并不会将隐式转换函数纳入考虑
>
> **当我们编写一个class template，而它所提供的与此template相关的函数需要支持`所有参数类型隐式转换时`，将这些函数定义为`class template内部的friend函数`**
>
> 因为定义于class内部的所有函数都是inlining，包括friend函数。因此，在必要的时候，可以将inline的friend函数作空实现，只是调用一个定义于class外部的辅助函数

![image-20210422105340467](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210422105340467.png)

![image-20210422105716033](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210422105716033.png)

![image-20210422110001946](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210422110001946.png)

> 但是存在编译问题连接问题：在template中声明的函数，应该定义该函数。而不是在class template外部去寻找定义式

### 请使用traits classes表现类型信息

1. STL的迭代器分类

![image-20210422111313581](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210422111313581.png)

![image-20210422112428250](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210422112428250.png)

![image-20210422112902094](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210422112902094.png)

![image-20210422112930770](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210422112930770.png)

### 认识template元编程(template metaprogramming)

> TMP的C++程序可能在每一方面都更高效：较小的可执行文件、较短的运行期、较少的内存需求。
>
> 但是将工作从运行期转移到编译器的另一个结果是，编译时间变长了
>
> `traits解法就是一种TMP`

![image-20210422145109029](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210422145109029.png)

> `enum hacks`

## 定制new和delete

> operator new和operator delete只适合用来分配单一对象，arrays所用的内存由`operator new[]`来分配并且由`delete[]`释放

### 了解new-handler的行为

![image-20210424105846420](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210424105846420.png)

```c++
//以下是operator new无法分配足够内存时，该被调用的函数
void outOfMem()
{
    std::cerr << "Unable to statisfy request for memory\n";
    std::abort();
}

int main()
{
    std::set_new_handler(outOfMem);
    int* pBigDataArray = new int[1000000000L];
}
```

### 了解new和delete的合理替换时机

**替换理由：**

![](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210424110548087.png)

![image-20210424110642114](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210424110642114.png)

#### 内存对齐

![image-20210424111105795](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210424111105795.png)

![image-20210424111316339](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210424111316339.png)

#### 何时可在“全局性的”或“class专属的”基础上合理替换缺省的new和delete

![image-20210424111511533](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210424111511533.png)

![image-20210424111557338](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210424111557338.png)

![image-20210424111629228](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210424111629228.png)

### 编写new和delete时需固守常规

![image-20210424111856534](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210424111856534.png)

```c++
//一个non-member operator new伪码
void* operator new(std::size_t size) throw(std::bad_alloc)
{
    using namespace std;
    if(size == 0) size = 1; //C++规定即使客户要求0bytes，也会返回一个合法指针
    while(true)
    {
        尝试分配size bytes;
        if(分配成功) return (一个指针：指向分配得来的内存);
        
        //分配失败:找出目前的new-handling函数
        new_handler globalHandler = set_new_handler(0);
        set_new_handler(globalHandler);
        
        if(globalHandler) (*globalHandler)();
        else thorw std::bad_alloc();
    }
}
```

![image-20210424112534884](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210424112534884.png)

![image-20210424112637714](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210424112637714.png)

#### 定制array new即operator new[]的注意事项

![image-20210424113540086](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210424113540086.png)

![image-20210424113616090](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210424113616090.png)

#### 重载operator new的注意事项

![image-20210424113705967](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210424113705967.png)

### 写了placement new 也要写placement delete

> placement new和placement delete的定义：
>
> 如果operator new接受的参数除了一定会有的那个size_t之外还有其他，这就可以被称为一个placement new
>
> 如果operator delete接受额外参数，那么就称为placement deletes
>
> **要求**：**在提供placement new的时候，提供一个正常的operator delete用于构造期间无任何异常被抛出，和一个placement delete用于构造期间又异常被抛出`后者的额外参数必须和operator new一样`**

![image-20210424145849126](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210424145849126.png)

![image-20210424150015892](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210424150015892.png)

#### **声明在class中的new隐藏问题**

![image-20210424150344442](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210424150344442.png)

> 在缺省情况下，C++在global作用域内提供以下形式的operator new
>
> 如果在class内声明operator news，他会遮掩global中的标准形式
>
> 完成上述所言的一个简单做法是：建立一个base class，内含所有正常形式的new和delete
>
> 凡是想以自定形式扩充标准形式的客户，`可以通过public继承并使用using取得标准形式。`

![image-20210424150652124](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210424150652124.png)

## 杂项讨论

> 争取让自己的程序无警告，或者在忽略每个警告的时候，知道警告的意义，发生原因

<img src="C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210424151429522.png" alt="image-20210424151429522" style="zoom:67%;" />



# 自己的理解

## 关于宏定义`#define`

1. 无法利用宏定义创建一个class专属常量，因为#defines并不重视作用域
2. 一旦宏被定义，在其后的编译过程中都有效（除非被#undefine）
3. #defines不仅不能够用来定义class专属常量，也不能够提供任何封装性

## 关于const关键字

## 构造函数与析构函数

1. 尽量将构造函数写为`参数列表初始化`

   - 尽量在初始化列表中列出所有的成员变量
   - 声明次序为罗列次序

   ```c++
   //拷贝构造
   ABEntry::ABEntry (const std::string& name, const std::string& address)
       	:theName(name), theAddress(address)
   {} //这些也都是初始化操作，而非赋值操作
   
   ABEntry::ABEntry (const std::string& name, const std::string& address) 	
   {
       theName = name;
       theAddress = address;
   } //非赋值操作
   ```

2. default构造函数的写法

   ```c++
   //default构造函数
   ABEntry::ABEntry ():theName(), theAddress()
   {} //这些也都是初始化操作，而非赋值操作
   
   ```

### 成员初始化次序

1. 基类
2. 派生类
3. 成员变量总是以其声明的次序被初始化

### 析构函数的运作方式

析构函数的运作方式是：

1. 最深层派生类的析构函数最先被调用，

2. 其次是每一个基类的析构函数。

   `编译器会在抽象父类的派生类的析构函数中创建一个对抽象父类的析构函数的调用，因此必须为这个函数提供一份定义`
   
3. class析构函数（无论是编译器生成的还是用户自定的）会自动调用其non-static成员变量的析构函数

## static关键字

### static对象

static对象寿命从构造出来到程序结束为止

- 全局的static对象
- namespace作用域内的对象
- classes，func，file作用域内的static对象

函数体内的static对象成为`local static`对象。其他的成为`non-local static对象`程序结束时会被自动销毁，即`在main()函数结束的时候自动调用析构函数`

**但是c++对于定义在不同的编译单元内的non-local static对象的初始化次序没有明确的定义**

> 解决这一问题：将non-local转为local static，即将每个`non-local static`对象搬到自己的专属函数内，然后在该函数内声明该对象为`static`,再返回一个reference指向它所含的对象

```c++
//解决跨编译单元之non-local static的初始化次序问题
class FileSystem 
{
	std::size_t numDisks() const;
};
FileSystem& tfs()
{
    static FileSystem fs;
    return fs;
}

class Directory {
    std::size_t disks = tfs().numsDisks(); //改动
};

Directory& tempDir()
{
    static Directory td;
    return td;
}
```

## 友元friend

### friend class

A friend class can access private and protected members of other class in which it is declared as friend. It is sometimes useful to allow a particular class to access private members of other class. For example, a LinkedList class may be allowed to access private members of Node. 

```c++
class Node {
private:
	int key;
	Node* next;
	/* Other members of Node Class */
// Now class LinkedList can
// access private members of Node
friend class LinkedList;
};
```


### friend function

**Friend Function** Like friend class, a friend function can be given a special grant to access private and protected members. A friend function can be: 
a) A member of another class 
b) A global function

```c++
class Node {
private:
	int key;
	Node* next;
/* Other members of Node Class */
friend int LinkedList::search();
// Only search() of linkedList
// can access internal members
};
```
**Following are some important points about friend functions and classes:** 
**1)** Friends should be used only for limited purpose. **too many functions or external classes are declared as friends of a class with protected or private data,** it lessens the value of encapsulation of separate classes in object-oriented programming.
**2)** **Friendship is not mutual.** If class A is a friend of B, then B doesn’t become a friend of A automatically.
**3)** Friendship is **not inherited** (See [this ](https://www.geeksforgeeks.org/g-fact-34/)for more details)
**4)** The concept of friends is not there in Java. 
**A simple and complete C++ program to demonstrate friend Class** 

## 虚函数virtual

**任何class只要带有virtual都几乎确定应该有一个virtual析构函数。**当一个class不包含virtual函数的时候，通常表明它太会被用来充当基类

> 心得：只有当class至少有一个virtual函数的时候，才为他声明virtual析构函数

### 为什么不要要无端将析构函数声明为virtual析构函数

如果该class不准备被用来充当base class，那么就不要将其成员函数包括析构函数声明为virtual

这是因为，c++在实现虚函数机制的过程中，对象必须携带虚函数表指针，指向一个由函数指针构成的数组

当对象调用某一个虚函数的时候，实际上取决于vptr指向的虚函数指针

**最重要的是，会增加对象的大小**

### 纯虚函数

带有纯虚函数的class是一个抽象类，不能实例化，即不能为之创建对象。并且一个抽象类通常被用来充当父类，而父类又要求有一个virtual析构函数。因此

**将希望成为抽象类的析构函数声明为纯虚析构函数**

```c++
class MOV{
public:
    virtual ~MOV() = 0; //声明为纯虚析构函数
};
//并为纯虚析构函数函数提供一份定义
MOV::~MOV()
{
    
}
```

![image-20210420171952543](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210420171952543.png)

> 声明非纯虚函数的目的：让derived classes继承该函数的接口和缺省实现
>
> 但是在继承虚函数实现的时候，如果在base class中已经对该虚函数进行重载，那么在derived class中选择部分重写后，需要通过`using声明`来默认继承其他的实现

#### 纯虚函数的两个特性

1. 纯虚函数要求任何`继承了它们`的具象class重新晟敏

2. 而且他们在抽象class中通常没有定义

3. 声明一个纯虚函数的目的是为了让派生类只继承函数接口

   > 我们仍然可以为pure virtual函数提供定义，但是在调用的过程中，必须明确指出其class名称

   ![image-20210420172346311](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210420172346311.png)

## 多态的理解

![image-20210407221129498](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210407221129498.png)

<img src="C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210407221859772.png" alt="image-20210407221859772" style="zoom: 67%;" />

### 多态的理解2

> 如上所述，当父类的成员函数被声明为virtual的时候，多态特性就已经建立。在此后，无论是继承还是继承的继承类也好。只要通过父类指针指向派生类的对象，都会调用覆盖的版本
>
> 但是，如果在对于没有声明为virtual的成员函数，多态特性仍然不成立

![image-20210414155140363](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210414155140363.png)

### 多态的理解3

> 在父类中声明为virtual的成员函数，如果在派生类中有对其进行重写，那么父类指针就会调用该重写版本的函数。而如果没有重写该版本，则默认调用父类的virtual版本成员函数

## tr1::shared_ptr

![image-20210413221944169](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210413221944169.png)

## 定义式和声明式

![image-20210419103833591](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210419103833591.png)

![image-20210419104615004](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210419104615004.png)

## empty class

![image-20210421103824544](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210421103824544.png)

> 在大多数编译器中，`sizeof(empty)`大小为1，因为面对“大小为零的独立（非附属）对象“，通常C++官方会勒令安插一个char到空对象内，

```c++
class Empty{

};

void TestEmpty()
{
    Empty e;
    std::cout << "size of empty class:" << sizeof(e) << std::endl;
    //size of empty class:1
}
```

## 钻石型多重继承（菱形继承）

![image-20210421105651572](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210421105651572.png)

> **思考问题：**
>
> 1. 是否打算让base class内的成员变量经由每一条路径被复制？那么每个对象应该有两份fileName成员变量（`缺省做法`）
> 2. IOFile对象只该有一个文件名称，所以它继承自两个base classes而来的fileName不该重复。**如果要实现这一操作，应该要令那个带有此数据的class（File）成为一个virtual base class，并且令所有直接继承自它的classes采用virtual继承**
>
> ```c++
> class File{...};
> class InputFile: virtual public File {...};
> class OutputFile: virtual public File {...};
> class IOFile: public InputFile, public OutputFile {};
> ```

![image-20210421110311580](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210421110311580.png)

![image-20210421110740326](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210421110740326.png)

## 面向对象编程vs泛型编程

> 面向对象编程世界总是以`显示接口和运行期多态解决问题`
>
> 而Templates及泛型编程的世界，与面向对象有根本的不同，在此世界中显示接口和运行期多态仍然存在，但是重要性降低。反倒是`隐式接口和编译器多态`占据主要地位

```c++
//面向对象编程
class Wdiget{
public:
    Widget();
    virtual ~Widget();
    virtual std::size_t size() const;
    virtual void normalize();
    virtual swap(Widget& other);
};

void doProcessing(Widget& w)
{
    if(w.size() > 10 && w != someNastyWidget){
        Widget temp(w);
        temp.normalize();
        temp.swap(w);
    }
}
/*
由于w的类型被声明为Widget，所以w必须支持Widget接口，这些接口称为显示接口
在源码中明确可见
其次：由于Widget的部分成员函数是virtual，w对于那些函数的调用将表现出运行期多态
*/

//Templates及泛型编程
template<typename T>
void doProcessing(T& w)
{
    if(w.size() > 10 && w != someNastyWidget)
    {
        T temp(w);
        temp.normalize();
        temp.swap(w);
    }
}
/*
w必须支持哪一种接口，是有template中执行于w身上的操作来决定的。
本例中，类型T好像必须支持size，normalize和swap成员函数，copy Ctor,以及operator!=
这一组表达式就是T必须支持的隐式接口
凡涉及w的任何函数调用，比如operator>和operator!=有可能造成template具现化行为发生在编译器
“以不同的template参数具现化function templates”会调用不同的函数，这便是所谓的编译器多态
*/
```



### 运行期多态和编译期多态

1. 运行期多态：由于base class的某些成员函数是virtual，因此handle对于那些函数的调用将表现出`运行期多态（runtime polymorphism）`也就是在运行期间根据w的动态类型决定究竟调用哪个函数
2. 编译器多态：“以不同的template参数具现化function templates”会调用不同的函数，这便是所谓的编译器多态

> 哪一个重载函数被调用（发生在编译器）和哪一个virtual函数该被绑定（发生在编译器）

### 隐式接口和显示接口

![image-20210421112925902](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210421112925902.png)

![image-20210421113054515](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210421113054515.png)

![image-20210421113200199](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210421113200199.png)

## c++的traits是什么？

![image-20210422111933259](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210422111933259.png)

![image-20210422112145253](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210422112145253.png)

![image-20210422112110689](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210422112110689.png)

**对于指针（也是一种迭代器）的实现：偏特化版本**

![image-20210422112234255](C:\Users\唐昆\AppData\Roaming\Typora\typora-user-images\image-20210422112234255.png)

### 函数重载的特性

**重载机制的应用：advance，根据迭代器类型做不同的工作**

当重载某个函数f时，必须详细叙述各个重载件的参数类型，但调用f时，编译器便根据传来的实参选择最适当的重载件。

编译器态度是：如果这个重载件最匹配传递过来的实参，就调用这个f，如果那个重载件最匹配，就调用那个f。

**这正是编译期间的条件语句**



