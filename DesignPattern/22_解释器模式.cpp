#include <iostream>
#include <sstream>
#include <stack>
#include <vector>
#include <stdexcept>
#include <iterator>
#include <regex>
using namespace std;
// 抽象表达式类
class Expression
{
public:
    virtual int interpret() = 0;
    virtual ~Expression() = default;
};
// 终结符表达式类 - 数字
class NumberExpression : public Expression
{
private:
    int m_value;

public:
    NumberExpression(int value) : m_value(value) {}
    virtual int interpret() override
    {
        return m_value;
    }
};
// 非终结符表达式类 - 加法操作
class AddExpression : public Expression
{
private:
    Expression *m_left;
    Expression *m_right;

public:
    AddExpression(Expression *left, Expression *right) : m_left(left), m_right(right) {}
    virtual int interpret() override
    {
        return m_left->interpret() + m_right->interpret();
    }
};
// 非终结符表达式类 - 乘法操作
class MultiplyExpression : public Expression
{
private:
    Expression *m_left;
    Expression *m_right;

public:
    MultiplyExpression(Expression *left, Expression *right) : m_left(left), m_right(right) {}
    virtual int interpret() override
    {
        return m_left->interpret() * m_right->interpret();
    }
};
// 非终结符表达式类 - 操作符
class OperatorExpression : public Expression
{
private:
    string m_operator;

public:
    OperatorExpression(const string &op) : m_operator(op) {}
    virtual int interpret() override
    {
        throw runtime_error("OperatorExpression does not support interpretation");
    }
    string getOperator() const
    {
        return m_operator;
    }
};
// 解析表达式字符串
int parsrExpression(const string &expressionStr)
{
    istringstream in_str(expressionStr);
    vector<string> elements(istream_iterator<string>{in_str}, istream_iterator<string>());
    stack<Expression *> stack;
    for (const auto &element : elements)
    {
        if (regex_match(element, regex("\\d+")))
        {
            stack.push(new NumberExpression(stoi(element)));
        }
        else if (element == "+" || element == "*")
        {
            stack.push(new OperatorExpression(element));
        }
        else
        {
            throw invalid_argument("Invalid element in expression: " + element);
        }
    }
    while (stack.size() > 1)
    {
        Expression *right = stack.top();
        stack.pop();
        Expression *operatorExp = stack.top();
        stack.pop();
        Expression *left = stack.top();
        stack.pop();
        if (auto *opExp = dynamic_cast<OperatorExpression *>(operatorExp))
        {
            string op = opExp->getOperator();
            if (op == "+")
            {
                stack.push(new AddExpression(left, right));
            }
            else if (op == "*")
            {
                stack.push(new MultiplyExpression(left, right));
            }
        }
        else
        {
            throw invalid_argument("Invalid operator type in expression");
        }
    }
    int result = stack.top()->interpret();
    delete stack.top();
    return result;
}
int main()
{
    vector<string> input_lines;
    string line;
    while (getline(cin, line) && !line.empty())
    {
        input_lines.push_back(line);
    }
    for (size_t i = 0; i < input_lines.size(); i++)
    {
        try
        {
            int result = parsrExpression(input_lines[i]);
            cout << result << endl;
        }
        catch (const exception &e)
        {
            cout << "Error - " << e.what() << endl;
        }
    }
}