#include <windows.h>
#include <iostream>
#include <fstream>
#include <time.h>
using namespace std;

CRITICAL_SECTION cs;

const int k = 10;//потоки
int n = 50;//simvoli
std::fstream f;
char ch;

int sum = 0;

// функция потока
DWORD WINAPI func(LPVOID param)
{	//Каждый i-й поток, i = 0, 1,…,k-1 должен обрабатывать только символы с номерами i + k *s, где s = 0, 1, 2,... – шаг работы потока
	int s = 0;
	int t = 0;
	for (int s = 0; s < n&&t < n; s++)
	{
		//i + k *s
		t = (int)param + k * s;
		// Вход в критическую секцию
		EnterCriticalSection(&cs);
		f.seekg(t);
		f.get(ch);

		sum += (int)ch;
		// Выход из критической секции
		LeaveCriticalSection(&cs);
	}
	return 0;
}


int main()
{
	setlocale(LC_ALL, "rus");

	HANDLE hThread[k];	 //массив потоков
	DWORD dwThreadID[k]; //массив идентификаторов
	int lParam[k];       // массив параметров? A pointer to a variable to be passed to the thread.

						 //open file
	f.open("C:\\Users\\alran\\Documents\\Visual Studio 2015\\Projects\\ZOS 1.1\\ZOS 1.1\\Debug\\1.txt");
	if (!f)
	{
		std::cout << "File doesnt exist" << '\n';
		system("pause");
		return 0;
	}
	// Инициализация критической секции
	InitializeCriticalSection(&cs);



	// заполнение параметров?
	for (int i = 0; i < k; i++)
		lParam[i] = i;


	// Запуск потоков
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
	std::cout << "Сумма(по модулю 256) = " << sum << '\n';

	// Закрытие потоков
	for (int i = 0; i < k; i++)
		CloseHandle(hThread[i]);

	//Закрытие файла
	f.close();

	system("pause");
	return 0;
}
