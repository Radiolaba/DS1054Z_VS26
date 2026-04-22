#include "files.h" // подключение заголовочного файла
#include <fstream> // подключение средства работы с файлыми (в т.ч. с потоками std::ofstream)
#include <iomanip> // подключение манипуляторов форматирования вывода (в т.ч. std::fixed, std::setprecision())
#include <vector> // для CSV
#include <string> // для CSV


void saveSignalToTxt(const std::vector<double>& waveform, const double timebase_s, const std::string& filename) // реализация saveSignalToTxt
//waveform - входной вектор данных типа double
//timebase_s - 2-й параметр, пока не используется
//filename - имя файла, куда будет записан результат
{
	std::ofstream file(filename); // создание объекта потока вывода и открытие (создание и открытие) файла с именем filename для записи (перезаписи) в него
	file << std::fixed << std::setprecision(6);  // настройка вывода числа в файл: десятичная запись, 6 знаков после запятой
	
	// запись каждого элемента вектора waveform в файл отдельной строкой
	for (double v : waveform)  // каждый элемент вектора waveform записывается в переменную v
	{
		file << v << '\n'; // запись текущего значения v в файл и перенос строки
	}
	file.close();
}

// Запись трёх столбцов: V1, V2, t в CSV без заголовков
void saveToCsv3Columns(const std::vector<double>& ch1,
    const std::vector<double>& ch2,
    const std::vector<double>& time,
    const std::string& filename)
{
    if (ch1.size() != ch2.size() || ch1.size() != time.size()) {
        throw std::runtime_error("saveToCsv3Columns: vector sizes differ");
    }

    std::ofstream csv(filename);
    if (!csv.is_open()) {
        throw std::runtime_error("Cannot open CSV file");
    }

    csv.setf(std::ios::scientific);
    csv.precision(12);

    const size_t N = ch1.size();
    for (size_t i = 0; i < N; ++i) {
        csv << ch1[i] << ";"   // 1 столбец: напряжение CH1
            << ch2[i] << ";"   // 2 столбец: напряжение CH2
            << time[i] << "\n"; // 3 столбец: время, сек
    }

    csv.close();
}