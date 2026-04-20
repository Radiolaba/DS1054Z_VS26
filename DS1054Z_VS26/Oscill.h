#pragma once
#include <vector>
#include <Windows.h>
#include "include/visa.h"

#pragma comment(lib, "include/visa64.lib")

#pragma once
using namespace std;


class OscilloscopeRigol_DS1054Z {
public:
    ~OscilloscopeRigol_DS1054Z() = default;
    void connect();
    void disconnect();
    void setup();
    bool trigger();
    vector<uint16_t> getRaw8BitSignal(const unsigned short& CHANNEL,
        const uint16_t& EMPTY_TICKS,
        const uint32_t& TICKS);
    double rawTickToVolts(double signal_tick);

    int ask_and_print_answer(ViConstString inquiry);
    double ask_and_get_double(ViConstString inquire);
    void writeCommand(ViConstString cmd);   // ← добавили

private:
    ViSession DEVICE;
    ViSession RESOURCE_MANAGER;
    bool connection;
};
