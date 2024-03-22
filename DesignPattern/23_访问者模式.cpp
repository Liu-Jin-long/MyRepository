#include <iostream>
#include <cmath>
#include <vector>
using namespace std;
// 访问者接口
class Visitor
{
public:
    virtual void visit(class Circle &circle) = 0;
    virtual void visit(class Rectangle &rectangle) = 0;
};
// 元素接口
class Shape
{
public:
    virtual ~Shape() = default;
    virtual void accept(Visitor &visitor) = 0;
};
// 具体元素类
class Circle : public Shape
{
private:
    int m_radius;

public:
    Circle(int radius) : m_radius(radius){};
    int getRadius() const
    {
        return m_radius;
    }
    virtual void accept(Visitor &visitor) override
    {
        visitor.visit(*this);
    }
};
// 具体元素类
class Rectangle : public Shape
{
private:
    int m_width;
    int m_height;

public:
    Rectangle(int width, int height) : m_width(width), m_height(height){};
    int getWidth() const
    {
        return m_width;
    }
    int getHeight() const
    {
        return m_height;
    }
    virtual void accept(Visitor &visitor) override
    {
        visitor.visit(*this);
    }
};
// 具体访问者类
class AreaCalculator : public Visitor
{
public:
    virtual void visit(Circle &circle) override
    {
        double area = 3.14 * pow(circle.getRadius(), 2);
        cout << area << endl;
    }
    virtual void visit(Rectangle &rectangle) override
    {
        double area = rectangle.getHeight() * rectangle.getWidth();
        cout << area << endl;
    }
};
// 对象结构类
class Drawing
{
private:
    vector<Shape *> m_shapes;

public:
    Drawing(const vector<Shape *> &shapes) : m_shapes(shapes) {}
    void accept(Visitor &visitor)
    {
        for (Shape *shape : m_shapes)
        {
            shape->accept(visitor);
        }
    }
};
int main()
{
    int count;
    cin >> count;
    vector<Shape *> shapes;
    while (count-- > 0)
    {
        string type;
        cin >> type;
        if (type == "Circle")
        {
            int radius;
            cin >> radius;
            shapes.push_back(new Circle(radius));
        }
        else if (type == "Rectangle")
        {
            int width, height;
            cin >> width >> height;
            shapes.push_back(new Rectangle(width, height));
        }
        else
        {
            cout << "Invalid input" << endl;
            return -1;
        }
    }
    Drawing drawing(shapes);
    AreaCalculator areaCalculator;
    drawing.accept(areaCalculator);
    for (Shape *shape : shapes)
    {
        delete shape;
    }
    return 0;
}