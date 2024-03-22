#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
// 观察者接口
class Observer
{
public:
    virtual void update(int hour) = 0;
    virtual ~Observer() = default;
};
// 主题接口
class Subject
{
public:
    virtual void registerObserver(Observer *observer) = 0;
    virtual void removeObserver(Observer *observer) = 0;
    virtual void notifyObservers() = 0;
    virtual ~Subject() = default;
};
// 具体主题实现
class Clock : public Subject
{
private:
    vector<Observer *> m_observers;
    int m_hour;

public:
    Clock() : m_hour(0) {}
    virtual void registerObserver(Observer *observer) override
    {
        m_observers.push_back(observer);
    }
    virtual void removeObserver(Observer *observer) override
    {
        vector<Observer *>::iterator it = find(m_observers.begin(), m_observers.end(), observer);
        if (it != m_observers.end())
        {
            m_observers.erase(it);
        }
    }
    virtual void notifyObservers() override
    {
        for (Observer *observer : m_observers)
        {
            observer->update(m_hour);
        }
    }
    // 添加获取观察者的函数
    const vector<Observer *> &getObservers() const
    {
        return m_observers;
    }
    void tick()
    {
        //模拟时间的推移
        m_hour = (m_hour + 1) % 24;
        notifyObservers();
    }
};
// 具体观察者实现
class Student : public Observer
{
private:
    string m_name;

public:
    Student(const string &name) : m_name(name) {}
    virtual void update(int hour) override
    {
        cout << m_name << " " << hour << endl;
    }
};
int main()
{
    int count;
    cin >> count;
    Clock clock;
    // 注册学生观察者
    for (int i = 0; i < count; i++)
    {
        string studentName;
        cin >> studentName;
        clock.registerObserver(new Student(studentName));
    }
    // 读取时钟更新次数
    int updates;
    cin >> updates;
    // 模拟时钟每隔一个小时更新一次
    for (int i = 0; i < updates; i++)
    {
        clock.tick();
    }
    for (Observer *observer : clock.getObservers())
    {
        delete observer;
    }
    return 0;
}