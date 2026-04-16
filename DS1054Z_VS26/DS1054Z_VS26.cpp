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
	std::vector<uint16_t> data = oscill.getRaw8BitSignal(1000, 100000);
	saveSignalToTxt(to_double_vector(data),200e-9, "test.txt");
	//auto res = ;	// вызов метода чтения данных
	//запрос сырых данных из памяти осциллографа и возвращение vector<uint16_t>

	return 0;
}
// необходимо добавить преобразование vector<uint16_t> в пары (time, voltage) и запись их в файл
// необходимо добавить oscill.disconnect() в конец