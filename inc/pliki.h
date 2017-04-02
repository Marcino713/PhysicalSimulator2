#pragma once
#ifndef pliki_h
#define pliki_h

#include "dane.h"
#include "dodatki.h"
#include "mainwnd.h"
#include "anim_dodatki.h"
#include <Windows.h>
#include <fstream>

using std::ifstream;
using std::ofstream;
using std::ios;

const string NAGLOWEK_PLIK1 = "Pole eg 1#";
const string NAGLOWEK_PLIK2 = "Pole eg 2#";
const int DLUGOSC_NAZWY = 500;
const string BLAD_POZA_BUFOREM = "Probowano odczytac dane spoza bufora.";
const char* const NAZWA_TYPU_PLIKU = "Pliki pola (*.pole)\0*.pole\0";
const string ROZSZERZENIE = "pole";

const double PROTON_MASA_1 = 1.67e-27;
const double PROTON_LADUNEK_1 = 1.6e-19;
const double ELEKTRON_MASA_1 = 9.11e-31;
const double ELEKTRON_LADUNEK_1 = -1.6e-19;
const double PROMIEN_1 = 0.18;

//Flagi
const unsigned char PLIK2_UST_ODBICIASCIANY = 1;

const unsigned char PLIK2_POLECENT_BRAK = 1;
const unsigned char PLIK2_POLECENT_JEST = 2;
const unsigned char PLIK2_POLECENT_NIERUCHOME = 4;
const unsigned char PLIK2_POLECENT_WYBUCHA = 8;

const unsigned char PLIK2_POLEJEDN_GRAWGORA = 1;
const unsigned char PLIK2_POLEJEDN_GRAWDOL = 2;
const unsigned char PLIK2_POLEJEDN_GRAWLEWO = 4;
const unsigned char PLIK2_POLEJEDN_GRAWPRAWO = 8;
const unsigned char PLIK2_POLEJEDN_GRAWPRZOD = 16;
const unsigned char PLIK2_POLEJEDN_GRAWTYL = 32;

const unsigned char PLIK2_POLEJEDN_ELGORADOL = 1;
const unsigned char PLIK2_POLEJEDN_ELGORAPLUS = 2;
const unsigned char PLIK2_POLEJEDN_ELLEWOPRAWO = 4;
const unsigned char PLIK2_POLEJEDN_ELLEWOPLUS = 8;
const unsigned char PLIK2_POLEJEDN_ELPRZODTYL = 16;
const unsigned char PLIK2_POLEJEDN_ELPRZODPLUS = 32;

const int DLUGOSC_DOUBLE = 8;
const int DLUGOSC_INT = 4;
const int DLUGOSC_BOOLEAN = 1;
const int DLUGOSC_CHAR = 1;

class CzytajDane {
private:
	char* dane;
	size_t dlugosc;

public:
	CzytajDane(char* Bufor, size_t Dlugosc);
	double CzytajDouble();
	void CzytajString(char* tekst);
	int CzytajInt();
	bool CzytajBoolean();
	unsigned char CzytajUChar();
};

class PiszDane {
private:
	char* dane;
	char* dane_pocz;
	size_t dlugosc;
	size_t wykorzystano;

public:
	PiszDane(char* Bufor, size_t Dlugosc);
	void Pisz(bool wartosc);
	void Pisz(string wartosc);
	void Pisz(double wartosc);
	void Pisz(unsigned char wartosc);
	void Pisz(int wartosc);
	char* PobierzDane() { return this->dane_pocz; }
	size_t Dlugosc() { return this->wykorzystano; }
};

bool CzyOtworzPlikiCmd(PWSTR Argumenty);
bool OtwPlikuPobierzNazwe(HWND wndOkno);
bool OtworzPlik();
bool OtworzPlik1(char* dane, size_t dlugosc);
bool OtworzPlik2(char* dane, size_t dlugosc);
bool ZapiszPlik(HWND wndOkno);
void ZapiszPlik2(PiszDane& bufor);
size_t ObliczDlugoscPlik2();

#endif