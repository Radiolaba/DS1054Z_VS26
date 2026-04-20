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
	
	
	/*
	{
		std::vector<uint16_t> data1 = oscill.getRaw8BitSignal(1, 1, 60000);
		system("del ch1.txt");
		saveSignalToTxt(to_double_vector(data1), 200e-9, "ch1.txt");
	}
	*/

	//новое
	{
		// CH1 в вольтах

		// 1. Выбираем CH1 как источник для параметров масштабирования
		oscill.writeCommand(":WAVeform:SOURce CHANnel1\n");

		// 2. Считываем вертикальные параметры CH1
		double yinc = oscill.ask_and_get_double(":WAVeform:YINCrement?\n");   // В/код [web:298][web:290]
		double yor = oscill.ask_and_get_double(":WAVeform:YORigin?\n");      // В [web:298][web:290]
		double yref = oscill.ask_and_get_double(":WAVeform:YREFerence?\n");   // опорный код [web:298][web:290]

		// 3. Считываем сырые коды CH1 (как раньше)
		const uint16_t offset = 1;
		const uint32_t ticks = 60000;
		std::vector<uint16_t> data1 = oscill.getRaw8BitSignal(1, offset, ticks);

		// 4. Пересчитываем коды в вольты по формуле Rigol:
		//    Volt = (raw - YORigin - YREFerence) * YINCrement [web:298][web:410]
		std::vector<double> ch1_volts(data1.size());
		for (size_t i = 0; i < data1.size(); ++i)
		{
			double raw = static_cast<double>(data1[i]);
			ch1_volts[i] = (raw - yor - yref) * yinc;
		}

		// 5. Сохраняем CH1 в вольтах
		system("del ch1.txt");
		saveSignalToTxt(ch1_volts, 200e-9, "ch1.txt");
	}
	//новое



	/*
	{
		std::vector<uint16_t> data2 = oscill.getRaw8BitSignal(2, 1, 60000);
		system("del ch2.txt");
		saveSignalToTxt(to_double_vector(data2), 200e-9, "ch2.txt");
	}
	*/
	return 0;
}
// необходимо добавить преобразование vector<uint16_t> в пары (time, voltage) и запись их в файл
// необходимо добавить oscill.disconnect() в конец