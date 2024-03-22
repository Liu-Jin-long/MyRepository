#include <iostream>
#include <vector>
using namespace std;
class AirConditioner
{
public:
    void turnOff()
    {
        cout << "Air Conditioner is turned off." << endl;
    }
};
class DeskLamp
{
public:
    void turnOff()
    {
        cout << "Desk Lamp is turned off." << endl;
    }
};
class Television
{
public:
    void turnOff()
    {
        cout << "Television is turned off." << endl;
    }
};
class PowerSwitchFacade
{
private:
    AirConditioner airConditioner;
    DeskLamp deskLamp;
    Television television;

public:
    void turnOffDevice(int deviceCode)
    {
        switch (deviceCode)
        {
        case 1:
            airConditioner.turnOff();
            break;
        case 2:
            deskLamp.turnOff();
            break;
        case 3:
            television.turnOff();
            break;
        case 4:
            cout << "All devices are off." << endl;
            break;
        default:
            cout << "Invalid device code." << endl;
        }
    }
};
int main()
{
    int count;
    cin >> count;
    vector<int> inputs(count);
    for (int i = 0; i < count; i++)
    {
        cin >> inputs[i];
    }
    PowerSwitchFacade powerSwitchFacade;
    for (int i = 0; i < count; i++)
    {
        powerSwitchFacade.turnOffDevice(inputs[i]);
    }
    return 0;
}