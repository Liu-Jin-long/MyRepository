#include <iostream>
#include <vector>
#include <unordered_map>
#include <sstream>
using namespace std;
enum ShapeType
{
    CIRCLE,
    RECTANGLE,
    TRIANGLE
};
string shapeTypeToString(ShapeType type)
{
    switch (type)
    {
    case CIRCLE:
        return "CIRCLE";
    case RECTANGLE:
        return "RECTANGLE";
    case TRIANGLE:
        return "TRIANGLE";
    default:
        return "UNKNOWN";
    }
}
class Position
{
private:
    int m_x;
    int m_y;

public:
    Position(int x, int y) : m_x(x), m_y(y) {}
    int getX() const
    {
        return m_x;
    }
    int getY() const
    {
        return m_y;
    }
};
class Shape
{
public:
    virtual void draw(const Position &position) = 0;
    virtual ~Shape() = default;
};
class ConcreteShape : public Shape
{
private:
    ShapeType m_shapeType;
    bool m_isFirstTime;

public:
    ConcreteShape(ShapeType shapeType) : m_shapeType(shapeType), m_isFirstTime(true) {}
    virtual void draw(const Position &position) override
    {
        cout << shapeTypeToString(m_shapeType) << (m_isFirstTime ? " drawn" : " shared")
             << " at (" << position.getX() << ", " << position.getY() << ")" << endl;
    }
    void setFirstTime(bool firstTime)
    {
        m_isFirstTime = firstTime;
    }
};
class ShapeFactory
{
private:
    unordered_map<ShapeType, Shape *> shapes;

public:
    Shape *getShape(ShapeType type)
    {
        if (shapes.find(type) == shapes.end())
        {
            shapes[type] = new ConcreteShape(type);
        }
        return shapes[type];
    }
    ~ShapeFactory()
    {
        for (const auto &entry : shapes)
        {
            delete entry.second;
        }
    }
};
void processCommand(ShapeFactory &factory, const string &command)
{
    istringstream istr(command);
    string shapeTypeStr;
    int x, y;
    istr >> shapeTypeStr >> x >> y;
    ShapeType type;
    if (shapeTypeStr == "CIRCLE")
    {
        type = CIRCLE;
    }
    else if (shapeTypeStr == "RECTANGLE")
    {
        type = RECTANGLE;
    }
    else if (shapeTypeStr == "TRIANGLE")
    {
        type = TRIANGLE;
    }
    else
    {
        std::cerr << "Invalid shape type: " << shapeTypeStr << std::endl;
        return;
    }
    Shape *shape = factory.getShape(type);
    shape->draw(Position(x, y));
    dynamic_cast<ConcreteShape *>(shape)->setFirstTime(false);
}
int main()
{
    ShapeFactory factory;
    string command;
    while (getline(cin, command))
    {
        processCommand(factory, command);
    }
    return 0;
}
