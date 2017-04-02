#include "..\inc\fizyka.h"

extern strStanProgramu* StanProgramu;
extern Fizyka* SilnikFizyczny;
extern Animacja3D* Animacja;
extern MainWindow* Okno;
extern DaneSymulacji* Dane;

#pragma warning( disable : 4244 )	//konwersja double->float

void Fizyka::Inicjuj(){
	this->evtAnimacja = CreateEvent(NULL, true, true, NULL);
	this->evtPauza = CreateEvent(NULL, true, true, NULL);

	this->ObliczFizyke_p = ObliczFizyke1;
	this->PrzygotujDane_p = PrzygotujDane1;
	this->CzyscDane_p = CzyscDane1;
	this->Wybuch_p = Wybuch1;
	this->PobierzPredkosc_p = PobierzPredkosc1;

	this->DaneFiz = 0;
	this->GrDaneCial = 0;
}

void Fizyka::ObliczFizyke(){
	this->t2 = clock();

	while(this->t2 - this->t1 == 0) {
		Sleep(1);
		this->t2 = clock();
	}

	double DeltaT = (this->t2 - this->t1) / 1000.0;
	this->t1 = this->t2;
	this->ObliczFizyke_p(this->DaneFiz, DeltaT, Animacja->DaneGr.PobierzWskCiala());
}

void Fizyka::PrzygotujDane(){
	this->DaneFiz = this->PrzygotujDane_p();
	this->t1 = clock();
}

void Fizyka::CzyscDane(){
	this->CzyscDane_p(this->DaneFiz);
}

void Fizyka::WyswietlCiala(){

	switch (Animacja->Kamera) {
	case AnimKamera::WysrodkujNaCiele: {

		if (Okno->PobierzZaznId() == -1) break;		
		if (Animacja->DaneGr.PobierzWskDaneCial()[Okno->PobierzZaznId()].puste) break;

		Punkt* c = &Animacja->DaneGr.PobierzWskCiala()[Okno->PobierzZaznId()];
		Animacja->KameraPatrzNa = D3DXVECTOR3(c->x, c->y, c->z);

		break;
	}

	case  AnimKamera::WKierunkuPredkosci: {

		if (Okno->PobierzZaznId() == -1) break;
		if (Animacja->DaneGr.PobierzWskDaneCial()[Okno->PobierzZaznId()].puste) break;

		Punkt* c = &Animacja->DaneGr.PobierzWskCiala()[Okno->PobierzZaznId()];
		Punkt v;
		this->PobierzPredkosc_p(this->DaneFiz, Okno->PobierzZaznId(), &v);
		Punkt dv;

		if (v.x == 0 && v.y == 0 && v.z == 0) {

			Animacja->KameraPatrzNa = D3DXVECTOR3(0, 0, 0);
		
		} else {

			double dl = 5 / sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
			dv.x = c->x + v.x * dl;
			dv.y = c->y + v.y * dl;
			dv.z = c->z + v.z * dl;

			Animacja->KameraPatrzNa = D3DXVECTOR3(dv.x, dv.y, dv.z);
		}

		Animacja->KameraPatrzZ  = D3DXVECTOR3(c->x, c->y, c->z);

		break;

	}
	}

	Animacja->Rysuj();
}

void Fizyka::Wybuch(int IdCiala){
	this->Wybuch_p(this->DaneFiz, IdCiala);
	Animacja->DaneGr.PobierzWskDaneCial()[IdCiala].puste = true;
}

void Fizyka::ZaznaczCialo(int id) {
	if (!StanProgramu->Animacja) return;

	for (int i = 0; i < Dane->PoleCent.rozmiar_tablicy(); i++) {
		if (Dane->PoleCent[i] == NULL) continue;

		if (i == id) {
			Animacja->DaneGr.PobierzWskDaneCial()[i].kolor = AnimKolor::Zielony;
		} else {
			Animacja->DaneGr.PobierzWskDaneCial()[i].kolor = (Dane->PoleCent[i]->Wybucha) ? AnimKolor::Czerwony : AnimKolor::Niebieski;
		}
	}
}

void UruchomSymulacje(void* args) {

	ResetEvent(SilnikFizyczny->evtAnimacja);

	SilnikFizyczny->PrzygotujDane();
	SilnikFizyczny->ZaznaczCialo(Okno->PobierzZaznId());

	while (StanProgramu->Animacja) {
		SilnikFizyczny->ObliczFizyke();
		SilnikFizyczny->WyswietlCiala();

		if (StanProgramu->Pauza) {
			WaitForSingleObject(SilnikFizyczny->evtPauza, INFINITE);
			SilnikFizyczny->ZerujCzas();
		}

		if (!StanProgramu->Animacja) break;

	}

	SilnikFizyczny->CzyscDane();
	SetEvent(SilnikFizyczny->evtAnimacja);
	RysujCiala();

	_endthread();
}