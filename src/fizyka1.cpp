#include "..\inc\fizyka1.h"

extern DaneSymulacji* Dane;

void ObliczFizyke1(long long Dane, double DeltaT, Punkt* Ciala) {
	DaneFizyka1* d = (DaneFizyka1*)Dane;
	CialoDaneFizyka1* Cd;
	CialoParametryFizyka1* Cp;

	//Czysc sile
	for (int i = 0; i < d->Ilosc; i++) {
		if (d->Atrybuty[i].Puste) continue;

		Cd = &d->Dane[i];

		Cd->Fx = 0.0;
		Cd->Fy = 0.0;
		Cd->Fz = 0.0;
	}

	//Wybuch
	double r, proc;
	CialoParametryFizyka1* Cpi;
	CialoDaneFizyka1* Cdj;
	CialoParametryFizyka1* Cpj;

	for (int i = 0; i < d->Ilosc; i++) {
		if (d->Atrybuty[i].Puste) continue;

		if (d->Atrybuty[i].Wybucha) {
			d->Atrybuty[i].Wybucha = false;
			d->Atrybuty[i].Puste = true;

			Cpi = &d->Ciala[i];

			for (int j = 0; j < d->Ilosc; j++) {
				if (d->Atrybuty[j].Puste || i == j) continue;

				Cdj = &d->Dane[j];
				Cpj = &d->Ciala[j];

				r = sqrt(pow(Cpi->Xm - Cpj->Xm, 2) + pow(Cpi->Ym - Cpj->Ym, 2) + pow(Cpi->Zm - Cpj->Zm, 2));
				if (r < Cpi->Promien) {

					//Ile procent sily wybuchu zadziala na cialo
					proc = 1 - r / Cpi->Promien;

					Cdj->Fx = -proc * Cpi->Sila * (Cpi->Xm - Cpj->Xm) / r;
					Cdj->Fy = -proc * Cpi->Sila * (Cpi->Ym - Cpj->Ym) / r;
					Cdj->Fz = -proc * Cpi->Sila * (Cpi->Zm - Cpj->Zm) / r;

				}

			}

		}

	}

	//Przetworzenie cial
	for (int i = 0; i < d->Ilosc; i++) {
		if (d->Atrybuty[i].Puste) continue;

		Cd = &d->Dane[i];
		Cp = &d->Ciala[i];

		//Nieruchome
		if (d->Atrybuty[i].Nieruchome) {
			//Polozenie
			Ciala[i].x = Cp->Xm / d->Jednostka;
			Ciala[i].y = Cp->Ym / d->Jednostka;
			Ciala[i].z = Cp->Zm / d->Jednostka;

			continue;
		}

		//Kontakt z krawedziami
		if (Cp->Ym >= (d->Rozmiar - Cp->Rm)) Cd->Gora = true; else Cd->Gora = false;
		if (Cp->Ym <= Cp->Rm) Cd->Dol = true; else Cd->Dol = false;
		if (Cp->Xm <= Cp->Rm) Cd->Lewo = true; else Cd->Lewo = false;
		if (Cp->Xm >= (d->Rozmiar - Cp->Rm)) Cd->Prawo = true; else Cd->Prawo = false;
		if (Cp->Zm <= Cp->Rm) Cd->Przod = true; else Cd->Przod = false;
		if (Cp->Zm >= (d->Rozmiar - Cp->Rm)) Cd->Tyl = true; else Cd->Tyl = false;

		//Pole jednorodne grawitacyjne
		if (d->PoleJedn.Gora)  Cd->Fy += d->PoleJedn.GoraIle  * Cp->Masa;
		if (d->PoleJedn.Dol)   Cd->Fy -= d->PoleJedn.DolIle   * Cp->Masa;
		if (d->PoleJedn.Lewo)  Cd->Fx -= d->PoleJedn.LewoIle  * Cp->Masa;
		if (d->PoleJedn.Prawo) Cd->Fx += d->PoleJedn.PrawoIle * Cp->Masa;
		if (d->PoleJedn.Przod) Cd->Fz -= d->PoleJedn.PrzodIle * Cp->Masa;
		if (d->PoleJedn.Tyl)   Cd->Fz += d->PoleJedn.TylIle   * Cp->Masa;

		//Pole jednorodne elektrostatyczne
		if (d->PoleJedn.GoraDol) {
			if (d->PoleJedn.GoraPlus) {
				Cd->Fy -= d->PoleJedn.GoraDolIle * Cp->Ladunek;
			} else {
				Cd->Fy += d->PoleJedn.GoraDolIle * Cp->Ladunek;
			}
		}

		if (d->PoleJedn.LewoPrawo) {
			if (d->PoleJedn.LewoPlus) {
				Cd->Fx += d->PoleJedn.LewoPrawoIle * Cp->Ladunek;
			}
			else {
				Cd->Fx -= d->PoleJedn.LewoPrawoIle * Cp->Ladunek;
			}
		}

		if (d->PoleJedn.PrzodTyl) {
			if (d->PoleJedn.PrzodPlus) {
				Cd->Fz += d->PoleJedn.PrzodTylIle * Cp->Ladunek;
			}
			else {
				Cd->Fz -= d->PoleJedn.PrzodTylIle * Cp->Ladunek;
			}
		}

		
		//Pole centralne
		double r, wsp;
		for (int j = i-1; j < d->Ilosc - 1; j++) {

			if (d->Atrybuty[j].Puste) continue;
			if (i == j) continue;

			r = sqrt(pow(d->Ciala[i].Xm - d->Ciala[j].Xm, 2) + pow(d->Ciala[i].Ym - d->Ciala[j].Ym, 2) + pow(d->Ciala[i].Zm - d->Ciala[j].Zm, 2));
			if (r == 0) continue;

			//Pole centralne grawitacyjne
			wsp = (G * d->Ciala[i].Masa * d->Ciala[j].Masa) / pow(r, 3);
			d->Dane[i].Fx += wsp * (d->Ciala[j].Xm - d->Ciala[i].Xm);
			d->Dane[j].Fx -= wsp * (d->Ciala[j].Xm - d->Ciala[i].Xm);
			d->Dane[i].Fy += wsp * (d->Ciala[j].Ym - d->Ciala[i].Ym);
			d->Dane[j].Fy -= wsp * (d->Ciala[j].Ym - d->Ciala[i].Ym);
			d->Dane[i].Fz += wsp * (d->Ciala[j].Zm - d->Ciala[i].Zm);
			d->Dane[j].Fz -= wsp * (d->Ciala[j].Zm - d->Ciala[i].Zm);

			//Pole centralne elektrostatyczne
			wsp = (K * d->Ciala[i].Ladunek * d->Ciala[j].Ladunek) / pow(r, 3);
			d->Dane[i].Fx -= wsp * (d->Ciala[j].Xm - d->Ciala[i].Xm);
			d->Dane[j].Fx += wsp * (d->Ciala[j].Xm - d->Ciala[i].Xm);
			d->Dane[i].Fy -= wsp * (d->Ciala[j].Ym - d->Ciala[i].Ym);
			d->Dane[j].Fy += wsp * (d->Ciala[j].Ym - d->Ciala[i].Ym);
			d->Dane[i].Fz -= wsp * (d->Ciala[j].Zm - d->Ciala[i].Zm);
			d->Dane[j].Fz += wsp * (d->Ciala[j].Zm - d->Ciala[i].Zm);

		}

		//Tarcie
		//Gora
		if (Cd->Gora) {
			if (Cp->VXm < 0) {
				Cd->Fx += d->Tarcie.KinGora * Cd->Fy;
			}
			else if (Cp->VXm > 0) {
				Cd->Fx -= d->Tarcie.KinGora * Cd->Fy;
			}

			if (Cp->VZm < 0) {
				Cd->Fz += d->Tarcie.KinGora * Cd->Fy;
			}
			else if (Cp->VZm > 0) {
				Cd->Fz -= d->Tarcie.KinGora * Cd->Fy;
			}

		}

		//Dol
		if (Cd->Dol) {
			if (Cp->VXm < 0) {
				Cd->Fx -= d->Tarcie.KinDol * Cd->Fy;
			} else if(Cp->VXm > 0) {
				Cd->Fx += d->Tarcie.KinDol * Cd->Fy;
			}
			
			if (Cp->VZm < 0) {
				Cd->Fz -= d->Tarcie.KinDol * Cd->Fy;
			} else if (Cp->VZm > 0) {
				Cd->Fz += d->Tarcie.KinDol * Cd->Fy;
			}

		}

		//Lewo
		if (Cd->Lewo) {
			if (Cp->VYm < 0) {
				Cd->Fy -= d->Tarcie.KinLewo * Cd->Fx;
			}
			else if (Cp->VYm > 0) {
				Cd->Fy += d->Tarcie.KinLewo * Cd->Fx;
			}

			if (Cp->VZm < 0) {
				Cd->Fz -= d->Tarcie.KinLewo * Cd->Fx;
			}
			else if (Cp->VZm > 0) {
				Cd->Fz += d->Tarcie.KinLewo * Cd->Fx;
			}

		}

		//Prawo
		if (Cd->Prawo) {
			if (Cp->VYm < 0) {
				Cd->Fy += d->Tarcie.KinPrawo * Cd->Fx;
			}
			else if (Cp->VYm > 0) {
				Cd->Fy -= d->Tarcie.KinPrawo * Cd->Fx;
			}

			if (Cp->VZm < 0) {
				Cd->Fz += d->Tarcie.KinPrawo * Cd->Fx;
			}
			else if (Cp->VZm > 0) {
				Cd->Fz -= d->Tarcie.KinPrawo * Cd->Fx;
			}

		}

		//Przod
		if (Cd->Przod) {
			if (Cp->VXm < 0) {
				Cd->Fx -= d->Tarcie.KinPrzod * Cd->Fz;
			} else if (Cp->VXm > 0) {
				Cd->Fx += d->Tarcie.KinPrzod * Cd->Fz;
			}

			if (Cp->VYm < 0) {
				Cd->Fy -= d->Tarcie.KinPrzod * Cd->Fz;
			} else if (Cp->VYm > 0) {
				Cd->Fy += d->Tarcie.KinPrzod * Cd->Fz;
			}

		}

		//Tyl
		if (Cd->Tyl) {
			if (Cp->VXm < 0) {
				Cd->Fx += d->Tarcie.KinTyl * Cd->Fz;
			}
			else if (Cp->VXm > 0) {
				Cd->Fx -= d->Tarcie.KinTyl * Cd->Fz;
			}

			if (Cp->VYm < 0) {
				Cd->Fy += d->Tarcie.KinTyl * Cd->Fz;
			}
			else if (Cp->VYm > 0) {
				Cd->Fy -= d->Tarcie.KinTyl * Cd->Fz;
			}

		}

		//Zeruj sile przy kontakcie z krawedzia
		if (Cd->Gora && Cd->Fy > 0) Cd->Fy = 0;
		if (Cd->Dol && Cd->Fy < 0) Cd->Fy = 0;
		if (Cd->Lewo && Cd->Fx < 0) Cd->Fx = 0;
		if (Cd->Prawo && Cd->Fx > 0) Cd->Fx = 0;
		if (Cd->Przod && Cd->Fz < 0) Cd->Fz = 0;
		if (Cd->Tyl && Cd->Fz > 0) Cd->Fz = 0;


		//Przyspieszenie
		Cd->Ax = Cd->Fx / Cp->Masa;
		Cd->Ay = Cd->Fy / Cp->Masa;
		Cd->Az = Cd->Fz / Cp->Masa;
		
		//Odbicia od scian
		if (d->OdbiciaSciany) {
			//Gora/dol
			if (Cd->Gora && Cp->VYm > 0) {
				Cp->VYm = -(Cp->VYm * Cp->WspRestytucji);
				Cp->Ym = d->Rozmiar - Cp->Rm;
			}

			if (Cd->Dol && Cp->VYm < 0) {
				Cp->VYm = -(Cp->VYm * Cp->WspRestytucji);
				Cp->Ym = Cp->Rm;
			}

			//Lewo/prawo
			if (Cd->Lewo && Cp->VXm < 0) {
				Cp->VXm = -(Cp->VXm * Cp->WspRestytucji);
				Cp->Xm = Cp->Rm;
			}

			if (Cd->Prawo && Cp->VXm > 0) {
				Cp->VXm = -(Cp->VXm * Cp->WspRestytucji);
				Cp->Xm = d->Rozmiar - Cp->Rm;
			}

			//Przod/tyl
			if (Cd->Przod && Cp->VZm < 0) {
				Cp->VZm = -(Cp->VZm * Cp->WspRestytucji);
				Cp->Zm = Cp->Rm;
			}

			if (Cd->Tyl && Cp->VZm > 0) {
				Cp->VZm = -(Cp->VZm * Cp->WspRestytucji);
				Cp->Zm = d->Rozmiar - Cp->Rm;
			}
		}


		//Droga
		Cp->Xm += Cp->VXm * DeltaT + 0.5 * Cd->Ax * DeltaT * DeltaT;
		Cp->Ym += Cp->VYm * DeltaT + 0.5 * Cd->Ay * DeltaT * DeltaT;
		Cp->Zm += Cp->VZm * DeltaT + 0.5 * Cd->Az * DeltaT * DeltaT;

		//Predkosc
		Cp->VXm += Cd->Ax * DeltaT;
		Cp->VYm += Cd->Ay * DeltaT;
		Cp->VZm += Cd->Az * DeltaT;

		//Polozenie
		Ciala[i].x = Cp->Xm / d->Jednostka;
		Ciala[i].y = Cp->Ym / d->Jednostka;
		Ciala[i].z = Cp->Zm / d->Jednostka;
		
	}

}

long long PrzygotujDane1() {
	DaneFizyka1* DaneFiz = new DaneFizyka1;
	int ile = Dane->PoleCent.rozmiar_tablicy();
	strCialo* Cd;
	double Jedn0 = Dane->Ustawienia.Jednostka0;
	double JednJ = JEDNOSTKI[Dane->Ustawienia.JednostkaJedn].Mnoznik;
	CialoParametryFizyka1* Cf;
	DaneFiz->Ciala = new CialoParametryFizyka1[ile];
	DaneFiz->Dane = new CialoDaneFizyka1[ile];
	DaneFiz->Atrybuty = new CialoAtrybutyFizyka1[ile];
	DaneFiz->OdbiciaSciany = Dane->Ustawienia.OdbiciaSciany;

	for (int i = 0; i < ile; i++) {

		Cd = Dane->PoleCent[i];
		Cf = &DaneFiz->Ciala[i];

		if (Cd == nullptr) {
			DaneFiz->Atrybuty[i].Puste = true;
			continue;
		}else{
			DaneFiz->Atrybuty[i].Puste = false;
		}

		DaneFiz->Atrybuty[i].Nieruchome = Cd->Nieruchome;
		DaneFiz->Atrybuty[i].Wybucha = false;

		//Wspolrzedne
		Cf->Xm = Cd->X0 * JednJ;
		Cf->Ym = Cd->Y0 * JednJ;
		Cf->Zm = Cd->Z0 * JednJ;
		Cf->Rm = Cd->R0 * JednJ;

		//Predkosc
		Cf->VXm = Cd->V0Xt * JednJ;
		Cf->VYm = Cd->V0Yt * JednJ;
		Cf->VZm = Cd->V0Zt * JednJ;

		//Wybuch
		Cf->Sila = Cd->Sila0 * JEDNOSTKI[Cd->SilaJedn].Mnoznik;
		Cf->Promien = Cd->Promien0 * JednJ;

		//Masa, ladunek
		Cf->Masa = Cd->Masa0 * JEDNOSTKI[Cd->MasaJedn].Mnoznik / 1000.0;
		Cf->Ladunek = Cd->Ladunek0 * JEDNOSTKI[Cd->LadunekJedn].Mnoznik;

		Cf->WspRestytucji = Cd->WspRestytucji;
	}

	DaneFiz->PoleJedn = Dane->PoleJedn;
	DaneFiz->Tarcie = Dane->Tarcie;
	DaneFiz->Ilosc = ile;
	DaneFiz->Jednostka = Dane->Ustawienia.Jednostka0 * JEDNOSTKI[Dane->Ustawienia.JednostkaJedn].Mnoznik;
	DaneFiz->Rozmiar = DaneFiz->Jednostka*Dane->Ustawienia.JednIlosc;

	return (long long)DaneFiz;
}

void CzyscDane1(long long Dane) {
	DaneFizyka1* d = (DaneFizyka1*)Dane;
	delete[] d->Ciala;
	delete[] d->Dane;
	delete[] d->Atrybuty;
	delete d;
}

void Wybuch1(long long Dane, int IdCiala){
	DaneFizyka1* DaneFiz = (DaneFizyka1*)Dane;
	DaneFiz->Atrybuty[IdCiala].Wybucha = true;
}

void PobierzPredkosc1(long long Dane, int IdCiala, Punkt* Predkosc){
	DaneFizyka1* DaneFiz = (DaneFizyka1*)Dane;
	Predkosc->x = DaneFiz->Ciala[IdCiala].VXm;
	Predkosc->y = DaneFiz->Ciala[IdCiala].VYm;
	Predkosc->z = DaneFiz->Ciala[IdCiala].VZm;
}