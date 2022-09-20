#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h> //system("CLS")
#include <conio.h> //_getch
#include <Windows.h> //kolory
#include <time.h> //random
#include <filesystem>//ilość plików w folderze
#include <set>
#include "Haslo.h"
#define SET_DEFAULT_COLOR SetConsoleTextAttribute(console, 7)
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

void gotoxy(int x, int y)
{
	COORD c;
	c.X = x - 1;
	c.Y = y - 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

using namespace std;

char c_klawiatura[26];
int b_klawiatura[26];
int centrum;

//ILOŚĆ PLIKÓW W FOLDERZE
namespace fs = filesystem;
size_t number_of_files_in_directory(fs::path path)
{
	return (size_t)distance(fs::directory_iterator{ path }, fs::directory_iterator{});
}
int liczbaPlikow = number_of_files_in_directory(fs::path("hasla/"));


void wypelnij_klawiature();
void wydrukuj_klawiature();
int szerokosc_terminala();
void wczytaj_plansze(int);
void wypisz_haslo(Haslo haslo);
void setup(Haslo);

void sprawdzanie_litery(char, Haslo&);
int* pomylkiIpoprawne();

void policz_iloscSlowWPliku(int* slowa)
{
	for (int i = 0; i < liczbaPlikow; i++)
	{
		slowa[i] = 0;

		string path = string("hasla/") + to_string(i + 1) + ".txt";
		ifstream plik(path);
		string linia;

		while (getline(plik, linia))
		{
			slowa[i] += 1;
		}
		plik.close();
	}
}

void wczytaj_slowa(string** slowa)
{
	for (int i = 0; i < liczbaPlikow; i++)
	{
		string path = string("hasla/") + to_string(i + 1) + ".txt";
		ifstream plik(path);
		string linia;

		int  j = 0;
		while (getline(plik, linia))
		{
			slowa[i][j] = linia;
			j++;
		}
		plik.close();
	}
}

int main()
{
	centrum = szerokosc_terminala() + 50;
	////***tworzenie tablicy na hasla***///
	int* iloscSlowWPliku = new int[liczbaPlikow];
	policz_iloscSlowWPliku(iloscSlowWPliku);

	string** slowa = new string * [liczbaPlikow];
	for (int i = 0; i < liczbaPlikow; i++)
	{
		slowa[i] = new string[iloscSlowWPliku[i]];
	}
	wczytaj_slowa(slowa);
	////*********************************///

	//od nowa (po zakończonej grze)
x:
	system("CLS");

	//losowanie slowa
	srand(time(0));
	int nrPliku = rand() % liczbaPlikow;
	string kategoria = slowa[nrPliku][0];

	int nrSlowa = (rand() % (iloscSlowWPliku[nrPliku] - 2)) + 1;
	string slowo = slowa[nrPliku][nrSlowa];

	Haslo haslo(slowo, kategoria);
	setup(haslo);

	gotoxy(10, 5);
	cout << "Kategoria: " << haslo.kategoria;

	gotoxy(10, 6);
	cout << "Podaj litere: ";

	//zgadywanie
	while (char litera = _getch())
	{
		litera = toupper(litera);
		sprawdzanie_litery(litera, haslo);

		int pomylki = pomylkiIpoprawne()[0];
		int poprawne = pomylkiIpoprawne()[1];
		if (pomylki > 10)
		{
			gotoxy(60, 20);
			SetConsoleTextAttribute(console, 12);
			cout << "Przegrana! ";
			break;
		}
		if (poprawne == (haslo.unikalneLitery.size() - haslo.unikalneLitery.count(' ')))
		{
			gotoxy(60, 20);
			SetConsoleTextAttribute(console, 10);
			cout << "Wygrana! ";
			break;
		}
		gotoxy(10, 6);
		cout << "Podaj litere: ";
	}
	SET_DEFAULT_COLOR;

	//powtorka
	gotoxy(50, 21);
	cout << "Chcesz zagrac jeszcze raz? (T/N): ";
	while (char decyzja = _getch())
	{
		decyzja = toupper(decyzja);
		if (decyzja == 'T')
			goto x;
		else if (decyzja == 'N')
			break;
	}

	for (int i = 0; i < liczbaPlikow; i++)
	{
		delete[] slowa[i];
	}
	delete[] slowa;

	return 0;
}


int* pomylkiIpoprawne()
{
	int pomylki = 0, poprawne = 0;
	for (int i = 0; i < 26; i++)
	{
		if (b_klawiatura[i] == 1)
			poprawne += 1;
		else if (b_klawiatura[i] == -1)
			pomylki += 1;
	}
	int tab[2] = { pomylki, poprawne };
	return tab;
}

int szerokosc_terminala()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	return columns;
}

void wczytaj_plansze(int nr) {
	string path = string("hangman_states/wisielec_") + to_string(nr) + ".txt";
	ifstream plik(path);
	string linia;
	gotoxy(1, 9);
	while (getline(plik, linia))
		cout << linia << endl;
	plik.close();
}

void wypisz_haslo(Haslo haslo)
{
	//centrowanie hasła
	int centrumHasla = centrum / 2 - haslo.kreskowane.length() / 2;
	gotoxy(centrumHasla, 4);
	cout << haslo.kreskowane;
}

void setup(Haslo haslo)
{
	cout << "Witaj w wisielcu!" << endl << "Zgadnij slowo: ";
	wypelnij_klawiature();

	wypisz_haslo(haslo);

	wydrukuj_klawiature();
}

void wypelnij_klawiature()
{
	for (int i = 65, j = 0; i <= 90; i++, j++)
	{
		c_klawiatura[j] = i;
		b_klawiatura[j] = 0;
	}
}

void wydrukuj_klawiature()
{
	int litera = 0;
	for (int i = 1; i <= 3; i++)
	{
		gotoxy(centrum / 2 - 9, 5 + 2 * i);
		if (i == 3) cout << " "; //if do zrobienia odstepu
		for (int j = 1; j <= 9 && litera <= 25; j++)
		{
			switch (b_klawiatura[litera])
			{
			case -1:
				SetConsoleTextAttribute(console, 12);
				break;
			case 1:
				SetConsoleTextAttribute(console, 10);
				break;
			}

			cout << c_klawiatura[litera];
			SET_DEFAULT_COLOR;
			cout << " ";
			litera++;
		}
	}
}

void sprawdzanie_litery(char klawisz, Haslo& haslo)
{
	//czyszczenie zaznaczenia o powtórzonej literze
	gotoxy(10, 7);
	cout << "                       ";

	//sprawdzanie
	bool wystepuje = false;
	if (b_klawiatura[int(klawisz) - 65] == 0)
	{
		for (int i = 0; i < haslo.slowo.length(); i++)
		{
			if (haslo.slowo[i] == klawisz)
			{
				wystepuje = true;
				haslo.kreskowanePodstawa[i] = klawisz;
				haslo.kreskowane_ze_spacjami();
				b_klawiatura[int(klawisz) - 65] = 1;
			}
		}
		if (wystepuje == false)
		{
			b_klawiatura[int(klawisz) - 65] = -1;
			wczytaj_plansze(pomylkiIpoprawne()[0]);
		}
		wydrukuj_klawiature();
	}
	else
	{
		gotoxy(10, 7);
		cout << "Podales juz ta litere!";
	}
	wypisz_haslo(haslo);
}