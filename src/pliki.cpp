#include "..\inc\pliki.h"
#include "..\inc\animacja.h"
#include <corecrt_wstring.h>

using std::exception;

extern MainWindow* Okno;
extern DaneSymulacji* Dane;
extern strStanProgramu* StanProgramu;
extern Animacja3D* Animacja;

//-------------------Klasa CzytajDane----------------------
CzytajDane::CzytajDane(char* Bufor, size_t Dlugosc) {
	this->dane = Bufor;
	this->dlugosc = Dlugosc;
}

double CzytajDane::CzytajDouble() {
	if ((this->dlugosc -= DLUGOSC_DOUBLE) < 0) throw exception(BLAD_POZA_BUFOREM.c_str());

	double d = *(double*)this->dane;
	this->dane += DLUGOSC_DOUBLE;
	return d;
}

void CzytajDane::CzytajString(char * tekst) {
	if ((this->dlugosc -= DLUGOSC_CHAR) < 0) throw exception(BLAD_POZA_BUFOREM.c_str());

	char dl = *this->dane;
	this->dane += DLUGOSC_CHAR;

	if ((this->dlugosc -= dl) < 0) throw exception(BLAD_POZA_BUFOREM.c_str());

	memcpy_s(tekst, dl, this->dane, dl);
	tekst[dl] = 0;
	this->dane += dl;
}

int CzytajDane::CzytajInt() {
	if ((this->dlugosc -= DLUGOSC_INT) < 0) throw exception(BLAD_POZA_BUFOREM.c_str());

	int l = *(int*)this->dane;
	this->dane += DLUGOSC_INT;
	return l;
}

bool CzytajDane::CzytajBoolean() {
	if ((this->dlugosc -= DLUGOSC_BOOLEAN) < 0) throw exception(BLAD_POZA_BUFOREM.c_str());

	bool b = *(bool*)this->dane;
	this->dane += DLUGOSC_BOOLEAN;
	return b;
}

unsigned char CzytajDane::CzytajUChar() {
	if ((this->dlugosc -= DLUGOSC_CHAR) < 0) throw exception(BLAD_POZA_BUFOREM.c_str());

	unsigned char u = *(unsigned char*)this->dane;
	this->dane += DLUGOSC_CHAR;
	return u;
}

//------------------Klasa PiszDane----------------------
PiszDane::PiszDane(char* Bufor, size_t Dlugosc) {
	this->dane = Bufor;
	this->dane_pocz = Bufor;
	this->dlugosc = Dlugosc;
	this->wykorzystano = 0;
	ZeroMemory(Bufor, Dlugosc);
}

void PiszDane::Pisz(bool wartosc) {
	if((this->wykorzystano += DLUGOSC_BOOLEAN) > this->dlugosc) throw exception(BLAD_POZA_BUFOREM.c_str());

	*((bool*)this->dane) = wartosc;
	this->dane += DLUGOSC_BOOLEAN;
}

void PiszDane::Pisz(string wartosc) {
	size_t dl = wartosc.size();

	if ((this->wykorzystano += (dl + DLUGOSC_CHAR)) > this->dlugosc) {
		throw exception(BLAD_POZA_BUFOREM.c_str());
	}

	*this->dane = (char)dl;
	this->dane += DLUGOSC_CHAR;
	memcpy_s(this->dane, dl, wartosc.c_str(), dl);
	this->dane += dl;
}

void PiszDane::Pisz(double wartosc) {
	if ((this->wykorzystano += DLUGOSC_DOUBLE) > this->dlugosc) {
		throw exception(BLAD_POZA_BUFOREM.c_str());
	}

	*((double*)this->dane) = wartosc;
	this->dane += DLUGOSC_DOUBLE;
}

void PiszDane::Pisz(unsigned char wartosc) {
	if ((this->wykorzystano += DLUGOSC_CHAR) > this->dlugosc) {
		throw exception(BLAD_POZA_BUFOREM.c_str());
	}

	*this->dane = wartosc;
	this->dane += DLUGOSC_CHAR;	
}

void PiszDane::Pisz(int wartosc) {
	if ((this->wykorzystano += DLUGOSC_INT) > this->dlugosc) {
		throw exception(BLAD_POZA_BUFOREM.c_str());
	}

	*((int*)this->dane) = wartosc;
	this->dane += DLUGOSC_INT;
}

//-----------Otwieranie/zapisywanie plikow---------------

//Zwraca True, jesli podano argumenty wiersza polecen
bool CzyOtworzPlikiCmd(PWSTR Argumenty) {
	size_t dl = wcslen(Argumenty);
	if (dl == 0) return false;

	char* arg = new char[dl + 1];
	int i;

	if (Argumenty[0] == '"') {
		i = 1;

		while (Argumenty[i] != L'"') {
			arg[i-1] = wctob(Argumenty[i]);
			i++;
		}

		arg[i-1] = 0;

	} else {
		i = 0;

		while (Argumenty[i] != L' ' && Argumenty[i] != L'\0') {
			arg[i] = wctob(Argumenty[i]);
			i++;
		}

		arg[i] = 0;

	}

	StanProgramu->NazwaPliku = arg;
	delete[] arg;

	return true;
}

//Wyswietla okno pobierania nazwy pliku; zwraca true, jesli uzytkownik wybral plik
bool OtwPlikuPobierzNazwe(HWND wndOkno) {
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = wndOkno;
	ofn.lpstrFilter = NAZWA_TYPU_PLIKU;
	char* nazwa_pliku = new char[DLUGOSC_NAZWY];
	ZeroMemory(nazwa_pliku, DLUGOSC_NAZWY);

	ofn.nMaxFile = DLUGOSC_NAZWY;
	ofn.lpstrFile = nazwa_pliku;
	ofn.lpstrDefExt = ROZSZERZENIE.c_str();
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	if (GetOpenFileName(&ofn) == 0 || nazwa_pliku[0] == 0) {

		delete[] nazwa_pliku;
		return false;

	} else {

		StanProgramu->NazwaPliku = nazwa_pliku;
		delete[] nazwa_pliku;
		return true;

	}
}

//Otwiera plik, zwraca true w przypadku powodzenia
bool OtworzPlik() {
	char* dane;
	size_t dl;

	ifstream plik;
	plik.open(StanProgramu->NazwaPliku, ios::binary | ios::in);

	if (!plik.is_open()) {
		MessageBox(Okno->wndMain, ("B³¹d podczas otwierania pliku - plik prawdopodobnie nie istnieje.\n" + StanProgramu->NazwaPliku).c_str(), "B³¹d przy otwieraniu pliku", MB_OK);
		return false;
	}

	plik.seekg(0, ios::end);
	dl = plik.tellg();
	plik.seekg(0, ios::beg);
	dane = new char[dl];
	plik.read(dane, dl);
	plik.close();

	CzyscDane();

	bool otwarty = false;

	//Wersja pliku 1
	if (strcmp(dane + 1, NAGLOWEK_PLIK1.c_str()) == 0) {
		try {
			otwarty = OtworzPlik1(dane, dl);
		}
		catch (exception e) {
			otwarty = false;
		}
	}

	//Wersja pliku 2
	if (strcmp(dane + 1, NAGLOWEK_PLIK2.c_str()) == 0) {
		try {
			otwarty = OtworzPlik2(dane, dl);
		} catch (exception e) {
			otwarty = false;
		}
	}

	delete[] dane;
	Okno->UstawKontrolki();
	StanProgramu->Zapisany = true;
	Okno->ZmienUstawieniaGrafiki();
	RysujCiala();

	if (!otwarty) {
		MessageBox(Okno->wndMain, ("B³¹d podczas otwierania pliku - plik ma nieprawid³owy format.\n" + StanProgramu->NazwaPliku).c_str(), "B³¹d przy otwieraniu pliku", MB_OK);
		return false;
	}

	return otwarty;
}

//Otwiera plik zapisany w formacie Pole 1
bool OtworzPlik1(char* dane, size_t dlugosc) {
	int ix = 0;
	CzytajDane cz(dane, dlugosc);
	char* t = new char[DLUGOSC_TEKSTU];
	strCialo* cialo;
	int liczba;
	string tekst;
	double promien;

	cz.CzytajString(t);

	if (strcmp(t, NAGLOWEK_PLIK1.c_str()) != 0) {
		delete[] t;
		return false;
	}

	//Ustawienia
	Dane->Ustawienia.Jednostka0 = cz.CzytajDouble()*100;
	cz.CzytajString(t);
	Dane->Ustawienia.JednostkaJedn = ZnajdzJednostke(t[1] == 'µ' ? "µ" : t);

	Dane->Ustawienia.OdbiciaSciany = false;

	//Cialo probne
	cialo = new strCialo();

	cialo->X0 = cz.CzytajDouble();
	cialo->Y0 = cz.CzytajDouble();
	cialo->Masa0 = cz.CzytajDouble();
	cz.CzytajString(t);
	cialo->MasaJedn = ZnajdzJednostke(t[1] == 'µ' ? "µ" : t);
	cialo->Ladunek0 = cz.CzytajDouble();
	cz.CzytajString(t);
	cialo->LadunekJedn = ZnajdzJednostke(t[1] == 'µ'? "µ" : t);

	cialo->V0Xt = cz.CzytajDouble();
	cialo->V0Yt = cz.CzytajDouble();
	cialo->V0t = sqrt(pow(cialo->V0Xt, 2.0) + pow(cialo->V0Yt, 2.0));
	liczba = cz.CzytajInt();

	switch (liczba) {
	case 0:		//Proton
		cialo->Masa0 = PROTON_MASA_1;
		cialo->MasaJedn = ZnajdzJednostke("k");
		cialo->Ladunek0 = PROTON_LADUNEK_1;
		cialo->LadunekJedn = ZnajdzJednostke("");
		break;

	case 1:		//Elektron
		cialo->Masa0 = ELEKTRON_MASA_1;
		cialo->MasaJedn = ZnajdzJednostke("k");
		cialo->Ladunek0 = ELEKTRON_LADUNEK_1;
		cialo->LadunekJedn = ZnajdzJednostke("");
		break;

	}

	promien = Dane->Ustawienia.Jednostka0 * PROMIEN_1;
	cialo->R0 = promien;

	Okno->OstPromien = promien;
	Okno->DodajCialo(cialo);

	//Pole centralne
	liczba = cz.CzytajInt();

	for (int i = 0; i < liczba; i++) {
		cialo = new strCialo();

		cialo->X0 = cz.CzytajDouble();
		cialo->Y0 = cz.CzytajDouble();
		cialo->Masa0 = cz.CzytajDouble();
		cz.CzytajString(t);
		cialo->MasaJedn = ZnajdzJednostke(t[1] == 'µ' ? "µ" : t);
		cialo->Ladunek0 = cz.CzytajDouble();
		cz.CzytajString(t);
		cialo->LadunekJedn = ZnajdzJednostke(t[1] == 'µ' ? "µ" : t);

		cialo->R0 = promien;
		cialo->Nieruchome = true;

		Okno->DodajCialo(cialo);
	}

	//Pole jednorodne
	Dane->PoleJedn.Gora = cz.CzytajBoolean();
	Dane->PoleJedn.GoraIle = cz.CzytajDouble();
	Dane->PoleJedn.Dol = cz.CzytajBoolean();
	Dane->PoleJedn.DolIle = cz.CzytajDouble();
	Dane->PoleJedn.Lewo = cz.CzytajBoolean();
	Dane->PoleJedn.LewoIle = cz.CzytajDouble();
	Dane->PoleJedn.Prawo = cz.CzytajBoolean();
	Dane->PoleJedn.PrawoIle = cz.CzytajDouble();
	Dane->PoleJedn.GoraDol = cz.CzytajBoolean();
	Dane->PoleJedn.GoraPlus = cz.CzytajBoolean();
	Dane->PoleJedn.GoraDolIle = cz.CzytajDouble();
	Dane->PoleJedn.LewoPrawo = cz.CzytajBoolean();
	Dane->PoleJedn.LewoPlus = cz.CzytajBoolean();
	Dane->PoleJedn.LewoPrawoIle = cz.CzytajDouble();

	delete[] t;

	return true;
}

//Otwiera plik zapisany w formacie Pole 2
bool OtworzPlik2(char* dane, size_t dlugosc) {
	int ix = 0;
	CzytajDane cz(dane, dlugosc);
	char* t = new char[DLUGOSC_TEKSTU];
	strCialo* cialo;
	int liczba;
	unsigned char l2;
	string tekst;
	int ZaznId;
	D3DXVECTOR3 dvObrot, dvPatrzNa, dvPatrzZ;

	cz.CzytajString(t);

	if (strcmp(t, NAGLOWEK_PLIK2.c_str()) != 0) {
		delete[] t;
		return false;
	}

	//Ustawienia
	Dane->Ustawienia.Jednostka0 = cz.CzytajDouble();
	cz.CzytajString(t);
	Dane->Ustawienia.JednostkaJedn = ZnajdzJednostke(t);
	Dane->Ustawienia.JednIlosc = cz.CzytajDouble();
	Dane->Ustawienia.UstJednPodzialka = cz.CzytajDouble();

	l2 = cz.CzytajUChar();
	Dane->Ustawienia.OdbiciaSciany = ((l2 & PLIK2_UST_ODBICIASCIANY) == PLIK2_UST_ODBICIASCIANY);

	ZaznId = cz.CzytajInt();

	//Kamera
	dvObrot.x = cz.CzytajDouble();
	dvObrot.y = cz.CzytajDouble();
	dvObrot.z = cz.CzytajDouble();

	dvPatrzNa.x = cz.CzytajDouble();
	dvPatrzNa.y = cz.CzytajDouble();
	dvPatrzNa.z = cz.CzytajDouble();

	dvPatrzZ.x = cz.CzytajDouble();
	dvPatrzZ.y = cz.CzytajDouble();
	dvPatrzZ.z = cz.CzytajDouble();

	Animacja->UstawKamere(dvObrot, dvPatrzNa, dvPatrzZ);
	Animacja->Kamera = (AnimKamera)cz.CzytajUChar();

	//Pole centralne
	liczba = cz.CzytajInt();

	for (int i = 0; i < liczba; i++) {

		l2 = cz.CzytajUChar();

		if ((l2 & PLIK2_POLECENT_BRAK) == PLIK2_POLECENT_BRAK) {
			Okno->DodajCialo(NULL);
			continue;
		}

		cialo = new strCialo();

		cialo->Nieruchome = ((l2 & PLIK2_POLECENT_NIERUCHOME) == PLIK2_POLECENT_NIERUCHOME);
		cialo->Wybucha = ((l2 & PLIK2_POLECENT_WYBUCHA) == PLIK2_POLECENT_WYBUCHA);

		//Polozenie, promien
		cialo->X0 = cz.CzytajDouble();
		cialo->Y0 = cz.CzytajDouble();
		cialo->Z0 = cz.CzytajDouble();
		cialo->R0 = cz.CzytajDouble();

		//Predkosc
		cialo->V0Xt = cz.CzytajDouble();
		cialo->V0Yt = cz.CzytajDouble();
		cialo->V0Zt = cz.CzytajDouble();
		cialo->V0t = sqrt(pow(cialo->V0Xt, 2.0) + pow(cialo->V0Yt, 2.0) + pow(cialo->V0Zt, 2.0));

		cialo->WspRestytucji = cz.CzytajDouble();

		//Wybuch
		cialo->Sila0 = cz.CzytajDouble();
		cz.CzytajString(t);
		cialo->SilaJedn = ZnajdzJednostke(t);
		cialo->Promien0 = cz.CzytajDouble();

		//Masa, ladunek
		cialo->Masa0 = cz.CzytajDouble();
		cz.CzytajString(t);
		cialo->MasaJedn = ZnajdzJednostke(t);
		cialo->Ladunek0 = cz.CzytajDouble();
		cz.CzytajString(t);
		cialo->LadunekJedn = ZnajdzJednostke(t);

		Okno->DodajCialo(cialo);
	}

	//Pole jednorodne
	l2 = cz.CzytajUChar();
	Dane->PoleJedn.Gora  = ((l2 & PLIK2_POLEJEDN_GRAWGORA)  == PLIK2_POLEJEDN_GRAWGORA);
	Dane->PoleJedn.Dol   = ((l2 & PLIK2_POLEJEDN_GRAWDOL)   == PLIK2_POLEJEDN_GRAWDOL);
	Dane->PoleJedn.Lewo  = ((l2 & PLIK2_POLEJEDN_GRAWLEWO)  == PLIK2_POLEJEDN_GRAWLEWO);
	Dane->PoleJedn.Prawo = ((l2 & PLIK2_POLEJEDN_GRAWPRAWO) == PLIK2_POLEJEDN_GRAWPRAWO);
	Dane->PoleJedn.Przod = ((l2 & PLIK2_POLEJEDN_GRAWPRZOD) == PLIK2_POLEJEDN_GRAWPRZOD);
	Dane->PoleJedn.Tyl   = ((l2 & PLIK2_POLEJEDN_GRAWTYL)   == PLIK2_POLEJEDN_GRAWTYL);

	Dane->PoleJedn.GoraIle  = cz.CzytajDouble();
	Dane->PoleJedn.DolIle   = cz.CzytajDouble();
	Dane->PoleJedn.LewoIle  = cz.CzytajDouble();
	Dane->PoleJedn.PrawoIle = cz.CzytajDouble();
	Dane->PoleJedn.PrzodIle = cz.CzytajDouble();
	Dane->PoleJedn.TylIle   = cz.CzytajDouble();

	l2 = cz.CzytajUChar();
	Dane->PoleJedn.GoraDol   = ((l2 & PLIK2_POLEJEDN_ELGORADOL)   == PLIK2_POLEJEDN_ELGORADOL);
	Dane->PoleJedn.GoraPlus  = ((l2 & PLIK2_POLEJEDN_ELGORAPLUS)  == PLIK2_POLEJEDN_ELGORAPLUS);
	Dane->PoleJedn.LewoPrawo = ((l2 & PLIK2_POLEJEDN_ELLEWOPRAWO) == PLIK2_POLEJEDN_ELLEWOPRAWO);
	Dane->PoleJedn.LewoPlus  = ((l2 & PLIK2_POLEJEDN_ELLEWOPLUS)  == PLIK2_POLEJEDN_ELLEWOPLUS);
	Dane->PoleJedn.PrzodTyl  = ((l2 & PLIK2_POLEJEDN_ELPRZODTYL)  == PLIK2_POLEJEDN_ELPRZODTYL);
	Dane->PoleJedn.PrzodPlus = ((l2 & PLIK2_POLEJEDN_ELPRZODPLUS) == PLIK2_POLEJEDN_ELPRZODPLUS);

	Dane->PoleJedn.GoraDolIle = cz.CzytajDouble();
	Dane->PoleJedn.LewoPrawoIle = cz.CzytajDouble();
	Dane->PoleJedn.PrzodTylIle = cz.CzytajDouble();

	//Tarcie
	Dane->Tarcie.StGora   = cz.CzytajDouble();
	Dane->Tarcie.KinGora  = cz.CzytajDouble();
	Dane->Tarcie.StDol    = cz.CzytajDouble();
	Dane->Tarcie.KinDol   = cz.CzytajDouble();
	Dane->Tarcie.StLewo   = cz.CzytajDouble();
	Dane->Tarcie.KinLewo  = cz.CzytajDouble();
	Dane->Tarcie.StPrawo  = cz.CzytajDouble();
	Dane->Tarcie.KinPrawo = cz.CzytajDouble();
	Dane->Tarcie.StPrzod  = cz.CzytajDouble();
	Dane->Tarcie.KinPrzod = cz.CzytajDouble();
	Dane->Tarcie.StTyl    = cz.CzytajDouble();
	Dane->Tarcie.KinTyl   = cz.CzytajDouble();

	delete[] t;

	Okno->ZaznaczCialo(ZaznId);

	return true;
}

bool ZapiszPlik(HWND wndOkno) {

	if (StanProgramu->NazwaPliku == "") {
		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = wndOkno;
		ofn.lpstrFilter = NAZWA_TYPU_PLIKU;
		char* nazwa_pliku = new char[DLUGOSC_NAZWY];
		ZeroMemory(nazwa_pliku, DLUGOSC_NAZWY);

		ofn.nMaxFile = DLUGOSC_NAZWY;
		ofn.lpstrFile = nazwa_pliku;
		ofn.lpstrDefExt = ROZSZERZENIE.c_str();
		ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

		if (GetSaveFileName(&ofn) == 0){
			delete[] nazwa_pliku;
			return false;
		}

		StanProgramu->NazwaPliku = nazwa_pliku;
		delete[] nazwa_pliku;
	}

	size_t dl = ObliczDlugoscPlik2();
	char* b = new char[dl];
	PiszDane buf(b, dl);

	try {
		ZapiszPlik2(buf);
	} catch (exception e) {

		delete[] b;
		MessageBox(wndOkno, "B³¹d podczas zapisywania pliku.", "B³¹d", MB_OK);
		return false;

	}

	ofstream plik;
	plik.open(StanProgramu->NazwaPliku, ios::binary | ios::out);

	if (!plik.is_open()) {
		delete[] b;
		MessageBox(wndOkno, "B³¹d podczas zapisywania pliku.", "B³¹d", MB_OK);
		return false;
	}

	plik.write(buf.PobierzDane(), buf.Dlugosc());
	plik.close();

	delete[] b;
	StanProgramu->Zapisany = true;

	return true;
}

void ZapiszPlik2(PiszDane& bufor) {
	unsigned char liczba;
	int dl;
	strCialo* c;
	string str = NAGLOWEK_PLIK2;

	//Naglowek
	bufor.Pisz(str);

	//Ustawienia
	bufor.Pisz(Dane->Ustawienia.Jednostka0);
	bufor.Pisz(JEDNOSTKI[Dane->Ustawienia.JednostkaJedn].Przedrostek);
	bufor.Pisz(Dane->Ustawienia.JednIlosc);
	bufor.Pisz(Dane->Ustawienia.UstJednPodzialka);

	liczba = 0;
	if(Dane->Ustawienia.OdbiciaSciany) liczba |= PLIK2_UST_ODBICIASCIANY;
	bufor.Pisz(liczba);

	bufor.Pisz(Okno->PobierzZaznId());

	//Kamera
	D3DXVECTOR3 dv = Animacja->PobierzKatyObrotu();
	bufor.Pisz((double)dv.x);
	bufor.Pisz((double)dv.y);
	bufor.Pisz((double)dv.z);

	dv = Animacja->PobierzKamerePatrzNa();
	bufor.Pisz((double)dv.x);
	bufor.Pisz((double)dv.y);
	bufor.Pisz((double)dv.z);

	dv = Animacja->PobierzKamerePatrzZ();
	bufor.Pisz((double)dv.x);
	bufor.Pisz((double)dv.y);
	bufor.Pisz((double)dv.z);

	bufor.Pisz((unsigned char)Animacja->Kamera);
		
	//Pole centralne
	dl = Dane->PoleCent.rozmiar_tablicy();
	bufor.Pisz(dl);

	for (int i = 0; i < dl; i++) {

		if (Dane->PoleCent[i] == NULL) {
			bufor.Pisz(PLIK2_POLECENT_BRAK);
			continue;
		}

		c = Dane->PoleCent[i];
		liczba = PLIK2_POLECENT_JEST;
		if (c->Nieruchome) liczba |= PLIK2_POLECENT_NIERUCHOME;
		if (c->Wybucha) liczba |= PLIK2_POLECENT_WYBUCHA;
		bufor.Pisz(liczba);

		bufor.Pisz(c->X0);
		bufor.Pisz(c->Y0);
		bufor.Pisz(c->Z0);
		bufor.Pisz(c->R0);

		bufor.Pisz(c->V0Xt);
		bufor.Pisz(c->V0Yt);
		bufor.Pisz(c->V0Zt);

		bufor.Pisz(c->WspRestytucji);

		bufor.Pisz(c->Sila0);
		bufor.Pisz(JEDNOSTKI[c->SilaJedn].Przedrostek);
		bufor.Pisz(c->Promien0);

		bufor.Pisz(c->Masa0);
		bufor.Pisz(JEDNOSTKI[c->MasaJedn].Przedrostek);
		bufor.Pisz(c->Ladunek0);
		bufor.Pisz(JEDNOSTKI[c->LadunekJedn].Przedrostek);
	}


	//Pole jednorodne
	liczba = 0;
	if (Dane->PoleJedn.Gora)  liczba |= PLIK2_POLEJEDN_GRAWGORA;
	if (Dane->PoleJedn.Dol)   liczba |= PLIK2_POLEJEDN_GRAWDOL;
	if (Dane->PoleJedn.Lewo)  liczba |= PLIK2_POLEJEDN_GRAWLEWO;
	if (Dane->PoleJedn.Prawo) liczba |= PLIK2_POLEJEDN_GRAWPRAWO;
	if (Dane->PoleJedn.Przod) liczba |= PLIK2_POLEJEDN_GRAWPRZOD;
	if (Dane->PoleJedn.Tyl)   liczba |= PLIK2_POLEJEDN_GRAWTYL;
	bufor.Pisz(liczba);

	bufor.Pisz(Dane->PoleJedn.GoraIle);
	bufor.Pisz(Dane->PoleJedn.DolIle);
	bufor.Pisz(Dane->PoleJedn.LewoIle);
	bufor.Pisz(Dane->PoleJedn.PrawoIle);
	bufor.Pisz(Dane->PoleJedn.PrzodIle);
	bufor.Pisz(Dane->PoleJedn.TylIle);

	liczba = 0;
	if (Dane->PoleJedn.GoraDol)   liczba |= PLIK2_POLEJEDN_ELGORADOL;
	if (Dane->PoleJedn.GoraPlus)  liczba |= PLIK2_POLEJEDN_ELGORAPLUS;
	if (Dane->PoleJedn.LewoPrawo) liczba |= PLIK2_POLEJEDN_ELLEWOPRAWO;
	if (Dane->PoleJedn.LewoPlus)  liczba |= PLIK2_POLEJEDN_ELLEWOPLUS;
	if (Dane->PoleJedn.PrzodTyl)  liczba |= PLIK2_POLEJEDN_ELPRZODTYL;
	if (Dane->PoleJedn.PrzodPlus) liczba |= PLIK2_POLEJEDN_ELPRZODPLUS;
	bufor.Pisz(liczba);

	bufor.Pisz(Dane->PoleJedn.GoraDolIle);
	bufor.Pisz(Dane->PoleJedn.LewoPrawoIle);
	bufor.Pisz(Dane->PoleJedn.PrzodTylIle);


	//Tarcie
	bufor.Pisz(Dane->Tarcie.StGora);
	bufor.Pisz(Dane->Tarcie.KinGora);
	bufor.Pisz(Dane->Tarcie.StDol);
	bufor.Pisz(Dane->Tarcie.KinDol);
	bufor.Pisz(Dane->Tarcie.StLewo);
	bufor.Pisz(Dane->Tarcie.KinLewo);
	bufor.Pisz(Dane->Tarcie.StPrawo);
	bufor.Pisz(Dane->Tarcie.KinPrawo);
	bufor.Pisz(Dane->Tarcie.StPrzod);
	bufor.Pisz(Dane->Tarcie.KinPrzod);
	bufor.Pisz(Dane->Tarcie.StTyl);
	bufor.Pisz(Dane->Tarcie.KinTyl);
}

size_t ObliczDlugoscPlik2() {
	return 0xFFFF;
}