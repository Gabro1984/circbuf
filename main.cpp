#include <random>
#include <string>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <thread>

#include "MailBoxImpl.hpp"

constexpr int MAX_SLEEP = 100;
MailBoxImpl<std::string> mailQueue;

std::string getTimestamp() {
  const auto now = std::chrono::system_clock::now();
  const auto nowAsTimeT = std::chrono::system_clock::to_time_t(now);
  const auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
      now.time_since_epoch()) % 1000;
  std::stringstream nowSs;
  nowSs
      << std::put_time(std::localtime(&nowAsTimeT), "%a %b %d %Y %T")
      << '.' << std::setfill('0') << std::setw(3) << nowMs.count();
  return nowSs.str();
}

std::string randomStr() {
    std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    std::random_device rd;
    std::mt19937 generator(rd());
    std::shuffle(str.begin(), str.end(), generator);

    return str.substr(0, 32);
}

void logMsg(const std::string &str) {
    std::cout << " [" << getTimestamp() << "] " << str << std::endl;
}

void randSleep(uint32_t maxSleep) {
    using namespace std;
    this_thread::sleep_for(chrono::milliseconds(rand() % maxSleep));
}

void writer() {
    std::string msg;
    for (;;) {
        msg = randomStr();
        if (!mailQueue.IsFull()) {
            mailQueue.SendData(msg);
        }
        randSleep(MAX_SLEEP);
    }
}

void reader() {
    std::optional<std::string> msg;
    for (;;) {
        if (!mailQueue.IsEmpty()) {
            mailQueue.GetData(msg);
        }
        randSleep(MAX_SLEEP*4);
    }
}

int main() {
    logMsg("Start test");
    std::thread th1(writer);
    std::thread th2(reader);
    th1.join();
    th2.join();

    return 0;
}