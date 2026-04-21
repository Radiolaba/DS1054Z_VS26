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
		std::vector<uint16_t> data1 = oscill.getRaw8BitSignal(1, 1, 60000); // возвращение отсчетов BYTE-данных, приведенных к uint16_t
		system("del ch1.txt"); // очистка/удаление старых данных (или файла?)
		saveSignalToTxt(to_double_vector(data1), 200e-9, "ch1.txt"); // запись отсчетов в ch1.txt
	}
	*/

	//новое
	{
		// CH1 в вольтах

		// 1. Выбираем CH1 как источник для параметров вертикального масштабирования
		oscill.writeCommand(":WAVeform:SOURce CHANnel1\n");

		// 2. Считываем параметры вертикального масштабирования CH1
		double yinc = oscill.ask_and_get_double(":WAVeform:YINCrement?\n");   // шаг по оси Y между соседними отсчетами, В/отсчет
		double yor = oscill.ask_and_get_double(":WAVeform:YORigin?\n");      // вертикальное смещение относительно опоры по оси Y, В
		double yref = oscill.ask_and_get_double(":WAVeform:YREFerence?\n");   // опора пл оси Y, отсчет

		// 3. Считываем сырые коды CH1 (как раньше)
		const uint16_t offset = 1;
		const uint32_t ticks = 60000;
		std::vector<uint16_t> data1 = oscill.getRaw8BitSignal(1, offset, ticks);

		// 4. Пересчитываем коды в вольты по формуле Rigol:
		//    Volt = (raw - YORigin - YREFerence) * YINCrement
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

	// ---------- CH2 в Вольтах ----------
	{
		// 1. Выбрать CH2 как источник
		oscill.writeCommand(":WAVeform:SOURce CHANnel2\n");

		// 2. Параметры вертикального масштабирования CH2
		double yinc2 = oscill.ask_and_get_double(":WAVeform:YINCrement?\n");   // В/код [web:290][web:392]
		double yor2 = oscill.ask_and_get_double(":WAVeform:YORigin?\n");      // В [web:290][web:392]
		double yref2 = oscill.ask_and_get_double(":WAVeform:YREFerence?\n");   // опорный код [web:290][web:392]

		// 3. Сырые отсчёты CH2
		const uint16_t offset = 1;
		const uint32_t ticks = 60000;
		std::vector<uint16_t> data2 = oscill.getRaw8BitSignal(2, offset, ticks);

		// 4. Пересчёт CH2 в Вольты
		std::vector<double> ch2_volts(data2.size());
		for (size_t i = 0; i < data2.size(); ++i)
		{
			double raw = static_cast<double>(data2[i]);
			ch2_volts[i] = (raw - yor2 - yref2) * yinc2;  // та же формула [web:290][web:392]
		}

		// 5. Сохранение CH2
		system("del ch2.txt");
		saveSignalToTxt(ch2_volts, 200e-9, "ch2.txt");
	}

	return 0;
}
// необходимо добавить преобразование vector<uint16_t> в пары (time, voltage) и запись их в файл
// необходимо добавить oscill.disconnect() в конец