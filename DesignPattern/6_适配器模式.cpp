#include <iostream>
using namespace std;
// USB 接口
class USB
{
public:
    virtual void charge() = 0;
};
// TypeC 接口
class TypeC
{
public:
    virtual void chargeWithTypeC() = 0;
};
// // 适配器类
// class TypeCAdapter : public USB
// {
// private:
//     TypeC *m_typeC;

// public:
//     TypeCAdapter(TypeC *typeC) : m_typeC(typeC) {}
//     virtual void charge() override
//     {
//         m_typeC->chargeWithTypeC();
//     }
// };
// 新电脑类，使用 TypeC 接口
class NewComputer : public TypeC
{
public:
    virtual void chargeWithTypeC() override
    {
        cout << "TypeC" << endl;
    }
};
// 适配器充电器类，使用 USB 接口
class AdapterCharger : public USB
{
public:
    virtual void charge() override
    {
        cout << "USB Adapter" << endl;
    }
};
int main()
{
    int count;
    cin >> count;
    for (int i = 0; i < count; i++)
    {
        int choice;
        cin >> choice;
        if (1 == choice)
        {
            TypeC *newComputer = new NewComputer();
            newComputer->chargeWithTypeC();
            delete newComputer;
        }
        else if (2 == choice)
        {
            USB *usbAdapter = new AdapterCharger();
            usbAdapter->charge();
            delete usbAdapter;
        }
    }
    return 0;
}