#include "files.h" // подключение заголовочного файла
#include <fstream> // подключение средства работы с файлыми (в т.ч. с потоками std::ofstream)
#include <iomanip> // подключение манипуляторов форматирования вывода (в т.ч. std::fixed, std::setprecision())

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