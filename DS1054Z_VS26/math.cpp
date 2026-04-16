#include <vector> // подключение типа динамического массива (вектора) std::vector
#include <cstdint> // подключение типов фиксированной ширины, в т.ч. uint16_t
#include <algorithm> // подключение стандартных алгоритмов, в т.ч. std::transform
#include <iterator> // подключение итераторов, в т.ч. std::back_inserter

// объявление функции преобразования вектора std::vector<uint16_t> в вектор std::vector<double> 
std::vector<double> to_double_vector(const std::vector<uint16_t>& input) 
{
    std::vector<double> output; // создание пустовго вектора
    output.reserve(input.size()); // выделяется память под количество элементов, равное размеру input

    std::transform(input.begin(), input.end(), std::back_inserter(output), [](uint16_t x)
        {
            return static_cast<double>(x);
        }
    );

    return output;
}