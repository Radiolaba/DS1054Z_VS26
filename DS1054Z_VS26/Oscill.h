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
	vector<uint16_t> getRaw16BitSignal(const uint16_t& EMPTY_TICKS, const uint32_t& TICKS);
	double rawTickToVolts(double signal_tick);

private:
	ViSession DEVICE;
	ViSession RESOURCE_MANAGER;
	int ask_and_print_answer(ViConstString inquiry);

	bool connection;
};