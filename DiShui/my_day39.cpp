#include<iostream>
#include<Windows.h>

using namespace std;

int main(int argc, char* argv[])
{
	wchar_t temp[] = L"ÖÐ¹ú";
	wchar_t data[30];
	wprintf(L"%s\n", temp);


	cout << wcslen(temp) << endl; // 2;
	wcscpy_s(data, temp);
	wprintf(L"%s", data);

	wchar_t aa[] = L"AAAA";
	wcscat_s(data, aa);
	wprintf(L"%s\n", data);
	
	cout << wcscmp(temp, data) << endl;

	WCHAR *sss = wcsstr(temp, data);

	wprintf(L"%s\n", sss);

	return 0;
}