#include "..\inc\anim_dodatki.h"

extern Animacja3D* Animacja;
extern strStanProgramu* StanProgramu;
extern MainWindow* Okno;
extern DaneSymulacji* Dane;

void PrzetworzKlawisz(WPARAM wParam) {

	switch (wParam) {
	case 87:	//W
		Animacja->PrzesunObraz(0, 0, 1);
		StanProgramu->Zapisany = false;
		break;

	case 83:	//S
		Animacja->PrzesunObraz(0, 0, -1);
		StanProgramu->Zapisany = false;
		break;

	case 90:	//Z
		Animacja->PrzesunObraz(-1, 0, 0);
		StanProgramu->Zapisany = false;
		break;

	case 88:	//X
		Animacja->PrzesunObraz(1, 0, 0);
		StanProgramu->Zapisany = false;
		break;

	case 70:	//F
		Animacja->PrzesunObraz(0, -1, 0);
		StanProgramu->Zapisany = false;
		break;

	case 82:	//R
		Animacja->PrzesunObraz(0, 1, 0);
		StanProgramu->Zapisany = false;
		break;

	case 65:	//A
		Animacja->ObrocObrazY(-KAT_OBROTU);
		StanProgramu->Zapisany = false;
		break;

	case 68:	//D
		Animacja->ObrocObrazY(KAT_OBROTU);
		StanProgramu->Zapisany = false;
		break;

	case 84:	//T
		Animacja->ObrocObrazX(KAT_OBROTU);
		StanProgramu->Zapisany = false;
		break;

	case 71:	//G
		Animacja->ObrocObrazX(-KAT_OBROTU);
		StanProgramu->Zapisany = false;
		break;

	case 81:	//Q
		Animacja->ObrocObrazZ(KAT_OBROTU);
		StanProgramu->Zapisany = false;
		break;

	case 69:	//E
		Animacja->ObrocObrazZ(-KAT_OBROTU);
		StanProgramu->Zapisany = false;
		break;

	case VK_F5:	//Pelny ekran
		break;

	case VK_F6: //Resetuj kamere
		Animacja->ResetujKamere();
		StanProgramu->Zapisany = false;
		break;

	case VK_F7:	//Nie wysrodkowuj na ciele
		Animacja->Kamera = AnimKamera::Domyslna;
		RysujCiala();
		StanProgramu->Zapisany = false;
		break;

	case VK_F8:	//Wysrodkuj na ciele
		Animacja->Kamera = AnimKamera::WysrodkujNaCiele;
		RysujCiala();
		StanProgramu->Zapisany = false;
		break;

	case VK_F9:	//Ustaw kamere zgodnie z wektorem predkosci
		Animacja->Kamera = AnimKamera::WKierunkuPredkosci;
		RysujCiala();
		StanProgramu->Zapisany = false;
		break;

	}
}

void RysujCiala() {

	if (StanProgramu->Animacja) {
		Animacja->Rysuj();
		return;
	}

	DaneCiala* CialoD = Animacja->DaneGr.PobierzWskDaneCial();
	Punkt* Cialo = Animacja->DaneGr.PobierzWskCiala();

	for (int i = 0; i < Dane->PoleCent.rozmiar_tablicy(); i++) {

		if (Dane->PoleCent[i] == nullptr) {
			CialoD[i].puste = true;
			continue;
		} else {
			CialoD[i].puste = false;
		}

		char *t = new char[10];
		itoid(t, i + 1);
		CialoD[i].nazwa = t;
		delete[] t;

		CialoD[i].kolor = AnimKolor::Niebieski;
		if (Dane->PoleCent[i]->Wybucha) CialoD[i].kolor = AnimKolor::Czerwony;
		if (Okno->PobierzZaznId() == i) CialoD[i].kolor = AnimKolor::Zielony;

		Cialo[i].x = Dane->PoleCent[i]->X0 / Dane->Ustawienia.Jednostka0;
		Cialo[i].y = Dane->PoleCent[i]->Y0 / Dane->Ustawienia.Jednostka0;
		Cialo[i].z = Dane->PoleCent[i]->Z0 / Dane->Ustawienia.Jednostka0;
		CialoD[i].promien = Dane->PoleCent[i]->R0 / Dane->Ustawienia.Jednostka0;

		if (Animacja->Kamera == AnimKamera::WysrodkujNaCiele && Okno->PobierzZaznId() == i) {
			Animacja->KameraPatrzNa = D3DXVECTOR3(Cialo[i].x, Cialo[i].y, Cialo[i].z);
		}
	}

	Animacja->Rysuj();
}