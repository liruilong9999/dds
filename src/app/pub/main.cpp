#include <ddscxx/dds/dds.hpp> // 必须包含 Cyclone DDS 的头文件
#include <example/example.hpp>

using namespace org::eclipse::cyclonedds;

// 主函数，程序入口
int main(int argc, char * argv[])
{
    std::cout << "=== [Publisher] Create writer." << std::endl;

    // 创建域参与者
    dds::domain::DomainParticipant participant(domain::default_id());

    std::cout << "domain_id: " << participant.domain_id() << std::endl;

    // 创建主题
    dds::topic::Topic<test::Data> topic(participant, "HelloWorldData_Msg");

    // 创建发布者
    dds::pub::Publisher publisher(participant);

    // 创建数据写入器
    dds::pub::DataWriter<test::Data> writer(publisher, topic);

    std::cout << "=== [Publisher] Waiting for subscriber." << std::endl;
    // 等待订阅者连接
    while (writer.publication_matched_status().current_count() == 0)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    // 创建消息
    test::Data msg(1, "Hello World");

    /* Write the message. */
    std::cout << "=== [Publisher] Write sample." << std::endl;
    // 写入消息
    writer.write(msg);

    std::cout << "=== [Publisher] Waiting for sample to be accepted." << std::endl;
    // 等待消息被接受
    while (writer.publication_matched_status().current_count() > 0)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    return 0;
}
