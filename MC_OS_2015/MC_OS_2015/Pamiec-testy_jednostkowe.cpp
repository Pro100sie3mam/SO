#include "zarzadzanie_pamiecia.h"
#include <iostream>
#include <cstdlib>
using namespace std;

void test1()
{
	ZarzadzaniePamiecia* pam = new ZarzadzaniePamiecia(16 * 16);
	int adrs_1 = pam->XA(1);
	int adrs_2 = pam->XA(2);
	pam->Zapisz(adrs_1, (char*)"ABC", 3);
	pam->Zapisz(adrs_2, (char*)"GH12ABC", 7);
	pam->Zapisz(adrs_1, (char*)"PODMIENIONO", 11);
	int adrs_3 = pam->XA(1);
	pam->Zapisz(adrs_3, (char*)"AAA", 3);
	for (int i = 0; i < 10; i++) pam->Zapisz(pam->XA(1), (char*)"TEST", 4);
	pam->Zapisz(pam->XA(1), (char*)"ABCD", 4);

	//SPRAWDZENIE XB
	//pam->XB(adrs_3);

	//SPRAWDZENIE ODCZYTU
	int ile_znakow = 3;
	char* odczyt = pam->Odczytaj(adrs_3,ile_znakow);
	cout << "[ODCZYT z adresu " << adrs_3 << " ] : ";
	for (int i = 0; i < ile_znakow; i++) cout << odczyt[0];
	cout << endl;

	pam->PrintRAM();
	pam->PrintFSB();
	pam->PrintStoS();
	pam->PrintTabliceStronic();
	pam->PrintDYSK();
}

void zad_1(ZarzadzaniePamiecia*pam)
{
	for (int i = 0; i < 17; i++)
	{
		cout << "Prosba o przydzielenie bloku pamieci dla procesu nr." << i << endl;
		int adrs=pam->XA(1);
		cout << "Zapisanie do pamieci zaczynajac od adresu poczatkowego " << adrs << endl;
		char* temp = new char[8];
		if (i != 16)for (int j = 0; j < 16; j++)temp[j] = 'Z'; else for (int j = 0; j < 16; j++)temp[j] = 'D';
		cout << "Wysylane dane to : "; for (int j = 0; j < 16; j++)cout << temp[j]; cout << endl;
		pam->Zapisz(adrs, temp, 16);
		cin.get();

	}
	pam->PrintDYSK();
	cin.get();
	pam->PrintFSB();
	cin.get();
	pam->PrintRAM();
	cin.get();
	pam->PrintStoS();
	cin.get();
	pam->PrintTabliceStronic();
	system("cls");
	cout << "Proces nr 0 ktorego pamiec zostala usunieta z RAM chce odczytac dane " << endl;
	char* temp = pam->Odczytaj(0, 16);
	for (int j = 0; j < 16; j++)cout << temp[j]; cout << endl;
	pam->PrintDYSK();
	cin.get();
	pam->PrintFSB();
	cin.get();
	pam->PrintRAM();
	cin.get();
	pam->PrintStoS();
	cin.get();
	pam->PrintTabliceStronic();
	cin.get();
	system("cls");
	cout << "Proces nr 0 chce nadpisac dane w ramce" << endl;
	temp[0] = 'Z'; temp[1] = 'M'; temp[2] = 'I'; temp[3] = 'A'; temp[4] = 'N'; temp[5] = 'A';
	pam->Zapisz(0, temp, 6);
	cin.get();
	pam->PrintDYSK();
	cin.get();
	pam->PrintFSB();
	cin.get();
	pam->PrintRAM();
	cin.get();
	pam->PrintStoS();
	cin.get();
	pam->PrintTabliceStronic();
	cin.get();
	system("cls");
}
void menu(ZarzadzaniePamiecia*pam)
{
	int opcja;
	cout << "|ZARZADZANIE PAMIECIA OPERACYJNA ORAZ WIRTUALNA ZA POMOCA STOSU LRU |" << endl << endl;
	zad_1(pam);
}


int main()
{
	ZarzadzaniePamiecia* pam = new ZarzadzaniePamiecia(16 * 16);
	menu(pam);
	//test1();
	cin.get();
}