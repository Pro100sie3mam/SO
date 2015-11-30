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
public:
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
	int Zajdz_wolna_ramke(int numerStrony)
	{
		int adres = -1;
		Strona* strona = swap->tablica_stronic[numerStrony];
		// Znajdz wolna strone pamieci
		bool breakuj = false;
		for (int i = 0; i < (rozmiar_pamieci / rozmiar_bloku); i++)
		{
			if (zajete_ramki[i] == false)
			{
				adres = i*rozmiar_bloku;
				zajete_ramki[i] = true;
				strona->JestZmapowanaWPamieciFizycznej = true;
				strona->AdresFizyczny = i * 16;
				cout << "Znaleziono wolna ramke" << i << endl;
				breakuj = true;

			}
			if (breakuj == true) break;
		}
		return adres;
	}
	void dodaj_na_stos(int numerStrony)
	{
		stos_LRU.push_front(numerStrony);
	}
	int Usun_ramke()
	{
		list<int>::iterator i = stos_LRU.end(); --i;
		int numer_strony_do_usuniecia = *i;
		cout << "Najdawniej uzywana strona to : " << *i << endl;
		cout << "Zostanie przeniesiona na dysk - Ramka zostaje zwolniona" << endl;
		cout << "Zostaja ustawione bity w tablicy stronic " << endl;
		swap->tablica_stronic[numer_strony_do_usuniecia]->JestNaDysku = true;
		swap->tablica_stronic[numer_strony_do_usuniecia]->JestNaStosie = false;
		int adresWolnejRamki = swap->tablica_stronic[numer_strony_do_usuniecia]->AdresFizyczny;
		cout << "W tablicy stronic adres Ramki ustawiamy na -1" << endl;
		swap->tablica_stronic[numer_strony_do_usuniecia]->AdresFizyczny = -1;
		swap->tablica_stronic[numer_strony_do_usuniecia]->JestZmapowanaWPamieciFizycznej = false;
		stos_LRU.pop_back();
		char* dane_do_zapisu = new char[16];
		for (int i = 0; i < rozmiar_bloku; i++)
		{
			dane_do_zapisu[i] = RAM[adresWolnejRamki + i];
		}
		swap->Zapisz(numer_strony_do_usuniecia, dane_do_zapisu);

		cout << "Ramka w pamieci operacyjnej zostaje wyzerowana " << endl;
		for (int i = 0; i < rozmiar_bloku; i++)
		{
			RAM[adresWolnejRamki + i] = 0;
		}
		cout << "Nastepuje przekazanie adresu wolnej Ramki" << endl;
		return adresWolnejRamki;
	}

public :

	//// ZNALEZIENIE_BLOKU_PAMIECI
	int XA(int iloscBlokow)
	{
		FSB* nowyBlok;
		if (FSBPTR == NULL)
		{
			cout << "Proces uzykal pierwszy blok wolnej pamieci - ustawiono wskaznik FSBPTR na ten objekt FSB" << endl;
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
			nowyBlok = new FSB(temp->StronaPoczatkowa + temp->Blokow*16, iloscBlokow);
			temp->Nastepny = nowyBlok;
		}
		for (int i = 0; i < nowyBlok->Blokow;i++)
		{
			swap->tablica_stronic.push_back(new Strona());
			swap->tablica_stronic[swap->tablica_stronic.size() - 1]->AdresPoczatkowy = nowyBlok->StronaPoczatkowa + 16*i;
			swap->tablica_stronic[swap->tablica_stronic.size() - 1]->JestNaDysku = false;
			swap->tablica_stronic[swap->tablica_stronic.size() - 1]->JestNaStosie = false;
			swap->tablica_stronic[swap->tablica_stronic.size() - 1]->JestZmapowanaWPamieciFizycznej = false;
			cout << "ustawiono pole w tablicy stronic : Adres Logiczny : " << swap->tablica_stronic[(nowyBlok->StronaPoczatkowa / 16) + i]->AdresPoczatkowy << endl;
		}
		cout << "Zwrocono adres wirtualny ktory powinien zostac umieszczony w bloku PCB" << endl;
		return nowyBlok->StronaPoczatkowa;
	}
	void XB(int adresPoczatkowy)
	{
	int numerStrony = adresPoczatkowy / 16;
	FSB* do_zwolnienia = NULL;
	FSB* poprzedniBlok = NULL;
	FSB* temp = FSBPTR;
	if (swap->tablica_stronic[numerStrony]->JestZmapowanaWPamieciFizycznej == 1)
	{
		swap->tablica_stronic[numerStrony]->JestZmapowanaWPamieciFizycznej = false;
		int i = 0;
		while (temp != NULL)
		{
			if (temp->StronaPoczatkowa/16 == numerStrony)
			{
				do_zwolnienia = temp;
				break;
			}
			poprzedniBlok = temp;
			temp = temp->Nastepny;
			i=i+16;
		}
		if (do_zwolnienia == NULL)
		{
			cout << "[XB] zly adres ";
			return;
		}
		if (poprzedniBlok != NULL) poprzedniBlok->Nastepny = do_zwolnienia->Nastepny;
		for (int i = 0; i < do_zwolnienia->Blokow; i++)
		{
			for (int j = 0; j<16; j++)RAM[(swap->tablica_stronic[numerStrony]->AdresFizyczny) + j] = '0';
			cout << "[XB] Strona o adresie poczatkowym : " << numerStrony * 16 << " - usunieto z blokow,ze stosu , z RAM" << endl;
			swap->tablica_stronic[numerStrony]->AdresFizyczny = -1;
		}
		swap->tablica_stronic[numerStrony]->JestNaStosie = false;
		bool znaleziono_juz_ten_numer_na_stosie = false;
		for (list<int>::iterator iter = stos_LRU.begin(); iter != stos_LRU.end(); iter++)
		{
			if (*iter == numerStrony)
			{
				list<int>::iterator temp = stos_LRU.erase(iter);
				znaleziono_juz_ten_numer_na_stosie = true;
			}
			if (znaleziono_juz_ten_numer_na_stosie == true)break;
		}
		
	}
	else
	{
		cout << "[XB] blok zostal juz wczesniej usuniety !";
	}
	
	
	}
	char* Odczytaj(int adresWirtualny,int dlugosc)
	{
		if (swap->tablica_stronic[adresWirtualny / 16]->JestZmapowanaWPamieciFizycznej)
		{
			char* temp=new char[dlugosc];
			for (int i = 0; i < dlugosc; i++) temp[i]= RAM[(swap->tablica_stronic[adresWirtualny / 16]->AdresFizyczny) + i];
			list<int>::iterator inter = stos_LRU.begin();
			if (*inter != adresWirtualny / 16)
			{
				bool znaleziono_juz_ten_numer_na_stosie = false;
				for (list<int>::iterator a = stos_LRU.begin(); a!= stos_LRU.end(); a++)
				{
					if (*a == adresWirtualny/16)
					{
						list<int>::iterator temp = stos_LRU.erase(a);
						stos_LRU.push_front(adresWirtualny/16);
						znaleziono_juz_ten_numer_na_stosie = true;
					}
					if (znaleziono_juz_ten_numer_na_stosie == true)break;
				}
				cout << "[STOS_LRU] Strona : " << adresWirtualny / 16 << " przeniesiona na szczyt stosu " << endl;
			}
			return  temp;
		}
		else
		{

			if (swap->tablica_stronic[adresWirtualny / 16]->JestNaDysku = true)
			{
				cout << "Strona " << adresWirtualny / 16 << "nie znajdowala sie w pamieci operacyjnej ale istnieje na dysku" << endl;
				cout << "Odczytany zostal bit w tablicy stronic 'JestNaDysku' " << endl;
				char* temp = new char[16];
				cout << "Nastepuje odczytanie strony z dysku oraz przydzielenia spowrotem mu ramki " << endl;
				temp = swap->Odczytaj(adresWirtualny / 16);
				Zapisz(adresWirtualny, temp, 16);
				char*dane = new char[dlugosc];
				for (int i = 0; i < dlugosc; i++)dane[i] = RAM[(swap->tablica_stronic[adresWirtualny / 16]->AdresFizyczny) + i];
				cout << "Zwracane zostaja dane z ramki" << endl;
				return dane;
			}
			else
			{
				cout << "[Odczytaj] Blad odwolania sie do adresu" << endl;
			}
			
			
		}
		return 0;
	}
	void Zapisz(int adresWirtualny, char* wartosc , int dlugosc)
	{
		int numer_strony = adresWirtualny / 16;
		if (swap->tablica_stronic[numer_strony]->JestZmapowanaWPamieciFizycznej == true)
		{
			cout << "Strona znajdowala sie juz w pamieci RAM - nadpisanie" << endl;
			for (int i = 0; i < dlugosc; i++)
			{
				RAM[(swap->tablica_stronic[numer_strony]->AdresFizyczny) + i] = wartosc[i];
			}
			if (swap->tablica_stronic[numer_strony]->JestNaDysku == true)
			{
				swap->tablica_stronic[numer_strony]->PamiecBylaModyfikowana == true;
			}

			bool znaleziono_juz_ten_numer_na_stosie = false;
			for (list<int>::iterator iter = stos_LRU.begin(); iter != stos_LRU.end(); iter++)
			{
				if (*iter == numer_strony)
				{
					list<int>::iterator temp = stos_LRU.erase(iter);
					stos_LRU.push_front(numer_strony);
					znaleziono_juz_ten_numer_na_stosie = true;
					cout << "Strona znajdowala sie juz na stosie - przesuniecie jej na szczyt" << endl;
				}
				if (znaleziono_juz_ten_numer_na_stosie == true)break;
			}
			if (znaleziono_juz_ten_numer_na_stosie == false)
			{
				stos_LRU.push_front(numer_strony);
				cout << "Strona nie znajdowala sie na stosie - ustawienie jej na szczycie stosu" << endl;
			}

			if (swap->tablica_stronic[numer_strony]->JestNaDysku == true)
			{
				cout << "Strona znajduje sie na dysku - bit modyfikacji w tablicy stronic ustawiamy na 1" << endl;
				cout << "Na dysku nie znajduje sie juz wierna kopia " << endl;
				swap->tablica_stronic[numer_strony]->PamiecBylaModyfikowana = true;
			}
		}
		else
		{
			cout << "Szukamy wolnej ramki pamieci " << endl;
			int Adres_RAM = Zajdz_wolna_ramke(numer_strony);
			if (Adres_RAM != -1)
			{
				cout << "Znaleziono wolna ramke o adresie fizycznym (poczatek) : " << Adres_RAM << endl;
				swap->tablica_stronic[numer_strony]->JestZmapowanaWPamieciFizycznej = true;
				swap->tablica_stronic[numer_strony]->AdresFizyczny = Adres_RAM;
				cout << "Zapisanie danych do ramki " << endl;
				for (int i = 0; i < dlugosc; i++)
				{
					RAM[(swap->tablica_stronic[numer_strony]->AdresFizyczny) + i] = wartosc[i];
				}
				cout << "Ustawienie na szczycie stosu strony : " << numer_strony << endl;
				dodaj_na_stos(numer_strony);
				swap->tablica_stronic[numer_strony]->JestNaStosie = true;
			}
			else
			{
				cout << "Nie znaleziono wolnej ramki pamieci - nastapi usuniecie innej wg. LRU" << endl;
				int adres_wolnej_ramki = Usun_ramke();
				swap->tablica_stronic[numer_strony]->JestZmapowanaWPamieciFizycznej = true;
				swap->tablica_stronic[numer_strony]->AdresFizyczny = adres_wolnej_ramki;
				for (int i = 0; i < dlugosc; i++)
				{
					RAM[(swap->tablica_stronic[numer_strony]->AdresFizyczny) + i] = wartosc[i];
				}
				cout << "Przydzielono ramke o adresie fizycznym (poczatek) : " << adres_wolnej_ramki<< endl;
				cout << "Ustawiono pola w tablicy stronic : JestWRAM , JestNaStosie, Numer Ramki - na true" << endl;
				swap->tablica_stronic[numer_strony]->JestNaStosie = true;
				stos_LRU.push_front(numer_strony);
				cout << "Strona nie znajdowala sie na stosie - ustawienie jej na szczycie stosu" << endl;
			}
		}
		cout << endl;
	}

	///////////////////////////////////////////////////////////////////////////////
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
		cout << "----------------TABLICA STRONIC -------" << endl;
		for (int i = 0; i < swap->tablica_stronic.size(); i++)
			cout << "Strona : " << i << " AdresPoczatkowy : " <<swap->tablica_stronic[i]->AdresPoczatkowy << " DYSK : " << swap->tablica_stronic[i]->JestNaDysku << " STOS : " << swap->tablica_stronic[i]->JestNaStosie << " RAM : " << swap->tablica_stronic[i]->JestZmapowanaWPamieciFizycznej << " bit modyfikacji : " << swap->tablica_stronic[i]->PamiecBylaModyfikowana << endl;
		cout << "---------------------------------------" << endl;
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
	void PrintFSB()
	{
		if (FSBPTR == NULL) cout << "Brak blokow pamieci !";
		else
		{
			FSB* temp = FSBPTR;
			cout << endl;
			cout << "-------------BLOKI PAMIECI------------------------------------------- " << endl;
			while (temp != NULL)
			{
				cout << temp->print_status() << endl;
				temp = temp->Nastepny;
			}
			cout << "----------------------------------------------------------------------" << endl;
		}
			

	}
	void PrintDYSK()
	{
		cout << "-----------STRONY wyslane na dysk -------------" << endl;
		for (int i = 0; i < swap->tablica_stronic.size(); i++)
		{
			if (swap->tablica_stronic[i]->JestNaDysku == true)
			{
				cout << "Numer strony : " << i << " Dane : ";
				char* temp = new char[16];
				temp = swap->Odczytaj(i);
				for(int j=0;j<16;j++)cout << temp[j];
				cout << endl;
			}
		}
		cout << "------------------------------------------------" << endl;
	}
	///////////////////////////////////////////////////////////////////////////////
};