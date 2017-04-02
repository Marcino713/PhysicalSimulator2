#include "..\inc\dodatki.h"

//Zamienia przecinki na kropki
void zamien_przecinki(char* &tekst) {
	for (int i = 0; i < strlen(tekst); i++) {
		if (tekst[i] == ',') tekst[i] = '.';
	}
}

//Zwraca true, jesli podany tekst moze zostac przekonwertowany na liczbe
bool czy_liczba(char* tekst) {
	size_t znaki_dl = strlen(ZNAKI_LICZBA);
	bool liczba;

	for (int i = 0; i < strlen(tekst); i++) {

		liczba = false;

		for (size_t j = 0; j < znaki_dl; j++) {
			if (tekst[i] == ZNAKI_LICZBA[j]) {
				liczba = true;
				break;
			}
		}

		if (!liczba) return false;

	}

	return true;

}

//Konwertuje double na string
void ftoa(char* &tekst, double liczba){
	ostringstream s;
	s.precision(PRECYZJA);
	s << liczba;

	string str = s.str();
	
	for (int i = 0; i < str.size(); i++) {
		if (str[i] == '.') tekst[i] = ','; else tekst[i] = str[i];
	}
	tekst[str.size()] = 0;
}

//Konwertuje int na string, na poczatku wstawia litere c
void itoid(char *& tekst, int liczba){
	ostringstream s;
	s << "c";
	s << liczba;

	string str = s.str();

	for (int i = 0; i < str.size(); i++) {
		tekst[i] = str[i];
	}
	tekst[str.size()] = 0;
}

//Konwertuje double na string i dopisuje jednostke
void itod(char *& tekst, double liczba, string jednostka){
	ostringstream s;
	s.precision(PRECYZJA);
	s << liczba << " " << jednostka;

	string str = s.str();

	for (int i = 0; i < str.size(); i++) {
		if (str[i] == '.') tekst[i] = ','; else tekst[i] = str[i];
	}
	tekst[str.size()] = 0;
}

//Konwertuje string (z litera c na poczatku) na int
void idtoi(int & liczba, char* tekst){
	string t = tekst + 1;
	size_t dl = t.size();
	liczba = std::stoi(t, &dl, 10);
}