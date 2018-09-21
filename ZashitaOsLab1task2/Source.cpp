#include <windows.h>
#include <iostream>
#include <fstream>
#include <time.h>
using namespace std;

CRITICAL_SECTION cs;

int k;//потоки
double answer = 0;
double a;
double b;
double aPotoka;
double bPotoka;
int sum = 0;
double step;

// функция потока
DWORD WINAPI func(LPVOID param)
{	//Каждый i-й поток, i = 0, 1,…,k-1 должен обрабатывать только символы с номерами i + k *s, где s = 0, 1, 2,... – шаг работы потока
	int s = 0;
	int t = 0;
	double myA;
	double myB;
	EnterCriticalSection(&cs);
	myA = aPotoka;
	myB = bPotoka;
	aPotoka += step;
	bPotoka += step;
	answer += myB * myB / 2 + myB - myA - myA * myA / 2;
	LeaveCriticalSection(&cs);
	return 0;
}


int main()
{
	setlocale(LC_ALL, "rus");
	cout << "Введите количество потоков: ";
	cin >> k;
	cout << endl;
	cout << "Введите левую границу: ";
	cin >> a;
	cout << endl;
	cout << "Введите правую границу: ";
	cin >> b;
	cout << endl;
	step = (b - a) / (double)k;
	bPotoka += step;
	HANDLE* hThread = new HANDLE[k];	 //массив потоков
	DWORD* dwThreadID = new DWORD[k]; //массив идентификаторов
	int* lParam = new int[k];       // массив параметров? A pointer to a variable to be passed to the thread.

						 //open file
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
	cout << "Площадь трапеции = " << answer << '\n';

	// Закрытие потоков
//	for (int i = 0; i < k; i++)
//		CloseHandle(hThread[i]);

	//Закрытие файла

	system("pause");
	return 0;
}
