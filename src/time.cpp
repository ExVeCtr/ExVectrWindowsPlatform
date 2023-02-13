#include <chrono>

#include "ExVectrCore/time_base.hpp"
#include "ExVectrCore/clock_source.hpp"


class PlatformClock : public VCTR::Core::Clock_Source {
public:

    const VCTR::Core::Timestamped<int64_t>& getCounter() const override {

        static auto lastTime = std::chrono::steady_clock::now();//std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now()).count();
        static int64_t timeCount = 0;

        auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - lastTime).count();
        if (time > 0) {
            lastTime = std::chrono::steady_clock::now();
            timeCount += time;
        }

        return timeCount;

    }

};

const VCTR::Core::Clock_Source& VCTR::Core::getPlatformClock() {

    static PlatformClock clockSource;

    return clockSource;

}