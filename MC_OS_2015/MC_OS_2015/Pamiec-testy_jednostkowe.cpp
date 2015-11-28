#include "zarzadzanie_pamiecia.h"
#include <iostream>
using namespace std;

void test1()
{
	ZarzadzaniePamiecia* pam = new ZarzadzaniePamiecia(16 * 16);
	int wartosc_wirtualna = pam->XA(1); // jako parametr MUSICIE PODAC ilosc blokow = rozmiar_waszej pamieci / 16
	char* text = new char[16];
	text = "123456789ABCDEFG";
	for (int i = 0; i<16; i++) pam->Zapisz(wartosc_wirtualna + i, text[i]);
	int wartosc_wirtualna2 = pam->XA(1);
	text = "WSZYSTKO_DZIALA!";
	for (int i = 0; i<16; i++) pam->Zapisz(wartosc_wirtualna2 + i, text[i]);
	pam->PrintTabliceStronic();
	cout << endl;
	pam->PrintRAM();
	pam->PrintStoS();
}
int main()
{
	test1();
	cin.get();
}