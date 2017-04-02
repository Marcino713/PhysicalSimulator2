#include "..\inc\wndproc.h"
#include "..\inc\fizyka.h"
#include "..\inc\bledy.h"
#include <process.h>

extern strStanProgramu* StanProgramu;
extern Fizyka* SilnikFizyczny;
extern MainWindow* Okno;
extern Animacja3D* Animacja;
extern DaneSymulacji* Dane;
extern SprawdzanieBledow* Bledy;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CLOSE: {

		if (StanProgramu->Animacja) {
			SendMessage(Okno->wndMain, WM_COMMAND, NULL, (LPARAM)Okno->btnStop);
			WaitForSingleObject(SilnikFizyczny->evtAnimacja, INFINITE);
		}

		if (!StanProgramu->Zapisany) {
			if (Okno->ZapytajIZapiszPlik()) return 0;
		}

		break;
	}

	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW));

		EndPaint(hwnd, &ps);
		return 0;
	}

	case WM_SIZE: {
		Okno->ZmienRozmiar(LOWORD(lParam), HIWORD(lParam));
		InvalidateRect(Okno->pnlObraz, NULL, true);
		return 0;
	}

	case WM_SETCURSOR: {
		SetCursor(Okno->curStrzalka);
		break;
	}

	case WM_KEYDOWN: {

		if (wParam == VK_F5) {
			Animacja->UstawPelnyEkran(true);
		}

		PrzetworzKlawisz(wParam);

		break;
	}

	case WM_COMMAND: {

		if (LOWORD(wParam) == ID_PLIK_NOWY) {
			if (!StanProgramu->Zapisany) {
				if(Okno->ZapytajIZapiszPlik()) break;
			}
				
			CzyscDane();
			Okno->UstawKontrolki();
			Okno->ZmienUstawieniaGrafiki();
			Animacja->ResetujKamere();
			StanProgramu->Zapisany = true;
			StanProgramu->NazwaPliku = "";
			RysujCiala();
		}

		if (LOWORD(wParam) == ID_PLIK_OTWORZ) {
			if (!StanProgramu->Zapisany) {
				if (Okno->ZapytajIZapiszPlik()) break;
			}

			if (!OtwPlikuPobierzNazwe(hwnd)) {
				break;
			}

			OtworzPlik();
		}

		if (LOWORD(wParam) == ID_PLIK_ZAPISZ) {
			if (!StanProgramu->Zapisany) {
				ZapiszPlik(hwnd);
			}
		}

		if (LOWORD(wParam) == ID_PLIK_ZAPISZJAKO) {
			StanProgramu->Zapisany = false;
			string nazwa = StanProgramu->NazwaPliku;
			StanProgramu->NazwaPliku = "";

			if (!ZapiszPlik(hwnd)) {
				StanProgramu->NazwaPliku = nazwa;
			}
		}

		if (lParam == (LPARAM)Okno->btnStart) {

			if (StanProgramu->Animacja) {

				if (StanProgramu->Pauza) {
					SetEvent(SilnikFizyczny->evtPauza);
					StanProgramu->Pauza = false;
				} else {
					ResetEvent(SilnikFizyczny->evtPauza);
					StanProgramu->Pauza = true;
				}
				

			} else {

				if (Bledy->PokazBledy()) return 0;
				if (Dane->PoleCent.czy_puste()) {
					MessageBox(Okno->wndMain, "Nie dodano ¿adnego cia³a.", "B³¹d", MB_OK);
					return 0;
				}
				
				EnableMenuItem(Okno->mnuMenu, ID_PLIK_NOWY, MF_GRAYED);
				EnableMenuItem(Okno->mnuMenu, ID_PLIK_OTWORZ, MF_GRAYED);
				EnableMenuItem(Okno->mnuMenu, ID_PLIK_ZAPISZ, MF_GRAYED);
				EnableMenuItem(Okno->mnuMenu, ID_PLIK_ZAPISZJAKO, MF_GRAYED);
				Okno->EnableAnimacja(false);
				Okno->PokazPrzyciskPauza(true);
				StanProgramu->Pauza = false;
				StanProgramu->Animacja = true;

				_beginthread(UruchomSymulacje, 4096, 0);

			}

			SetFocus(Okno->wndMain);

		}

		if (lParam == (LPARAM)Okno->btnStop) {
			StanProgramu->Animacja = false;
			StanProgramu->Pauza = false;
			SetEvent(SilnikFizyczny->evtPauza);
			Okno->EnableAnimacja(true);
			Okno->PokazPrzyciskPauza(false);
			EnableMenuItem(Okno->mnuMenu, ID_PLIK_NOWY, MF_ENABLED);
			EnableMenuItem(Okno->mnuMenu, ID_PLIK_OTWORZ, MF_ENABLED);
			EnableMenuItem(Okno->mnuMenu, ID_PLIK_ZAPISZ, MF_ENABLED);
			EnableMenuItem(Okno->mnuMenu, ID_PLIK_ZAPISZJAKO, MF_ENABLED);

			SetFocus(Okno->wndMain);
		}

		return 0;
	}

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK wpTextBox(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {

	case WM_SETCURSOR:
		SetCursor(Okno->curTekst);
		return true;

	}
	return CallWindowProc(Okno->wpoTextBox, hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK wpbtnKarta(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {

	case WM_CREATE: {
		SetWindowLongPtr(hwnd, 0, (LONG)false);
		break;
	}

	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		//Tlo
		FillRect(hdc, &ps.rcPaint, Okno->brSzary);

		//Ikona
		HDC hdcIkona = CreateCompatibleDC(hdc);
		HGDIOBJ hdcStary;

		LONG_PTR a = GetWindowLongPtr(hwnd, 0);
		

		if (GetWindowLongPtr(hwnd, 0) == (LONG)TRUE) {	//Minus
			hdcStary = SelectObject(hdcIkona, Okno->imgMinus);
		}
		else {	//Plus
			hdcStary = SelectObject(hdcIkona, Okno->imgPlus);
		}

		BitBlt(hdc, 7, 7, 9, 9, hdcIkona, 0, 0, SRCCOPY);
		SelectObject(hdcIkona, hdcStary);
		DeleteDC(hdcIkona);

		//Tekst
		RECT p2;
		ZeroMemory(&p2, sizeof(p2));
		p2.left = 22;
		p2.right = 190;
		p2.top = 5;
		p2.bottom = 20;

		HFONT domyslna;
		domyslna = (HFONT)SelectObject(hdc, Okno->fntCzcionka);

		SetBkColor(hdc, (COLORREF)0xE3E3E3);

		char *t1 = new char[30];
		GetWindowText(hwnd, t1, 30);

		if (IsWindowEnabled(hwnd)) {
			SetTextColor(hdc, 0);
		} else {
			SetTextColor(hdc, (COLORREF)0x999999);
		}

		DrawText(hdc, t1, -1, &p2, NULL);

		delete[] t1;
		SelectObject(hdc, domyslna);

		EndPaint(hwnd, &ps);
		return 0;
	}

	case WM_LBUTTONDOWN: {
		SendMessage(GetParent(hwnd), WM_COMMAND, (WPARAM)MAKELONG((WORD)GetWindowLong(hwnd, GWL_ID), BN_CLICKED), (LPARAM)hwnd);
		break;
	}

	case WM_ENABLE: {
		InvalidateRect(hwnd, NULL, true);
		break;
	}

	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK wppnlObraz(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_LBUTTONDOWN: {
		SetFocus(Okno->wndMain);
		return 0;
	}

	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);

		RysujCiala();

		return 0;
	}

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK wppnlDane(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND: {
		if (lParam == (LPARAM)Okno->btnKartaUstawienia) {
			Okno->btnKartaUstawienia_Click();
			return 0;
		}
		else if (lParam == (LPARAM)Okno->btnKartaPoleCent) {
			Okno->btnKartaPoleCent_Click();
			return 0;
		}
		else if (lParam == (LPARAM)Okno->btnKartaPoleJedn) {
			Okno->btnKartaPoleJedn_Click();
			return 0;
		}
		else if (lParam == (LPARAM)Okno->btnKartaTarcie) {
			Okno->btnKartaTarcie_Click();
			return 0;
		}
		break;
	}
	}

	return CallWindowProc(Okno->wpopnlDane, hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK wppnlUstawieniaDane(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND:

		if (!Okno->Zdarzenia) break;

		if (lParam == (LPARAM)Okno->txtUstJednostka && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			if (Bledy->PowyzejZeraNP(Okno->txtUstJednostka, "D³ugoœæ jednostki", &Dane->Ustawienia.Jednostka0)) {
				Okno->ZmienUstawieniaGrafiki();
			}
		}

		if (lParam == (LPARAM)Okno->cboUstJednostka && HIWORD(wParam) == CBN_SELCHANGE) {
			StanProgramu->Zapisany = false;
			int ix = ComboBox_GetCurSel((HWND)lParam);
			if (ix != CB_ERR) {
				Okno->UstawJednostkeSkali(ix);
				Okno->ZmienUstawieniaGrafiki();
			}
		}

		if (lParam == (LPARAM)Okno->txtUstIloscJednostek && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->PowyzejZeraNP(Okno->txtUstIloscJednostek, "Iloœæ jednostek", &Dane->Ustawienia.JednIlosc);
			Okno->ZmienUstawieniaGrafiki();
		}

		if (lParam == (LPARAM)Okno->txtUstJednPodzialka && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->PowyzejZeraNP(Okno->txtUstJednPodzialka, "Podzia³ka jednostki", &Dane->Ustawienia.UstJednPodzialka);
			Okno->ZmienUstawieniaGrafiki();
		}

		if (lParam == (LPARAM)Okno->cbUstOdbiciaSciany) {
			StanProgramu->Zapisany = false;
			if (Button_GetCheck(Okno->cbUstOdbiciaSciany) == BST_CHECKED) {
				Button_SetCheck(Okno->cbUstOdbiciaSciany, BST_UNCHECKED);
				Dane->Ustawienia.OdbiciaSciany = false;
			}
			else {
				Button_SetCheck(Okno->cbUstOdbiciaSciany, BST_CHECKED);
				Dane->Ustawienia.OdbiciaSciany = true;
			}
		}

		RysujCiala();

		return 0;
	}
	return CallWindowProc(Okno->wpopnlUstawieniaDane, hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK wppnlPoleCentDane(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {

	case WM_COMMAND: {
		if (!Okno->Zdarzenia) break;

		if ((HWND)lParam == Okno->btnPoleCentDodaj) {
			StanProgramu->Zapisany = false;
			int id = Okno->DodajCialo(0, 0, 0, Okno->OstPromien);
			Okno->ZaznaczCialo(id);
			SetFocus(Okno->wndMain);
			RysujCiala();
		}

		return 0;
	}

	case WM_NOTIFY: {
		if (!Okno->Zdarzenia) break;

		NMHDR* n = (NMHDR*)lParam;

		if (n->code == NM_CLICK){
			NMITEMACTIVATE* ia = (NMITEMACTIVATE*)lParam;
			if (ia->iItem == -1) {
				Okno->ZaznaczCialo(-1);
				RysujCiala();
				return 0;
			}

			char*t = new char[DLUGOSC_TEKSTU];

			LVITEM lvi = {};
			lvi.iItem = ia->iItem;
			lvi.iSubItem = 0;
			lvi.pszText = t;
			lvi.cchTextMax = DLUGOSC_TEKSTU;
			lvi.mask = LVIF_TEXT;
			ListView_GetItem(Okno->lvCiala, &lvi);
			int id = 0;
			idtoi(id, lvi.pszText);
			id--;
			Okno->ZaznaczCialo(id);

			delete[] t;

			RysujCiala();
			
			return 0;
		}

		break;

	}

	case WM_VSCROLL: {

		SCROLLINFO si;
		ZeroMemory(&si, sizeof(si));
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_POS | SIF_PAGE | SIF_TRACKPOS;
		GetScrollInfo(Okno->vscPoleCentPrzewin, SB_CTL, &si);

		int pozycja = si.nPos;

		switch (LOWORD(wParam)) {
		case SB_TOP:
			pozycja = 0;
			break;
		case SB_BOTTOM:
			pozycja = Okno->KARTA_POLECENTPRZEWIN_ORYG_WYS;
			break;
		case SB_LINEUP:
			if (pozycja > 0) {
				pozycja--;
			}
			break;
		case SB_LINEDOWN:
			if (pozycja < (Okno->KARTA_POLECENTPRZEWIN_ORYG_WYS - Okno->KARTA_POLECENTPRZEWIN_WYS)) {
				pozycja++;
			}
			break;
		case SB_PAGEUP:
			pozycja -= si.nPage;
			if (pozycja < 0) {
				pozycja = 0;
			}
			break;
		case SB_PAGEDOWN:
			pozycja += si.nPage;
			if (pozycja > Okno->KARTA_POLECENTPRZEWIN_ORYG_WYS) {
				pozycja = Okno->KARTA_POLECENTPRZEWIN_ORYG_WYS;
			}
			break;
		case SB_THUMBPOSITION:
			pozycja = si.nTrackPos;
			break;
		case SB_THUMBTRACK:
			pozycja = si.nTrackPos;
			break;
		}

		int dy = -(pozycja - si.nPos);
		ScrollWindowEx(Okno->pnlPoleCentPrzewin, 0, dy, (CONST RECT *) NULL, (CONST RECT *) NULL, (HRGN)NULL, (LPRECT)NULL, SW_SCROLLCHILDREN | SW_INVALIDATE | SW_ERASE);
		UpdateWindow(Okno->pnlPoleCentPrzewin);

		ZeroMemory(&si, sizeof(si));
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_POS;
		si.nPos = pozycja;

		SetScrollInfo(Okno->vscPoleCentPrzewin, SB_CTL, &si, TRUE);
		return 0;
	}
	}
	return CallWindowProc(Okno->wpopnlPoleCentDane, hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK wppnlPoleCentPrzewin(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND: {

		int id = Okno->PobierzZaznId();
		if (!Okno->Zdarzenia || id == -1) break;

		//Usun
		if (lParam == (LPARAM)Okno->btnPoleCentUsun) {
			Okno->UsunCialo(id);
		}

		//Polozenie, promien
		if (lParam == (LPARAM)Okno->txtPoleCentX && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			if (Bledy->CokolwiekNP(Okno->txtPoleCentX, "Wspó³rzêdna X", &Dane->PoleCent[id]->X0, id)) {
				RysujCiala();
			}
		}

		if (lParam == (LPARAM)Okno->txtPoleCentY && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			if (Bledy->CokolwiekNP(Okno->txtPoleCentY, "Wspó³rzêdna Y", &Dane->PoleCent[id]->Y0, id)) {
				RysujCiala();
			}
		}

		if (lParam == (LPARAM)Okno->txtPoleCentZ && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			if (Bledy->CokolwiekNP(Okno->txtPoleCentZ, "Wspó³rzêdna Z", &Dane->PoleCent[id]->Z0, id)) {
				RysujCiala();
			}
		}

		if (lParam == (LPARAM)Okno->txtPoleCentR && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			if (Bledy->PowyzejZeraZeroNP(Okno->txtPoleCentR, "Promieñ", &Dane->PoleCent[id]->R0, id)) {
				Okno->OstPromien = Dane->PoleCent[id]->R0;
				RysujCiala();
			}
		}

		//Predkosc
		if (lParam == (LPARAM)Okno->cbPoleCentNieruchome) {
			StanProgramu->Zapisany = false;
			if (Button_GetCheck(Okno->cbPoleCentNieruchome) == BST_CHECKED) {
				Button_SetCheck(Okno->cbPoleCentNieruchome, BST_UNCHECKED);
				Okno->EnablePoleCentPredkosc(true);
				Dane->PoleCent[id]->Nieruchome = false;
			}
			else {
				Button_SetCheck(Okno->cbPoleCentNieruchome, BST_CHECKED);
				Okno->EnablePoleCentPredkosc(false);
				Dane->PoleCent[id]->Nieruchome = true;
			}
		}

		if (lParam == (LPARAM)Okno->txtPoleCentV0X && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->CokolwiekNP(Okno->txtPoleCentV0X, "Prêdkoœæ w osi X", &Dane->PoleCent[id]->V0Xt, id);
			Okno->ObliczPredkosc();
		}

		if (lParam == (LPARAM)Okno->txtPoleCentV0Y && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->CokolwiekNP(Okno->txtPoleCentV0Y, "Prêdkoœæ w osi Y", &Dane->PoleCent[id]->V0Yt, id);
			Okno->ObliczPredkosc();
		}

		if (lParam == (LPARAM)Okno->txtPoleCentV0Z && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->CokolwiekNP(Okno->txtPoleCentV0Z, "Prêdkoœæ w osi Z", &Dane->PoleCent[id]->V0Zt, id);
			Okno->ObliczPredkosc();
		}

		//Wspolczynnik restytucji
		if (lParam == (LPARAM)Okno->txtPoleCentK && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->UlamekZeroP(Okno->txtPoleCentK, "Wspó³czynnik restytucji", &Dane->PoleCent[id]->WspRestytucji, id);
		}

		//Masa
		if (lParam == (LPARAM)Okno->txtPoleCentMasa && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->PowyzejZeraNP(Okno->txtPoleCentMasa, "Masa", &Dane->PoleCent[id]->Masa0, id);

			Okno->ZmienTekstListView(1, Dane->PoleCent[id]->Masa0, JEDNOSTKI[Dane->PoleCent[id]->MasaJedn].Przedrostek + "g");
		}

		if (lParam == (LPARAM)Okno->cboPoleCentMasa && HIWORD(wParam) == CBN_SELCHANGE) {
			StanProgramu->Zapisany = false;
			int ix = ComboBox_GetCurSel((HWND)lParam);
			if (ix != CB_ERR) {
				Dane->PoleCent[id]->MasaJedn = ix;
				Okno->ZmienTekstListView(1, Dane->PoleCent[id]->Masa0, JEDNOSTKI[Dane->PoleCent[id]->MasaJedn].Przedrostek + "g");
			}

		}

		//Ladunek
		if (lParam == (LPARAM)Okno->txtPoleCentLadunek && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->CokolwiekNP(Okno->txtPoleCentLadunek, "£adunek", &Dane->PoleCent[id]->Ladunek0, id);

			Okno->ZmienTekstListView(2, Dane->PoleCent[id]->Ladunek0, JEDNOSTKI[Dane->PoleCent[id]->LadunekJedn].Przedrostek + "C");
		}

		if (lParam == (LPARAM)Okno->cboPoleCentLadunek && HIWORD(wParam) == CBN_SELCHANGE) {
			StanProgramu->Zapisany = false;
			int ix = ComboBox_GetCurSel((HWND)lParam);
			if (ix != CB_ERR) {
				Dane->PoleCent[id]->LadunekJedn = ix;
				Okno->ZmienTekstListView(2, Dane->PoleCent[id]->Ladunek0, JEDNOSTKI[Dane->PoleCent[id]->LadunekJedn].Przedrostek + "C");
			}
		}

		return 0;
	}
	}
	return CallWindowProc(Okno->wpopnlPoleCentPrzewin, hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK wpgrpWybuch(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND: {

		int id = Okno->PobierzZaznId();
		if (!Okno->Zdarzenia || id == -1) break;

		//Czy wybucha
		if (lParam == (LPARAM)Okno->cbPoleCentWybuch) {
			StanProgramu->Zapisany = false;
			if (Button_GetCheck(Okno->cbPoleCentWybuch) == BST_CHECKED) {
				Button_SetCheck(Okno->cbPoleCentWybuch, BST_UNCHECKED);
				Okno->EnablePoleCentWybuch(false);
				Dane->PoleCent[id]->Wybucha = false;
			}
			else {
				Button_SetCheck(Okno->cbPoleCentWybuch, BST_CHECKED);
				Okno->EnablePoleCentWybuch(true);
				Dane->PoleCent[id]->Wybucha = true;
			}
		}

		if (lParam == (LPARAM)Okno->btnPoleCentWybuch && StanProgramu->Animacja) {
			SilnikFizyczny->Wybuch(id);
		}

		//Sila
		if (lParam == (LPARAM)Okno->txtPoleCentWybuchF && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->PowyzejZeraZeroNP(Okno->txtPoleCentWybuchF, "Si³a wybuchu", &Dane->PoleCent[id]->Sila0, id);
		}

		if (lParam == (LPARAM)Okno->cboPoleCentWybuchF && HIWORD(wParam) == CBN_SELCHANGE) {
			StanProgramu->Zapisany = false;
			int ix = ComboBox_GetCurSel((HWND)lParam);
			if (ix != CB_ERR) Dane->PoleCent[id]->SilaJedn = ix;
		}

		//Promien
		if (lParam == (LPARAM)Okno->txtPoleCentWybuchR && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->PowyzejZeraZeroNP(Okno->txtPoleCentWybuchR, "Promieñ wybuchu", &Dane->PoleCent[id]->Promien0, id);
		}

		return 0;
	}
	}
	return CallWindowProc(Okno->wpogrpWybuch, hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK wpgrpPoleJednGraw(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND: {

		if (!Okno->Zdarzenia) break;

		if (lParam == (LPARAM)Okno->cbPoleJednGrawGora) {
			StanProgramu->Zapisany = false;
			if (Button_GetCheck(Okno->cbPoleJednGrawGora) == BST_CHECKED) {
				Button_SetCheck(Okno->cbPoleJednGrawGora, BST_UNCHECKED); //;(
				EnableWindow(Okno->txtPoleJednGrawGora, false);
				Dane->PoleJedn.Gora = false;
			}
			else {
				Button_SetCheck(Okno->cbPoleJednGrawGora, BST_CHECKED);
				EnableWindow(Okno->txtPoleJednGrawGora, true);
				Dane->PoleJedn.Gora = true;
			}
		}

		if (lParam == (LPARAM)Okno->cbPoleJednGrawDol) {
			StanProgramu->Zapisany = false;
			if (Button_GetCheck(Okno->cbPoleJednGrawDol) == BST_CHECKED) {
				Button_SetCheck(Okno->cbPoleJednGrawDol, BST_UNCHECKED);
				EnableWindow(Okno->txtPoleJednGrawDol, false);
				Dane->PoleJedn.Dol = false;
			}
			else {
				Button_SetCheck(Okno->cbPoleJednGrawDol, BST_CHECKED);
				EnableWindow(Okno->txtPoleJednGrawDol, true);
				Dane->PoleJedn.Dol = true;
			}
		}

		if (lParam == (LPARAM)Okno->cbPoleJednGrawLewo) {
			StanProgramu->Zapisany = false;
			if (Button_GetCheck(Okno->cbPoleJednGrawLewo) == BST_CHECKED) {
				Button_SetCheck(Okno->cbPoleJednGrawLewo, BST_UNCHECKED);
				EnableWindow(Okno->txtPoleJednGrawLewo, false);
				Dane->PoleJedn.Lewo = false;
			}
			else {
				Button_SetCheck(Okno->cbPoleJednGrawLewo, BST_CHECKED);
				EnableWindow(Okno->txtPoleJednGrawLewo, true);
				Dane->PoleJedn.Lewo = true;
			}
		}

		if (lParam == (LPARAM)Okno->cbPoleJednGrawPrawo) {
			StanProgramu->Zapisany = false;
			if (Button_GetCheck(Okno->cbPoleJednGrawPrawo) == BST_CHECKED) {
				Button_SetCheck(Okno->cbPoleJednGrawPrawo, BST_UNCHECKED);
				EnableWindow(Okno->txtPoleJednGrawPrawo, false);
				Dane->PoleJedn.Prawo = false;
			}
			else {
				Button_SetCheck(Okno->cbPoleJednGrawPrawo, BST_CHECKED);
				EnableWindow(Okno->txtPoleJednGrawPrawo, true);
				Dane->PoleJedn.Prawo = true;
			}
		}

		if (lParam == (LPARAM)Okno->cbPoleJednGrawPrzod) {
			StanProgramu->Zapisany = false;
			if (Button_GetCheck(Okno->cbPoleJednGrawPrzod) == BST_CHECKED) {
				Button_SetCheck(Okno->cbPoleJednGrawPrzod, BST_UNCHECKED);
				EnableWindow(Okno->txtPoleJednGrawPrzod, false);
				Dane->PoleJedn.Przod = false;
			}
			else {
				Button_SetCheck(Okno->cbPoleJednGrawPrzod, BST_CHECKED);
				EnableWindow(Okno->txtPoleJednGrawPrzod, true);
				Dane->PoleJedn.Przod = true;
			}
		}

		if (lParam == (LPARAM)Okno->cbPoleJednGrawTyl) {
			StanProgramu->Zapisany = false;
			if (Button_GetCheck(Okno->cbPoleJednGrawTyl) == BST_CHECKED) {
				Button_SetCheck(Okno->cbPoleJednGrawTyl, BST_UNCHECKED);
				EnableWindow(Okno->txtPoleJednGrawTyl, false);
				Dane->PoleJedn.Tyl = false;
			}
			else {
				Button_SetCheck(Okno->cbPoleJednGrawTyl, BST_CHECKED);
				EnableWindow(Okno->txtPoleJednGrawTyl, true);
				Dane->PoleJedn.Tyl = true;
			}
		}

		//Pola tekstowe
		if (lParam == (LPARAM)Okno->txtPoleJednGrawGora && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->PowyzejZeraZeroP(Okno->txtPoleJednGrawGora, "Pole jednorodne grawitacyjne góra", &Dane->PoleJedn.GoraIle);
		}

		if (lParam == (LPARAM)Okno->txtPoleJednGrawDol && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->PowyzejZeraZeroP(Okno->txtPoleJednGrawDol, "Pole jednorodne grawitacyjne dó³", &Dane->PoleJedn.DolIle);
		}

		if (lParam == (LPARAM)Okno->txtPoleJednGrawLewo && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->PowyzejZeraZeroP(Okno->txtPoleJednGrawLewo, "Pole jednorodne grawitacyjne lewo", &Dane->PoleJedn.LewoIle);
		}

		if (lParam == (LPARAM)Okno->txtPoleJednGrawPrawo && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->PowyzejZeraZeroP(Okno->txtPoleJednGrawPrawo, "Pole jednorodne grawitacyjne prawo", &Dane->PoleJedn.PrawoIle);
		}

		if (lParam == (LPARAM)Okno->txtPoleJednGrawPrzod && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->PowyzejZeraZeroP(Okno->txtPoleJednGrawPrzod, "Pole jednorodne grawitacyjne przód", &Dane->PoleJedn.PrzodIle);
		}

		if (lParam == (LPARAM)Okno->txtPoleJednGrawTyl && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->PowyzejZeraZeroP(Okno->txtPoleJednGrawTyl, "Pole jednorodne grawitacyjne ty³", &Dane->PoleJedn.TylIle);
		}

		return 0;
	}
	}
	return CallWindowProc(Okno->wpogrpPoleJednGraw, hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK wpgrpPoleJednEl(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND: {

		if (!Okno->Zdarzenia) break;

		if (lParam == (LPARAM)Okno->cbPoleJednElGora) {
			StanProgramu->Zapisany = false;
			if (Button_GetCheck(Okno->cbPoleJednElGora) == BST_CHECKED) {
				Button_SetCheck(Okno->cbPoleJednElGora, BST_UNCHECKED);
				Okno->EnablePoleJednElGora(false);
				Dane->PoleJedn.GoraDol = false;
			}
			else {
				Button_SetCheck(Okno->cbPoleJednElGora, BST_CHECKED);
				Okno->EnablePoleJednElGora(true);
				Dane->PoleJedn.GoraDol = true;
			}
		}

		if (lParam == (LPARAM)Okno->cbPoleJednElLewo) {
			StanProgramu->Zapisany = false;
			if (Button_GetCheck(Okno->cbPoleJednElLewo) == BST_CHECKED) {
				Button_SetCheck(Okno->cbPoleJednElLewo, BST_UNCHECKED);
				Okno->EnablePoleJednElLewo(false);
				Dane->PoleJedn.LewoPrawo = false;
			}
			else {
				Button_SetCheck(Okno->cbPoleJednElLewo, BST_CHECKED);
				Okno->EnablePoleJednElLewo(true);
				Dane->PoleJedn.LewoPrawo = true;
			}
		}

		if (lParam == (LPARAM)Okno->cbPoleJednElPrzod) {
			StanProgramu->Zapisany = false;
			if (Button_GetCheck(Okno->cbPoleJednElPrzod) == BST_CHECKED) {
				Button_SetCheck(Okno->cbPoleJednElPrzod, BST_UNCHECKED);
				Okno->EnablePoleJednElPrzod(false);
				Dane->PoleJedn.PrzodTyl = false;
			}
			else {
				Button_SetCheck(Okno->cbPoleJednElPrzod, BST_CHECKED);
				Okno->EnablePoleJednElPrzod(true);
				Dane->PoleJedn.PrzodTyl = true;
			}
		}

		return 0;
	}
	}
	return CallWindowProc(Okno->wpogrpPoleJednEl, hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK wppnlPoleJednEl(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND: {

		if (!Okno->Zdarzenia) break;

		//Przyciski radiowe
		if (lParam == (LPARAM)Okno->rbPoleJednElGoraPlus) {
			StanProgramu->Zapisany = false;
			if (Button_GetCheck(Okno->rbPoleJednElGoraPlus) == BST_UNCHECKED) {
				Button_SetCheck(Okno->rbPoleJednElGoraPlus, BST_CHECKED);
				Button_SetCheck(Okno->rbPoleJednElGoraMinus, BST_UNCHECKED);
				Dane->PoleJedn.GoraPlus = true;
			}
		}

		if (lParam == (LPARAM)Okno->rbPoleJednElGoraMinus) {
			StanProgramu->Zapisany = false;
			if (Button_GetCheck(Okno->rbPoleJednElGoraMinus) == BST_UNCHECKED) {
				Button_SetCheck(Okno->rbPoleJednElGoraMinus, BST_CHECKED);
				Button_SetCheck(Okno->rbPoleJednElGoraPlus, BST_UNCHECKED);
				Dane->PoleJedn.GoraPlus = false;
			}
		}

		if (lParam == (LPARAM)Okno->rbPoleJednElLewoPlus) {
			StanProgramu->Zapisany = false;
			if (Button_GetCheck(Okno->rbPoleJednElLewoPlus) == BST_UNCHECKED) {
				Button_SetCheck(Okno->rbPoleJednElLewoPlus, BST_CHECKED);
				Button_SetCheck(Okno->rbPoleJednElLewoMinus, BST_UNCHECKED);
				Dane->PoleJedn.LewoPlus = true;
			}
		}

		if (lParam == (LPARAM)Okno->rbPoleJednElLewoMinus) {
			StanProgramu->Zapisany = false;
			if (Button_GetCheck(Okno->rbPoleJednElLewoMinus) == BST_UNCHECKED) {
				Button_SetCheck(Okno->rbPoleJednElLewoMinus, BST_CHECKED);
				Button_SetCheck(Okno->rbPoleJednElLewoPlus, BST_UNCHECKED);
				Dane->PoleJedn.LewoPlus = false;
			}
		}

		if (lParam == (LPARAM)Okno->rbPoleJednElPrzodPlus) {
			StanProgramu->Zapisany = false;
			if (Button_GetCheck(Okno->rbPoleJednElPrzodPlus) == BST_UNCHECKED) {
				Button_SetCheck(Okno->rbPoleJednElPrzodPlus, BST_CHECKED);
				Button_SetCheck(Okno->rbPoleJednElPrzodMinus, BST_UNCHECKED);
				Dane->PoleJedn.PrzodPlus = true;
			}
		}

		if (lParam == (LPARAM)Okno->rbPoleJednElPrzodMinus) {
			StanProgramu->Zapisany = false;
			if (Button_GetCheck(Okno->rbPoleJednElPrzodMinus) == BST_UNCHECKED) {
				Button_SetCheck(Okno->rbPoleJednElPrzodMinus, BST_CHECKED);
				Button_SetCheck(Okno->rbPoleJednElPrzodPlus, BST_UNCHECKED);
				Dane->PoleJedn.PrzodPlus = false;
			}
		}

		//Pola tekstowe
		if (lParam == (LPARAM)Okno->txtPoleJednElGoraE && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->PowyzejZeraZeroP(Okno->txtPoleJednElGoraE, "Pole jednorodne elektrostatyczne góra", &Dane->PoleJedn.GoraDolIle);
		}

		if (lParam == (LPARAM)Okno->txtPoleJednElLewoE && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->PowyzejZeraZeroP(Okno->txtPoleJednElLewoE, "Pole jednorodne elektrostatyczne lewo", &Dane->PoleJedn.LewoPrawoIle);
		}

		if (lParam == (LPARAM)Okno->txtPoleJednElPrzodE && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->PowyzejZeraZeroP(Okno->txtPoleJednElPrzodE, "Pole jednorodne elektrostatyczne przód", &Dane->PoleJedn.PrzodTylIle);
		}

		return 0;
	}
	}
	return CallWindowProc(Okno->wpopnlPoleJednEl, hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK wpgrpTarcieWsp(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {

	case WM_COMMAND: {
		//Gora
		if (lParam == (LPARAM)Okno->txtTarcieStGora && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->UlamekZeroP(Okno->txtTarcieStGora, "Tarcie statyczne góra", &Dane->Tarcie.StGora);
		}

		if (lParam == (LPARAM)Okno->txtTarcieKinGora && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->UlamekZeroP(Okno->txtTarcieKinGora, "Tarcie kinetyczne góra", &Dane->Tarcie.KinGora);
		}

		//Dol
		if (lParam == (LPARAM)Okno->txtTarcieStDol && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->UlamekZeroP(Okno->txtTarcieStDol, "Tarcie statyczne dó³", &Dane->Tarcie.StDol);
		}

		if (lParam == (LPARAM)Okno->txtTarcieKinDol && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->UlamekZeroP(Okno->txtTarcieKinDol, "Tarcie kinetyczne dó³", &Dane->Tarcie.KinDol);
		}

		//Lewo
		if (lParam == (LPARAM)Okno->txtTarcieStLewo && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->UlamekZeroP(Okno->txtTarcieStLewo, "Tarcie statyczne lewo", &Dane->Tarcie.StLewo);
		}

		if (lParam == (LPARAM)Okno->txtTarcieKinLewo && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->UlamekZeroP(Okno->txtTarcieKinLewo, "Tarcie kinetyczne lewo", &Dane->Tarcie.KinLewo);
		}

		//Prawo
		if (lParam == (LPARAM)Okno->txtTarcieStPrawo && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->UlamekZeroP(Okno->txtTarcieStPrawo, "Tarcie statyczne prawo", &Dane->Tarcie.StPrawo);
		}

		if (lParam == (LPARAM)Okno->txtTarcieKinPrawo && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->UlamekZeroP(Okno->txtTarcieKinPrawo, "Tarcie kinetyczne prawo", &Dane->Tarcie.KinPrawo);
		}

		//Przod
		if (lParam == (LPARAM)Okno->txtTarcieStPrzod && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->UlamekZeroP(Okno->txtTarcieStPrzod, "Tarcie statyczne przód", &Dane->Tarcie.StPrzod);
		}

		if (lParam == (LPARAM)Okno->txtTarcieKinPrzod && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->UlamekZeroP(Okno->txtTarcieKinPrzod, "Tarcie kinetyczne przód", &Dane->Tarcie.KinPrzod);
		}

		//Tyl
		if (lParam == (LPARAM)Okno->txtTarcieStTyl && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->UlamekZeroP(Okno->txtTarcieStTyl, "Tarcie statyczne ty³", &Dane->Tarcie.StTyl);
		}

		if (lParam == (LPARAM)Okno->txtTarcieKinTyl && HIWORD(wParam) == EN_KILLFOCUS) {
			StanProgramu->Zapisany = false;
			Bledy->UlamekZeroP(Okno->txtTarcieKinTyl, "Tarcie kinetyczne ty³", &Dane->Tarcie.KinTyl);
		}

		return 0;
	}

	}
	return CallWindowProc(Okno->wpogrpTarcieWsp, hwnd, uMsg, wParam, lParam);
}