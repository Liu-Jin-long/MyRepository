#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
// 抽象购物优惠策略接口
class DiscountStrategy
{
public:
    virtual int applyDiscount(int originalPrice) = 0;
    virtual ~DiscountStrategy() = default;
};

// 九折优惠策略
class DiscountStrategy1 : public DiscountStrategy
{
public:
    virtual int applyDiscount(int originalPrice) override
    {
        return static_cast<int>(round(originalPrice * 0.9));
    }
};
// 满减优惠策略
class DiscountStrategy2 : public DiscountStrategy
{
private:
    int thresholds[4] = {100, 150, 200, 300};
    int discounts[4] = {5, 15, 25, 40};

public:
    virtual int applyDiscount(int originalPrice) override
    {
        for (int i = sizeof(thresholds) / sizeof(thresholds[0]) - 1; i >= 0; i--)
        {
            if (originalPrice >= thresholds[i])
            {
                return originalPrice - discounts[i];
            }
        }
        return originalPrice;
    }
};
// 上下文类
class DiscountContext
{
private:
    DiscountStrategy *m_discountStrategy;

public:
    void setDiscountStrategy(DiscountStrategy *discountStrategy)
    {
        m_discountStrategy = discountStrategy;
    }
    int applyDiscount(int originalPrice)
    {
        return m_discountStrategy->applyDiscount(originalPrice);
    }
};
int main()
{
    int count;
    cin >> count;
    for (int i = 0; i < count; i++)
    {
        int M, strategyType;
        cin >> M >> strategyType;
        // 根据优惠策略设置相应的打折策略
        DiscountStrategy *discountStrategy = nullptr;
        switch (strategyType)
        {
        case 1:
            discountStrategy = new DiscountStrategy1();
            break;
        case 2:
            discountStrategy = new DiscountStrategy2();
            break;
        default:
            std::cout << "Unknown strategy type" << std::endl;
            return -1;
        }
        // 设置打折策略
        DiscountContext context;
        context.setDiscountStrategy(discountStrategy);
        // 应用打折策略并输出优惠后的价格
        int discountedPrice = context.applyDiscount(M);
        cout << discountedPrice << endl;
        delete discountStrategy;
    }
    return 0;
}
