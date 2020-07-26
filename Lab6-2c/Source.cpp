//������ �.�. 5307
//����������-������
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
		std::cout << "��������� ������" << std::endl;
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
		std::cout << "���� ����������-�������:" << std::endl;
		std::cout << "1 - ������������ � ������������ ������;" << std::endl;
		std::cout << "2 - ��������� ������ � ����������� ������;" << std::endl;
		std::cout << "0 - �����." << std::endl;
		std::cout << ">> " << std::endl;
		std::cin >> Ch;

		switch (Ch)
		{
		case '0':
			break;
		case '1':
			ZeroMemory(chPipeName, 256);
			std::cout << "������� ��� ������������ ������, � �������� ����� ����������� �����������\n(\\\\.\\pipe\\pipename):" << std::endl;
			std::cin >> chPipeName;

			hConnectPipe = CreateFile(chPipeName, GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);

			if (hConnectPipe == INVALID_HANDLE_VALUE)
			{
				std::cout << "������ ��� ����������� � ������������ ������ " << chPipeName << "." << std::endl;
				std::cin.clear();
				std::cin.sync();
				break;
			};

			std::cout << "����������� � ������������ ������ " << chPipeName << "\n� ������������ " << hConnectPipe  << " ������� �����������." << std::endl;
			break;
		case '2':
			//��������� � ����������� �����
			while (!Exit)
			{
				OVERLAPPED overlap;
				ZeroMemory(&overlap, sizeof(overlap));

				if (!ReadFileEx(hConnectPipe, Buffer, 256, &overlap, FileIOCompletionRoutine))
				{
					std::cout << "������ ��� ������ ������ �� ������������ ������ " << chPipeName << " � ������������ " << hConnectPipe << "." << std::endl;
					std::cin.clear();
					std::cin.sync();
					break;
				};

				SleepEx(INFINITE, 1);
			};

			break;
		default:
			std::cout << "������! �������� �������� ����." << std::endl;
			std::cin.clear();
			std::cin.sync();
			break;
		};

		std::cout << std::endl;
	};

	getch();
};