#include <string>
using namespace std;
class FSB
{
public:
	int StronaPoczatkowa;
	FSB* Nastepny;

	FSB(int stronaPoczatkowa)
	{
		StronaPoczatkowa = stronaPoczatkowa;
	}

	string print_status()
	{
		int AdresPoczatkowy = StronaPoczatkowa;
		int Dlugosc = 16;
		int AdresKoncowy = AdresPoczatkowy + Dlugosc;
		return("Od: " + to_string(AdresPoczatkowy) + " Do : " + to_string(AdresKoncowy) + " Dlugosc: " + to_string(Dlugosc) + " bajtow");
	}
};