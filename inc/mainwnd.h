#pragma once
#ifndef mainwindow_h
#define mainwindow_h

#include <Windows.h>
#include <windowsx.h>
#include <CommCtrl.h>
#include "..\resource.h"
#include <string>
#include "dane.h"
#include "pliki.h"
#include "anim_dodatki.h"

using std::string;

enum PanelZazn { Nic, Ustawienia, PoleCent, PoleJedn, Tarcie };

class MainWindow {
private:
	//Dane okna
	const LPCSTR NAZWA_KLASY = "wndSymulatorFizyczny";
	const LPCSTR TYTUL_OKNA = "Symulator fizyczny";
	const int WNDMAIN_SZER = 900;
	const int WNDMAIN_WYS = 700;

	//Nazwy klas kontrolek
	const LPCSTR PRZYCISK = "BUTTON";
	const LPCSTR STATYCZNA = "STATIC";
	const LPCSTR POLETEKSTOWE = "EDIT";
	const LPCSTR POLEKOMBI = "COMBOBOX";
	const LPCSTR PASEKPRZEWIJANIA = "SCROLLBAR";

	//Teksty w kontrolkach
	const LPCSTR TBTNSTART = "Start";
	const LPCSTR TBTNSTOP = "Stop";
	const LPCSTR TBTNSTART2 = "Pauza";
	const LPCSTR TBTNKARTAPOLECENT = "Pole centralne";
	const LPCSTR TBTNKARTAPOLEJEDN = "Pole jednorodne";
	const LPCSTR TBTNKARTATARCIE = "Tarcie";
	const LPCSTR TBTNKARTAUSTAWIENIA = "Ustawienia";
	const LPCSTR TLABEL5 = "1 jednostka =";
	const LPCSTR TLABEL3 = "Ilość jednostek:";
	const LPCSTR TLABEL4 = "Co ile jednostek wyświetlić liczby:";
	const LPCSTR TCBUSTODBICIASCIANY = "Odbicia od ścian";
	const LPCSTR TGRPPOLEJEDNGRAW = "Pole grawitacyjne V";	//Ɣ
	const LPCSTR TGRPPOLEJEDNEL = "Pole elektrostatyczne E";
	const LPCSTR TLBLGORA = "Góra:";
	const LPCSTR TLBLDOL = "Dół:";
	const LPCSTR TLBLLEWO = "Lewo:";
	const LPCSTR TLBLPRAWO = "Prawo:";
	const LPCSTR TLBLPRZOD = "Przód:";
	const LPCSTR TLBLTYL = "Tył:";
	const LPCSTR TLBLPOLEJEDNGRAWJEDN = "N/m";
	const LPCSTR TCBPOLEJEDNELGORA = "Góra/dół";
	const LPCSTR TCBPOLEJEDNELLEWO = "Lewo/prawo";
	const LPCSTR TCBPOLEJEDNELPRZOD = "Przód/tył";
	const LPCSTR TLBLPLUS = "+";
	const LPCSTR TLBLMINUS = "-";
	const LPCSTR TLBLPOLEJEDNELJEDN = "N/C";
	const LPCSTR TGRPTARCIEWSP = "Współczynniki tarcia";
	const LPCSTR TLABEL1 = "Statyczne";
	const LPCSTR TLABEL48 = "Kinetyczne";
	const LPCSTR TLBLRESTWSPJEDN = "N/m";
	const LPSTR  TLVCCIALO = "Ciało";
	const LPSTR  TLVCMASA = "Masa";
	const LPSTR  TLVCLADUNEK = "Ładunek";
	const LPCSTR TLABEL11 = "Ciało:";
	const LPCSTR TBTNPOLECENTUSUN = "Usuń";
	const LPCSTR TLBLX = "X:";
	const LPCSTR TLBLY = "Y:";
	const LPCSTR TLBLZ = "Z:";
	const LPCSTR TLBLR = "R:";
	const LPCSTR TCBPOLECENTNIERUCHOME = "Nieruchome";
	const LPCSTR TLBLV0  = "v0:";
	const LPCSTR TLBLV0X = "v0x:";
	const LPCSTR TLBLV0Y = "v0y:";
	const LPCSTR TLBLV0Z = "v0z:";
	const LPCSTR TLABEL43 = "Współczynnik restytucji:";
	const LPCSTR TGRPWYBUCH = "Eksplozja";
	const LPCSTR TCBPOLECENTWYBUCH = "Ciało wybucha";
	const LPCSTR TLBLF = "F:";
	const LPCSTR TLBLN = "N";
	const LPCSTR TLBLM = "M:";
	const LPCSTR TLBLQ = "Q:";
	const LPCSTR TBTNPOLECENTDODAJ = "Dodaj ciało";
	const LPCSTR TBTNPOLECENTWYBUCH = "Detonuj";

	//Klasy nowych kontrolek
	const LPCSTR NTEXTBOX = "TextBox";
	const LPCSTR NBTNKARTA = "btnKarta";
	const LPCSTR NPNLOBRAZ = "pnlObraz";
	const LPCSTR NPNLDANE = "pnlDane";
	const LPCSTR NPNLUSTAWIENIADANE = "pnlUstawieniaDane";
	const LPCSTR NGRPPOLEJEDNGRAW = "grpPoleJednGraw";
	const LPCSTR NGRPPOLEJEDNEL = "grpPoleJednEl";
	const LPCSTR NPNLPOLEJEDNEL = "pnlPoleJednEl";
	const LPCSTR NGRPTARCIEWSP = "grpTarcieWsp";
	const LPCSTR NPNLPOLECENTDANE = "pnlPoleCentDane";
	const LPCSTR NPNLPOLECENTPRZEWIN = "pnlPoleCentPrzewin";
	const LPCSTR NGRPWYBUCH = "grpWybuch";
	
	int ZaznId = -1;

	void UtworzObiektyGdi();
	void UsunObiektyGdi();

public:

	double OstPromien = 1.0;

	//Obiekty GDI
	HBITMAP imgZapisz;
	HBITMAP imgOtworz;
	HBITMAP imgNowy;
	HBITMAP imgPlus;
	HBITMAP imgMinus;
	HICON icIkona;

	COLORREF clrSzary = 0xE3E3E3;
	HFONT fntCzcionka;
	HBRUSH brSzary;

	HCURSOR curStrzalka;
	HCURSOR curTekst;

	//Wymiary kontrolek
	const int BTN_STARTSTOP_SZER = 75;
	const int BTN_STARTSTOP_WYS = 23;
	const int BTN_STARTSTOP_DOL = 35;
	const int BTN_START_LEWO = 12;
	const int BTN_STOP_LEWO = 93;

	const int PNL_OBRAZ_DOL = 40;
	const int PNL_OBRAZ_PRAWO = 241;

	const int PNL_DANE_SZER = 238;

	const int KARTA_ODSTEP = 3;
	const int KARTA_USTAWIENIA_WYS = 139;
	const int KARTA_POLECENT_WYS = 426;
	const int KARTA_POLECENTPRZEWIN_ORYG_WYS = 442;
	const int KARTA_POLECENTPRZEWIN_WYS = 292;
	const int KARTA_POLEJEDN_WYS = 427;
	const int KARTA_TARCIE_WYS = 378;

	const int KARTA_DANE_LEWO = 0;
	const int KARTA_DANE_SZER = 227;
	const int KARTA_LEWO = 3;
	const int KARTA_SZER = 232;
	const int KARTA_WYS = 22;


	//Dane okna
	HWND wndMain;
	HINSTANCE hInstance;
	int nCmdShow;

	//Kontrolki
	HMENU mnuMenu;
	HWND btnStart;
	HWND btnStop;
	HWND pnlObraz;


	//------------------------Dane symulacji--------------------
	HWND pnlDane;

	HWND btnKartaUstawienia;
	HWND btnKartaPoleCent;
	HWND btnKartaPoleJedn;
	HWND btnKartaTarcie;
	

	//-------------------------------Ustawienia-------------------
	HWND pnlUstawieniaDane;
	HWND Label5;
	HWND txtUstJednostka;
	HWND cboUstJednostka;
	HWND Label3;
	HWND txtUstIloscJednostek;
	HWND Label4;
	HWND txtUstJednPodzialka;
	HWND cbUstOdbiciaSciany;
	

	//------------------------Pole centralne----------------
	HWND pnlPoleCentDane;

	//Zbior cial
	HWND lvCiala;
	HWND pnlPoleCentPrzewin;
	HWND vscPoleCentPrzewin;
	HWND Label11;
	HWND lblCialo;
	HWND btnPoleCentUsun;

	//Wspolrzedne, promien
	HWND Label13;
	HWND txtPoleCentX;
	HWND lblPoleCentXJedn;
	HWND Label14;
	HWND txtPoleCentY;
	HWND lblPoleCentYJedn;
	HWND Label22;
	HWND txtPoleCentZ;
	HWND lblPoleCentZJedn;
	HWND Label41;
	HWND txtPoleCentR;
	HWND lblPoleCentRJedn;

	//Predkosc
	HWND cbPoleCentNieruchome;
	HWND Label35;
	HWND txtPoleCentV0;
	HWND lblPoleCentV0Jedn;
	HWND Label34;
	HWND txtPoleCentV0X;
	HWND lblPoleCentV0XJedn;
	HWND Label33;
	HWND txtPoleCentV0Y;
	HWND lblPoleCentV0YJedn;
	HWND Label29;
	HWND txtPoleCentV0Z;
	HWND lblPoleCentV0ZJedn;

	//Wsp restytucji
	HWND Label43;
	HWND txtPoleCentK;
	HWND Label42;

	//Wybuch
	HWND grpWybuch;
	HWND cbPoleCentWybuch;
	HWND btnPoleCentWybuch;
	HWND Label39;
	HWND txtPoleCentWybuchF;
	HWND cboPoleCentWybuchF;
	HWND Label38;
	HWND txtPoleCentWybuchR;
	HWND lblPoleCentWybuchRJedn;

	//Masa i ladunek
	HWND Label27;
	HWND txtPoleCentMasa;
	HWND cboPoleCentMasa;
	HWND Label26;
	HWND txtPoleCentLadunek;
	HWND cboPoleCentLadunek;

	//Dodaj cialo
	HWND btnPoleCentDodaj;

	//--------------------------------Pole jednorodne-----------
	HWND pnlPoleJednDane;

	//Pole grawitacyjne
	HWND grpPoleJednGraw;
	HWND cbPoleJednGrawGora;
	HWND txtPoleJednGrawGora;
	HWND Label12;
	HWND cbPoleJednGrawDol;
	HWND txtPoleJednGrawDol;
	HWND Label15;
	HWND cbPoleJednGrawLewo;
	HWND txtPoleJednGrawLewo;
	HWND Label16;
	HWND cbPoleJednGrawPrawo;
	HWND txtPoleJednGrawPrawo;
	HWND Label17;
	HWND cbPoleJednGrawPrzod;
	HWND txtPoleJednGrawPrzod;
	HWND Label45;
	HWND cbPoleJednGrawTyl;
	HWND txtPoleJednGrawTyl;
	HWND Label44;
	
	//Pole elektrostatyczne
	HWND grpPoleJednEl;

	HWND cbPoleJednElGora;
	HWND pnlPoleJednElGora;
	HWND Label18;
	HWND rbPoleJednElGoraPlus;
	HWND rbPoleJednElGoraMinus;
	HWND txtPoleJednElGoraE;
	HWND Label20;

	HWND cbPoleJednElLewo;
	HWND pnlPoleJednElLewo;
	HWND Label21;
	HWND rbPoleJednElLewoPlus;
	HWND rbPoleJednElLewoMinus;
	HWND txtPoleJednElLewoE;
	HWND Label23;

	HWND cbPoleJednElPrzod;
	HWND pnlPoleJednElPrzod;
	HWND Label46;
	HWND rbPoleJednElPrzodPlus;
	HWND rbPoleJednElPrzodMinus;
	HWND txtPoleJednElPrzodE;
	HWND Label47;

	//----------------------------------Tarcie------------------
	HWND pnlTarcieDane;
	HWND grpTarcieWsp;
	HWND Label1;
	HWND Label48;

	HWND Label49;
	HWND txtTarcieStGora;
	HWND txtTarcieKinGora;
	HWND Label50;
	HWND txtTarcieStDol;
	HWND txtTarcieKinDol;

	HWND Label51;
	HWND txtTarcieStLewo;
	HWND txtTarcieKinLewo;
	HWND Label54;
	HWND txtTarcieStPrawo;
	HWND txtTarcieKinPrawo;

	HWND Label53;
	HWND txtTarcieStPrzod;
	HWND txtTarcieKinPrzod;
	HWND Label52;
	HWND txtTarcieStTyl;
	HWND txtTarcieKinTyl;

	//Dane okna
	PanelZazn PanelZaznaczony = Nic;
	bool Zdarzenia = true;
	bool Zamknij = false;

	//Stare procedury WndProc
	WNDPROC wpopnlDane;
	WNDPROC wpopnlUstawieniaDane;
	WNDPROC wpogrpPoleJednGraw;
	WNDPROC wpogrpPoleJednEl;
	WNDPROC wpopnlPoleJednEl;
	WNDPROC wpoTextBox;
	WNDPROC wpogrpTarcieWsp;
	WNDPROC wpopnlPoleCentDane;
	WNDPROC wpopnlPoleCentPrzewin;
	WNDPROC wpogrpWybuch;

	//Konstruktor, destruktor
	MainWindow(HINSTANCE hInstance, int nCmdShow);
	~MainWindow();
	void Inicjalizuj();

	//Tworzenie kontrolek
	void UtworzKontrolke(LPCSTR KlasaBazowa, LPCSTR Nazwa, WNDPROC WndProcNowy, WNDPROC &WndProcStary);
	void UtworzNowaKontrolke(LPCSTR Nazwa, WNDPROC WndProc, int DodatkoweBajtyOkno);

	//Funkcje zarzadzania oknem
	bool InitializeComponent();
	void Show(){ ShowWindow(this->wndMain, this->nCmdShow); }

	void UstawPanele();
	void ZwinPanele();
	void UstawZaznaczenieKarty(HWND karta, bool zaznaczona);
	bool SprawdzZaznaczenieKarty(HWND karta);

	void btnKartaUstawienia_Click();
	void btnKartaPoleCent_Click();
	void btnKartaPoleJedn_Click();
	void btnKartaTarcie_Click();
	void ZmienRozmiar(int szer, int wys);
	void UstawKontrolki();
	void UstawKontrolkiPoleCent(int ix);
	void UtworzJednostki(string nazwa, HWND ComboBox);
	void EnablePoleCent(bool enable);
	void EnablePoleCentPredkosc(bool enable);
	void EnablePoleCentWybuch(bool enable);
	void EnablePoleJednElGora(bool enable);
	void EnablePoleJednElLewo(bool enable);
	void EnablePoleJednElPrzod(bool enable);
	void EnableAnimacja(bool enable);
	void CzyscPoleCent();
	void UstawJednostkeSkali(int Jednostka);
	void PokazPrzyciskPauza(bool pokaz);
	void ZmienUstawieniaGrafiki();
	bool ZapytajIZapiszPlik();

	//Zarzadzanie polem centralnym
	int DodajCialo(strCialo* cialo);
	int DodajCialo(double x, double y, double z, double r);
	void ZaznaczCialo(int id);
	void UsunCialo(int IdCiala);
	int PobierzZaznId() { return this->ZaznId; }
	void ObliczPredkosc();
	int ZnajdzCialoListView(int id);
	void ZmienTekstListView(int podelement, double liczba, string jednostka);

};

#endif