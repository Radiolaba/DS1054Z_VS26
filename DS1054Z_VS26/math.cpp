#include <vector>
#include <cstdint>
#include <algorithm>
#include <iterator>

std::vector<double> to_double_vector(const std::vector<uint16_t>& input)
{
    std::vector<double> output;
    output.reserve(input.size());

    std::transform(input.begin(), input.end(),
        std::back_inserter(output),
        [](uint16_t x) { return static_cast<double>(x); });

    return output;
}