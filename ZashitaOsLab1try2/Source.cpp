#include <windows.h>
#include <iostream>
#include <fstream>
#include <time.h>
using namespace std;

CRITICAL_SECTION cs;

int k;//потоки
std::fstream f;
char ch;
int number = 0;

int sum = 0;

// функция потока
DWORD WINAPI func(LPVOID param)
{	//Каждый i-й поток, i = 0, 1,…,k-1 должен обрабатывать только символы с номерами i + k *s, где s = 0, 1, 2,... – шаг работы потока
	int s = 0;
	int t = 0;
	int nomerPotoka;
	EnterCriticalSection(&cs);
	nomerPotoka = number++;
	LeaveCriticalSection(&cs);
	for (int s = 0;; s++)
	{
		t = nomerPotoka + k * s;
		// Вход в критическую секцию
		EnterCriticalSection(&cs);
		f.seekg(t);
		f >> ch;
		if (f.eof()) {
			f.close();
			f.open("g:\\1.txt");
			LeaveCriticalSection(&cs);
			break;
		}

		sum += (int)ch%256;
		// Выход из критической секции
		LeaveCriticalSection(&cs);
	}
	return 0;
}


int main()
{
	setlocale(LC_ALL, "rus");
	cout << "Введите количество потоков: ";
	cin >> k;
	cout << endl;
	HANDLE* hThread = new HANDLE[k];	 //массив потоков
	DWORD* dwThreadID = new DWORD[k]; //массив идентификаторов
	int* lParam = new int[k];       // массив параметров? A pointer to a variable to be passed to the thread.

						 //open file
	f.open("g:\\1.txt");
	if (!f)
	{
		cout << "File doesnt exist" << '\n';
		system("pause");
		return 0;
	}
	// Инициализация критической секции
	InitializeCriticalSection(&cs);



	// заполнение параметров?
	for (int i = 0; i < k; i++)
		lParam[i] = i;


	 //Запуск потоков
	for (int i = 0; i < k; i++)
	{
		hThread[i] = CreateThread(NULL, 0,
			func, (LPVOID)lParam, 0, &(dwThreadID[i]));

		if (hThread[i] == NULL)
		{
			std::cout << "Ошибка создания потока "
				<< GetLastError() << '\n';
		}
	}

	// Ожидание завершения потоков
	WaitForMultipleObjects(k, hThread,
		true, INFINITE);

	//Вывод результатов
	cout << "Сумма(по модулю 256) = " << sum << '\n';

	// Закрытие потоков
//	for (int i = 0; i < k; i++)
//		CloseHandle(hThread[i]);

	//Закрытие файла
	f.close();

	system("pause");
	return 0;
}
