//
// Created by chiheb on 01/05/24.
//

#ifndef SMARTHOME_WAITDURATION_HPP
#define SMARTHOME_WAITDURATION_HPP
#include <chrono>
#include <thread>
#include <type_traits>

namespace pitools {
    namespace timers {
        template<typename Derived>
        class WaitDuration {
        public:
            WaitDuration() = default;
            WaitDuration(const WaitDuration&)=delete;
            WaitDuration(const WaitDuration&&)=delete;
            Derived &waitFor(const std::chrono::duration<float> &sleep_duration) {
                static_assert(std::is_base_of<WaitDuration, Derived>::value, "Subclass must inherit from WaitDuration");
                std::this_thread::sleep_for(sleep_duration);
                return static_cast<Derived &>(*this);
            }

        };
    }
}

#endif //SMARTHOME_WAITDURATION_HPP
