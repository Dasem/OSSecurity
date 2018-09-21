#include <windows.h>
#include <iostream>
#include <fstream>
#include <time.h>
using namespace std;

CRITICAL_SECTION cs;

int k;//������
std::fstream f;
char ch;
int number = 0;

int sum = 0;

// ������� ������
DWORD WINAPI func(LPVOID param)
{	//������ i-� �����, i = 0, 1,�,k-1 ������ ������������ ������ ������� � �������� i + k *s, ��� s = 0, 1, 2,... � ��� ������ ������
	int s = 0;
	int t = 0;
	int nomerPotoka;
	EnterCriticalSection(&cs);
	nomerPotoka = number++;
	LeaveCriticalSection(&cs);
	for (int s = 0;; s++)
	{
		t = nomerPotoka + k * s;
		// ���� � ����������� ������
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
		// ����� �� ����������� ������
		LeaveCriticalSection(&cs);
	}
	return 0;
}


int main()
{
	setlocale(LC_ALL, "rus");
	cout << "������� ���������� �������: ";
	cin >> k;
	cout << endl;
	HANDLE* hThread = new HANDLE[k];	 //������ �������
	DWORD* dwThreadID = new DWORD[k]; //������ ���������������
	int* lParam = new int[k];       // ������ ����������? A pointer to a variable to be passed to the thread.

						 //open file
	f.open("g:\\1.txt");
	if (!f)
	{
		cout << "File doesnt exist" << '\n';
		system("pause");
		return 0;
	}
	// ������������� ����������� ������
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
	cout << "�����(�� ������ 256) = " << sum << '\n';

	// �������� �������
//	for (int i = 0; i < k; i++)
//		CloseHandle(hThread[i]);

	//�������� �����
	f.close();

	system("pause");
	return 0;
}
