#include <vector>
#include <string>
#pragma once

// объявление saveSignalToTxt()
void saveSignalToTxt(const std::vector<double>& waveform,
    const double timebase_s,
    const std::string& filename);

void saveToCsv3Columns(const std::vector<double>& ch1,
    const std::vector<double>& ch2,
    const std::vector<double>& time,
    const std::string& filename);