#include <vector> // подключение типа динамического массива (вектора) std::vector
#pragma once // защита от повторного включения math.h

std::vector<double> to_double_vector(const std::vector<uint16_t>& input); // объявление функции