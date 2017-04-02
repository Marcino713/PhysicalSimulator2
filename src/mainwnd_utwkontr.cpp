#include "..\inc\mainwnd.h"

void MainWindow::UtworzKontrolke(LPCSTR KlasaBazowa, LPCSTR Nazwa, WNDPROC WndProcNowy, WNDPROC &WndProcStary) {
	WNDCLASS wc;
	GetClassInfo(hInstance, KlasaBazowa, &wc);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = Nazwa;
	wc.hInstance = this->hInstance;
	WndProcStary = wc.lpfnWndProc;
	wc.lpfnWndProc = WndProcNowy;

	RegisterClass(&wc);
}

void MainWindow::UtworzNowaKontrolke(LPCSTR Nazwa, WNDPROC WndProc, int DodatkoweBajtyOkno) {
	WNDCLASS wc = {};
	wc.style = 0;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = Nazwa;
	wc.hInstance = this->hInstance;
	wc.lpfnWndProc = WndProc;
	wc.cbWndExtra = DodatkoweBajtyOkno;
	wc.cbClsExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);

	RegisterClass(&wc);
}