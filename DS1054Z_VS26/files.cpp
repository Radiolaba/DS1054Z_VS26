#include "files.h"
#include <fstream>
#include <iomanip>
void saveSignalToTxt(const std::vector<double>& waveform, const double timebase_s, const std::string& filename) {
	std::ofstream file(filename);
	file << std::fixed << std::setprecision(6);  // 6 знаков после запятой

	for (double v : waveform) {
		file << v << '\n';
	}
	file.close();
}