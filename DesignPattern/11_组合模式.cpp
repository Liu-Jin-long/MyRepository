#include <iostream>
#include <vector>
#include <sstream>
using namespace std;
class Component
{
public:
    virtual void display(int depth) = 0;
};
class Department : public Component
{
private:
    string m_name;
    vector<Component *> m_children;

public:
    Department(const string &name) : m_name(name) {}
    void add(Component *component)
    {
        m_children.push_back(component);
    }
    virtual void display(int depth) override
    {
        string indent(depth * 2, ' ');
        cout << indent << m_name << endl;
        for (Component *compoent : m_children)
        {
            compoent->display(depth + 1);
        }
    }
};
class Employee : public Component
{
private:
    string m_name;
    Department *m_root;

public:
    Employee(const string &name) : m_name(name), m_root(new Department(name)) {}
    virtual void display(int depth) override
    {
        string indent(++depth * 2, ' ');
        cout << indent << m_name << endl;
    }
};
class Company
{
private:
    string m_name;
    Department *m_root;

public:
    Company(const string &name) : m_name(name), m_root(new Department(name)) {}
    void add(Component *component)
    {
        m_root->add(component);
    }
    void display()
    {
        cout << "Company Structure:" << endl;
        m_root->display(0);
    }
};
int main()
{
    string companyName;
    getline(cin, companyName);
    Company company(companyName);
    int count;
    cin >> count;
    cin.ignore();
    for (int i = 0; i < count; i++)
    {
        string type, name;
        cin >> type;
        getline(cin >> ws, name);
        if (type == "D")
        {
            Department *department = new Department(name);
            company.add(department);
        }
        else if (type == "E")
        {
            Employee *employee = new Employee(name);
            company.add(employee);
        }
    }
    company.display();
    return 0;
}
