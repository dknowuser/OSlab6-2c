//Марчук Л.Б. 5307
//Приложение-клиент
#include <conio.h>
#include <iostream>
#include <Windows.h>

bool Exit = false;
char* Buffer = new char[256];

VOID CALLBACK FileIOCompletionRoutine(DWORD, DWORD, LPOVERLAPPED)
{
	if (!strcmp(Buffer, "stop"))
		Exit = true;
	else
	{
		std::cout << "Считанные данные" << std::endl;
		std::cout << Buffer << std::endl << std::endl;
	};
};

void main(void)
{
	setlocale(LC_ALL, "Russian");

	char Ch = 0;
	char* chPipeName = new char[80];
	HANDLE hConnectPipe = NULL;

	while (Ch != '0')
	{
		std::cout << "Меню приложения-клиента:" << std::endl;
		std::cout << "1 - подключиться к именованному каналу;" << std::endl;
		std::cout << "2 - считывать данные в асинхронном режиме;" << std::endl;
		std::cout << "0 - выход." << std::endl;
		std::cout << ">> " << std::endl;
		std::cin >> Ch;

		switch (Ch)
		{
		case '0':
			break;
		case '1':
			ZeroMemory(chPipeName, 256);
			std::cout << "Введите имя именованного канала, к которому будет произведено подключение\n(\\\\.\\pipe\\pipename):" << std::endl;
			std::cin >> chPipeName;

			hConnectPipe = CreateFile(chPipeName, GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);

			if (hConnectPipe == INVALID_HANDLE_VALUE)
			{
				std::cout << "Ошибка при подключении к именованному каналу " << chPipeName << "." << std::endl;
				std::cin.clear();
				std::cin.sync();
				break;
			};

			std::cout << "Подключение к именованному каналу " << chPipeName << "\nс дескриптором " << hConnectPipe  << " успешно произведено." << std::endl;
			break;
		case '2':
			//Считываем в бесконечном цикле
			while (!Exit)
			{
				OVERLAPPED overlap;
				ZeroMemory(&overlap, sizeof(overlap));

				if (!ReadFileEx(hConnectPipe, Buffer, 256, &overlap, FileIOCompletionRoutine))
				{
					std::cout << "Ошибка при чтении данных из именованного канала " << chPipeName << " с дескриптором " << hConnectPipe << "." << std::endl;
					std::cin.clear();
					std::cin.sync();
					break;
				};

				SleepEx(INFINITE, 1);
			};

			break;
		default:
			std::cout << "Ошибка! Неверный параметр меню." << std::endl;
			std::cin.clear();
			std::cin.sync();
			break;
		};

		std::cout << std::endl;
	};

	getch();
};