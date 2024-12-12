#include "testclass.h"

#include <list>
#include <memory>
#include <iostream>

// 模板类 EventBus，负责管理事件的注册、注销以及事件的触发
template <typename EventClass>
class EventBus
{
public:
    // 构造函数，使用默认构造函数
    EventBus() = default;

    // 析构函数，使用默认析构函数
    ~EventBus() = default;

    // 静态方法，用于注册事件
    // 参数为 std::weak_ptr 指向的事件对象，防止循环引用
    static void Register(std::weak_ptr<EventClass> event)
    {
        // 遍历已注册的事件，防止重复注册相同事件
        for (std::weak_ptr<EventClass> & registeredEvent : registeredEvents_)
        {
            // 使用 weak_ptr 的 lock() 方法提升为 shared_ptr
            std::shared_ptr<EventClass> registeredEventPtr = registeredEvent.lock();
            std::shared_ptr<EventClass> eventPtr           = event.lock();

            // 如果注册的事件和传入的事件相同，输出日志并跳过注册
            if (registeredEventPtr == eventPtr)
            {
                std::cout << "Skip: event already registered!" << std::endl;
                return;
            }
        }
        // 将事件添加到事件列表中
        registeredEvents_.emplace_back(event);
    }

    // 静态方法，用于注销事件
    // 参数为 std::weak_ptr 指向的事件对象
    static void Unregister(std::weak_ptr<EventClass> event)
    {
        // 遍历已注册的事件
        for (auto eventIt = registeredEvents_.begin(); eventIt != registeredEvents_.end(); ++eventIt)
        {
            // 将 weak_ptr 提升为 shared_ptr
            std::shared_ptr<EventClass> registeredEventPtr = eventIt->lock();
            std::shared_ptr<EventClass> eventPtr           = event.lock();

            // 如果找到匹配的事件，则从列表中移除该事件
            if (registeredEventPtr == eventPtr)
            {
                registeredEvents_.erase(eventIt);
                return;
            }
        }
    }

    // 静态模板方法，用于触发事件
    // 参数 fun 为成员函数指针，args 为可变参数
    template <typename EventFunction, typename... EventArgs>
    static void OnEvent(EventFunction eventFunction, EventArgs &&... args)
    {
        // 遍历已注册的事件
        for (const std::weak_ptr<EventClass> & registeredEvent : registeredEvents_)
        {
            // 将 weak_ptr 提升为 shared_ptr
            std::shared_ptr<EventClass> eventPtr = registeredEvent.lock();

            // 如果事件不存在（已销毁），输出错误日志并跳过
            if (!eventPtr)
            {
                std::cerr << "Skip: event no longer exists!" << std::endl;
                continue;
            }

            // 使用成员函数指针触发事件，调用事件对象的成员函数
            EventClass * eventObject = eventPtr.get();
            (eventObject->*eventFunction)(std::forward<EventArgs>(args)...);
        }
    }

private:
    // 静态成员变量，存储所有注册的事件对象（使用 weak_ptr 以避免循环引用）
    static std::list<std::weak_ptr<EventClass>> registeredEvents_;
};

// 初始化静态成员变量
template <typename EventClass>
std::list<std::weak_ptr<EventClass>> EventBus<EventClass>::registeredEvents_;

// 能否修改为发布订阅的事件总线,接口参数为:
// publish("event_code"，QVariant var);
// int ret = subscribe("event_code"，调用函数(QVariant var)); //ret为回调存根，每个回调+1，不重复
// 订阅处的 subscribe 同时有注册功能，
// unsubscribe(ret);取消回调并取消注册，能实现吗？
