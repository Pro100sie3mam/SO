#include <iostream>
using namespace std;

class StackLRU
{
public:
	int numer_strony;
protected:
	StackLRU* first;
	StackLRU* next;
	int TempNumberOfPages = 0;
public:

	StackLRU::StackLRU()
	{
		first = NULL;
	}
	void PushPage(int numer)
	{
		StackLRU *nowa = new StackLRU;
		nowa->numer_strony = numer;

		if (first == NULL)
		{
			first = nowa;
			first->next = NULL;
		}
		else
		{
			StackLRU* temp = first;
			bool znaleziono_juz_na_stosie = false;
			int ile_przesuniec = 0;
			while (temp->next != NULL && znaleziono_juz_na_stosie==false)
			{
				
				if (temp->numer_strony == numer)
				{
					if (temp->numer_strony != first->numer_strony)
					{
						StackLRU* poprzednik = first;
						for (int i = 0; i < ile_przesuniec - 1; i++) poprzednik = poprzednik->next;
						poprzednik->next = temp->next;
						StackLRU* temp = first;
						first = nowa;
						nowa->next = temp;
						znaleziono_juz_na_stosie = true;
					}
					else
					{
						znaleziono_juz_na_stosie = true;
					}

				}
				temp = temp->next;
				ile_przesuniec++;

			}
			if (znaleziono_juz_na_stosie == false)
			{
				StackLRU* temp = first;
				first = nowa;
				nowa->next = temp;
			}
		}
	
	



	}
	int PopPage()
	{
		StackLRU* temp = first;
		while (temp->next->next != NULL) temp = temp->next;
		int numer = temp->next->numer_strony;
		temp->next = NULL;
		return numer;
	}
	void Print()
	{
		if (first == NULL) cout << "Empty";
		else
		{
			StackLRU* temp = first;
			while (temp != NULL)


			{
				cout << temp->numer_strony<< " -> ";
				temp = temp->next;
			}
			cout << "NULL";
		}
		cout << endl;
	}

};