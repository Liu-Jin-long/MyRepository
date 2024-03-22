#include <iostream>
#include <string>
#include <memory>
using namespace std;
// 抽象类
class CoffeeMakerTemplate
{
private:
    string m_coffeeName;

public:
    // 构造函数，接受咖啡名称参数
    CoffeeMakerTemplate(const string &coffeeName) : m_coffeeName(coffeeName) {}
    // 模板方法定义咖啡制作过程
    virtual void makeCoffee()
    {
        cout << "Making " << m_coffeeName << ":" << endl;
        grindCoffeeBeans();
        brewCoffee();
        addCondiments();
    }
    // 具体步骤的具体实现由子类提供
    virtual void grindCoffeeBeans() = 0;
    virtual void brewCoffee() = 0;
    // 添加调料的默认实现
    virtual void addCondiments()
    {
        cout << "Adding condiments" << endl;
    }
};
// 具体的美式咖啡类
class AmericanCoffeeMaker : public CoffeeMakerTemplate
{
public:
    // 构造函数传递咖啡名称
    AmericanCoffeeMaker() : CoffeeMakerTemplate("American Coffee") {}
    virtual void grindCoffeeBeans() override
    {
        cout << "Grinding coffee beans" << endl;
    }
    virtual void brewCoffee() override
    {
        cout << "Brewing coffee" << endl;
    }
};
// 具体的拿铁咖啡类
class LatteCoffeeMaker : public CoffeeMakerTemplate
{
public:
    // 构造函数传递咖啡名称
    LatteCoffeeMaker() : CoffeeMakerTemplate("Latte") {}
    virtual void grindCoffeeBeans() override
    {
        cout << "Grinding coffee beans" << endl;
    }
    virtual void brewCoffee() override
    {
        cout << "Brewing coffee" << endl;
    }
    // 添加调料的特定实现
    virtual void addCondiments() override
    {
        cout << "Adding milk" << endl;
        cout << "Adding condiments" << endl;
    }
};
int main()
{
    unique_ptr<CoffeeMakerTemplate> coffeeMaker;
    int coffeeType;
    while (cin >> coffeeType)
    {
        if (1 == coffeeType)
        {
            coffeeMaker = make_unique<AmericanCoffeeMaker>();
        }
        else if (2 == coffeeType)
        {
            coffeeMaker = make_unique<LatteCoffeeMaker>();
        }
        else
        {
            cout << "Invalid coffee type\n";
            continue;
        }
        // 制作咖啡
        coffeeMaker->makeCoffee();
    }
    return 0;
}