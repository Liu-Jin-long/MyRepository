#include <iostream>
#include <vector>
#include <string>
using namespace std;
// 状态接口
class State
{
public:
    // 处理状态的方法
    virtual string handle() = 0;
};
// 具体状态类
class OnState : public State
{
public:
    virtual string handle() override
    {
        return "Light is ON";
    }
};
class OffState : public State
{
public:
    virtual string handle() override
    {
        return "Light is OFF";
    }
};
class BlinkState : public State
{
public:
    virtual string handle() override
    {
        return "Light is Blinking";
    }
};
// 上下文类
class Light
{
private:
    // 当前状态
    State *m_state;

public:
    // 初始状态为关闭
    Light() : m_state(new OffState()) {}
    // 设置新的状态
    void setState(State *newState)
    {
        // 释放之前的状态对象
        if (m_state != nullptr)
        {
            delete m_state;
        }
        m_state = newState;
    }
    // 执行当前状态的操作
    string performOperation()
    {
        return m_state->handle();
    }
    ~Light()
    {
        delete m_state;
    }
};
int main()
{
    int count;
    cin >> count;
    Light light;
    while (count-- > 0)
    {
        string command;
        cin >> command;
        // 根据命令执行相应的操作
        if (command == "ON")
        {
            light.setState(new OnState());
        }
        else if (command == "OFF")
        {
            light.setState(new OffState());
        }
        else if (command == "BLINK")
        {
            light.setState(new BlinkState());
        }
        else
        {
            cout << "Invalid command: " << command << endl;
        }
        // 在每个命令后显示当前状态
        cout << light.performOperation() << endl;
    }
    return 0;
}