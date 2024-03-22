#include <iostream>
#include <sstream>
using namespace std;
// 步骤1: 创建实现化接口
class TV
{
public:
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
    virtual void switchChannel() = 0;
    virtual ~TV() = default;
};
// 步骤2: 创建具体实现化类
class SonyTV : public TV
{
public:
    virtual void turnOn() override
    {
        cout << "Sony TV is ON" << endl;
    }
    virtual void turnOff() override
    {
        cout << "Sony TV is OFF" << endl;
    }
    virtual void switchChannel() override
    {
        cout << "Switching Sony TV channel" << endl;
    }
};
class TCLTV : public TV
{
public:
    virtual void turnOn() override
    {
        cout << "TCL TV is ON" << endl;
    }
    virtual void turnOff() override
    {
        cout << "TCL TV is OFF" << endl;
    }
    virtual void switchChannel() override
    {
        cout << "Switching TCL TV channel" << endl;
    }
};
// 步骤3: 创建抽象化接口
class RemoteControl
{
protected:
    TV *m_tv;

public:
    RemoteControl(TV *tv) : m_tv(tv) {}
    virtual void performOperation() = 0;
    virtual ~RemoteControl() = default;
};
// 步骤4: 创建扩充抽象化类
class PowerOperation : public RemoteControl
{
public:
    PowerOperation(TV *tv) : RemoteControl(tv) {}
    virtual void performOperation() override
    {
        m_tv->turnOn();
    }
};
class OffOperation : public RemoteControl
{
public:
    OffOperation(TV *tv) : RemoteControl(tv) {}
    virtual void performOperation() override
    {
        m_tv->turnOff();
    }
};
class ChannelSwitchOperation : public RemoteControl
{
public:
    ChannelSwitchOperation(TV *tv) : RemoteControl(tv) {}
    virtual void performOperation() override
    {
        m_tv->switchChannel();
    }
};
int main()
{
    int count;
    cin >> count;
    cin.ignore();
    for (int i = 0; i < count; i++)
    {
        string input;
        getline(cin, input);
        istringstream in_str(input);
        int brand, operation;
        in_str >> brand >> operation;
        TV *tv;
        if (0 == brand)
        {
            tv = new SonyTV();
        }
        else if (1 == brand)
        {
            tv = new TCLTV();
        }
        RemoteControl *remoteControl;
        if (2 == operation)
        {
            remoteControl = new PowerOperation(tv);
        }
        else if (3 == operation)
        {
            remoteControl = new OffOperation(tv);
        }
        else if (4 == operation)
        {
            remoteControl = new ChannelSwitchOperation(tv);
        }
        remoteControl->performOperation();
        delete tv;
        delete remoteControl;
    }
    return 0;
}