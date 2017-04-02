#pragma once
#ifndef dane_h
#define dane_h

#include <string>
#include "dodatki.h"

using std::string;

//Jednostki
struct Jednostka {
	string Przedrostek;
	double Mnoznik;
};

const int JEDNOSTKA_DOMYSLNA = 10;
const int ILOSC_JEDNOSTEK = 21;

const Jednostka JEDNOSTKI[ILOSC_JEDNOSTEK] = {
 { "Y", 1e24 },
 { "Z", 1e21 },
 { "E", 1e18 },
 { "P", 1e15 },
 { "T", 1e12 },
 { "G", 1e9 },
 { "M", 1e6 },
 { "k", 1e3 },
 { "h", 1e2 },
 { "da",1e1 },
 { "",  1 },
 { "d", 1e-1 },
 { "c", 1e-2 },
 { "m", 1e-3 },
 { "µ", 1e-6 },
 { "n", 1e-9 },
 { "p", 1e-12 },
 { "f", 1e-15 },
 { "a", 1e-18 },
 { "z", 1e-21 },
 { "y", 1e-24 }
};

int ZnajdzJednostke(string Przedrostek);
void CzyscDane();

//Dane symulacji
struct strUstawienia {
	double Jednostka0;
	int JednostkaJedn;
	double JednIlosc;
	double UstJednPodzialka;
	bool OdbiciaSciany;
};

struct strCialo {	
	//Polozenie, promien
	double X0;
	double Y0;
	double Z0;
	double R0;
	
	//Predkosc
	bool Nieruchome;
	double V0t;
	double V0Xt;
	double V0Yt;
	double V0Zt;

	double WspRestytucji;

	//Wybuch
	bool Wybucha;
	int SilaJedn;
	double Sila0;
	double Promien0;

	//Masa, ladunek
	double Masa0;
	int MasaJedn;

	double Ladunek0;
	int LadunekJedn;

	strCialo(double x, double y, double z, double r);
	strCialo() :strCialo(0, 0, 0, 1) {};
};

struct strPoleJedn {

	//Pole grawitacyjne
	bool Gora;
	double GoraIle;
	bool Dol;
	double DolIle;
	bool Lewo;
	double LewoIle;
	bool Prawo;
	double PrawoIle;
	bool Przod;
	double PrzodIle;
	bool Tyl;
	double TylIle;

	//Pole elektrostatyczne
	bool GoraDol;
	bool GoraPlus;
	double GoraDolIle;
	bool LewoPrawo;
	bool LewoPlus;
	double LewoPrawoIle;
	bool PrzodTyl;
	bool PrzodPlus;
	double PrzodTylIle;
};

struct strTarcie {

	//Tarcie statyczne
	double StGora;
	double StDol;
	double StLewo;
	double StPrawo;
	double StPrzod;
	double StTyl;

	//Tarcie kinetyczne
	double KinGora;
	double KinDol;
	double KinLewo;
	double KinPrawo;
	double KinPrzod;
	double KinTyl;
};

struct DaneSymulacji {
	strUstawienia Ustawienia;
	Tablica<strCialo> PoleCent;
	strPoleJedn PoleJedn;
	strTarcie Tarcie;
};

#endif