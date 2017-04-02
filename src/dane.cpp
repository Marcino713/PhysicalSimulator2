#include "..\inc\dane.h"
#include "..\inc\mainwnd.h"

extern DaneSymulacji* Dane;
extern MainWindow* Okno;

void CzyscDane(){

	//Ustawienia
	Dane->Ustawienia.Jednostka0 = 1.0;
	Dane->Ustawienia.JednostkaJedn = ZnajdzJednostke("");
	Dane->Ustawienia.JednIlosc = 10.0;
	Dane->Ustawienia.UstJednPodzialka = 5.0;
	Dane->Ustawienia.OdbiciaSciany = true;

	//Pole centralne
	Dane->PoleCent.czysc();
	Okno->ZaznaczCialo(-1);
	Okno->OstPromien = 1;

	//Pole grawitacyjne
	Dane->PoleJedn.Gora = false;
	Dane->PoleJedn.GoraIle = 0.0;
	Dane->PoleJedn.Dol = false;
	Dane->PoleJedn.DolIle = 0.0;
	Dane->PoleJedn.Lewo = false;
	Dane->PoleJedn.LewoIle = 0.0;
	Dane->PoleJedn.Prawo = false;
	Dane->PoleJedn.PrawoIle = 0.0;
	Dane->PoleJedn.Przod = false;
	Dane->PoleJedn.PrzodIle = 0.0;
	Dane->PoleJedn.Tyl = false;
	Dane->PoleJedn.TylIle = 0.0;

	//Pole elektrostatyczne
	Dane->PoleJedn.GoraDol = false;
	Dane->PoleJedn.GoraPlus = true;
	Dane->PoleJedn.GoraDolIle = 0.0;
	Dane->PoleJedn.LewoPrawo = false;
	Dane->PoleJedn.LewoPlus = true;
	Dane->PoleJedn.LewoPrawoIle = 0.0;
	Dane->PoleJedn.PrzodTyl = false;
	Dane->PoleJedn.PrzodPlus = true;
	Dane->PoleJedn.PrzodTylIle = 0.0;

	//Tarcie statyczne
	Dane->Tarcie.StGora = 0.0;
	Dane->Tarcie.StDol = 0.0;
	Dane->Tarcie.StLewo = 0.0;
	Dane->Tarcie.StPrawo = 0.0;
	Dane->Tarcie.StPrzod = 0.0;
	Dane->Tarcie.StTyl = 0.0;

	//Tarcie kinetyczne
	Dane->Tarcie.KinGora = 0.0;
	Dane->Tarcie.KinDol = 0.0;
	Dane->Tarcie.KinLewo = 0.0;
	Dane->Tarcie.KinPrawo = 0.0;
	Dane->Tarcie.KinPrzod = 0.0;
	Dane->Tarcie.KinTyl = 0.0;
}

strCialo::strCialo(double x, double y, double z, double r){
	//Polozenie, promien
	this->X0 = x;
	this->Y0 = y;
	this->Z0 = z;
	this->R0 = r;

	//Predkosc
	this->Nieruchome = false;
	this->V0t = 0.0;
	this->V0Xt = 0.0;
	this->V0Yt = 0.0;
	this->V0Zt = 0.0;

	this->WspRestytucji = 0.0;

	//Wybuch
	this->Wybucha = false;
	this->SilaJedn = ZnajdzJednostke("");
	this->Sila0 = 0.0;
	this->Promien0 = 0.0;

	//Masa, ladunek
	this->Masa0 = 1.0;
	this->MasaJedn = ZnajdzJednostke("k");
	this->Ladunek0 = 0.0;
	this->LadunekJedn = ZnajdzJednostke("");
}

int ZnajdzJednostke(string Przedrostek) {
	for (int i = 0; i < ILOSC_JEDNOSTEK; i++) {
		if (JEDNOSTKI[i].Przedrostek == Przedrostek) return i;
	}

	return JEDNOSTKA_DOMYSLNA;
}