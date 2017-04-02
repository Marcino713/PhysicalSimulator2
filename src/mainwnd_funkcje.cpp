#include "..\inc\mainwnd.h"
#include "..\inc\Dane.h"
#include "..\inc\animacja.h"
#include "..\inc\dodatki.h"
#include "..\inc\bledy.h"
#include <WinUser.h>

extern MainWindow* Okno;
extern Animacja3D* Animacja;
extern DaneSymulacji* Dane;
extern strStanProgramu* StanProgramu;
extern SprawdzanieBledow* Bledy;
extern Fizyka* SilnikFizyczny;

//Karty z nazwami kategorii ustawieñ
void MainWindow::UstawZaznaczenieKarty(HWND karta, bool zaznaczona) {
	SetWindowLongPtr(karta, 0, (LONG)zaznaczona);
	InvalidateRect(karta, NULL, true);
}

bool MainWindow::SprawdzZaznaczenieKarty(HWND karta) { return (GetWindowLongPtr(karta, 0) == 0 ? 0 : 1); }

//Panele z ustawieniami
void MainWindow::UstawPanele() {
	MoveWindow(this->pnlUstawieniaDane, 0, 0, 0, 0, true);
	MoveWindow(this->pnlPoleCentDane, 0, 0, 0, 0, true);
	MoveWindow(this->pnlPoleJednDane, 0, 0, 0, 0, true);
	MoveWindow(this->pnlTarcieDane, 0, 0, 0, 0, true);
}

void MainWindow::ZwinPanele(){
	if (this->PanelZaznaczony == Nic) return;
	int y = 0;

	//Ustawienia
	UstawZaznaczenieKarty(this->btnKartaUstawienia, false);
	MoveWindow(this->pnlUstawieniaDane, 0, 0, 0, 0, true);

	y += 3 * this->KARTA_ODSTEP + this->KARTA_WYS;

	//Pole centralne
	UstawZaznaczenieKarty(this->btnKartaPoleCent, false);
	MoveWindow(this->btnKartaPoleCent, this->KARTA_LEWO, y, this->KARTA_SZER, this->KARTA_WYS, true);
	MoveWindow(this->pnlPoleCentDane, 0, 0, 0, 0, true);

	y += 2 * this->KARTA_ODSTEP + this->KARTA_WYS;

	//Pole jednorodne
	UstawZaznaczenieKarty(this->btnKartaPoleJedn, false);
	MoveWindow(this->btnKartaPoleJedn, this->KARTA_LEWO, y, this->KARTA_SZER, this->KARTA_WYS, true);
	MoveWindow(this->pnlPoleJednDane, 0, 0, 0, 0, true);

	y += 2 * this->KARTA_ODSTEP + this->KARTA_WYS;

	//Tarcie
	UstawZaznaczenieKarty(this->btnKartaTarcie, false);
	MoveWindow(this->btnKartaTarcie, this->KARTA_LEWO, y, this->KARTA_SZER, this->KARTA_WYS, true);
	MoveWindow(this->pnlTarcieDane, 0, 0, 0, 0, true);
}

void MainWindow::btnKartaUstawienia_Click(){
	this->ZwinPanele();

	if (this->PanelZaznaczony != Ustawienia) {
		UstawZaznaczenieKarty(this->btnKartaUstawienia, true);

		int y = 2 * this->KARTA_ODSTEP + this->KARTA_WYS;
		MoveWindow(this->pnlUstawieniaDane, this->KARTA_DANE_LEWO, y, this->KARTA_DANE_SZER, this->KARTA_USTAWIENIA_WYS, true);

		y += this->KARTA_USTAWIENIA_WYS + this->KARTA_ODSTEP;
		MoveWindow(this->btnKartaPoleCent, this->KARTA_LEWO, y, this->KARTA_SZER, this->KARTA_WYS, true);

		y += 2 * this->KARTA_ODSTEP + this->KARTA_WYS;
		MoveWindow(this->btnKartaPoleJedn, this->KARTA_LEWO, y, this->KARTA_SZER, this->KARTA_WYS, true);

		y += 2 * this->KARTA_ODSTEP + this->KARTA_WYS;
		MoveWindow(this->btnKartaTarcie, this->KARTA_LEWO, y, this->KARTA_SZER, this->KARTA_WYS, true);

		this->PanelZaznaczony = Ustawienia;
	}
	else {
		UstawZaznaczenieKarty(this->btnKartaUstawienia, false);
		this->PanelZaznaczony = Nic;
	}
}

void MainWindow::btnKartaPoleCent_Click(){
	this->ZwinPanele();

	if (this->PanelZaznaczony != PoleCent) {
		UstawZaznaczenieKarty(this->btnKartaPoleCent, true);

		int y = 4 * this->KARTA_ODSTEP + 2 * this->KARTA_WYS;
		MoveWindow(this->pnlPoleCentDane, this->KARTA_DANE_LEWO, y, this->KARTA_DANE_SZER, this->KARTA_POLECENT_WYS, true);

		y += this->KARTA_POLECENT_WYS + this->KARTA_ODSTEP;
		MoveWindow(this->btnKartaPoleJedn, this->KARTA_LEWO, y, this->KARTA_SZER, this->KARTA_WYS, true);

		y += 2 * this->KARTA_ODSTEP + this->KARTA_WYS;
		MoveWindow(this->btnKartaTarcie, this->KARTA_LEWO, y, this->KARTA_SZER, this->KARTA_WYS, true);

		this->PanelZaznaczony = PoleCent;
	}
	else {
		UstawZaznaczenieKarty(this->btnKartaPoleCent, false);
		this->PanelZaznaczony = Nic;
	}
}

void MainWindow::btnKartaPoleJedn_Click(){
	this->ZwinPanele();

	if (this->PanelZaznaczony != PoleJedn) {
		UstawZaznaczenieKarty(this->btnKartaPoleJedn, true);

		int y = 6 * this->KARTA_ODSTEP + 3 * this->KARTA_WYS;
		MoveWindow(this->pnlPoleJednDane, this->KARTA_DANE_LEWO, y, this->KARTA_DANE_SZER, this->KARTA_POLEJEDN_WYS, true);

		y += this->KARTA_POLEJEDN_WYS + this->KARTA_ODSTEP;
		MoveWindow(this->btnKartaTarcie, this->KARTA_LEWO, y, this->KARTA_SZER, this->KARTA_WYS, true);

		this->PanelZaznaczony = PoleJedn;
	}
	else {
		UstawZaznaczenieKarty(this->btnKartaPoleJedn, false);
		this->PanelZaznaczony = Nic;
	}
}

void MainWindow::btnKartaTarcie_Click(){
	this->ZwinPanele();

	if (this->PanelZaznaczony != Tarcie) {
		UstawZaznaczenieKarty(this->btnKartaTarcie, true);

		int y = 8 * this->KARTA_ODSTEP + 4 * this->KARTA_WYS;
		MoveWindow(this->pnlTarcieDane, this->KARTA_DANE_LEWO, y, this->KARTA_DANE_SZER, this->KARTA_TARCIE_WYS, true);

		this->PanelZaznaczony = Tarcie;
	}
	else {
		UstawZaznaczenieKarty(this->btnKartaTarcie, false);
		this->PanelZaznaczony = Nic;
	}
}

//Ustawia kontrolki na nowych pozycjach po zmianie rozmiaru okna
void MainWindow::ZmienRozmiar(int szer, int wys){	
	//Start, stop
	MoveWindow(this->btnStart, this->BTN_START_LEWO, wys - this->BTN_STARTSTOP_DOL, this->BTN_STARTSTOP_SZER, this->BTN_STARTSTOP_WYS, true);
	MoveWindow(this->btnStop, this->BTN_STOP_LEWO, wys - this->BTN_STARTSTOP_DOL, this->BTN_STARTSTOP_SZER, this->BTN_STARTSTOP_WYS, true);

	//Dane
	MoveWindow(this->pnlDane, szer - this->PNL_DANE_SZER, 0, this->PNL_DANE_SZER, wys, true);

	//Obraz
	MoveWindow(this->pnlObraz, 0, 0, szer - this->PNL_OBRAZ_PRAWO, wys - this->PNL_OBRAZ_DOL, true);
	Animacja->UstawRozmiarObrazu(szer - this->PNL_OBRAZ_PRAWO, wys - this->PNL_OBRAZ_DOL);

	InvalidateRect(this->pnlDane, NULL, true);
}

//Zablokuj/odblokuj kontrolki
void MainWindow::EnablePoleCent(bool enable){
	EnableWindow(this->lblCialo, enable);
	EnableWindow(this->Label11, enable);
	EnableWindow(this->btnPoleCentUsun, enable);
	EnableWindow(this->Label13, enable);
	EnableWindow(this->txtPoleCentX, enable);
	EnableWindow(this->lblPoleCentXJedn, enable);
	EnableWindow(this->Label14, enable);
	EnableWindow(this->txtPoleCentY, enable);
	EnableWindow(this->lblPoleCentYJedn, enable);
	EnableWindow(this->Label22, enable);
	EnableWindow(this->txtPoleCentZ, enable);
	EnableWindow(this->lblPoleCentZJedn, enable);
	EnableWindow(this->Label41, enable);
	EnableWindow(this->txtPoleCentR, enable);
	EnableWindow(this->lblPoleCentRJedn, enable);
	EnableWindow(this->cbPoleCentNieruchome, enable);
	EnableWindow(this->Label43, enable);
	EnableWindow(this->txtPoleCentK, enable);
	EnableWindow(this->Label42, enable);
	EnableWindow(this->grpWybuch, enable);
	EnableWindow(this->cbPoleCentWybuch, enable);
	EnableWindow(this->Label27, enable);
	EnableWindow(this->txtPoleCentMasa, enable);
	EnableWindow(this->cboPoleCentMasa, enable);
	EnableWindow(this->Label26, enable);
	EnableWindow(this->txtPoleCentLadunek, enable);
	EnableWindow(this->cboPoleCentLadunek, enable);
}

void MainWindow::EnablePoleCentPredkosc(bool enable){
	EnableWindow(this->Label35, enable);
	EnableWindow(this->lblPoleCentV0Jedn, enable);
	EnableWindow(this->Label34, enable);
	EnableWindow(this->txtPoleCentV0X, enable);
	EnableWindow(this->lblPoleCentV0XJedn, enable);
	EnableWindow(this->Label33, enable);
	EnableWindow(this->txtPoleCentV0Y, enable);
	EnableWindow(this->lblPoleCentV0YJedn, enable);
	EnableWindow(this->Label29, enable);
	EnableWindow(this->txtPoleCentV0Z, enable);
	EnableWindow(this->lblPoleCentV0ZJedn, enable);
}

void MainWindow::EnablePoleCentWybuch(bool enable){
	EnableWindow(this->Label39, enable);
	EnableWindow(this->txtPoleCentWybuchF, enable);
	EnableWindow(this->cboPoleCentWybuchF, enable);
	EnableWindow(this->btnPoleCentWybuch, enable);
	EnableWindow(this->Label38, enable);
	EnableWindow(this->txtPoleCentWybuchR, enable);
	EnableWindow(this->lblPoleCentWybuchRJedn, enable);
}

void MainWindow::EnablePoleJednElGora(bool enable){
	EnableWindow(this->Label18, enable);
	EnableWindow(this->rbPoleJednElGoraPlus, enable);
	EnableWindow(this->rbPoleJednElGoraMinus, enable);
	EnableWindow(this->txtPoleJednElGoraE, enable);
	EnableWindow(this->Label20, enable);
}

void MainWindow::EnablePoleJednElLewo(bool enable){
	EnableWindow(this->Label21, enable);
	EnableWindow(this->rbPoleJednElLewoPlus, enable);
	EnableWindow(this->rbPoleJednElLewoMinus, enable);
	EnableWindow(this->txtPoleJednElLewoE, enable);
	EnableWindow(this->Label23, enable);
}

void MainWindow::EnablePoleJednElPrzod(bool enable){
	EnableWindow(this->Label46, enable);
	EnableWindow(this->rbPoleJednElPrzodPlus, enable);
	EnableWindow(this->rbPoleJednElPrzodMinus, enable);
	EnableWindow(this->txtPoleJednElPrzodE, enable);
	EnableWindow(this->Label47, enable);
}

void MainWindow::EnableAnimacja(bool enable) {
	if (this->PanelZaznaczony != PanelZazn::PoleCent && !enable) {
		this->ZwinPanele();
		this->PanelZaznaczony = PanelZazn::Nic;
	}
	EnableWindow(this->btnKartaUstawienia, enable);
	EnableWindow(this->btnKartaPoleJedn, enable);
	EnableWindow(this->btnKartaTarcie, enable);
	EnableWindow(this->btnPoleCentDodaj, enable);
	SetFocus(this->wndMain);

	if (enable) {
		int id = this->PobierzZaznId();
		if (id == -1) {
			this->EnablePoleCent(false);
			this->EnablePoleCentPredkosc(false);
			this->EnablePoleCentWybuch(false);
		} else {
			this->EnablePoleCent(true);
			this->EnablePoleCentPredkosc(!Dane->PoleCent[id]->Nieruchome);
			this->EnablePoleCentWybuch(Dane->PoleCent[id]->Wybucha);
		}
	} else {
		this->EnablePoleCent(enable);
		this->EnablePoleCentPredkosc(enable);
		this->EnablePoleCentWybuch(enable);
	}

}

void MainWindow::CzyscPoleCent(){
	char* t = new char;
	t[0] = 0;

	SetWindowText(this->lblCialo, t);
	SetWindowText(this->txtPoleCentX, t);
	SetWindowText(this->txtPoleCentY, t);
	SetWindowText(this->txtPoleCentZ, t);
	SetWindowText(this->txtPoleCentR, t);
	Button_SetCheck(this->cbPoleCentNieruchome, false);
	SetWindowText(this->txtPoleCentV0, t);
	SetWindowText(this->txtPoleCentV0X, t);
	SetWindowText(this->txtPoleCentV0Y, t);
	SetWindowText(this->txtPoleCentV0Z, t);
	SetWindowText(this->txtPoleCentK, t);
	Button_SetCheck(this->cbPoleCentWybuch, false);
	SetWindowText(this->txtPoleCentWybuchF, t);
	SetWindowText(this->txtPoleCentWybuchR, t);
	SetWindowText(this->txtPoleCentMasa, t);
	SetWindowText(this->txtPoleCentLadunek, t);

	delete t;
}

void MainWindow::UstawJednostkeSkali(int Jednostka){
	Dane->Ustawienia.JednostkaJedn = Jednostka;

	string odleglosc = JEDNOSTKI[Jednostka].Przedrostek + "m";
	string predkosc = JEDNOSTKI[Jednostka].Przedrostek + "m/s";

	SetWindowText(this->lblPoleCentXJedn, odleglosc.c_str());
	SetWindowText(this->lblPoleCentYJedn, odleglosc.c_str());
	SetWindowText(this->lblPoleCentZJedn, odleglosc.c_str());
	SetWindowText(this->lblPoleCentRJedn, odleglosc.c_str());

	SetWindowText(this->lblPoleCentV0Jedn,  predkosc.c_str());
	SetWindowText(this->lblPoleCentV0XJedn, predkosc.c_str());
	SetWindowText(this->lblPoleCentV0YJedn, predkosc.c_str());
	SetWindowText(this->lblPoleCentV0ZJedn, predkosc.c_str());

	SetWindowText(this->lblPoleCentWybuchRJedn, odleglosc.c_str());
}

void MainWindow::PokazPrzyciskPauza(bool pokaz) {
	if (pokaz) {
		SetWindowText(this->btnStart, this->TBTNSTART2);		
	} else {
		SetWindowText(this->btnStart, this->TBTNSTART);
	}

	EnableWindow(this->btnStop, pokaz);
}

void MainWindow::ZmienUstawieniaGrafiki() {
	UstawieniaSceny ust;
	ust.JednIlosc = Dane->Ustawienia.JednIlosc;
	ust.JednNazwa = JEDNOSTKI[Dane->Ustawienia.JednostkaJedn].Przedrostek + "m";
	ust.Jednostka0 = Dane->Ustawienia.Jednostka0;
	ust.UstJednPodzialka = Dane->Ustawienia.UstJednPodzialka;
	Animacja->UstawUstawienia(ust);
}

bool MainWindow::ZapytajIZapiszPlik() {
	int zapisz = MessageBox(this->wndMain, ("Zapisaæ plik?\n" + StanProgramu->NazwaPliku).c_str(), "Zapisywanie pliku", MB_YESNOCANCEL);

	switch (zapisz) {
	case IDYES:
		if (Bledy->PokazBledy()) return true;
		if (!ZapiszPlik(this->wndMain)) return true;
		return false;

	case IDNO:
		return false;

	default:
		return true;
	}
}

int MainWindow::DodajCialo(double x, double y, double z, double r) {
	strCialo* cialo = new strCialo(x, y, z, r);
	return this->DodajCialo(cialo);
}

int MainWindow::DodajCialo(strCialo* cialo){
	int id = Dane->PoleCent.dodaj(cialo);

	if (cialo == NULL) return -1;

	char* s = new char[DLUGOSC_TEKSTU];
	itoid(s, id+1);

	LVITEM lvi;
	lvi.mask = LVIF_TEXT;
	lvi.iItem = id;
	lvi.iSubItem = 0;
	lvi.pszText = s;
	
	int lv_id = ListView_InsertItem(this->lvCiala, &lvi);
	ListView_SetItemState(this->lvCiala, lv_id, LVIS_SELECTED, LVIS_SELECTED);

	itod(s, cialo->Masa0, JEDNOSTKI[cialo->MasaJedn].Przedrostek + "g");
	ListView_SetItemText(this->lvCiala, id, 1, s);
	itod(s, cialo->Ladunek0, JEDNOSTKI[cialo->LadunekJedn].Przedrostek + "C");
	ListView_SetItemText(this->lvCiala, id, 2, s);
	
	delete[] s;
	return id;
}

void MainWindow::ZaznaczCialo(int id){
	this->Zdarzenia = false;

	if (Bledy->PokazBledyPoleCent()) {
		this->Zdarzenia = true;
		return;
	}

	this->ZaznId = id;
	SilnikFizyczny->ZaznaczCialo(id);

	if (id == -1) {	//Usun zaznaczenie

		this->CzyscPoleCent();
		if (!StanProgramu->Animacja) {
			this->EnablePoleCent(false);
			this->EnablePoleCentPredkosc(false);
			this->EnablePoleCentWybuch(false);
		} else {
			EnableWindow(this->btnPoleCentWybuch, false);
		}

	} else {	//Zaznacz

		if (!StanProgramu->Animacja) {
			this->EnablePoleCent(true);
		} else {
			EnableWindow(this->btnPoleCentWybuch, Dane->PoleCent[id]->Wybucha);
		}
		this->UstawKontrolkiPoleCent(id);

	}

	this->Zdarzenia = true;
}

void MainWindow::UsunCialo(int IdCiala){
	char* tekst = new char[DLUGOSC_TEKSTU];
	itoid(tekst, IdCiala + 1);
	string s = "Usun¹æ cia³o ";
	s += tekst;
	s += "?";

	if (MessageBox(this->wndMain, s.c_str(), "Usuwanie cia³a", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES) {
		ListView_DeleteItem(this->lvCiala, Okno->ZnajdzCialoListView(IdCiala));
		this->ZaznaczCialo(-1);
		Dane->PoleCent.usun(IdCiala);
		StanProgramu->Zapisany = false;
	}

	RysujCiala();

	delete[] tekst;
}

void MainWindow::ObliczPredkosc(){
	strCialo* c = Dane->PoleCent[this->PobierzZaznId()];
	char* t = new char[DLUGOSC_TEKSTU];

	c->V0t = sqrt(pow(c->V0Xt, 2.0) + pow(c->V0Yt, 2.0) + pow(c->V0Zt, 2.0));
	ftoa(t, c->V0t);
	SetWindowText(this->txtPoleCentV0, t);

	delete[] t;
}

int MainWindow::ZnajdzCialoListView(int id) {
	int id2;
	LVFINDINFO lvf;
	ZeroMemory(&lvf, sizeof(lvf));
	lvf.flags = LVFI_STRING;

	char* t = new char[DLUGOSC_TEKSTU];

	itoid(t, id+1);
	lvf.psz = t;
	id2 = ListView_FindItem(this->lvCiala, -1, &lvf);

	delete[] t;
	return id2;
}

void MainWindow::ZmienTekstListView(int podelement, double liczba, string jednostka) {
	int id = ZnajdzCialoListView(this->ZaznId);

	char* s = new char[DLUGOSC_TEKSTU];
	itod(s, liczba, jednostka);
	ListView_SetItemText(Okno->lvCiala, id, podelement, s);

	delete[] s;
}

void MainWindow::UstawKontrolki() {
	this->Zdarzenia = false;
	char* str = new char[DLUGOSC_TEKSTU];

	//Ustawienia
	ftoa(str, Dane->Ustawienia.Jednostka0);
	SetWindowText(this->txtUstJednostka, str);

	ComboBox_SetCurSel(this->cboUstJednostka, Dane->Ustawienia.JednostkaJedn);
	Okno->UstawJednostkeSkali(Dane->Ustawienia.JednostkaJedn);

	ftoa(str, Dane->Ustawienia.JednIlosc);
	SetWindowText(this->txtUstIloscJednostek, str);

	ftoa(str, Dane->Ustawienia.UstJednPodzialka);
	SetWindowText(this->txtUstJednPodzialka, str);

	Button_SetCheck(this->cbUstOdbiciaSciany, Dane->Ustawienia.OdbiciaSciany);

	//Pole jednorodne grawitacyjne
	Button_SetCheck(this->cbPoleJednGrawGora, Dane->PoleJedn.Gora);
	ftoa(str, Dane->PoleJedn.GoraIle);
	SetWindowText(this->txtPoleJednGrawGora, str);
	EnableWindow(this->txtPoleJednGrawGora, Dane->PoleJedn.Gora);
	
	Button_SetCheck(this->cbPoleJednGrawDol, Dane->PoleJedn.Dol);
	ftoa(str, Dane->PoleJedn.DolIle);
	SetWindowText(this->txtPoleJednGrawDol, str);
	EnableWindow(this->txtPoleJednGrawDol, Dane->PoleJedn.Dol);

	Button_SetCheck(this->cbPoleJednGrawLewo, Dane->PoleJedn.Lewo);
	ftoa(str, Dane->PoleJedn.LewoIle);
	SetWindowText(this->txtPoleJednGrawLewo, str);
	EnableWindow(this->txtPoleJednGrawLewo, Dane->PoleJedn.Lewo);

	Button_SetCheck(this->cbPoleJednGrawPrawo, Dane->PoleJedn.Prawo);
	ftoa(str, Dane->PoleJedn.PrawoIle);
	SetWindowText(this->txtPoleJednGrawPrawo, str);
	EnableWindow(this->txtPoleJednGrawPrawo, Dane->PoleJedn.Prawo);

	Button_SetCheck(this->cbPoleJednGrawPrzod, Dane->PoleJedn.Przod);
	ftoa(str, Dane->PoleJedn.PrzodIle);
	SetWindowText(this->txtPoleJednGrawPrzod, str);
	EnableWindow(this->txtPoleJednGrawPrzod, Dane->PoleJedn.Przod);

	Button_SetCheck(this->cbPoleJednGrawTyl, Dane->PoleJedn.Tyl);
	ftoa(str, Dane->PoleJedn.TylIle);
	SetWindowText(this->txtPoleJednGrawTyl, str);
	EnableWindow(this->txtPoleJednGrawTyl, Dane->PoleJedn.Tyl);

	//Pole jednorodne elektrostatyczne
	Button_SetCheck(this->cbPoleJednElGora, Dane->PoleJedn.GoraDol);
	Button_SetCheck(this->rbPoleJednElGoraPlus, Dane->PoleJedn.GoraPlus);
	Button_SetCheck(this->rbPoleJednElGoraMinus, !Dane->PoleJedn.GoraPlus);
	ftoa(str, Dane->PoleJedn.GoraDolIle);
	SetWindowText(this->txtPoleJednElGoraE, str);
	EnablePoleJednElGora(Dane->PoleJedn.GoraDol);

	Button_SetCheck(this->cbPoleJednElLewo, Dane->PoleJedn.LewoPrawo);
	Button_SetCheck(this->rbPoleJednElLewoPlus, Dane->PoleJedn.LewoPlus);
	Button_SetCheck(this->rbPoleJednElLewoMinus, !Dane->PoleJedn.LewoPlus);
	ftoa(str, Dane->PoleJedn.LewoPrawoIle);
	SetWindowText(this->txtPoleJednElLewoE, str);
	EnablePoleJednElLewo(Dane->PoleJedn.LewoPrawo);

	Button_SetCheck(this->cbPoleJednElPrzod, Dane->PoleJedn.PrzodTyl);
	Button_SetCheck(this->rbPoleJednElPrzodPlus, Dane->PoleJedn.PrzodPlus);
	Button_SetCheck(this->rbPoleJednElPrzodMinus, !Dane->PoleJedn.PrzodPlus);
	ftoa(str, Dane->PoleJedn.PrzodTylIle);
	SetWindowText(this->txtPoleJednElPrzodE, str);
	EnablePoleJednElPrzod(Dane->PoleJedn.PrzodTyl);

	//Tarcie
	ftoa(str, Dane->Tarcie.StGora);
	SetWindowText(this->txtTarcieStGora, str);
	ftoa(str, Dane->Tarcie.KinGora);
	SetWindowText(this->txtTarcieKinGora, str);

	ftoa(str, Dane->Tarcie.StDol);
	SetWindowText(this->txtTarcieStDol, str);
	ftoa(str, Dane->Tarcie.KinDol);
	SetWindowText(this->txtTarcieKinDol, str);

	ftoa(str, Dane->Tarcie.StLewo);
	SetWindowText(this->txtTarcieStLewo, str);
	ftoa(str, Dane->Tarcie.KinLewo);
	SetWindowText(this->txtTarcieKinLewo, str);

	ftoa(str, Dane->Tarcie.StPrawo);
	SetWindowText(this->txtTarcieStPrawo, str);
	ftoa(str, Dane->Tarcie.KinPrawo);
	SetWindowText(this->txtTarcieKinPrawo, str);

	ftoa(str, Dane->Tarcie.StPrzod);
	SetWindowText(this->txtTarcieStPrzod, str);
	ftoa(str, Dane->Tarcie.KinPrzod);
	SetWindowText(this->txtTarcieKinPrzod, str);

	ftoa(str, Dane->Tarcie.StTyl);
	SetWindowText(this->txtTarcieStTyl, str);
	ftoa(str, Dane->Tarcie.KinTyl);
	SetWindowText(this->txtTarcieKinTyl, str);

	delete[] str;
	this->Zdarzenia = true;	
}

void MainWindow::UstawKontrolkiPoleCent(int ix){
	this->Zdarzenia = false;
	strCialo* c = Dane->PoleCent[ix];

	if (c == NULL) {
		this->Zdarzenia = true;
		return;
	}

	char* str = new char[DLUGOSC_TEKSTU];
	itoid(str, ix + 1);
	SetWindowText(this->lblCialo, str);

	//Polozenie
	ftoa(str, c->X0);
	SetWindowText(this->txtPoleCentX, str);

	ftoa(str, c->Y0);
	SetWindowText(this->txtPoleCentY, str);

	ftoa(str, c->Z0);
	SetWindowText(this->txtPoleCentZ, str);

	ftoa(str, c->R0);
	SetWindowText(this->txtPoleCentR, str);

	//Predkosc
	Button_SetCheck(this->cbPoleCentNieruchome, c->Nieruchome);
	if(!StanProgramu->Animacja) this->EnablePoleCentPredkosc(!c->Nieruchome);

	ftoa(str, c->V0t);
	SetWindowText(this->txtPoleCentV0, str);

	ftoa(str, c->V0Xt);
	SetWindowText(this->txtPoleCentV0X, str);

	ftoa(str, c->V0Yt);
	SetWindowText(this->txtPoleCentV0Y, str);

	ftoa(str, c->V0Zt);
	SetWindowText(this->txtPoleCentV0Z, str);

	//Wspolczynnik restytucji
	ftoa(str, c->WspRestytucji);
	SetWindowText(this->txtPoleCentK, str);

	//Wybuch
	Button_SetCheck(this->cbPoleCentWybuch, c->Wybucha);
	if (!StanProgramu->Animacja) this->EnablePoleCentWybuch(c->Wybucha);

	ftoa(str, c->Sila0);
	SetWindowText(this->txtPoleCentWybuchF, str);

	ComboBox_SetCurSel(this->cboPoleCentWybuchF, c->SilaJedn);

	ftoa(str, c->Promien0);
	SetWindowText(this->txtPoleCentWybuchR, str);

	//Masa, ladunek
	ftoa(str, c->Masa0);
	SetWindowText(this->txtPoleCentMasa, str);

	ComboBox_SetCurSel(this->cboPoleCentMasa, c->MasaJedn);

	ftoa(str, c->Ladunek0);
	SetWindowText(this->txtPoleCentLadunek, str);

	ComboBox_SetCurSel(this->cboPoleCentLadunek, c->LadunekJedn);

	delete[] str;

	this->Zdarzenia = true;
}


void MainWindow::UtworzJednostki(string nazwa, HWND ComboBox){
	for (int i = 0; i < ILOSC_JEDNOSTEK; i++) {
		ComboBox_InsertString(ComboBox, i, (LPARAM)(JEDNOSTKI[i].Przedrostek + nazwa).c_str());
	}
}