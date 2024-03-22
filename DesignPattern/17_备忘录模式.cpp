#include <iostream>
#include <stack>
using namespace std;
// 备忘录
class Memento
{
private:
    int m_value;

public:
    Memento(int value) : m_value(value) {}
    int getValue() const
    {
        return m_value;
    }
};
// 发起人（Originator）
class Counter
{
private:
    int m_value;
    stack<Memento> undoStack;
    stack<Memento> redoStack;

public:
    void increment()
    {
        // 清空 redoStack
        redoStack = stack<Memento>();
        undoStack.push(Memento(m_value));
        m_value++;
    }

    void decrement()
    {
        // 清空 redoStack
        redoStack = stack<Memento>();
        undoStack.push(Memento(m_value));
        m_value--;
    }
    void undo()
    {
        if (!undoStack.empty())
        {
            redoStack.push(Memento(m_value));
            m_value = undoStack.top().getValue();
            undoStack.pop();
        }
    }
    void redo()
    {
        if (!redoStack.empty())
        {
            undoStack.push(Memento(m_value));
            m_value = redoStack.top().getValue();
            redoStack.pop();
        }
    }
    int getValue() const
    {
        return m_value;
    }
};
int main()
{
    Counter counter;
    string operation;
    // 处理计数器应用的输入
    while (cin >> operation)
    {
        if (operation == "Increment")
        {
            counter.increment();
        }
        else if (operation == "Decrement")
        {
            counter.decrement();
        }
        else if (operation == "Undo")
        {
            counter.undo();
        }
        else if (operation == "Redo")
        {
            counter.redo();
        }
        // 输出当前计数器的值
        cout << counter.getValue() << endl;
    }
    return 0;
}