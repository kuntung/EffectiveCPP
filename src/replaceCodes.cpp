#include <algorithm>
#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include <typeinfo>
using namespace std;

class Person{
public:
    Person(const std::string& name, int age, float height): _mName(name), _mAge(age), _mHeight(height)
    {
        cout << "parameters initialization" << endl;
    }
    
    void showInfo() const //����const���͵ĳ�Ա����
    {
        cout << "Name: " << this->_mName << "age: " << this->_mAge << "Height: " << this->_mHeight << endl;
    }

    float getHei() const //��̬��Ա������ֻ������ȡ��Ա����
    {
        return this->_mHeight;
    }

    string getName() const//��ȡ�����ӿ�, ��̬
    {
        return this->_mName;
    }
private:
    string _mName;
    int _mAge;
    float _mHeight;
};

void showPerson(const vector<Person>& vPerson)
{
    //����for_each��ӡ��ص�Person��Ϣ
    for_each(vPerson.begin(), vPerson.end(), [](const Person& tp){
        tp.showInfo();
    });
}

void genPerson(vector<Person>& vPerson)
{
    string nameSeed = "ABCDE";
    int ageSeed[] = {18, 17, 20, 24, 21};
    float heiSeed[] = {165.3, 125.2, 174.2, 184.7, 155.3};
    for(int i = 0; i < nameSeed.length(); ++i)
    {
        string name = string("small")+ nameSeed[i];
        int age = ageSeed[i];
        float height = heiSeed[i];
        vPerson.push_back(Person(name, age, height));
    }

    return;
}

class replaceRule{
public:
    bool operator()(const Person& p){
        return p.getHei() > 155;
    }
};
void replacePerson(vector<Person>& vPerson)
{
    int lower = 155;
    cout << "ReplaceName with predicate" << endl;    
    replace_if(vPerson.begin(), vPerson.end(), replaceRule(), Person("smallPig", 25, 178.3));
    showPerson(vPerson);
    cout << "ReplaceName with lambda func" << endl;
    string newName = "smallPig";
    replace_if(vPerson.begin(), vPerson.end(), 
        [newName](const Person& tp){ return tp.getName() == newName; }, Person("smallDog", 25, 178.3));
    showPerson(vPerson);

    return;
}


void test01()
{
    vector<Person> vTest;
    genPerson(vTest);
    showPerson(vTest); //��ӡ��ʾ��ʼ���ɵ�Person����
    replacePerson(vTest); //�滻����������person
}
int main(){

    cout << "begin test01" << endl;
    test01();

    // cout << typeid(string("small")).name() << endl;
    return 0;
}