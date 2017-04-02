#include "..\inc\main.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

//Zmienne globalne
MainWindow* Okno = nullptr;
DaneSymulacji* Dane = nullptr;
SprawdzanieBledow* Bledy = nullptr;
Animacja3D* Animacja = nullptr;
Fizyka* SilnikFizyczny = nullptr;
strStanProgramu* StanProgramu = nullptr;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow){

	//Utworzenie okna programu
	MainWindow GlowneOkno(hInstance, nCmdShow);
	
	strStanProgramu StanProg;
	StanProgramu = &StanProg;

	//Silnik graficzny
	Animacja3D AnimacjaObraz;
	Animacja = &AnimacjaObraz;

	Okno = &GlowneOkno;
	GlowneOkno.Inicjalizuj();

	if (GlowneOkno.wndMain == NULL) {
		MessageBox(NULL, "Nie uda³o siê utworzyæ okna programu.", "B³¹d", 0);
		return 0;
	}	

	DaneSymulacji DaneCial;
	DaneCial.PoleCent.inicjalizuj([](int ile) { Animacja->DaneGr.UstawIlosc(ile); }, GlowneOkno.lvCiala);
	Dane = &DaneCial;

	SprawdzanieBledow SprBledow(GlowneOkno.wndMain);
	Bledy = &SprBledow;
	
	AnimacjaObraz.Inicjuj(GlowneOkno.pnlObraz, hInstance, PrzetworzKlawisz);

	CzyscDane();

	//Otworz plik
	if (CzyOtworzPlikiCmd(pCmdLine)) {

		OtworzPlik();

	} else {

		GlowneOkno.UstawKontrolki();
		GlowneOkno.ZaznaczCialo(-1);
	}
	
	UstawieniaSceny ust;
	ust.JednIlosc = DaneCial.Ustawienia.JednIlosc;
	ust.JednNazwa = JEDNOSTKI[DaneCial.Ustawienia.JednostkaJedn].Przedrostek + "m";
	ust.Jednostka0 = DaneCial.Ustawienia.Jednostka0;
	ust.UstJednPodzialka = DaneCial.Ustawienia.UstJednPodzialka;
	AnimacjaObraz.UstawUstawienia(ust);

	//Silnik fizyczny
	Fizyka SilnikFiz;
	SilnikFizyczny = &SilnikFiz;
	SilnikFiz.Inicjuj();

	//Wyswietlenie okna
	GlowneOkno.Show();
	UpdateWindow(GlowneOkno.wndMain);

	//Petla komunikatow
	MSG msg = {};
	int ret;

	while ((ret = GetMessage(&msg, NULL, 0, 0)) != 0) {
		if (ret == -1) {
			return 0;
		}
		else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
	}
	
	return 0;
}