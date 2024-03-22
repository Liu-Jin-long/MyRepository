#include <iostream>
#include <vector>
using namespace std;
// 抽象积木接口
class AbstractBlock
{
public:
    virtual void produce() = 0;
};
// 具体圆形积木实现
class CircleBlock : public AbstractBlock
{
public:
    virtual void produce() override
    {
        cout << "Circle Block" << endl;
    }
};
// 具体方形积木实现
class SquareBlock : public AbstractBlock
{
public:
    virtual void produce() override
    {
        cout << "Square Block" << endl;
    }
};
//抽象积木工厂接口
class AbstractBlockFactory
{
public:
    virtual AbstractBlock *createBlock() = 0;
};
//具体圆形积木工厂实现
class CircleBlockFactory : public AbstractBlockFactory
{
public:
    virtual AbstractBlock *createBlock() override
    {
        return new CircleBlock();
    }
};
//具体方型积木工厂实现
class SquareBlockFactory : public AbstractBlockFactory
{
public:
    virtual AbstractBlock *createBlock() override
    {
        return new SquareBlock();
    }
};
//积木工厂系统
class BlockFactorySystem
{
private:
    vector<AbstractBlock *> blocks;

public:
    void produceBlock(AbstractBlockFactory *factory, int quantity)
    {
        for (int i = 0; i < quantity; ++i)
        {
            AbstractBlock *block = factory->createBlock();
            blocks.push_back(block);
            block->produce();
        }
    }
    const vector<AbstractBlock *> &getBlocks() const
    {
        return blocks;
    }
    ~BlockFactorySystem()
    {
        // 释放所有动态分配的积木对象
        for (AbstractBlock *&block : blocks)
        {
            if (block != nullptr)
            {
                delete block;
            }
        }
    }
};
int main()
{
    // 创建积木工厂系统
    BlockFactorySystem factorySystem;
    // 读取生产次数
    int produceCount;
    cin >> produceCount;
    // 读取每次生产的积木类型和数量
    for (int i = 0; i < produceCount; ++i)
    {
        string blockType;
        int quantity;
        cin >> blockType >> quantity;
        if (blockType == "Circle")
        {
            factorySystem.produceBlock(new CircleBlockFactory(), quantity);
        }
        else if (blockType == "Square")
        {
            factorySystem.produceBlock(new SquareBlockFactory(), quantity);
        }
    }
    return 0;
}
