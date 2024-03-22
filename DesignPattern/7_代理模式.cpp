#include <iostream>
using namespace std;
// 抽象主题
class HomePurchase
{
public:
    virtual void requestPurchase(int area) = 0;
};
// 真实主题
class HomeBuyer : public HomePurchase
{
public:
    virtual void requestPurchase(int area)
    {
        cout << "YES" << endl;
    }
};
// 代理类
class HomeAgentProxy : public HomePurchase
{
private:
    HomeBuyer homeBuyer;

public:
    virtual void requestPurchase(int area)
    {
        if (area > 100)
        {
            homeBuyer.requestPurchase(area);
        }
        else
        {
            cout << "NO" << endl;
        }
    }
};
int main()
{
    HomeAgentProxy *buyerProxy = new HomeAgentProxy();
    int count;
    cin >> count;
    for (int i = 0; i < count; i++)
    {
        int area;
        cin >> area;
        buyerProxy->requestPurchase(area);
    }
    delete buyerProxy;
    return 0;
}