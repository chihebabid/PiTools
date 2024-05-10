//
// Created by chiheb on 11/05/24.
//

#include "Interrupt.hpp"

namespace pitools {
    namespace interrupts {
        Interrupt::Interrupt(std::initializer_list<uint8_t> l,const InterruptType &t,std::function<void(const uint8_t&)> f):mCallback(f) {

        }
        Interrupt::~Interrupt() {
            disable();
        }

        void Interrupt::enable() {
            if (!mEnabled) {
                mEnabled=true;
                mThread=new std::thread{[this](){mainThread();}};
            }
        }

        void Interrupt::disable() {
            if (mEnabled) {
                mEnabled=false;
                mThread->join();
                delete mThread;
                mThread= nullptr;
            }
        }

        void Interrupt::mainThread() {
            while (mEnabled) {
                std::this_thread::yield();
            }
        }
    }
}