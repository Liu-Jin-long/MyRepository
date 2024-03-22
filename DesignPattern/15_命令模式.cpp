#include <iostream>
#include <vector>
#include <string>
using namespace std;
class DrinkMaker; // 前向声明
// 命令接口
class Command
{
public:
    virtual void execte() = 0;
    virtual ~Command() = default;
};
// 具体命令类 - 点餐命令
class OrderCommand : public Command
{
private:
    string m_drinkName;
    DrinkMaker *m_receiver; // 使用前向声明

public:
    OrderCommand(const string &drinkName, DrinkMaker *receiver) : m_drinkName(drinkName), m_receiver(receiver) {}
    virtual void execte() override;
};
// 接收者类 - 制作饮品
class DrinkMaker
{
public:
    void makeDrink(const string &drinkName)
    {
        cout << drinkName << " is ready!" << endl;
    }
};
// 实现 OrderCommand 的构造函数和 execute 函数
void OrderCommand::execte()
{
    m_receiver->makeDrink(m_drinkName);
}
// 调用者类 - 点餐机
class OrderMachine
{
private:
    Command *m_command;

public:
    void setCommand(Command *command)
    {
        m_command = command;
    }
    void executeOrder()
    {
        m_command->execte();
    }
};
int main()
{
    // 创建接收者和命令对象
    DrinkMaker drinkMaker;
    int count;
    cin >> count;
    while (count-- > 0)
    {
        string drinkName;
        cin >> drinkName;
        Command *command = new OrderCommand(drinkName, &drinkMaker);
        // 执行命令
        OrderMachine orderMachine;
        orderMachine.setCommand(command);
        orderMachine.executeOrder();
        delete command;
    }
    return 0;
}
