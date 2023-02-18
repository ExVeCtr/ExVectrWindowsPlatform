#include <chrono>
#include <iostream>

#include "ExVectrCore/time_base.hpp"
#include "ExVectrCore/clock_source.hpp"
#include "ExVectrCore/topic_subscribers.hpp"
#include "ExVectrCore/scheduler2.hpp"
#include "ExVectrCore/task_types.hpp"
#include "ExVectrCore/print.hpp"


void printOutE(const char* const &item) {

    std::cout << "!!!ERROR: " << item;

};

void printOutW(const char* const &item) {

    std::cout << "!WARNING: " << item;

};

void printOutM(const char* const &item) {

    std::cout << item;

};


/**
 * Does platform specific stuff like setting up the print output.
*/
class PlatformWindows : public VCTR::Core::Task_Periodic {
public:

    VCTR::Core::StaticCallback_Subscriber<const char*> printSubE;
    VCTR::Core::StaticCallback_Subscriber<const char*> printSubW;
    VCTR::Core::StaticCallback_Subscriber<const char*> printSubM;

    PlatformWindows() : VCTR::Core::Task_Periodic("Platform Windows Task", 1*VCTR::Core::SECONDS, 0) {

        VCTR::Core::getSystemScheduler().addTask(*this);
        setPriority(1000);

        printSubM.subscribe(VCTR::Core::getMessageTopic());
        printSubM.setCallbackFunction(printOutM);
        printSubW.subscribe(VCTR::Core::getWarningTopic());
        printSubW.setCallbackFunction(printOutW);
        printSubE.subscribe(VCTR::Core::getErrorTopic());
        printSubE.setCallbackFunction(printOutE);

    }


    void taskInit() override {

        

    }


    void taskThread() override {



    }

} platformWindows;


class PlatformClock : public VCTR::Core::Clock_Source {
public:

    const VCTR::Core::Timestamped<int64_t>& getCounter() const override {

        static auto lastTime = std::chrono::steady_clock::now();//std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now()).count();
        static int64_t timeCount = 0;
        static VCTR::Core::Timestamped<int64_t> timeStamped;

        auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - lastTime).count();
        if (time > 0) {
            lastTime = std::chrono::steady_clock::now();
            timeCount += time;
        }

    	timeStamped.data = timeStamped.timestamp = timeCount;

        return timeStamped;

    }

};

const VCTR::Core::Clock_Source& VCTR::Core::getPlatformClock() {

    static PlatformClock clockSource;

    return clockSource;

}