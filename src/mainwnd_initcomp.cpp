#include "..\inc\mainwnd.h"
#include "..\inc\wndproc.h"
#pragma comment(lib, "comctl32.lib")

//Tworzy glowne okno programu
bool MainWindow::InitializeComponent() {

	//Tworzy i rejestruje klase okna
	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = this->hInstance;
	wc.lpszClassName = this->NAZWA_KLASY;
	wc.hIcon = this->icIkona;

	RegisterClass(&wc);

	//Menu
	this->mnuMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));

	SetMenuItemBitmaps(this->mnuMenu, ID_PLIK_ZAPISZ, MF_BYCOMMAND, this->imgZapisz, this->imgZapisz);
	SetMenuItemBitmaps(this->mnuMenu, ID_PLIK_OTWORZ, MF_BYCOMMAND, this->imgOtworz, this->imgOtworz);
	SetMenuItemBitmaps(this->mnuMenu, ID_PLIK_NOWY, MF_BYCOMMAND, this->imgNowy, this->imgNowy);


	//Utworzenie okna
	this->wndMain = CreateWindowEx(
		0,                              // Optional window styles.
		this->NAZWA_KLASY,              // Window class
		this->TYTUL_OKNA,				// Window text
		WS_OVERLAPPEDWINDOW,            // Window style
										// Position and size
		CW_USEDEFAULT, CW_USEDEFAULT, this->WNDMAIN_SZER, this->WNDMAIN_WYS,
		NULL,							// Parent window    
		this->mnuMenu,					// Menu
		this->hInstance,				// Instance handle
		NULL							// Additional application data
	);

	if (!this->wndMain) return false;

	//Rozmiar obszaru rysowania
	RECT rc;
	GetClientRect(this->wndMain, &rc);

	//Inicjalizacja kontrolek
	InitCommonControls();
	this->UtworzKontrolke(this->POLETEKSTOWE, this->NTEXTBOX, wpTextBox, this->wpoTextBox);

	//Start, stop
	this->btnStart = CreateWindowEx(0, this->PRZYCISK, this->TBTNSTART, WS_CHILD | WS_VISIBLE, this->BTN_START_LEWO, rc.bottom - this->BTN_STARTSTOP_DOL, this->BTN_STARTSTOP_SZER, this->BTN_STARTSTOP_WYS, this->wndMain, NULL, this->hInstance, NULL);
	SendMessage(btnStart, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->btnStop = CreateWindowEx(0, this->PRZYCISK, this->TBTNSTOP, WS_CHILD | WS_VISIBLE, this->BTN_STOP_LEWO, rc.bottom - this->BTN_STARTSTOP_DOL, this->BTN_STARTSTOP_SZER, this->BTN_STARTSTOP_WYS, this->wndMain, NULL, this->hInstance, NULL);
	SendMessage(btnStop, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	EnableWindow(this->btnStop, false);

	this->UtworzNowaKontrolke(this->NPNLOBRAZ, wppnlObraz, 0);
	this->pnlObraz = CreateWindowEx(0, this->NPNLOBRAZ, NULL, WS_CHILD | WS_VISIBLE, 0, 0, rc.right - this->PNL_OBRAZ_PRAWO, rc.bottom - this->PNL_OBRAZ_DOL, this->wndMain, NULL, this->hInstance, NULL);
	


	//---------------------------Dane symulacji------------------------------
	this->UtworzKontrolke(this->STATYCZNA, this->NPNLDANE, wppnlDane, this->wpopnlDane);
	this->pnlDane = CreateWindowEx(0, this->NPNLDANE, NULL, WS_CHILD | WS_VISIBLE, rc.right-this->PNL_DANE_SZER, 0, this->PNL_DANE_SZER, rc.bottom, this->wndMain, NULL, this->hInstance, NULL);

	this->UtworzNowaKontrolke(this->NBTNKARTA, wpbtnKarta, 8);
	this->btnKartaUstawienia = CreateWindowEx(0, this->NBTNKARTA, this->TBTNKARTAUSTAWIENIA, WS_CHILD | WS_VISIBLE, this->KARTA_LEWO, 3, this->KARTA_SZER, this->KARTA_WYS, this->pnlDane, NULL, this->hInstance, NULL);
	this->btnKartaPoleCent = CreateWindowEx(0, this->NBTNKARTA, this->TBTNKARTAPOLECENT, WS_CHILD | WS_VISIBLE, this->KARTA_LEWO, 31, this->KARTA_SZER, this->KARTA_WYS, this->pnlDane, NULL, this->hInstance, NULL);
	this->btnKartaPoleJedn = CreateWindowEx(0, this->NBTNKARTA, this->TBTNKARTAPOLEJEDN, WS_CHILD | WS_VISIBLE, this->KARTA_LEWO, 59, this->KARTA_SZER, this->KARTA_WYS, this->pnlDane, NULL, this->hInstance, NULL);
	this->btnKartaTarcie = CreateWindowEx(0, this->NBTNKARTA, this->TBTNKARTATARCIE, WS_CHILD | WS_VISIBLE, this->KARTA_LEWO, 87, this->KARTA_SZER, this->KARTA_WYS, this->pnlDane, NULL, this->hInstance, NULL);


	//---------------------------------Ustawienia---------------------------------
	this->UtworzKontrolke(this->STATYCZNA, this->NPNLUSTAWIENIADANE, wppnlUstawieniaDane, this->wpopnlUstawieniaDane);

	this->pnlUstawieniaDane = CreateWindowEx(0, this->NPNLUSTAWIENIADANE, NULL, WS_CHILD | WS_VISIBLE, 42, 159, this->KARTA_DANE_SZER, this->KARTA_USTAWIENIA_WYS, this->pnlDane, NULL, this->hInstance, NULL);

	this->Label5 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 3, 6, 74, 13, this->pnlUstawieniaDane, NULL, this->hInstance, NULL);
	SendMessage(this->Label5, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label5, this->TLABEL5);

	this->txtUstJednostka = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 90, 6, 74, 20, this->pnlUstawieniaDane, NULL, this->hInstance, NULL);
	SendMessage(this->txtUstJednostka, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->cboUstJednostka = CreateWindowEx(WS_EX_CLIENTEDGE, this->POLEKOMBI, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | CBS_DROPDOWNLIST, 170, 3, 52, 200, this->pnlUstawieniaDane, NULL, this->hInstance, NULL);
	SendMessage(this->cboUstJednostka, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->Label3 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 3, 32, 81, 13, this->pnlUstawieniaDane, NULL, this->hInstance, NULL);
	SendMessage(this->Label3, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label3, this->TLABEL3);

	this->txtUstIloscJednostek = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 90, 32, 74, 20, this->pnlUstawieniaDane, NULL, this->hInstance, NULL);
	SendMessage(this->txtUstIloscJednostek, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->Label4 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 3, 55, 164, 13, this->pnlUstawieniaDane, NULL, this->hInstance, NULL);
	SendMessage(this->Label4, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label4, this->TLABEL4);

	this->txtUstJednPodzialka = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 90, 71, 74, 20, this->pnlUstawieniaDane, NULL, this->hInstance, NULL);
	SendMessage(this->txtUstJednPodzialka, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->cbUstOdbiciaSciany = CreateWindowEx(0, this->PRZYCISK, this->TCBUSTODBICIASCIANY, WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 6, 94, 105, 17, this->pnlUstawieniaDane, NULL, this->hInstance, NULL);
	SendMessage(this->cbUstOdbiciaSciany, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);


	//-------------------------------Pole centralne------------------------
	this->UtworzKontrolke(this->STATYCZNA, this->NPNLPOLECENTDANE, wppnlPoleCentDane, this->wpopnlPoleCentDane);
	this->pnlPoleCentDane = CreateWindowEx(0, this->NPNLPOLECENTDANE, NULL, WS_CHILD | WS_VISIBLE, 470, 4, this->KARTA_DANE_SZER, this->KARTA_POLECENT_WYS, this->pnlDane, NULL, this->hInstance, NULL);

	this->UtworzKontrolke(this->STATYCZNA, this->NPNLPOLECENTPRZEWIN, wppnlPoleCentPrzewin, this->wpopnlPoleCentPrzewin);
	this->pnlPoleCentPrzewin = CreateWindowEx(0, this->NPNLPOLECENTPRZEWIN, NULL, WS_CHILD | WS_VISIBLE, 0, 99, 205, 292, this->pnlPoleCentDane, NULL, this->hInstance, NULL);

	this->vscPoleCentPrzewin = CreateWindowEx(0, this->PASEKPRZEWIJANIA, NULL, WS_CHILD | WS_VISIBLE | SBS_VERT, 206, 99, 17, 292, this->pnlPoleCentDane, NULL, this->hInstance, NULL);

	SCROLLINFO si;
	ZeroMemory(&si, sizeof(si));

	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin = 0;
	si.nMax = KARTA_POLECENTPRZEWIN_ORYG_WYS;
	si.nPage = KARTA_POLECENTPRZEWIN_WYS;
	si.nPos = 0;

	SetScrollInfo(this->vscPoleCentPrzewin, SB_CTL, &si, true);

	//Lista cial
	this->lvCiala = CreateWindowEx(0, WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS, 0, 3, 217, 93, this->pnlPoleCentDane, NULL, this->hInstance, NULL);
	ListView_SetExtendedListViewStyle(this->lvCiala, LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);

	{
		LVCOLUMN lvc;
		lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

		lvc.iSubItem = 0;
		lvc.cx = 40;
		lvc.pszText = this->TLVCCIALO;
		ListView_InsertColumn(this->lvCiala, 0, &lvc);

		lvc.iSubItem = 1;
		lvc.cx = 88;
		lvc.pszText = this->TLVCMASA;
		ListView_InsertColumn(this->lvCiala, 1, &lvc);

		lvc.iSubItem = 2;
		lvc.cx = 89;
		lvc.pszText = this->TLVCLADUNEK;
		ListView_InsertColumn(this->lvCiala, 2, &lvc);
	}

	//Nr ciala i usuwanie
	this->Label11 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 3, 6, 35, 13, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->Label11, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label11, this->TLABEL11);

	this->lblCialo = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 44, 6, 60, 13, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->lblCialo, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->btnPoleCentUsun = CreateWindowEx(0, this->PRZYCISK, this->TBTNPOLECENTUSUN, WS_CHILD | WS_VISIBLE, 128, 1, 75, 23, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->btnPoleCentUsun, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	//X
	this->Label13 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 3, 27, 17, 13, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->Label13, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label13, this->TLBLX);

	this->txtPoleCentX = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 47, 24, 100, 20, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->txtPoleCentX, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->lblPoleCentXJedn = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 153, 27, 45, 13, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->lblPoleCentXJedn, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	//Y
	this->Label14 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 3, 53, 17, 13, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->Label14, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label14, this->TLBLY);

	this->txtPoleCentY = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 47, 50, 100, 20, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->txtPoleCentY, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->lblPoleCentYJedn = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 153, 53, 45, 13, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->lblPoleCentYJedn, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	//Z
	this->Label22 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 3, 79, 17, 13, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->Label22, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label22, this->TLBLZ);

	this->txtPoleCentZ = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 47, 76, 100, 20, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->txtPoleCentZ, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->lblPoleCentZJedn = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 153, 79, 45, 13, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->lblPoleCentZJedn, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	//R
	this->Label41 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 3, 105, 18, 13, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->Label41, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label41, this->TLBLR);

	this->txtPoleCentR = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 47, 102, 100, 20, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->txtPoleCentR, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->lblPoleCentRJedn = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 153, 105, 45, 13, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->lblPoleCentRJedn, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	//Nieruchome
	this->cbPoleCentNieruchome = CreateWindowEx(0, this->PRZYCISK, this->TCBPOLECENTNIERUCHOME, WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 47, 128, 83, 17, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->cbPoleCentNieruchome, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	//V0
	this->Label35 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 3, 154, 22, 13, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->Label35, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label35, this->TLBLV0);
	EnableWindow(this->Label35, false);

	this->txtPoleCentV0 = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 47, 151, 100, 20, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->txtPoleCentV0, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	EnableWindow(this->txtPoleCentV0, false);

	this->lblPoleCentV0Jedn = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 153, 154, 45, 13, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->lblPoleCentV0Jedn, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	EnableWindow(this->lblPoleCentV0Jedn, false);

	//V0X
	this->Label34 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 3, 180, 27, 13, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->Label34, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label34, this->TLBLV0X);

	this->txtPoleCentV0X = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 47, 177, 100, 20, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->txtPoleCentV0X, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->lblPoleCentV0XJedn = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 153, 180, 45, 13, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->lblPoleCentV0XJedn, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	//V0Y
	this->Label33 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 3, 206, 27, 13, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->Label33, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label33, this->TLBLV0Y);

	this->txtPoleCentV0Y = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 47, 203, 100, 20, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->txtPoleCentV0Y, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->lblPoleCentV0YJedn = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 153, 206, 45, 13, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->lblPoleCentV0YJedn, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	//V0Z
	this->Label29 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 3, 232, 27, 13, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->Label29, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label29, this->TLBLV0Z);

	this->txtPoleCentV0Z = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 47, 229, 100, 20, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->txtPoleCentV0Z, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->lblPoleCentV0ZJedn = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 153, 232, 45, 13, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->lblPoleCentV0ZJedn, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	//Wspolczynnik restytucji
	this->Label43 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 3, 252, 138, 13, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->Label43, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label43, this->TLABEL43);

	this->txtPoleCentK = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 47, 268, 100, 20, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->txtPoleCentK, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->Label42 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 153, 271, 45, 13, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->Label42, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label42, this->TLBLRESTWSPJEDN);

	//Wybuch
	this->UtworzKontrolke(this->PRZYCISK, this->NGRPWYBUCH, wpgrpWybuch, this->wpogrpWybuch);
	this->grpWybuch = CreateWindowEx(0, this->NGRPWYBUCH, this->TGRPWYBUCH, WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 3, 294, 200, 93, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->grpWybuch, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->cbPoleCentWybuch = CreateWindowEx(0, this->PRZYCISK, this->TCBPOLECENTWYBUCH, WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 6, 19, 97, 17, this->grpWybuch, NULL, this->hInstance, NULL);
	SendMessage(this->cbPoleCentWybuch, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->btnPoleCentWybuch = CreateWindowEx(0, this->PRZYCISK, this->TBTNPOLECENTWYBUCH, WS_CHILD | WS_VISIBLE, 110, 13, 85, 25, this->grpWybuch, NULL, this->hInstance, NULL);
	SendMessage(this->btnPoleCentWybuch, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	//Sila
	this->Label39 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 3, 45, 16, 13, this->grpWybuch, NULL, this->hInstance, NULL);
	SendMessage(this->Label39, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label39, this->TLBLF);

	this->txtPoleCentWybuchF = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 44, 42, 100, 20, this->grpWybuch, NULL, this->hInstance, NULL);
	SendMessage(this->txtPoleCentWybuchF, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->cboPoleCentWybuchF = CreateWindowEx(WS_EX_CLIENTEDGE, this->POLEKOMBI, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | CBS_DROPDOWNLIST, 153, 41, 45, 200, this->grpWybuch, NULL, this->hInstance, NULL);
	SendMessage(this->cboPoleCentWybuchF, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	//Promien
	this->Label38 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 3, 71, 18, 13, this->grpWybuch, NULL, this->hInstance, NULL);
	SendMessage(this->Label38, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label38, this->TLBLR);

	this->txtPoleCentWybuchR = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 44, 68, 100, 20, this->grpWybuch, NULL, this->hInstance, NULL);
	SendMessage(this->txtPoleCentWybuchR, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->lblPoleCentWybuchRJedn = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 150, 71, 45, 13, this->grpWybuch, NULL, this->hInstance, NULL);
	SendMessage(this->lblPoleCentWybuchRJedn, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	//Masa i ladunek
	this->Label27 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 3, 396, 19, 13, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->Label27, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label27, this->TLBLM);

	this->txtPoleCentMasa = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 47, 393, 100, 20, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->txtPoleCentMasa, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->cboPoleCentMasa = CreateWindowEx(WS_EX_CLIENTEDGE, this->POLEKOMBI, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | CBS_DROPDOWNLIST, 156, 392, 45, 200, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->cboPoleCentMasa, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->Label26 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 3, 422, 18, 13, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->Label26, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label26, this->TLBLQ);

	this->txtPoleCentLadunek = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 47, 419, 100, 20, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->txtPoleCentLadunek, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->cboPoleCentLadunek = CreateWindowEx(WS_EX_CLIENTEDGE, this->POLEKOMBI, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | CBS_DROPDOWNLIST, 156, 418, 45, 200, this->pnlPoleCentPrzewin, NULL, this->hInstance, NULL);
	SendMessage(this->cboPoleCentLadunek, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	//Dodaj cialo
	this->btnPoleCentDodaj = CreateWindowEx(0, this->PRZYCISK, NULL, WS_CHILD | WS_VISIBLE, 2, 394, 221, 26, this->pnlPoleCentDane, NULL, this->hInstance, NULL);
	SendMessage(this->btnPoleCentDodaj, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->btnPoleCentDodaj, this->TBTNPOLECENTDODAJ);

	//--------------------------------------------Pole jednorodne grawitacyjne-------------------
	this->pnlPoleJednDane = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 235, 7, this->KARTA_DANE_SZER, this->KARTA_POLEJEDN_WYS, this->pnlDane, NULL, this->hInstance, NULL);

	this->UtworzKontrolke(this->PRZYCISK, this->NGRPPOLEJEDNGRAW, wpgrpPoleJednGraw, this->wpogrpPoleJednGraw);
	this->grpPoleJednGraw = CreateWindowEx(0, this->NGRPPOLEJEDNGRAW, this->TGRPPOLEJEDNGRAW, WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 4, 3, 213, 176, this->pnlPoleJednDane, NULL, this->hInstance, NULL);
	SendMessage(this->grpPoleJednGraw, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	//Gora
	this->cbPoleJednGrawGora = CreateWindowEx(0, this->PRZYCISK, this->TLBLGORA, WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 6, 21, 49, 17, this->grpPoleJednGraw, NULL, this->hInstance, NULL);
	SendMessage(this->cbPoleJednGrawGora, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->txtPoleJednGrawGora = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 68, 19, 100, 20, this->grpPoleJednGraw, NULL, this->hInstance, NULL);
	SendMessage(this->txtPoleJednGrawGora, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->Label12 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 174, 22, 28, 13, this->grpPoleJednGraw, NULL, this->hInstance, NULL);
	SendMessage(this->Label12, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label12, this->TLBLPOLEJEDNGRAWJEDN);

	//Dol
	this->cbPoleJednGrawDol = CreateWindowEx(0, this->PRZYCISK, this->TLBLDOL, WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 6, 47, 44, 17, this->grpPoleJednGraw, NULL, this->hInstance, NULL);
	SendMessage(this->cbPoleJednGrawDol, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->txtPoleJednGrawDol = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 68, 45, 100, 20, this->grpPoleJednGraw, NULL, this->hInstance, NULL);
	SendMessage(this->txtPoleJednGrawDol, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->Label15 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 174, 48, 28, 13, this->grpPoleJednGraw, NULL, this->hInstance, NULL);
	SendMessage(this->Label15, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label15, this->TLBLPOLEJEDNGRAWJEDN);

	//Lewo
	this->cbPoleJednGrawLewo = CreateWindowEx(0, this->PRZYCISK, this->TLBLLEWO, WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 6, 73, 52, 17, this->grpPoleJednGraw, NULL, this->hInstance, NULL);
	SendMessage(this->cbPoleJednGrawLewo, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->txtPoleJednGrawLewo = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 68, 71, 100, 20, this->grpPoleJednGraw, NULL, this->hInstance, NULL);
	SendMessage(this->txtPoleJednGrawLewo, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->Label16 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 174, 74, 28, 13, this->grpPoleJednGraw, NULL, this->hInstance, NULL);
	SendMessage(this->Label16, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label16, this->TLBLPOLEJEDNGRAWJEDN);

	//Prawo
	this->cbPoleJednGrawPrawo = CreateWindowEx(0, this->PRZYCISK, this->TLBLPRAWO, WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 6, 99, 56, 17, this->grpPoleJednGraw, NULL, this->hInstance, NULL);
	SendMessage(this->cbPoleJednGrawPrawo, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->txtPoleJednGrawPrawo = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 68, 97, 100, 20, this->grpPoleJednGraw, NULL, this->hInstance, NULL);
	SendMessage(this->txtPoleJednGrawPrawo, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->Label17 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 174, 100, 28, 13, this->grpPoleJednGraw, NULL, this->hInstance, NULL);
	SendMessage(this->Label17, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label17, this->TLBLPOLEJEDNGRAWJEDN);

	//Przod
	this->cbPoleJednGrawPrzod = CreateWindowEx(0, this->PRZYCISK, this->TLBLPRZOD, WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 6, 125, 53, 17, this->grpPoleJednGraw, NULL, this->hInstance, NULL);
	SendMessage(this->cbPoleJednGrawPrzod, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->txtPoleJednGrawPrzod = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 68, 123, 100, 20, this->grpPoleJednGraw, NULL, this->hInstance, NULL);
	SendMessage(this->txtPoleJednGrawPrzod, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->Label45 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 174, 126, 28, 13, this->grpPoleJednGraw, NULL, this->hInstance, NULL);
	SendMessage(this->Label45, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label45, this->TLBLPOLEJEDNGRAWJEDN);

	//Tyl
	this->cbPoleJednGrawTyl = CreateWindowEx(0, this->PRZYCISK, this->TLBLTYL, WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 6, 150, 42, 17, this->grpPoleJednGraw, NULL, this->hInstance, NULL);
	SendMessage(this->cbPoleJednGrawTyl, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->txtPoleJednGrawTyl = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 68, 148, 100, 20, this->grpPoleJednGraw, NULL, this->hInstance, NULL);
	SendMessage(this->txtPoleJednGrawTyl, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->Label44 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 174, 151, 28, 13, this->grpPoleJednGraw, NULL, this->hInstance, NULL);
	SendMessage(this->Label44, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label44, this->TLBLPOLEJEDNGRAWJEDN);


	//---------------------------------Pole jednorodne elektrostatyczne--------------------
	this->UtworzKontrolke(this->STATYCZNA, this->NPNLPOLEJEDNEL, wppnlPoleJednEl, this->wpopnlPoleJednEl);

	this->UtworzKontrolke(this->PRZYCISK, this->NGRPPOLEJEDNEL, wpgrpPoleJednEl, this->wpogrpPoleJednEl);
	this->grpPoleJednEl = CreateWindowEx(0, this->NGRPPOLEJEDNEL, this->TGRPPOLEJEDNEL, WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 4, 185, 213, 238, this->pnlPoleJednDane, NULL, this->hInstance, NULL);
	SendMessage(this->grpPoleJednEl, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	//Gora/dol
	this->cbPoleJednElGora = CreateWindowEx(0, this->PRZYCISK, this->TCBPOLEJEDNELGORA, WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 6, 19, 70, 17, this->grpPoleJednEl, NULL, this->hInstance, NULL);
	SendMessage(this->cbPoleJednElGora, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->pnlPoleJednElGora = CreateWindowEx(0, this->NPNLPOLEJEDNEL, NULL, WS_CHILD | WS_VISIBLE, 6, 42, 142, 44, this->grpPoleJednEl, NULL, this->hInstance, NULL);

	this->Label18 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 3, 0, 33, 13, this->pnlPoleJednElGora, NULL, this->hInstance, NULL);
	SendMessage(this->Label18, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label18, this->TLBLGORA);

	this->rbPoleJednElGoraPlus = CreateWindowEx(0, this->PRZYCISK, this->TLBLPLUS, WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 45, -2, 31, 17, this->pnlPoleJednElGora, NULL, this->hInstance, NULL);
	SendMessage(this->rbPoleJednElGoraPlus, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->rbPoleJednElGoraMinus = CreateWindowEx(0, this->PRZYCISK, this->TLBLMINUS, WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 82, -2, 28, 17, this->pnlPoleJednElGora, NULL, this->hInstance, NULL);
	SendMessage(this->rbPoleJednElGoraMinus, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->txtPoleJednElGoraE = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 6, 21, 100, 20, this->pnlPoleJednElGora, NULL, this->hInstance, NULL);
	SendMessage(this->txtPoleJednElGoraE, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->Label20 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 112, 24, 27, 13, this->pnlPoleJednElGora, NULL, this->hInstance, NULL);
	SendMessage(this->Label20, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label20, this->TLBLPOLEJEDNELJEDN);

	//Lewo/prawo
	this->cbPoleJednElLewo = CreateWindowEx(0, this->PRZYCISK, this->TCBPOLEJEDNELLEWO, WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 6, 89, 86, 17, this->grpPoleJednEl, NULL, this->hInstance, NULL);
	SendMessage(this->cbPoleJednElLewo, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->pnlPoleJednElLewo = CreateWindowEx(0, this->NPNLPOLEJEDNEL, NULL, WS_CHILD | WS_VISIBLE, 6, 109, 142, 44, this->grpPoleJednEl, NULL, this->hInstance, NULL);

	this->Label21 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 3, 0, 33, 13, this->pnlPoleJednElLewo, NULL, this->hInstance, NULL);
	SendMessage(this->Label21, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label21, this->TLBLLEWO);

	this->rbPoleJednElLewoPlus = CreateWindowEx(0, this->PRZYCISK, this->TLBLPLUS, WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 45, -2, 31, 17, this->pnlPoleJednElLewo, NULL, this->hInstance, NULL);
	SendMessage(this->rbPoleJednElLewoPlus, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->rbPoleJednElLewoMinus = CreateWindowEx(0, this->PRZYCISK, this->TLBLMINUS, WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 82, -2, 28, 17, this->pnlPoleJednElLewo, NULL, this->hInstance, NULL);
	SendMessage(this->rbPoleJednElLewoMinus, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->txtPoleJednElLewoE = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 6, 21, 100, 20, this->pnlPoleJednElLewo, NULL, this->hInstance, NULL);
	SendMessage(this->txtPoleJednElLewoE, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->Label23 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 112, 24, 27, 13, this->pnlPoleJednElLewo, NULL, this->hInstance, NULL);
	SendMessage(this->Label23, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label23, this->TLBLPOLEJEDNELJEDN);

	//Przod/tyl
	this->cbPoleJednElPrzod = CreateWindowEx(0, this->PRZYCISK, this->TCBPOLEJEDNELPRZOD, WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 6, 163, 70, 17, this->grpPoleJednEl, NULL, this->hInstance, NULL);
	SendMessage(this->cbPoleJednElPrzod, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->pnlPoleJednElPrzod = CreateWindowEx(0, this->NPNLPOLEJEDNEL, NULL, WS_CHILD | WS_VISIBLE, 6, 183, 142, 44, this->grpPoleJednEl, NULL, this->hInstance, NULL);

	this->Label46 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 3, 0, 33, 13, this->pnlPoleJednElPrzod, NULL, this->hInstance, NULL);
	SendMessage(this->Label46, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label46, this->TLBLPRZOD);

	this->rbPoleJednElPrzodPlus = CreateWindowEx(0, this->PRZYCISK, this->TLBLPLUS, WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 45, -2, 31, 17, this->pnlPoleJednElPrzod, NULL, this->hInstance, NULL);
	SendMessage(this->rbPoleJednElPrzodPlus, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->rbPoleJednElPrzodMinus = CreateWindowEx(0, this->PRZYCISK, this->TLBLMINUS, WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 82, -2, 28, 17, this->pnlPoleJednElPrzod, NULL, this->hInstance, NULL);
	SendMessage(this->rbPoleJednElPrzodMinus, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->txtPoleJednElPrzodE = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 6, 21, 100, 20, this->pnlPoleJednElPrzod, NULL, this->hInstance, NULL);
	SendMessage(this->txtPoleJednElPrzodE, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->Label47 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 112, 24, 27, 13, this->pnlPoleJednElPrzod, NULL, this->hInstance, NULL);
	SendMessage(this->Label47, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label47, this->TLBLPOLEJEDNELJEDN);


	//------------------------------Tarcie------------------------------
	this->pnlTarcieDane = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 12, 7, this->KARTA_DANE_SZER, this->KARTA_TARCIE_WYS, this->pnlDane, NULL, this->hInstance, NULL);

	this->UtworzKontrolke(this->PRZYCISK, this->NGRPTARCIEWSP, wpgrpTarcieWsp, this->wpogrpTarcieWsp);
	this->grpTarcieWsp = CreateWindowEx(0, this->NGRPTARCIEWSP, this->TGRPTARCIEWSP, WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 4, 3, 213, 192, this->pnlTarcieDane, NULL, this->hInstance, NULL);
	SendMessage(this->grpTarcieWsp, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->Label1 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 46, 18, 54, 13, this->grpTarcieWsp, NULL, this->hInstance, NULL);
	SendMessage(this->Label1, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label1, this->TLABEL1);

	this->Label48 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 124, 18, 59, 13, this->grpTarcieWsp, NULL, this->hInstance, NULL);
	SendMessage(this->Label48, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label48, this->TLABEL48);

	//Gora
	this->Label49 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 6, 37, 30, 13, this->grpTarcieWsp, NULL, this->hInstance, NULL);
	SendMessage(this->Label49, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label49, this->TLBLGORA);

	this->txtTarcieStGora = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 49, 34, 72, 20, this->grpTarcieWsp, NULL, this->hInstance, NULL);
	SendMessage(this->txtTarcieStGora, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->txtTarcieKinGora = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 127, 34, 72, 20, this->grpTarcieWsp, NULL, this->hInstance, NULL);
	SendMessage(this->txtTarcieKinGora, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	//Dol
	this->Label50 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 6, 63, 25, 13, this->grpTarcieWsp, NULL, this->hInstance, NULL);
	SendMessage(this->Label50, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label50, this->TLBLDOL);

	this->txtTarcieStDol = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 49, 60, 72, 20, this->grpTarcieWsp, NULL, this->hInstance, NULL);
	SendMessage(this->txtTarcieStDol, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->txtTarcieKinDol = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 127, 60, 72, 20, this->grpTarcieWsp, NULL, this->hInstance, NULL);
	SendMessage(this->txtTarcieKinDol, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	//Lewo
	this->Label51 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 6, 89, 33, 13, this->grpTarcieWsp, NULL, this->hInstance, NULL);
	SendMessage(this->Label51, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label51, this->TLBLLEWO);

	this->txtTarcieStLewo = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 49, 86, 72, 20, this->grpTarcieWsp, NULL, this->hInstance, NULL);
	SendMessage(this->txtTarcieStLewo, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->txtTarcieKinLewo = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 127, 86, 72, 20, this->grpTarcieWsp, NULL, this->hInstance, NULL);
	SendMessage(this->txtTarcieKinLewo, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	//Prawo
	this->Label54 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 6, 115, 37, 13, this->grpTarcieWsp, NULL, this->hInstance, NULL);
	SendMessage(this->Label54, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label54, this->TLBLPRAWO);

	this->txtTarcieStPrawo = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 49, 112, 72, 20, this->grpTarcieWsp, NULL, this->hInstance, NULL);
	SendMessage(this->txtTarcieStPrawo, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->txtTarcieKinPrawo = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 127, 112, 72, 20, this->grpTarcieWsp, NULL, this->hInstance, NULL);
	SendMessage(this->txtTarcieKinPrawo, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	//Przod
	this->Label53 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 6, 141, 34, 13, this->grpTarcieWsp, NULL, this->hInstance, NULL);
	SendMessage(this->Label53, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label53, this->TLBLPRZOD);

	this->txtTarcieStPrzod = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 49, 138, 72, 20, this->grpTarcieWsp, NULL, this->hInstance, NULL);
	SendMessage(this->txtTarcieStPrzod, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->txtTarcieKinPrzod = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 127, 138, 72, 20, this->grpTarcieWsp, NULL, this->hInstance, NULL);
	SendMessage(this->txtTarcieKinPrzod, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	//Tyl
	this->Label52 = CreateWindowEx(0, this->STATYCZNA, NULL, WS_CHILD | WS_VISIBLE, 6, 167, 23, 13, this->grpTarcieWsp, NULL, this->hInstance, NULL);
	SendMessage(this->Label52, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);
	SetWindowText(this->Label52, this->TLBLTYL);

	this->txtTarcieStTyl = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 49, 164, 72, 20, this->grpTarcieWsp, NULL, this->hInstance, NULL);
	SendMessage(this->txtTarcieStTyl, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	this->txtTarcieKinTyl = CreateWindowEx(WS_EX_CLIENTEDGE, this->NTEXTBOX, NULL, WS_CHILD | WS_VISIBLE, 127, 164, 72, 20, this->grpTarcieWsp, NULL, this->hInstance, NULL);
	SendMessage(this->txtTarcieKinTyl, WM_SETFONT, (WPARAM)this->fntCzcionka, 0);

	return true;
}