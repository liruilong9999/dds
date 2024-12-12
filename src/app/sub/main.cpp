#include <QApplication>
#include <ddscxx/dds/dds.hpp> // 必须包含 Cyclone DDS 的头文件
#include <example/example.hpp>

using namespace org::eclipse::cyclonedds;

// 主函数，程序入口
int main(int argc, char * argv[])
{
    QApplication a(argc, argv);
    std::cout << "=== [Subscriber] Create reader." << std::endl;

    // 创建 DDS 域参与者
    dds::domain::DomainParticipant participant(domain::default_id());
    std::cout << "domain_id: " << participant.domain_id() << std::endl;

    // 创建 DDS 主题
    dds::topic::Topic<test::Data> topic(participant, "HelloWorldData_Msg");

    // 创建 DDS 订阅者
    dds::sub::Subscriber subscriber(participant);

    // 创建 DDS 数据读取器
    dds::sub::DataReader<test::Data> reader(subscriber, topic);

    std::cout << "=== [Subscriber] Wait for message." << std::endl;
    bool poll = true;
    while (poll)
    {
        dds::sub::LoanedSamples<test::Data> samples;

        // 读取数据
        samples = reader.take();

        if (samples.length() > 0)
        {
            dds::sub::LoanedSamples<test::Data>::const_iterator sample_iter;
            for (sample_iter = samples.begin();
                 sample_iter < samples.end();
                 ++sample_iter)
            {
                const test::Data &           msg  = sample_iter->data();
                const dds::sub::SampleInfo & info = sample_iter->info();

                if (info.valid())
                {
                    std::cout << "=== [Subscriber] Message received:" << std::endl;
                    std::cout << "    userID  : " << msg.id() << std::endl;
                    std::cout << "    Message : \"" << msg.message() << "\"" << std::endl;

                    poll = false;
                }
            }
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    }
    int ret = a.exec();
    return ret;
}
