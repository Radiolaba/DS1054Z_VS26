#include "Oscill.h" // подключение заголовочного файла, где объявлен класс OscilloscopeRigol_DS1054Z и его методы
#include <iostream> // подлкючение стандартной библиотеки C++ для ввода/вывода через консоль
#include "files.h"
#include "math.h"

int main() // главная функция
{
    OscilloscopeRigol_DS1054Z oscill; // объект для взаимодействия с осциллографом
    oscill.connect();                 // подключение к осциллографу
    Sleep(600);                       // пауза 600 мс
    oscill.setup();                   // настройка осциллографа
    Sleep(600);                       // пауза 600 мс

    // Останавливаем осциллограф перед считыванием буфера и параметров
    oscill.writeCommand(":STOP\n");

    std::vector<double> ch1_volts;
    std::vector<double> ch2_volts;

    // ---------- CH1 в Вольтах ----------
    {
        // 1. Выбираем CH1 как источник для параметров вертикального масштабирования
        oscill.writeCommand(":WAVeform:SOURce CHANnel1\n");

        // 2. Считываем параметры вертикального масштабирования CH1
        double yinc1 = oscill.ask_and_get_double(":WAVeform:YINCrement?\n"); // шаг по оси Y, В/отсчет [web:298][web:291]
        double yor1 = oscill.ask_and_get_double(":WAVeform:YORigin?\n");    // вертикальное смещение, В [web:298][web:291]
        double yref1 = oscill.ask_and_get_double(":WAVeform:YREFerence?\n"); // опора по оси Y, отсчет [web:298][web:291]

        // 3. Считываем сырые коды CH1
        const uint16_t offset = 1;
        const uint32_t ticks = 60000;
        std::vector<uint16_t> data1 = oscill.getRaw8BitSignal(1, offset, ticks);

        // 4. Пересчитываем коды в Вольты по формуле Rigol:
        //    Volt = (raw - YORigin - YREFerence) * YINCrement [web:392][web:298]
        ch1_volts.resize(data1.size());
        for (size_t i = 0; i < data1.size(); ++i)
        {
            double raw = static_cast<double>(data1[i]);
            ch1_volts[i] = (raw - yor1 - yref1) * yinc1;
        }

        // 5. Сохраняем CH1 в Вольтах
        system("del ch1.txt");
        saveSignalToTxt(ch1_volts, 200e-9, "ch1.txt");
    }

    // ---------- CH2: запись времени в секундах в ch2.txt ----------
    {
        // 1. Выбираем CH2 как источник
        oscill.writeCommand(":WAVeform:SOURce CHANnel2\n");

        // 2. Считываем параметры вертикального масштабирования CH2
        double yinc2 = oscill.ask_and_get_double(":WAVeform:YINCrement?\n"); // В/отсчет [web:298][web:291]
        double yor2 = oscill.ask_and_get_double(":WAVeform:YORigin?\n");    // В [web:298][web:291]
        double yref2 = oscill.ask_and_get_double(":WAVeform:YREFerence?\n"); // опорный код [web:298][web:291]

        // 3. Сырые отсчёты CH2
        const uint16_t offset = 1;
        const uint32_t ticks = 60000;
        std::vector<uint16_t> data2 = oscill.getRaw8BitSignal(2, offset, ticks);

        // 4. При желании пересчёт CH2 в Вольты (оставляем, чтобы ch2_volts был готов для будущего использования)
        ch2_volts.resize(data2.size());
        for (size_t i = 0; i < data2.size(); ++i)
        {
            double raw = static_cast<double>(data2[i]);
            ch2_volts[i] = (raw - yor2 - yref2) * yinc2;
        }

        // 5. Получаем временную шкалу от осциллографа (XINC и XORIG для текущего кадра)
        double xinc = 0.0;
        double xorig = 0.0;
        oscill.getTimeScale(xinc, xorig); // xincrement и xorigin, секунды [web:392][web:298]

        // 6. Формируем вектор времени: t[i] = xorig + xinc * i
        std::vector<double> time_sec(data2.size());
        for (size_t i = 0; i < data2.size(); ++i)
        {
            time_sec[i] = xorig + xinc * static_cast<double>(i);
        }

        // 7. Сохраняем время в секундах в ch2.txt
        system("del ch2.txt");
        saveSignalToTxt(time_sec, 200e-9, "ch2.txt");
    }

    // Возвращаем осциллограф в режим RUN
    oscill.writeCommand(":RUN\n");

    return 0;
}