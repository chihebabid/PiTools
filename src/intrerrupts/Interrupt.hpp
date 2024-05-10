//
// Created by chiheb on 11/05/24.
//

#ifndef SMARTHOME_INTERRUPT_HPP
#define SMARTHOME_INTERRUPT_HPP
#include <functional>
#include "pitools.hpp"
namespace pitools {
    namespace interrupts {
        enum class InterruptType {
            EDGE_RISING,
            EDGE_FALLING,
            BOTH
        };
        class Interrupt {
            bool mEnabled {false};
            std::function<void(const uint8_t&)> mCallback;
            std::vector<uint8_t> mPins;
            std::thread *mThread {nullptr};
            void mainThread();
        public:
            Interrupt(std::initializer_list<uint8_t> l,const InterruptType &t,std::function<void(const uint8_t&)> f);
            ~Interrupt();
            void enable();
            void disable();
        };
    }
}


#endif //SMARTHOME_INTERRUPT_HPP
