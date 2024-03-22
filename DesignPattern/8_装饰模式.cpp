#include <iostream>
#include <memory>
using namespace std;
// 咖啡接口
class Coffee
{
public:
    virtual ~Coffee() {}
    virtual void brew() = 0;
};
// 具体的黑咖啡类
class BlackCoffee : public Coffee
{
public:
    virtual void brew() override
    {
        cout << "Brewing Black Coffee" << endl;
    }
};
// 具体的拿铁类
class LatteCoffee : public Coffee
{
public:
    virtual void brew() override
    {
        cout << "Brewing Latte Coffee" << endl;
    }
};
// 装饰者抽象类
class Decorator : public Coffee
{
protected:
    unique_ptr<Coffee> m_coffee;

public:
    Decorator(unique_ptr<Coffee> coffee) : m_coffee(move(coffee)) {}
    virtual void brew() override
    {
        if (m_coffee)
        {
            m_coffee->brew();
        }
    }
};
// 具体的牛奶装饰者类
class MilkDecorator : public Decorator
{
public:
    MilkDecorator(unique_ptr<Coffee> coffee) : Decorator(move(coffee)) {}
    virtual void brew() override
    {
        Decorator::brew();
        if (m_coffee)
            cout << "Adding Milk" << endl;
    }
};
// 具体的糖装饰者类
class SugarDecorator : public Decorator
{
public:
    SugarDecorator(unique_ptr<Coffee> coffee) : Decorator(move(coffee)) {}
    virtual void brew() override
    {
        Decorator::brew();
        if (m_coffee)
            cout << "Adding Suger" << endl;
    }
};
int main()
{
    int coffeeType, condimentType;
    while (cin >> coffeeType >> condimentType)
    {
        // 根据输入制作咖啡
        unique_ptr<Coffee> coffee;
        if (1 == coffeeType)
        {
            coffee = make_unique<BlackCoffee>();
        }
        else if (2 == coffeeType)
        {
            coffee = make_unique<LatteCoffee>();
        }
        else
        {
            cout << "Invalid coffee type" << endl;
            continue;
        }
        // 根据输入添加调料
        if (1 == condimentType)
        {
            coffee = make_unique<MilkDecorator>(move(coffee));
        }
        else if (2 == condimentType)
        {
            coffee = make_unique<SugarDecorator>(move(coffee));
        }
        else
        {
            cout << "Invalid condiment type" << endl;
            continue;
        }
        // 输出制作过程
        coffee->brew();
    }
    return 0;
}