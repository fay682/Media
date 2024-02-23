#include "x_thread.h"
#include<thread>

void XThread::Start() {
    is_exist_ = false;
    thread_ = std::thread(&XThread::Run, this);
}

void XThread::Stop() {
    is_exist_ = true;
    Wait();
}

void XThread::Wait() {
    if (thread_.joinable()) {
        thread_.join();
    }
}

bool XThread::is_exist() {
    return is_exist_;
}
