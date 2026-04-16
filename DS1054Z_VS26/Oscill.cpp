//#include "stdafx.h"
#include <iostream>
#include "oscill.h"
#include <cstring>
#include <iomanip>
#include <string>

using namespace std;
// пойдет
void OscilloscopeRigol_DS1054Z::connect()
{
	cout << "Started OWON6102A connection" << endl; // OWON6102A connection has been started
	// Адрес прибора
	//const char* resource = "USB0::0x1AB1::0x04CE::DS1ZA231001099::INSTR"; //домашний осцилл
	const char* resource = "USB0::0x1AB1::0x04CE::DS1ZA255005161::INSTR"; //осцилл в лаборатории

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
		printf("OWON6102A connected succesfully\n"); //Oscilloscope has been connected
		OscilloscopeRigol_DS1054Z::connection = true;

	}
}
//
//// пойдет
void OscilloscopeRigol_DS1054Z::disconnect() {
	cout << "Запущено рассоединение с OWON6102A" << endl; //OWON6102A disconnection has been started
	viClose(DEVICE);
	viClose(RESOURCE_MANAGER);
	printf(" Осцилограф был успешно отсоединён\n"); // Oscilloscope has been disconnected
	OscilloscopeRigol_DS1054Z::connection = false;
}

void OscilloscopeRigol_DS1054Z::setup() {
	cout << "Started setup OWON6102A" << endl; // OWON6102A setup has been started



	string setup_commands[] = {
		":TIMebase[:MAIN]:SCALe 0.0002\n",  // horisontal scale

		":ACQuire:TYPE HRESolution\n",// 
		":ACQuire:MDEPth 120000\n",
		":TRIGger:COUPling DC\n",//
		":TRIGger:MODE EDGE\n",//
		//":TRIGger:SWEep SINGle\n", //
		":TRIGger:HOLDoff 0.0000002\n",//
		//":TRIG:SING:EDGE:SOUR CH2\n",
		":TRIGger:EDGe:LEVel 0\n",//
		":CHANnel1:SCALe 0.5\n",//
	};

	//											Начинаем настройку осцилографа
	for (string command : setup_commands) 
	{
		viPrintf(DEVICE, command.c_str());
		Sleep(100);
	}

	//											Передача стартовых команд
	cout << "Inquire" << "                                 " << '|' << "  " << "Answer\n";
	cout << "-----------------------------------------------------\n";
	ask_and_print_answer(":TIMebase:MAIN:SCALe? \n");
	ask_and_print_answer(":ACQuire:TYPE?\n");
	ask_and_print_answer(":CHANnel1:COUPling?\n");
	//ask_and_print_answer(":CH1:OFFSet?\n");
	ask_and_print_answer(":TRIGger:MODE?\n");//
	ask_and_print_answer(":TRIGger:COUPling?\n");//
	ask_and_print_answer(":TRIGger:SWEep?\n");//
	ask_and_print_answer(":TRIGger:HOLDoff?\n");//
	ask_and_print_answer(":ACQuire:MDEPth?\n");//
	ask_and_print_answer(":CHANnel1:SCALe?\n");//
	ask_and_print_answer(":TRIGger:EDGe:LEVel?\n");//
	ask_and_print_answer(":ACQuire:SRATe?\n"); // :ACQuire:SRATe? - покажет отсчеты/сек
	
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


vector<uint16_t> OscilloscopeRigol_DS1054Z::getRaw8BitSignal(
	const unsigned short& CHANNEL,
	const uint16_t& OFFSET, 
	const uint32_t& TICKS
) {
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
	viPrintf(DEVICE, ":STOP\n"); // остановка записи
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
	viPrintf(DEVICE, ":RUN\n");
	return result;
}

double OscilloscopeRigol_DS1054Z::rawTickToVolts(double signal_tick) {
	return (signal_tick - 8192) / 6400;
}