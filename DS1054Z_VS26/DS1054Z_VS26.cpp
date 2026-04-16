#include "Oscill.h" // подключение заголовочного файла, где объявлен класс OscilloscopeRigol_DS1054Z и его методы
#include <iostream> // подлкючение стандартной библиотеки C++ для ввода/вывода через консоль
#include "files.h"
#include "math.h"

int main()			// главная функция
{
	OscilloscopeRigol_DS1054Z oscill; // создание объекта oscill класса OscilloscopeRigol_DS1054Z для взаимодействия с осциллографом
	oscill.connect(); // вызов метода подключения к осциллографу
	Sleep(600);			// пауза 600 мс
	oscill.setup();		// вызов метода настройки осциллографа
	Sleep(600);			// пауза 600 мс

	{
		std::vector<uint16_t> data1 = oscill.getRaw8BitSignal(1, 1, 100000);
		system("del ch1.txt");
		saveSignalToTxt(to_double_vector(data1), 200e-9, "ch1.txt");
	}
	{
		std::vector<uint16_t> data2 = oscill.getRaw8BitSignal(2, 1, 100000);
		system("del ch2.txt");
		saveSignalToTxt(to_double_vector(data2), 200e-9, "ch2.txt");
	}
	return 0;
}
// необходимо добавить преобразование vector<uint16_t> в пары (time, voltage) и запись их в файл
// необходимо добавить oscill.disconnect() в конец