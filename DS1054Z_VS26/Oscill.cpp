//#include "stdafx.h"
#include <iostream>
#include "oscill.h"
#include <cstring>
#include <iomanip>
#include <string>

using namespace std;

// подключение к осциллографу (работает)
void OscilloscopeRigol_DS1054Z::connect()
{
	cout << "Started DS1054Z connection" << endl; // сообщение о начале соединения
	// Адрес прибора
	const char* resource = "USB0::0x1AB1::0x04CE::DS1ZA231001099::INSTR"; //домашний осцилл
	//const char* resource = "USB0::0x1AB1::0x04CE::DS1ZA255005161::INSTR"; //осцилл в лаборатории

	DEVICE = VI_NULL;
	RESOURCE_MANAGER = VI_NULL;
	ViStatus status;
	// Открываем Resource Manager
	status = viOpenDefaultRM(&RESOURCE_MANAGER);
	if (status != VI_SUCCESS)
	{
		throw std::exception("Resource manager error!");
	}
	//Открываем прибор
	status = viOpen(RESOURCE_MANAGER, resource, VI_NULL, VI_NULL, &DEVICE);
	if (status != VI_SUCCESS)
	{
		cout << "Device not found!\n";
		//throw "Device not found!\n";
	}
	else {
		printf("DS1054Z connected succesfully\n"); //Oscilloscope has been connected
		OscilloscopeRigol_DS1054Z::connection = true;

	}
}

// отключение от осциллографа (не используется)
void OscilloscopeRigol_DS1054Z::disconnect() {
	cout << "Запущено рассоединение с OWON6102A" << endl; //OWON6102A disconnection has been started
	viClose(DEVICE);
	viClose(RESOURCE_MANAGER);
	printf(" Осцилограф был успешно отсоединён\n"); // Oscilloscope has been disconnected
	OscilloscopeRigol_DS1054Z::connection = false;
}

void OscilloscopeRigol_DS1054Z::writeCommand(ViConstString cmd)
{
	viPrintf(DEVICE, cmd);
}

// настройка осциллографа (работает)
void OscilloscopeRigol_DS1054Z::setup()
{
	cout << "Started setup DS1054Z" << endl; // сообщение о начале настройки
	string setup_commands[] = {
		//":TIMebase[:MAIN]:SCALe 0.0002\n",  // развертка по времени
		":ACQuire:TYPE HRESolution\n",// режим сбора: Высокое разрешение
		":ACQuire:MDEPth 60000\n",// глубина записи: 60k
		//":TRIGger:COUPling DC\n",// связь для триггера
		//":TRIGger:MODE EDGE\n",// тип триггера: по фронту
		//":TRIGger:SWEep SINGle\n", // тип триггера: Single
		//":TRIGger:HOLDoff 0.0000002\n",// удержание триггера: 16нс
		//":TRIGger:EDGe:LEVel 0\n",// уровень триггера
		//":CHANnel1:SCALe 0.5\n",// развертка 1 канал по напряжению
		":CHANnel1:DISPlay ON\n", // включение 1 канала
		":CHANnel2:DISPlay ON\n", // включение 2 канала
		":CHANnel1:COUPling AC\n", // тип связи 1 канала: AC
		":CHANnel2:COUPling AC\n", // тип связи 2 канала: AC
	};

	
	for (string command : setup_commands) // применение настроек осциллографа
	{
		viPrintf(DEVICE, command.c_str());
		Sleep(100);
	}

	viPrintf(DEVICE, ":STOP\n"); // остановка перед опросом масшабирования каналов

	//Опрос настроек
	cout << "Inquire" << "                                 " << '|' << "  " << "Answer\n";
	cout << "-----------------------------------------------------\n";
	ask_and_print_answer(":ACQuire:TYPE?\n");			//режим сбора: Высокое разрешение
	ask_and_print_answer(":ACQuire:MDEPth? \n");		//глубина записи: 60k
	ask_and_print_answer(":CHANnel1:DISPlay? \n");		//включение 1 канала
	ask_and_print_answer(":CHANnel2:DISPlay? \n");		//включение 2 канала
	ask_and_print_answer(":CHANnel1:COUPling? \n");		//тип связи 1 канала: AC
	ask_and_print_answer(":CHANnel2:COUPling? \n");		//тип связи 2 канала: AC
	ask_and_print_answer(":CHANnel1:SCALe?\n");			//развертка 1 канала по напряжению
	ask_and_print_answer(":CHANnel2:SCALe?\n");			//развертка 2 канала по напряжению
	ask_and_print_answer(":TIMebase:MAIN:SCALe?\n");	//развертка каналов по времени
	ask_and_print_answer(":ACQuire:SRATe?\n");			//частоты дискретизации
}

// пойдет
int OscilloscopeRigol_DS1054Z::ask_and_print_answer(ViConstString inquire)
{
	char buffer[256];
	ViUInt32 bytes_read;
	ViStatus status;

	viPrintf(DEVICE, inquire);
	status = viRead(DEVICE, (ViBuf)buffer, 255, &bytes_read);

	if (status < VI_SUCCESS) {
		printf("Reading error: 0x%08X\n", status);
		buffer[bytes_read] = 0;
		cout << inquire << setw(40) << '|' << "  " << "Reading error: 0x%08X\n" << ' ' << status;
		return -1;
	}
	else {
		buffer[bytes_read] = 0;
		cout << inquire << setw(40) << '|' << "  " << buffer;
		// cout << "The ask is: " << inquire;
		// printf(" The answer is: %s\n", buffer);
		return 0;
	}
}

//новая функцмя
double OscilloscopeRigol_DS1054Z::ask_and_get_double(ViConstString inquire)
{
	char buffer[256];
	ViUInt32 bytes_read;
	ViStatus status;

	viPrintf(DEVICE, inquire);
	status = viRead(DEVICE, (ViBuf)buffer, 255, &bytes_read);

	if (status < VI_SUCCESS) {
		printf("Reading error: 0x%08X\n", status);
		throw std::runtime_error("Error reading double from oscilloscope");
	}

	buffer[bytes_read] = 0;
	return atof(buffer); // строка SCPI-ответа → double [web:298]
}

// нормально
bool OscilloscopeRigol_DS1054Z::trigger() {
	char status[16];
	viQueryf(DEVICE, ":TRIGger:STATus?\n", "%s", status);
	if (strstr(status, "TRIG\0")) {
		return true;
	}
	else {
		return false;
	}
}


vector<uint16_t> OscilloscopeRigol_DS1054Z::getRaw8BitSignal
(
	const unsigned short& CHANNEL,
	const uint16_t& OFFSET, 
	const uint32_t& TICKS
)

{
	if (CHANNEL > 4 || CHANNEL < 1) {
		throw "Incorrect channel number!";
	}

	vector<uint16_t> result(TICKS, 0);
	const string START = ":WAV:STAR " + to_string(OFFSET) + "\n";
	const string FINISH = ":WAV:STOP " + to_string(TICKS) + "\n";
	const string SOURCE_CHAN = ":WAVeform:SOURce CHANnel" + to_string(CHANNEL) + "\n";

	ViUInt32 bytes_read;
	unsigned char read_buf[301000];

	viPrintf(DEVICE, "*CLS\n");
	//viPrintf(DEVICE, ":STOP\n"); // остановка записи - не нужна, т.к. STOP перед опросом масштабирования и считыванием
	viPrintf(DEVICE, SOURCE_CHAN.c_str()); // канал считывания данных
	viPrintf(DEVICE, ":WAV:MODE RAW\n"); // сырые данные без обработки
	viPrintf(DEVICE, ":WAV:FORM BYTE\n"); //в виде байтов по одному на отсчет
	viPrintf(DEVICE, START.c_str()); //
	viPrintf(DEVICE, FINISH.c_str()); //
	viPrintf(DEVICE, ":WAV:DATA?\n"); //запрос данных
	

	viRead(DEVICE, read_buf, sizeof(read_buf) - 1, &bytes_read);
	read_buf[bytes_read] = '\0';

	// 4. Парсинг TMC + int16... (без изменений)

	if (bytes_read < 2 || read_buf[0] != '#') throw "Wrong format data packet from oscill!"; // if answer contains less then 2 bytes or has no header
	int n_digits = read_buf[1] - '0';						// amount of digits in data bytes number
	if (bytes_read < 2 + n_digits) throw "Empty data packet from oscill!";			// if there is no data after header

	std::string len_str((char*)read_buf + 2, n_digits);		// the string of data bytes number
	size_t data_len = std::stoul(len_str);					// integer data bytes number

	//						Проверка длины пакета данных и ее четности
	if (data_len & 1) {
		throw "Wrong format data packet from oscill!";
	}
	else {
		if (uint32_t(data_len) > TICKS)
			throw "Data packet longer than demanded ticks";
	}
	// Извлечение int16 из байтов (big-endian)
	const unsigned char* data_ptr = read_buf + 2 + n_digits;

	for (size_t i = 0; i < data_len; i += 1) {
		result[i] = data_ptr[i];
	}
	//viPrintf(DEVICE, ":RUN\n"); // не нужно - т.к. после опроса масштабирования и считывания
	return result;
}

double OscilloscopeRigol_DS1054Z::rawTickToVolts(double signal_tick) {
	return (signal_tick - 8192) / 6400;
}