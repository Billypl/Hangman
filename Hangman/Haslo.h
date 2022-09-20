#pragma once

#pragma once
#include <iostream>
#include <string>
#include <algorithm> //transform
#include <set>
using namespace std;

class Haslo {
public:
	string slowo;
	string kreskowanePodstawa;
	string kreskowane;
	string kategoria;
	set <char> unikalneLitery;

	//metody
	void zamien_na_kreski()
	{
		for (int i = 0; i < slowo.length(); i++)
		{
			unikalneLitery.insert(slowo[i]); //stworzenie seta, pomocne przy wygranej
			if (slowo[i] != ' ')
				kreskowanePodstawa += "_";
			else
				kreskowanePodstawa += '-';
		}
	}
	void kreskowane_ze_spacjami()
	{
		kreskowane = "";
		for (int i = 0; i < kreskowanePodstawa.length(); i++)
		{
			kreskowane = kreskowane + kreskowanePodstawa[i] + ' ';
		}
	}

	Haslo(string haslo, string kat)
	{
		transform(haslo.begin(), haslo.end(), haslo.begin(), ::toupper);
		slowo = haslo;
		kategoria = kat;
		zamien_na_kreski();
		kreskowane_ze_spacjami();
	}
};
