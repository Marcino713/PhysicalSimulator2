#include "..\inc\mainwnd.h"

void MainWindow::UtworzObiektyGdi() {
	this->brSzary = CreateSolidBrush((COLORREF)this->clrSzary);
	this->curStrzalka = LoadCursor(NULL, IDC_ARROW);
	this->curTekst = LoadCursor(NULL, IDC_IBEAM);

	//Czcionka
	this->fntCzcionka = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	//Wczytanie obrazow
	this->imgZapisz = LoadBitmap(this->hInstance, MAKEINTRESOURCE(IDB_ZAPISZ));
	this->imgOtworz = LoadBitmap(this->hInstance, MAKEINTRESOURCE(IDB_OTWORZ));
	this->imgNowy = LoadBitmap(this->hInstance, MAKEINTRESOURCE(IDB_NOWY));
	this->imgPlus = LoadBitmap(this->hInstance, MAKEINTRESOURCE(IDB_PLUS));
	this->imgMinus = LoadBitmap(this->hInstance, MAKEINTRESOURCE(IDB_MINUS));
	this->icIkona = LoadIcon(this->hInstance, MAKEINTRESOURCE(IDI_ICON2));
	
}

void MainWindow::UsunObiektyGdi() {
	DeleteObject(this->imgZapisz);
	DeleteObject(this->imgOtworz);
	DeleteObject(this->imgNowy);
	DeleteObject(this->imgPlus);
	DeleteObject(this->imgMinus);
	DeleteObject(this->icIkona);
	
	DeleteBrush(this->brSzary);
}

MainWindow::MainWindow(HINSTANCE hInstance, int nCmdShow) {
	this->hInstance = hInstance;
	this->nCmdShow = nCmdShow;
	this->ZaznId = -1;
}

MainWindow::~MainWindow() {
	this->UsunObiektyGdi();
}

void MainWindow::Inicjalizuj(){
	this->UtworzObiektyGdi();
	if (!this->InitializeComponent()) return;
	this->UstawPanele();

	//Wyswietlenie jednostek
	this->UtworzJednostki("m", this->cboUstJednostka);
	this->UtworzJednostki("N", this->cboPoleCentWybuchF);
	this->UtworzJednostki("g", this->cboPoleCentMasa);
	this->UtworzJednostki("C", this->cboPoleCentLadunek);
}