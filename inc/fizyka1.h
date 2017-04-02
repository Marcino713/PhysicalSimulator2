#pragma once
#ifndef fizyka1_h
#define fizyka1_h

#include "dane.h"

//Stale fizyczne
const double G = 6.67e-11;	//Stala grawitacji
const double K = 8.99e9;	//Stala elektrostatyczna

struct Punkt {
	double x;
	double y;
	double z;
	double n;
};

struct CialoParametryFizyka1 {
	//Wspolrzedne w metrach
	double Xm;
	double Ym;
	double Zm;
	double Rm;

	double VXm;
	double VYm;
	double VZm;

	double Sila;
	double Promien;
	double Masa;
	double Ladunek;

	double WspRestytucji;
};

struct CialoDaneFizyka1 {
	//Sila
	double Fx;
	double Fy;
	double Fz;

	//Przyspieszenie
	double Ax;
	double Ay;
	double Az;

	//Kontakt z krawedziami
	bool Gora;
	bool Dol;
	bool Lewo;
	bool Prawo;
	bool Przod;
	bool Tyl;
};

struct CialoAtrybutyFizyka1 {
	bool Puste;
	bool Nieruchome;
	bool Wybucha;
};

struct DaneFizyka1 {
	CialoParametryFizyka1* Ciala;
	CialoDaneFizyka1* Dane;
	CialoAtrybutyFizyka1* Atrybuty;
	strPoleJedn PoleJedn;
	strTarcie Tarcie;
	double Jednostka;
	bool OdbiciaSciany;
	double Rozmiar;
	int Ilosc;
};


void ObliczFizyke1(long long Dane, double DeltaT, Punkt* Ciala);
long long PrzygotujDane1();
void CzyscDane1(long long Dane);
void Wybuch1(long long Dane, int IdCiala);
void PobierzPredkosc1(long long Dane, int IdCiala, Punkt* Predkosc);

#endif