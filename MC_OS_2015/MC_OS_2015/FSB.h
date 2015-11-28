#include <string>
using namespace std;
class FSB
{
public:
	int StronaPoczatkowa;
	int Blokow;
	FSB* Nastepny;

	FSB(int stronaPoczatkowa, int blokow)
	{
		StronaPoczatkowa = stronaPoczatkowa;
		Blokow = blokow;
	}

	string print_status()
	{
		int AdresPoczatkowy = StronaPoczatkowa;
		int Dlugosc = Blokow*16;
		int AdresKoncowy = AdresPoczatkowy + Dlugosc;
		return("Od: " + to_string(AdresPoczatkowy) + " Do : " + to_string(AdresKoncowy) + " Dlugosc: " + to_string(Dlugosc) + " bajtow");
	}
};