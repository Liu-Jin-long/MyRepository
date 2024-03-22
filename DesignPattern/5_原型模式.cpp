#include <iostream>
#include <vector>
using namespace std;
// 抽象原型类
class Prototype
{
public:
    virtual Prototype *clone() const = 0;
    virtual string getDetails() const = 0;
    virtual ~Prototype(){};
};
// 具体矩形原型类
class RectanglePrototype : public Prototype
{
private:
    string m_color;
    int m_width;
    int m_height;

public:
    RectanglePrototype(string color, int width, int height) : m_color(color), m_width(width), m_height(height) {}
    virtual Prototype *clone() const override
    {
        return new RectanglePrototype(*this);
    }
    virtual string getDetails() const override
    {
        return "Color: " + m_color + ", Width: " + std::to_string(m_width) + ", Height: " + std::to_string(m_height);
    }
};
int main()
{
    vector<Prototype *> rectangles;
    int count;
    cin >> count;
    // 读取每个矩形的属性信息并创建矩形对象
    for (int i = 0; i < count; i++)
    {
        string color;
        int width, height;
        cin >> color >> width >> height;
        // 创建原型对象
        Prototype *originalRectangle = new RectanglePrototype(color, width, height);
        // 将原型对象保存到向量中
        rectangles.push_back(originalRectangle);
    }
    for (const auto &rectangle : rectangles)
    {
        // 克隆对象并输出详细信息
        RectanglePrototype *clonedRectangle = dynamic_cast<RectanglePrototype *>(rectangle->clone());
        cout << clonedRectangle->getDetails() << endl;
        delete clonedRectangle;
        delete rectangle;
    }
}