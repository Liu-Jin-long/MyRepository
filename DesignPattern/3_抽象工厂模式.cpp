#include <iostream>
using namespace std;
// 抽象椅子接口
class AbstractChair
{
public:
    virtual void showInfo() = 0;
};
// 具体现代风格椅子
class ModernChair : public AbstractChair
{
public:
    virtual void showInfo() override
    {
        cout << "modern chair" << endl;
    }
};
// 具体古典风格椅子
class ClassicalChair : public AbstractChair
{
public:
    virtual void showInfo() override
    {
        cout << "classical chair" << endl;
    }
};
// 抽象沙发接口
class AbstractSofo
{
public:
    virtual void displayInfo() = 0;
};
// 具体现代风格沙发
class ModernSofo : public AbstractSofo
{
public:
    virtual void displayInfo() override
    {
        cout << "modern chair" << endl;
    }
};
// 具体古典风格沙发
class ClassicalSofo : public AbstractSofo
{
public:
    virtual void displayInfo() override
    {
        cout << "classical chair" << endl;
    }
};
// 抽象家居工厂接口
class FurnitureFactory
{
public:
    virtual AbstractChair *createChair() = 0;
    virtual AbstractSofo *createSofo() = 0;
};
// 具体现代风格家居工厂
class ModernFurnitureFactory : public FurnitureFactory
{
public:
    AbstractChair *createChair() override
    {
        return new ModernChair();
    }
    AbstractSofo *createSofo() override
    {
        return new ModernSofo();
    }
};
// 具体古典风格家居工厂
class ClassicalFurnitureFactory : public FurnitureFactory
{
public:
    AbstractChair *createChair() override
    {
        return new ClassicalChair();
    }
    AbstractSofo *createSofo() override
    {
        return new ClassicalSofo();
    }
};
int main()
{
    // 读取订单数量
    int count;
    cin >> count;
    // 处理每个订单
    for (int i = 0; i < count; ++i)
    {
        // 读取家具类型
        string furnitureType;
        cin >> furnitureType;
        // 创建相应风格的家居装饰品工厂
        FurnitureFactory *factory = nullptr;
        if (furnitureType == "modern")
        {
            factory = new ModernFurnitureFactory();
        }
        else if (furnitureType == "classical")
        {
            factory = new ClassicalFurnitureFactory();
        }
        // 根据工厂生产椅子和沙发
        AbstractChair *chair = factory->createChair();
        AbstractSofo *sofa = factory->createSofo();
        chair->showInfo();
        sofa->displayInfo();
        delete chair;
        delete sofa;
        delete factory;
    }
    return 0;
}