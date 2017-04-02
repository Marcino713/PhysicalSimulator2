#include "..\inc\bledy.h"

inline int SprawdzanieBledow::pokazbl(int ix, string blad) { return MessageBox(this->okno, (this->bledy[ix].Nazwa_pola + blad).c_str(), BLT_BLAD.c_str(), MB_ICONERROR | MB_OKCANCEL); }

void SprawdzanieBledow::DodajBlad(HWND Pole_, string Nazwa_pola_, int Nr_bledu_, int Nr_ciala_){
	Blad bl = { Pole_, Nazwa_pola_, Nr_bledu_, Nr_ciala_};
	this->bledy.push_back(bl);
}

void SprawdzanieBledow::UsunBlad(HWND Pole_){
	bool znaleziony = true;

	while (znaleziony) {
		znaleziony = false;

		for (auto it = this->bledy.begin(); it != this->bledy.end(); it++) {

			if((*it).Pole == Pole_) {
				this->bledy.erase(it);
				znaleziony = true;
				break;
			}

		}

	}
}

int SprawdzanieBledow::pokazbl_polecent(int ix, string blad, int id_ciala){
	ostringstream s;
	s << "Pole centralne (cia³o nr " << (id_ciala + 1) << ")\n";
	s << this->bledy[ix].Nazwa_pola;
	s << blad;
	
	return MessageBox(this->okno, s.str().c_str(), BLT_BLAD.c_str(), MB_ICONERROR | MB_OKCANCEL);
}

//Sprawdza, czy pole nie jest puste (NP) oraz wartosc jest > 0
bool SprawdzanieBledow::PowyzejZeraNP(HWND PoleTekstowe, string NazwaPola, double * Wartosc, int IdCiala){
	this->UsunBlad(PoleTekstowe);

	char* b = new char[DLUGOSC_TEKSTU];
	GetWindowText(PoleTekstowe, b, DLUGOSC_TEKSTU);

	//Czy tekst
	if (!czy_liczba(b)) {
		this->DodajBlad(PoleTekstowe, NazwaPola, BL_TEKST, IdCiala);
		delete[] b;
		return false;
	}

	//Czy puste
	if (strlen(b) == 0) {
		this->DodajBlad(PoleTekstowe, NazwaPola, BL_PUSTE, IdCiala);
		delete[] b;
		return false;
	}

	zamien_przecinki(b);
	double d = atof(b);

	if (d <= 0) {
		this->DodajBlad(PoleTekstowe, NazwaPola, BL_MNIEJSZE0, IdCiala);
		delete[] b;
		return false;
	}

	*Wartosc = d;	
	delete[] b;
	return true;
}

//Sprawdza, czy wartosc jest >= 0, jesli pole jest puste zwraca 0 (P)
bool SprawdzanieBledow::PowyzejZeraZeroP(HWND PoleTekstowe, string NazwaPola, double * Wartosc, int IdCiala) {
	this->UsunBlad(PoleTekstowe);

	char* b = new char[DLUGOSC_TEKSTU];
	GetWindowText(PoleTekstowe, b, DLUGOSC_TEKSTU);

	//Czy tekst
	if (!czy_liczba(b)) {
		this->DodajBlad(PoleTekstowe, NazwaPola, BL_TEKST, IdCiala);
		delete[] b;
		return false;
	}

	zamien_przecinki(b);
	double d = atof(b);

	if (d < 0) {
		this->DodajBlad(PoleTekstowe, NazwaPola, BL_UJEMNE, IdCiala);
		delete[] b;
		return false;
	}

	*Wartosc = d;
	delete[] b;
	return true;
}

//Sprawdza, czy wartosc jest >= 0, pole nie moze byc puste (NP)
bool SprawdzanieBledow::PowyzejZeraZeroNP(HWND PoleTekstowe, string NazwaPola, double * Wartosc, int IdCiala){
	this->UsunBlad(PoleTekstowe);

	char* b = new char[DLUGOSC_TEKSTU];
	GetWindowText(PoleTekstowe, b, DLUGOSC_TEKSTU);

	//Czy tekst
	if (!czy_liczba(b)) {
		this->DodajBlad(PoleTekstowe, NazwaPola, BL_TEKST, IdCiala);
		delete[] b;
		return false;
	}

	//Czy puste
	if (strlen(b) == 0) {
		this->DodajBlad(PoleTekstowe, NazwaPola, BL_PUSTE, IdCiala);
		delete[] b;
		return false;
	}

	zamien_przecinki(b);
	double d = atof(b);

	if (d < 0) {
		this->DodajBlad(PoleTekstowe, NazwaPola, BL_UJEMNE, IdCiala);
		delete[] b;
		return false;
	}

	*Wartosc = d;
	delete[] b;
	return true;
}

//Sprawdza, czy wartosc jest w zakresie <0,1>, jesli pole jest puste zwraca 0 (P)
bool SprawdzanieBledow::UlamekZeroP(HWND PoleTekstowe, string NazwaPola, double* Wartosc, int IdCiala) {
	this->UsunBlad(PoleTekstowe);

	char* b = new char[DLUGOSC_TEKSTU];
	GetWindowText(PoleTekstowe, b, DLUGOSC_TEKSTU);

	//Czy tekst
	if (!czy_liczba(b)) {
		this->DodajBlad(PoleTekstowe, NazwaPola, BL_TEKST, IdCiala);
		delete[] b;
		return false;
	}

	zamien_przecinki(b);
	double d = atof(b);

	if (d < 0.0 || d > 1.0) {
		this->DodajBlad(PoleTekstowe, NazwaPola, BL_ULAMEK, IdCiala);
		delete[] b;
		return false;
	}

	*Wartosc = d;
	delete[] b;
	return true;
}

//Sprawdza, czy podano jakas wartosc, pole nie moze byc puste
bool SprawdzanieBledow::CokolwiekNP(HWND PoleTekstowe, string NazwaPola, double * Wartosc, int IdCiala){
	this->UsunBlad(PoleTekstowe);

	char* b = new char[DLUGOSC_TEKSTU];
	GetWindowText(PoleTekstowe, b, DLUGOSC_TEKSTU);

	//Czy tekst
	if (!czy_liczba(b)) {
		this->DodajBlad(PoleTekstowe, NazwaPola, BL_TEKST, IdCiala);
		delete[] b;
		return false;
	}

	//Czy puste
	if (strlen(b) == 0) {
		this->DodajBlad(PoleTekstowe, NazwaPola, BL_PUSTE, IdCiala);
		delete[] b;
		return false;
	}

	zamien_przecinki(b);
	double d = atof(b);

	*Wartosc = atof(b);
	delete[] b;
	return true;
}

//Wyswietla bledy i zwraca true; jesli nie ma bledow zwraca false
bool SprawdzanieBledow::PokazBledy(){
	if (this->bledy.size() == 0) return false;
	int p;
	bool bledy = false;

	for (int i = 0; i < this->bledy.size(); i++) {

		if (this->bledy[i].Nr_ciala != -1) continue;
		bledy = true;

		switch (this->bledy[i].Nr_bledu) {

		case BL_PUSTE:
			p = this->pokazbl(i, BLT_PUSTE);
			break;
		
		case BL_TEKST:
			p = this->pokazbl(i, BLT_TEKST);
			break;

		case BL_ZERO:
			p = this->pokazbl(i, BLT_ZERO);
			break;

		case BL_MNIEJSZE0:
			p = this->pokazbl(i, BLT_MNIEJSZE0);
			break;

		case BL_UJEMNE:
			p = this->pokazbl(i, BLT_UJEMNE);
			break;

		case BL_ULAMEK:
			p = this->pokazbl(i, BLT_ULAMEK);
			break;

		default:
			p = this->pokazbl(i, BLT_BRAK);

		}

		if (p == IDCANCEL) return true;

	}

	if (this->PokazBledyPoleCent()) return true;
	return bledy;
}

//Wyswietla bledy pola centralnego i zwraca true; jesli nie ma bledow zwraca false
bool SprawdzanieBledow::PokazBledyPoleCent() {
	if (this->bledy.size() == 0) return false;
	int p;
	int id;
	bool bledy = false;

	for (int i = 0; i < this->bledy.size(); i++) {

		id = this->bledy[i].Nr_ciala;
		if (id == -1) continue;
		bledy = true;

		switch (this->bledy[i].Nr_bledu) {

		case BL_PUSTE:
			p = this->pokazbl_polecent(i, BLT_PUSTE, id);
			break;

		case BL_TEKST:
			p = this->pokazbl_polecent(i, BLT_TEKST, id);
			break;

		case BL_ZERO:
			p = this->pokazbl_polecent(i, BLT_ZERO, id);
			break;

		case BL_MNIEJSZE0:
			p = this->pokazbl_polecent(i, BLT_MNIEJSZE0, id);
			break;

		case BL_UJEMNE:
			p = this->pokazbl_polecent(i, BLT_UJEMNE, id);
			break;

		case BL_ULAMEK:
			p = this->pokazbl_polecent(i, BLT_ULAMEK, id);
			break;

		default:
			p = this->pokazbl_polecent(i, BLT_BRAK, id);

		}

		if (p == IDCANCEL) return true;

	}

	return bledy;
}