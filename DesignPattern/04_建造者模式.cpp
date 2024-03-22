#include <iostream>
using namespace std;
// 自行车产品
class Bike
{
public:
    string frame;
    string tires;
    void setFrame(const string &frame)
    {
        this->frame = frame;
    }
    void setTires(const string &tires)
    {
        this->tires = tires;
    }
    friend ostream &operator<<(ostream &out, const Bike &bike)
    {
        out << bike.frame << " " << bike.tires;
        return out;
    }
};
// 自行车建造者接口
class BikeBuilder
{
public:
    virtual void buildFrame() = 0;
    virtual void buildTires() = 0;
    virtual Bike getResult() = 0;
};
// 山地自行车建造者
class MountainBikeBuilder : public BikeBuilder
{
private:
    Bike bike;

public:
    virtual void buildFrame() override
    {
        bike.setFrame("Aluminum Frame");
    }
    virtual void buildTires() override
    {
        bike.setTires("Knobby Tires");
    }
    Bike getResult() override
    {
        return bike;
    }
};
// 公路自行车建造者
class RoadBikeBuilder : public BikeBuilder
{
private:
    Bike bike;

public:
    virtual void buildFrame() override
    {
        bike.setFrame("Carbon Frame");
    }
    virtual void buildTires() override
    {
        bike.setTires("Slim Tires");
    }
    Bike getResult() override
    {
        return bike;
    }
};
// 自行车Director，负责构建自行车
class BikeDirector
{
public:
    Bike construct(BikeBuilder &builder)
    {
        builder.buildFrame();
        builder.buildTires();
        return builder.getResult();
    }
};
int main()
{
    int count;
    cin >> count;
    BikeDirector director;
    for (int i = 0; i < count; i++)
    {
        string bikeType;
        cin >> bikeType;
        // 根据输入类别，创建不同类型的具体建造者
        BikeBuilder *builder = nullptr;
        if (bikeType == "mountain")
        {
            builder = new MountainBikeBuilder();
        }
        else if (bikeType == "road")
        {
            builder = new RoadBikeBuilder();
        }
        // Director负责指导生产产品
        Bike bike = director.construct(*builder);
        cout << bike << endl;
        delete builder;
    }
}