#pragma once
#ifndef fizyka_h
#define fizyka_h

#include "fizyka1.h"
#include "animacja.h"
#include "dane.h"
#include "mainwnd.h"
#include "anim_dodatki.h"
#include <time.h>
#include <process.h>

class Fizyka {
private:
	//Wskazniki na funkcje
	void(*ObliczFizyke_p)(long long Dane, double DeltaT, Punkt* Ciala);
	long long (*PrzygotujDane_p)();
	void(*CzyscDane_p)(long long Dane);
	void(*Wybuch_p)(long long Dane, int IdCiala);
	void(*PobierzPredkosc_p)(long long Dane, int IdCiala, Punkt* Predkosc);

	//Dane fizyczne
	long long DaneFiz;
	long long GrDaneCial;
	clock_t t1;
	clock_t t2;

public:
	HANDLE evtAnimacja;
	HANDLE evtPauza;
	
	void Inicjuj();
	void ObliczFizyke();
	void PrzygotujDane();
	void CzyscDane();
	void WyswietlCiala();
	void Wybuch(int IdCiala);
	void ZaznaczCialo(int id);
	void inline ZerujCzas() { this->t1 = clock(); }

};

void UruchomSymulacje(void* args);

#endif