#include "Oscill.h"
#include <iostream>

int main()
{
	OscilloscopeRigol_DS1054Z oscill;
	oscill.connect();
	Sleep(600);
	oscill.setup();
	Sleep(600);
	auto res = oscill.getRaw16BitSignal(1000, 20000);
	return 0;
}