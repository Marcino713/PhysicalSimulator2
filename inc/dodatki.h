#pragma once
#ifndef dodatki_h
#define dodatki_h

#include <cstring>
#include <string>
#include <sstream>
#include <Windows.h>
#include <CommCtrl.h>

using std::ostringstream;
using std::string;

const char ZNAKI_LICZBA[] = "0123456789,.+-eE";
const int DLUGOSC_TEKSTU = 30;
const int PRECYZJA = 7;
const int POWIEKSZ_TABLICE = 5;

void zamien_przecinki(char* &tekst);
bool czy_liczba(char* tekst);
void ftoa(char* &tekst, double liczba);
void itoid(char* &tekst, int liczba);
void itod(char* &tekst, double liczba, string jednostka);
void idtoi(int& liczba, char* tekst);

struct strStanProgramu {
	bool Animacja = false;
	bool Pauza = false;
	string NazwaPliku = "";
	bool Zapisany = true;
};

template <typename T>
class Tablica {
private:

	T** dane;
	int dlugosc = 0;
	void(*UstawIloscCial_p)(int ilosc) = NULL;
	HWND listview;

public:

	void inicjalizuj(void(*UstawIloscCial)(int ilosc), HWND ListView) {
		this->UstawIloscCial_p = UstawIloscCial;
		this->listview = ListView;
	}

	~Tablica() {
		this->czysc();
	}

	int dodaj(T* element){
		//Znalezienie wolnej komorki
		for (int i = 0; i < this->dlugosc; i++) {
			if (this->dane[i] == nullptr) {
				this->dane[i] = element;
				return i;
			}
		}
		
		//Powieksz tablice
		bool pusta = (this->dlugosc == 0) ? true : false;
		this->dlugosc += POWIEKSZ_TABLICE;
		T** tab = new T*[this->dlugosc];

		for (int i = 0; i < (this->dlugosc - POWIEKSZ_TABLICE); i++) {
			tab[i] = this->dane[i];
		}

		for (int i = this->dlugosc - POWIEKSZ_TABLICE; i < this->dlugosc; i++) {
			tab[i] = nullptr;
		}

		if(!pusta) delete[] dane;
		this->dane = tab;
		
		this->dane[this->dlugosc-POWIEKSZ_TABLICE] = element;
		this->UstawIloscCial_p(this->dlugosc);
		return this->dlugosc-POWIEKSZ_TABLICE;
	}

	void usun(int ix) {
		delete dane[ix];
		dane[ix] = nullptr;
	}

	void czysc() {
		if (this->dlugosc == 0) return;

		for (int i = 0; i < this->dlugosc; i++) {
			delete this->dane[i];
		}

		delete[] this->dane;
		this->dlugosc = 0;
		this->UstawIloscCial_p(0);
		ListView_DeleteAllItems(this->listview);
	}

	bool czy_puste() {
		if (this->dlugosc == 0) return true;

		for (int i = 0; i < this->dlugosc; i++) {
			if (this->dane[i] != nullptr) return false;
		}

		return true;
	}

	int rozmiar_tablicy() { return this->dlugosc; }

	T* operator[](int ix) {
		return this->dane[ix];
	}

};

#endif