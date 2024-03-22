#include <iostream>
#include <vector>
#include <map>
#include <list>
using namespace std;
// 抽象中介者
class ChatRoomMediator;
// 抽象同事类
class ChatUser
{
private:
    string m_name;
    ChatRoomMediator *m_mediator;
    list<string> m_receivedMessages;

public:
    ChatUser(const string &name, ChatRoomMediator *mediator);
    string getName() const
    {
        return m_name;
    }
    void sendMessage(const string &message);
    virtual void receiveMessage(const string &sender, const string &message) = 0;
    list<string> getReceivedMessages() const
    {
        return m_receivedMessages;
    }

protected:
    void addReceivedMessage(const string &message)
    {
        m_receivedMessages.push_back(message);
    }
};
// 具体同事类
class ConcreteChatUser : public ChatUser
{
public:
    ConcreteChatUser(const string &sender, ChatRoomMediator *mediator) : ChatUser(sender, mediator) {}
    virtual void receiveMessage(const string &sender, const string &message) override
    {
        string receivedMessage = getName() + " received: " + message;
        addReceivedMessage(receivedMessage);
        cout << receivedMessage << endl;
    }
};
// 抽象中介者
class ChatRoomMediator
{
public:
    virtual void sendMessage(const string &sender, const string &message) = 0;
    virtual void addUser(ChatUser *user) = 0;
    virtual map<string, ChatUser *> getUsers() = 0;
    virtual ~ChatRoomMediator() = default;
};
// 具体中介者
class ChatRoomMediatorImpl : public ChatRoomMediator
{
private:
    map<string, ChatUser *> m_users;

public:
    virtual void sendMessage(const string &sender, const string &message) override
    {
        for (const pair<string, ChatUser *> &userPair : m_users)
        {
            if (userPair.first != sender)
            {
                userPair.second->receiveMessage(sender, message);
            }
        }
    }
    virtual void addUser(ChatUser *user) override
    {
        m_users[user->getName()] = user;
    }
    virtual map<string, ChatUser *> getUsers() override
    {
        return m_users;
    }
};
// 实现 ChatUser 类的成员函数
ChatUser::ChatUser(const string &name, ChatRoomMediator *mediator) : m_name(name), m_mediator(mediator)
{
    mediator->addUser(this);
}
void ChatUser::sendMessage(const string &message)
{
    m_mediator->sendMessage(m_name, message);
}
int main()
{
    vector<string> userNames;
    int count;
    cin >> count;
    while (count-- > 0)
    {
        string userName;
        cin >> userName;
        userNames.push_back(userName);
    }
    ChatRoomMediator *mediator = new ChatRoomMediatorImpl();
    for (const auto &userName : userNames)
    {
        new ConcreteChatUser(userName, mediator);
    }
    string sender, message;
    while (cin >> sender >> message)
    {
        ChatUser *user = mediator->getUsers()[sender];
        if (user != nullptr)
        {
            user->sendMessage(message);
        }
    }
    for (const auto &user : mediator->getUsers())
    {
        delete user.second;
    }
    delete mediator;
    return 0;
}