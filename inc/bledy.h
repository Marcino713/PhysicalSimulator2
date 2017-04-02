#pragma once
#ifndef bledy_h
#define bledy_h

#include <Windows.h>
#include <string>
#include <vector>
#include "dodatki.h"

using std::string;
using std::vector;

const int BL_PUSTE = 1;
const int BL_TEKST = 2;
const int BL_ZERO = 3;
const int BL_MNIEJSZE0 = 4;
const int BL_UJEMNE = 5;
const int BL_ULAMEK = 6;
const int BL_ZADUZO = 7;

const string BLT_BLAD = "B³¹d";
const string BLT_TEKST = "\nPodana wartoœæ nie jest liczb¹.";
const string BLT_PUSTE = "\nPole nie mo¿e byæ puste.";
const string BLT_ZERO = "\nWartoœæ nie mo¿e byæ równa zero.";
const string BLT_MNIEJSZE0 = "\nWartoœæ musi byæ wiêksza ni¿ 0.";
const string BLT_UJEMNE = "\nWartoœæ nie mo¿e byæ ujemna.";
const string BLT_ULAMEK = "\nWartoœæ musi mieœciæ siê w przedziale od 0 do 1.";
const string BLT_ZADUZO = "\nWartoœæ musi byæ mniejsza ni¿ ";
const string BLT_BRAK = "\nBrak b³êdu.";

struct Blad {
	HWND Pole;
	string Nazwa_pola;
	int Nr_bledu;
	int Nr_ciala;
};

class SprawdzanieBledow {
private:
	HWND okno;
	vector<Blad> bledy;

	void DodajBlad(HWND Pole_, string Nazwa_pola_, int Nr_bledu_, int Nr_ciala_);
	void UsunBlad(HWND Pole_);

	int pokazbl(int ix, string blad);
	int pokazbl_polecent(int ix, string blad, int id_ciala);

public:
	SprawdzanieBledow(HWND GlowneOkno){ this->okno = GlowneOkno; }

	//Jesli wartosci sa poprawne, funkcje zwracaja true
	bool PowyzejZeraNP(HWND PoleTekstowe, string NazwaPola, double* Wartosc, int IdCiala = -1);
	bool PowyzejZeraZeroP(HWND PoleTekstowe, string NazwaPola, double* Wartosc, int IdCiala = -1);
	bool PowyzejZeraZeroNP(HWND PoleTekstowe, string NazwaPola, double* Wartosc, int IdCiala = -1);
	bool UlamekZeroP(HWND PoleTekstowe, string NazwaPola, double* Wartosc, int IdCiala = -1);
	bool CokolwiekNP(HWND PoleTekstowe, string NazwaPola, double* Wartosc, int IdCiala = -1);

	bool PokazBledy();
	bool PokazBledyPoleCent();
};

#endif