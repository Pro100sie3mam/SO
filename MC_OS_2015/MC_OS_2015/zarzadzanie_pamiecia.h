#include <iostream>
#include <string.h>
#include <vector>
#include "FSB.h"
#include "Stronicowanie.h"
using namespace std;

class ZarzadzaniePamiecia
{
public:
	int rozmiar_bloku=16;
	FSB* FSBPTR;
	int rozmiar_pamieci;
private:
	char* RAM;
	bool* zajete_ramki;
	Stronicowanie* swap;
	list<int> stos_LRU;
	

public :
	ZarzadzaniePamiecia(int rozmiar)
	{
		this->rozmiar_pamieci = rozmiar;
		RAM = new char[rozmiar];
		for (int i = 0; i < rozmiar; i++) RAM[i] = '0';
		zajete_ramki = new bool[rozmiar / rozmiar_bloku];
		for (int i = 0; i < rozmiar / rozmiar_bloku; i++) zajete_ramki[i] = false;
		swap = new Stronicowanie();
	}

private :
// niejawna konwersja adresu logicznego na adres fizyczny
	int AdresWirtualnyNaFizyczny(int adresWirtualny)
	{
// Adres wirtualny traktujemy jako rozmiarStrony * numerStrony + offset wewnatrz strony do bajtu w pamieci
		int numerStrony = adresWirtualny / 16;
		if (numerStrony >= swap->tablica_stronic.size())
		{
			// Strona nie istnieje
			return -1;
		}

		Strona*temp = swap->tablica_stronic[numerStrony];
		auto offset = adresWirtualny % 16;
		if (temp->JestZmapowanaWPamieciFizycznej)
		{
			return temp->AdresPoczatkowy + offset;
		}
		return ZmapujStrone(numerStrony) + offset;
		return 0;
	}

public :
	int XA(int iloscBlokow)
	{
		FSB* nowyBlok;
		if (FSBPTR == NULL)
		{
			nowyBlok = new FSB(0, iloscBlokow);
			FSBPTR = nowyBlok;
			FSBPTR->Nastepny = NULL;
		}
		else
		{
			FSB * temp = FSBPTR;
			while (temp->Nastepny != NULL)
			{
				temp = temp->Nastepny;
			}
			nowyBlok = new FSB(temp->StronaPoczatkowa + temp->Blokow, iloscBlokow);
			temp->Nastepny = nowyBlok;
		}
		while (swap->tablica_stronic.size() < (nowyBlok->StronaPoczatkowa + nowyBlok->Blokow))
		{
			swap->tablica_stronic.push_back(new Strona());
			swap->tablica_stronic[swap->tablica_stronic.size() - 1]->JestNaDysku = false;
			swap->tablica_stronic[swap->tablica_stronic.size() - 1]->JestNaStosie = false;
			swap->tablica_stronic[swap->tablica_stronic.size() - 1]->JestZmapowanaWPamieciFizycznej = false;
		}
		return nowyBlok->StronaPoczatkowa * 16;
	}
	void XB(int adresPoczatkowy)
	{
		auto adresFizyczny = AdresWirtualnyNaFizyczny(adresPoczatkowy);
		int numerStrony = adresPoczatkowy / 16;
		FSB* do_zwolnienia = NULL;
		FSB* poprzedniBlok = NULL;
		FSB* temp = FSBPTR;
		while (temp != NULL)
		{
			if (temp->StronaPoczatkowa == numerStrony)
			{
				do_zwolnienia = temp;
				break;
			}
			poprzedniBlok = temp;
			temp = temp->Nastepny;
		}
		if (do_zwolnienia == NULL)
		{
			cout << "[XB] zly adres lub blok zostal juz zwolniony";
			return;
		}
		if (poprzedniBlok != NULL) poprzedniBlok->Nastepny = do_zwolnienia->Nastepny;
		for (int i = 0; i < do_zwolnienia->Blokow; i++)
		{
			Strona* strona_do_zwolnienia = swap->tablica_stronic[do_zwolnienia->StronaPoczatkowa + 1];
			strona_do_zwolnienia->JestZmapowanaWPamieciFizycznej = false;
			strona_do_zwolnienia->JestNaDysku = false;
			RAM[adresFizyczny] = 0;
		}
	}
	int ZmapujStrone(int numerStrony)
	{
		int adres = -1;
		Strona* strona = swap->tablica_stronic[numerStrony];
		// Znajdz wolna strone pamieci
		bool breakuj = false;
		for (int i = 0; i < rozmiar_pamieci / rozmiar_bloku; i++ )
		{
			if (zajete_ramki[i] == false)
			{
				adres = i*rozmiar_bloku;
				zajete_ramki[i] = true;
				strona->JestZmapowanaWPamieciFizycznej = true;

				breakuj = true;
				
			}
			if (breakuj == true) i = 100;
		}

		// Jesli nie znaleziono wolnej strony, swapuj jedna na dysk zeby zwolnic miejsce
		if (adres == -1)
		{
			list<int>::iterator i = stos_LRU.end(); --i;
			int numer_strony_do_usuniecia = *i;
			swap->tablica_stronic[numer_strony_do_usuniecia]->JestNaDysku = true;
			swap->tablica_stronic[numer_strony_do_usuniecia]->JestNaStosie = false;
			swap->tablica_stronic[numer_strony_do_usuniecia]->JestZmapowanaWPamieciFizycznej = false;
			stos_LRU.pop_back();
			char* dane_do_zapisu = new char[16];
			for (int i = 0; i < rozmiar_bloku; i++)
			{
				dane_do_zapisu[i] = RAM[swap->tablica_stronic[numer_strony_do_usuniecia]->AdresPoczatkowy + i ];
			}
			swap->Zapisz(numer_strony_do_usuniecia, dane_do_zapisu);
			adres = swap->tablica_stronic[numer_strony_do_usuniecia]->AdresPoczatkowy;
		}

		if (strona->JestNaDysku)
		{
			auto dane_z_dysku = swap->Odczytaj((short)numerStrony);
			for (int i = 0; i < rozmiar_bloku; i++)
			{
				RAM[adres + i] = dane_z_dysku[i];
			}
			strona->JestZmapowanaWPamieciFizycznej = true;
		}
		else
		{
			// Strona istnieje, ale nigdy wczesniej nie byla uzywana.
			// Mapujemy ja w pamieci i zerujemy
			for (int i = 0; i < rozmiar_bloku; i++)
			{
				RAM[adres + i] = 0;
			}
		}
		bool znaleziono_juz_ten_numer_na_stosie = false;
		for (list<int>::iterator iter = stos_LRU.begin(); iter != stos_LRU.end(); iter++)
		{
			if (*iter == adres/ 16)
			{
				list<int>::iterator temp = stos_LRU.erase(iter);
				stos_LRU.push_front(adres / 16);
				znaleziono_juz_ten_numer_na_stosie = true;
			}
		}
		if (znaleziono_juz_ten_numer_na_stosie == false)stos_LRU.push_front(adres / 16);
		strona->JestNaStosie = true;
		strona->PamiecBylaModyfikowana = false;
		strona->AdresPoczatkowy = adres;
		return adres;
	}
	char Odczytaj(int adresWirtualny)
	{
		auto adresFizyczny = AdresWirtualnyNaFizyczny(adresWirtualny);
		if (adresFizyczny == -1)
		{
			// TODO: blad, proba odczytu spod niezaalokowanego adresu
			return 0;
		}
		return RAM[adresFizyczny];
	}
	void Zapisz(int adresWirtualny, char wartosc)
	{
		auto adresFizyczny = AdresWirtualnyNaFizyczny(adresWirtualny);
		if (adresFizyczny == -1)
		{
			return;
		}
		RAM[adresFizyczny] = wartosc;
		if (swap->tablica_stronic[adresWirtualny / 16]->JestNaDysku == 1)
		swap->tablica_stronic[adresWirtualny / 16]->PamiecBylaModyfikowana = 1;
		
		
	}
	void PrintRAM()
	{
		cout << "------RAM-------" << endl;
		for (int i = 0; i < rozmiar_bloku; i++)
		{
			for (int j = 0; j < 16; j++) cout << RAM[16*i+j];
			cout << endl;
		}
		cout << "----------------" << endl;

	}
	void PrintTabliceStronic()
	{
		cout << "------------------TABLICA STRONIC ----------------------------" << endl;
		for (int i = 0; i < swap->tablica_stronic.size(); i++)
			cout << "Strona : " << i << " AdresPoczatkowy : " <<swap->tablica_stronic[i]->AdresPoczatkowy << " DYSK : " << swap->tablica_stronic[i]->JestNaDysku << " STOS : " << swap->tablica_stronic[i]->JestNaStosie << " RAM : " << swap->tablica_stronic[i]->JestZmapowanaWPamieciFizycznej << " bit modyfikacji : " << swap->tablica_stronic[i]->PamiecBylaModyfikowana << endl;
		cout << "--------------------------------------------------------------" << endl;
	}
	void PrintStoS()
	{
		cout << "------------------------------STOS--------------------------------------" << endl;
		for (list<int>::iterator iter = stos_LRU.begin(); iter != stos_LRU.end(); iter++)
		{
			cout << *iter << "  ";
		}
		cout << endl;
		cout << "------------------------------------------------------------------------";
	}
	
};