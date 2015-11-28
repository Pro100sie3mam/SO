#include <list>
class Strona
{
public:
	const int Rozmiar = 16;
	int AdresPoczatkowy;
	bool JestZmapowanaWPamieciFizycznej;
	bool JestNaDysku;
	bool JestNaStosie;
	bool PamiecBylaModyfikowana;
};

class Stronicowanie
{

private:
	struct StronaNaDysku
	{
	public:
		short Numer;
		char* Dane;
	};
	list<StronaNaDysku*> strony_na_dysku;

public:
	char* Odczytaj(short numerStrony)
	{
		for each (auto StronaNaDysku in strony_na_dysku)
		{
			if (StronaNaDysku->Numer != numerStrony) continue;
			char* copy = new char[16];
			copy = StronaNaDysku->Dane;
			return copy;
		}

	}
	void Zapisz(short numerStrony, char* dane)
	{
		// Szukamy strony na dysku, jesli byla juz utworzona, to ja nadpisujemy
		for each (auto strona in strony_na_dysku)
		{
			if (strona->Numer == numerStrony)
			{
				strona->Dane = dane;
				return;
			}
		}

		// Ta strona nie byla jeszcze na dysku, tworzymy nowa
		StronaNaDysku* nowaStrona = new StronaNaDysku;
		nowaStrona->Dane = new char[16];
		nowaStrona->Numer = numerStrony;
		nowaStrona->Dane = dane;
		strony_na_dysku.push_back(nowaStrona);
	}
	vector<Strona*> tablica_stronic;
};