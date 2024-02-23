#pragma once
#include<thread>

class XThread {
  public:
    virtual void Start();
    virtual void Stop();

    virtual void Wait();

    bool is_exist();

  private:
    //�����߳���Ϣ
    virtual void Run() = 0;

  private:
    bool is_exist_;
    std::thread thread_;
};

