#include <iostream>
#include <sstream>
using namespace std;
class LeaveHandler
{
public:
    virtual void handleRequest(const string &name, int days) = 0;
};
class Supervisor : public LeaveHandler
{
private:
    static const int MAX_DAYS_SUPERVISOR_CAN_APPROVE = 3;
    LeaveHandler *m_nextHandler;

public:
    Supervisor(LeaveHandler *nextHandler) : m_nextHandler(nextHandler) {}
    virtual void handleRequest(const string &name, int days)
    {
        if (days <= MAX_DAYS_SUPERVISOR_CAN_APPROVE)
        {
            cout << name << " Approved by Supervisor." << endl;
        }
        else if (m_nextHandler != nullptr)
        {
            m_nextHandler->handleRequest(name, days);
        }
        else
        {
            cout << name << " Denied by Supervisor." << endl;
        }
    }
};
class Manager : public LeaveHandler
{
private:
    static const int MAX_DAYS_MANAGER_CAN_APPROVE = 7;
    LeaveHandler *m_nextHandler;

public:
    Manager(LeaveHandler *nextHandler) : m_nextHandler(nextHandler) {}
    virtual void handleRequest(const string &name, int days)
    {
        if (days <= MAX_DAYS_MANAGER_CAN_APPROVE)
        {
            cout << name << " Approved by Manager." << endl;
        }
        else if (m_nextHandler != nullptr)
        {
            m_nextHandler->handleRequest(name, days);
        }
        else
        {
            cout << name << " Denied by Manager." << endl;
        }
    }
};
class Director : public LeaveHandler
{
private:
    static const int MAX_DAYS_DIRECTOR_CAN_APPROVE = 10;
    LeaveHandler *m_nextHandler;

public:
    Director(LeaveHandler *nextHandler = nullptr) : m_nextHandler(nextHandler) {}
    virtual void handleRequest(const string &name, int days)
    {
        if (days <= MAX_DAYS_DIRECTOR_CAN_APPROVE)
        {
            cout << name << " Approved by Director." << endl;
        }
        else if (m_nextHandler != nullptr)
        {
            m_nextHandler->handleRequest(name, days);
        }
        else
        {
            cout << name << " Denied by Director." << endl;
        }
    }
};
class LeaveRequest
{
private:
    string m_name;
    int m_days;

public:
    LeaveRequest(const string &name, int days) : m_name(name), m_days(days) {}
    string getName() const
    {
        return m_name;
    }
    int getDays() const
    {
        return m_days;
    }
};
int main()

{
    int count;
    cin >> count;
    cin.ignore();
    LeaveHandler *director = new Director();
    LeaveHandler *manager = new Manager(director);
    LeaveHandler *supervisor = new Supervisor(manager);
    while (count-- > 0)
    {
        string input;
        getline(cin, input);
        istringstream in_str(input);
        string name;
        int days;
        if (in_str >> name >> days)
        {
            LeaveRequest request(name, days);
            supervisor->handleRequest(name, days);
        }
        else
        {
            cout << "Invalid input" << endl;
            return -1;
        }
    }
    return 0;
}