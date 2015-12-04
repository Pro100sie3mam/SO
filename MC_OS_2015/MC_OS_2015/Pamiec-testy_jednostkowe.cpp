#include "zarzadzanie_pamiecia.h"
#include <iostream>
#include <cstdlib>
using namespace std;


void menu(ZarzadzaniePamiecia*pam)
{
	int opcja=0;
	while (opcja != 10)
	{
		cout << "|ZARZADZANIE PAMIECIA OPERACYJNA ORAZ WIRTUALNA ZA POMOCA STOSU LRU |" << endl << endl;
		cout << "--------------------------------" << endl;
		cout << "1.Przydzielanie stron (Adres wirtualny) ." << endl;
		cout << "2.Przydziel ramke (Miejsce w pamieci fizycznej)  i zapisz dane" << endl;
		cout << "3.Odczytaj dane ze strony" << endl;
		cout << "4.Usun strone pamieci" << endl;
		cout << "5.Wyswietl pamiec fizyczna" << endl;
		cout << "6.Wyswiet stan stosu" << endl;
		cout << "7.Wyswietl tablice stronic" << endl;
		cout << "10.EXIT" << endl;
		cout << "Opcja : "; cin >> opcja;
		switch (opcja)
		{
		case 1: {
			int ID;
			int Adres;
			cout << "Podaj numer Procesu : "; cin >> ID;
			Adres=pam->XA(ID);
			cout << "Przydzielono strone " << Adres / 16 << " o adresie " << Adres << endl;
			system("pause");
			break;
				}
		case 2: {
			int numer_strony;
			char* dane = new char[16];
			cout << "Podaj numer strony : "; cin >> numer_strony;
			cout << "Podaj dane do zapisu(maxymalnie 16bajtow-reszta zostanie pominieta) : "; cin >> dane;
			pam->Zapisz(numer_strony * 16, dane, 16);
			cout << "Dane zostaly zapisane w ramce nr "; pam->swap->tablica_stronic[numer_strony]->AdresFizyczny / 16;
			system("pasue");
			break;
				}
		case 3: {cout << "fasfas"; }
		case 4: {
			int numer_strony;
			char* dane = new char[16];
			cout << "Podaj numer strony : "; cin >> numer_strony;
			dane = pam->Odczytaj(numer_strony / 16, 16);
			cout << "Dane : " << dane << endl;
			break;
				}
		case 5: {pam->PrintRAM(); system("pause"); break; }
		case 6: {pam->PrintStoS(); system("pause"); break; }
		case 7: {pam->PrintTabliceStronic(); system("pause"); break; }
		}
		system("cls");

	}
	
	
}


int main()
{
	ZarzadzaniePamiecia* pam = new ZarzadzaniePamiecia(16 * 16);
	menu(pam);
	cin.get();
}