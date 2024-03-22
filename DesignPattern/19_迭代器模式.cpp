#include <iostream>
#include <vector>
using namespace std;
// 学生类
class Student
{
private:
    string m_name;
    string m_ID;

public:
    Student(const string &name, const string &ID) : m_name(name), m_ID(ID) {}
    string getName() const
    {
        return m_name;
    }
    string getStudentID() const
    {
        return m_ID;
    }
};
// 可迭代对象接口
class StudentCollection
{
public:
    virtual ~StudentCollection() = default;
    virtual vector<Student>::iterator begin() = 0;
    virtual vector<Student>::iterator end() = 0;
};
class ConcreteStudentCollection : public StudentCollection
{
private:
    vector<Student> m_students;

public:
    void addStudent(const Student &student)
    {
        m_students.push_back(student);
    }
    virtual vector<Student>::iterator begin() override
    {
        return m_students.begin();
    }
    virtual vector<Student>::iterator end() override
    {
        return m_students.end();
    }
};
int main()
{
    int count;
    cin >> count;
    ConcreteStudentCollection studentCollection;
    while (count-- > 0)
    {
        string name, studentID;
        cin >> name >> studentID;
        studentCollection.addStudent(Student(name, studentID));
    }
    for (auto it = studentCollection.begin(); it != studentCollection.end(); it++)
    {
        const Student &student = *it;
        cout << student.getName() << " " << student.getStudentID() << endl;
    }
    return 0;
}