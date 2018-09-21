#include <windows.h>
#include <iostream>
#include <fstream>
#include <time.h>
using namespace std;

CRITICAL_SECTION cs;

int k;//������
double answer = 0;
double a;
double b;
double aPotoka;
double bPotoka;
int sum = 0;
double step;

// ������� ������
DWORD WINAPI func(LPVOID param)
{	//������ i-� �����, i = 0, 1,�,k-1 ������ ������������ ������ ������� � �������� i + k *s, ��� s = 0, 1, 2,... � ��� ������ ������
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
	cout << "������� ���������� �������: ";
	cin >> k;
	cout << endl;
	cout << "������� ����� �������: ";
	cin >> a;
	cout << endl;
	cout << "������� ������ �������: ";
	cin >> b;
	cout << endl;
	step = (b - a) / (double)k;
	bPotoka += step;
	HANDLE* hThread = new HANDLE[k];	 //������ �������
	DWORD* dwThreadID = new DWORD[k]; //������ ���������������
	int* lParam = new int[k];       // ������ ����������? A pointer to a variable to be passed to the thread.

						 //open file
	InitializeCriticalSection(&cs);
	// ���������� ����������?
	for (int i = 0; i < k; i++)
		lParam[i] = i;


	//������ �������
	for (int i = 0; i < k; i++)
	{
		hThread[i] = CreateThread(NULL, 0,
			func, (LPVOID)lParam, 0, &(dwThreadID[i]));

		if (hThread[i] == NULL)
		{
			std::cout << "������ �������� ������ "
				<< GetLastError() << '\n';
		}
	}

	// �������� ���������� �������
	WaitForMultipleObjects(k, hThread,
		true, INFINITE);

	//����� �����������
	cout << "������� �������� = " << answer << '\n';

	// �������� �������
//	for (int i = 0; i < k; i++)
//		CloseHandle(hThread[i]);

	//�������� �����

	system("pause");
	return 0;
}
