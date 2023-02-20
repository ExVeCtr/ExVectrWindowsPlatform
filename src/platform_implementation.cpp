#include <chrono>
#include <iostream>

#include "ExVectrCore.hpp"
#include "ExVectrCore/time_base.hpp"
#include "ExVectrCore/clock_source.hpp"
#include "ExVectrCore/topic_subscribers.hpp"
#include "ExVectrCore/scheduler2.hpp"
#include "ExVectrCore/task_types.hpp"
#include "ExVectrCore/print.hpp"


namespace 
{
    VCTR::Core::ListArray<char> printList;
} // namespace 


void printOutE(const char *const &item)
{
    printList.append('!');
    printList.append('!');
    printList.append('!');
    printList.append('E');
    printList.append('R');
    printList.append('R');
    printList.append('O');
    printList.append('R');
    printList.append(':');
    printList.append(' ');

    size_t len = strnlen(item, 10000);
    for (size_t i = 0; i < len; i++) {
        printList.append(item[i]);
    }
};

void printOutW(const char *const &item)
{
    printList.append('!');
    printList.append('W');
    printList.append('A');
    printList.append('R');
    printList.append('N');
    printList.append('I');
    printList.append('N');
    printList.append('G');
    printList.append(':');
    printList.append(' ');

    size_t len = strnlen(item, 10000);
    for (size_t i = 0; i < len; i++) {
        printList.append(item[i]);
    }
};

void printOutM(const char *const &item)
{
    size_t len = strnlen(item, 10000);
    for (size_t i = 0; i < len; i++) {
        printList.append(item[i]);
    }
};

/**
 * Does platform specific stuff like setting up the print output.
 */
class PlatformWindows : public VCTR::Core::Task_Periodic
{
public:
    VCTR::Core::StaticCallback_Subscriber<const char *> printSubE;
    VCTR::Core::StaticCallback_Subscriber<const char *> printSubW;
    VCTR::Core::StaticCallback_Subscriber<const char *> printSubM;

    PlatformWindows() : VCTR::Core::Task_Periodic("Platform Windows Task", 1 * VCTR::Core::MILLISECONDS, 0)
    {

        VCTR::Core::getSystemScheduler().addTask(*this);
        setPriority(1000);

        printSubM.subscribe(VCTR::Core::getMessageTopic());
        printSubM.setCallbackFunction(printOutM);
        printSubM.setTaskToResume(*this);
        printSubW.subscribe(VCTR::Core::getWarningTopic());
        printSubW.setCallbackFunction(printOutW);
        printSubW.setTaskToResume(*this);
        printSubE.subscribe(VCTR::Core::getErrorTopic());
        printSubE.setCallbackFunction(printOutE);
        printSubE.setTaskToResume(*this);
    }

    void taskInit() override
    {
        setPaused(true);
    }

    void taskThread() override
    {
        printList.append(0);
        std::cout << printList.getPtr();
        printList.clear();
        setPaused(true);
    }

};

class PlatformClock : public VCTR::Core::Clock_Source
{
public:
    const VCTR::Core::Timestamped<int64_t> &getCounter() const override
    {

        static auto lastTime = std::chrono::steady_clock::now(); // std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now()).count();
        static int64_t timeCount = 0;
        static VCTR::Core::Timestamped<int64_t> timeStamped;

        auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - lastTime).count();
        if (time > 0)
        {
            lastTime = std::chrono::steady_clock::now();
            timeCount += time;
        }

        timeStamped.data = timeStamped.timestamp = timeCount;

        return timeStamped;
    }
};

const VCTR::Core::Clock_Source &VCTR::Core::getPlatformClock()
{

    static PlatformClock clockSource;

    return clockSource;
}

void VCTR::Core::initialise() {

    static PlatformWindows platformWindows;

}