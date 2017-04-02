#include "..\inc\animacja.h"

extern Animacja3D* Animacja;
extern Fizyka* SilnikFizyczny;
extern strStanProgramu* StanProgramu;
extern DaneSymulacji* Dane;
extern MainWindow* Okno;

void DaneGrafiki::Czysc() {
	this->Ilosc = 0;
	delete[] this->Ciala;
	delete[] this->DaneCial;
	this->Ciala = 0;
	this->DaneCial = 0;
}

DaneGrafiki::~DaneGrafiki() {
	this->Czysc();
}

void DaneGrafiki::UstawIlosc(int ilosc) {
	this->Czysc();
	this->Ciala = new Punkt[ilosc];
	this->DaneCial = new DaneCiala[ilosc];
	this->Ilosc = ilosc;
}

void Animacja3D::UstawPelnyEkran(bool PelnyEkran){

	if (PelnyEkran) {

		this->OknoZminimalizowane = false;
		this->PelnyEkran = true;

		D3DXMatrixPerspectiveFovLH(&this->mcPerspektywa, D3DX_PI / 4, this->DaneEkranu.Width / this->DaneEkranu.Height, 1.0f, 1000.0f);

		this->UtworzOknoAnimacji();
		this->UtworzUrzadzeniePelnyEkran();

		ShowWindow(this->OknoAnimacja, SW_SHOWDEFAULT);
		UpdateWindow(this->OknoAnimacja);
		this->Rysuj();

		MSG msg = {};
		while (GetMessage(&msg, NULL, 0, 0)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	} else {
		this->PelnyEkran = false;
		this->UstawRozmiarObrazu(this->okno_szer, this->okno_wys);
		this->Rysuj();
	}

}

void Animacja3D::UstawRozmiarObrazu(int szer, int wys){
	if (szer <= 0 || wys <= 0) {
		this->OknoZminimalizowane = true;
		return;
	}

	this->OknoZminimalizowane = false;

	this->okno_szer = szer;
	this->okno_wys = wys;

	D3DXMatrixPerspectiveFovLH(&this->mcPerspektywa, D3DX_PI / 4, 1.0f, 1.0f, 1000.0f);

	this->UtworzUrzadzenieOkno();
}

void Animacja3D::ZamknijPelnyEkran() {
	if (!this->PelnyEkran) return;
	this->UstawPelnyEkran(false);
	DestroyWindow(this->OknoAnimacja);
}

void Animacja3D::UstawUstawienia(UstawieniaSceny ust) {
	this->Ustawienia = ust;
	this->CzyscOpisyPodzialki();
	this->UtworzOpisyPodzialki();
}

void Animacja3D::UstawKamere(D3DXVECTOR3& Obroty, D3DXVECTOR3& PatrzNa, D3DXVECTOR3& PatrzZ) {
	this->PatrzNa = PatrzNa;
	this->PatrzZ = PatrzZ;
	this->ObrazObrotX = Obroty.x;
	this->ObrazObrotY = Obroty.y;
	this->ObrazObrotZ = Obroty.z;

	D3DXMatrixLookAtLH(&this->mcObrazKierunek, &this->PatrzZ, &this->PatrzNa, &this->Gora);
	D3DXMatrixRotationX(&this->mcObrazObrotX, this->ObrazObrotX);
	D3DXMatrixRotationY(&this->mcObrazObrotY, this->ObrazObrotY);
	D3DXMatrixRotationZ(&this->mcObrazObrotZ, this->ObrazObrotZ);

	this->mcObraz = this->mcObrazObrotX * this->mcObrazObrotY * this->mcObrazObrotZ * this->mcObrazKierunek;
}

void Animacja3D::Czysc(){
	this->CzyscUrzadzenie();

	if (this->D3DApi != NULL) this->D3DApi->Release();
	this->D3DApi = NULL;
}

void Animacja3D::CzyscUrzadzenie(){
#ifdef czajniczek
	if (this->Czajniczek != NULL) this->Czajniczek->Release();
	this->Czajniczek = NULL;
#endif

	for (int i = 0; i < ILOSC_TEKSTUR; i++) {
		if (this->Tekstury[i]) {
			this->Tekstury[i]->Release();
			this->Tekstury[i] = NULL;
		}
	}

	DaneCiala* dc;
	for (int i = 0; i < this->DaneGr.PobierzIlosc(); i++) {
		dc = &this->DaneGr.PobierzWskDaneCial()[i];
		if (dc->nazwa_m) {
			dc->nazwa_m->Release();
			dc->nazwa_m = NULL;
		}
	}

	this->ile_tekstur = 1;

	this->CzyscOpisyPodzialki();

	if (this->WierzcholkiKolor != NULL) this->WierzcholkiKolor->Release();
	if (this->WierzcholkiSwiatlo != NULL) this->WierzcholkiSwiatlo->Release();
	if (this->Urzadzenie != NULL) this->Urzadzenie->Release();

	this->WierzcholkiKolor = NULL;
	this->WierzcholkiSwiatlo = NULL;
	this->Urzadzenie = NULL;
}

void Animacja3D::PobierzDaneEkranu(){
	EnterCriticalSection(&this->DostepDoUrzadzenia);
	this->D3DApi->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &this->DaneEkranu);
	LeaveCriticalSection(&this->DostepDoUrzadzenia);
}

void Animacja3D::RejestrujOknoAnimacji(){
	WNDCLASSEX wc = {};

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = wpwndAnimacja;
	wc.hInstance = this->Instancja;
	wc.lpszClassName = NWNDANIMACJA;

	RegisterClassEx(&wc);
}

void Animacja3D::UtworzOknoAnimacji() {
	this->OknoAnimacja = CreateWindowEx(0, NWNDANIMACJA, TWNDANIMACJA, WS_POPUP, 0, 0, this->DaneEkranu.Width, this->DaneEkranu.Height, NULL, NULL, this->Instancja, NULL);
}

void Animacja3D::UtworzUrzadzenieOkno(){
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = this->DaneEkranu.Format;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.BackBufferWidth = this->okno_szer;
	d3dpp.BackBufferHeight = this->okno_wys;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.MultiSampleType = (D3DMULTISAMPLE_TYPE)this->AntyAlOkno;

	d3dpp.Windowed = TRUE;

	EnterCriticalSection(&this->DostepDoUrzadzenia);

	this->CzyscUrzadzenie();

	this->D3DApi->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, this->OknoGlowne, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &this->Urzadzenie);

	this->Urzadzenie->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	this->Urzadzenie->SetRenderState(D3DRS_LIGHTING, true);
	this->Urzadzenie->SetRenderState(D3DRS_ZENABLE, true);

	this->DodajWierzcholki();

	this->UtworzTekstury();
	this->UtworzOpisyPodzialki();

#ifdef czajniczek
	D3DXCreateTeapot(this->Urzadzenie, &this->Czajniczek, NULL);
#endif

	LeaveCriticalSection(&this->DostepDoUrzadzenia);
}

void Animacja3D::UtworzUrzadzeniePelnyEkran(){
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = this->DaneEkranu.Format;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.MultiSampleType = (D3DMULTISAMPLE_TYPE)this->AntyAlPelnyEkran;

	d3dpp.Windowed = FALSE;
	d3dpp.BackBufferWidth = this->DaneEkranu.Width;
	d3dpp.BackBufferHeight = this->DaneEkranu.Height;
	d3dpp.BackBufferFormat = this->DaneEkranu.Format;
	d3dpp.FullScreen_RefreshRateInHz = this->DaneEkranu.RefreshRate;

	EnterCriticalSection(&this->DostepDoUrzadzenia);

	this->CzyscUrzadzenie();

	this->D3DApi->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, this->OknoAnimacja, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &this->Urzadzenie);

	this->Urzadzenie->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	this->Urzadzenie->SetRenderState(D3DRS_LIGHTING, TRUE);
	this->Urzadzenie->SetRenderState(D3DRS_ZENABLE, TRUE);

	this->DodajWierzcholki();
	this->UtworzOpisyPodzialki();

	this->UtworzTekstury();

#ifdef czajniczek
	D3DXCreateTeapot(this->Urzadzenie, &this->Czajniczek, NULL);
#endif

	LeaveCriticalSection(&this->DostepDoUrzadzenia);
}

Animacja3D::Animacja3D(){
	InitializeCriticalSection(&this->DostepDoUrzadzenia);

	this->PatrzZ = D3DXVECTOR3(OKO_X, OKO_Y, OKO_Z);
	this->PatrzNa = D3DXVECTOR3(0, 0, 0);
	this->UtworzMacierze();
	this->UtworzMaterialy();
	this->UtworzWierzcholki();
}

Animacja3D::~Animacja3D(){
	DeleteCriticalSection(&this->DostepDoUrzadzenia);
	this->Czysc();
}

HRESULT Animacja3D::Inicjuj(HWND hWnd, HINSTANCE hInstance, void(*PrzetworzKlawisz_)(WPARAM wParam)){
	if ((this->D3DApi = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) return E_FAIL;

	this->PobierzDaneEkranu();
	this->AntyAlOkno = this->PobierzAntyaliasing(false);
	this->AntyAlPelnyEkran = this->PobierzAntyaliasing(true);
	this->PrzetworzKlawisz = PrzetworzKlawisz_;
	this->OknoGlowne = hWnd;
	this->Instancja = hInstance;
	this->RejestrujOknoAnimacji();

	this->UstawPelnyEkran(false);
	this->PelnyEkran = false;
	
	return S_OK;
}

void Animacja3D::UtworzTekst3D(LPD3DXMESH* ppMesh, LPCSTR pstrFont, LPCSTR tekst, DWORD dwSize, BOOL bBold, BOOL bItalic) {
	LPD3DXMESH pMeshNew = NULL;
	HDC hdc = CreateCompatibleDC(NULL);
	INT nHeight = -MulDiv(dwSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	HFONT hFont;
	HFONT hFontOld;

	hFont = CreateFont(nHeight, 0, 0, 0, bBold ? FW_BOLD : FW_NORMAL, bItalic, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		pstrFont);

	hFontOld = (HFONT)SelectObject(hdc, hFont);

	D3DXCreateText(this->Urzadzenie, hdc, tekst, 0.001f, 0.1f, &pMeshNew, NULL, NULL);

	SelectObject(hdc, hFontOld);
	DeleteObject(hFont);
	DeleteDC(hdc);

	*ppMesh = pMeshNew;
}

void Animacja3D::Rysuj(){
	if (this->OknoZminimalizowane) return;

	EnterCriticalSection(&this->DostepDoUrzadzenia);

	this->Urzadzenie->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	if (SUCCEEDED(this->Urzadzenie->BeginScene()))	{
		this->Urzadzenie->SetTexture(0, 0);

		this->Urzadzenie->SetTransform(D3DTS_PROJECTION, &this->mcPerspektywa);

		//Ustaw kamere
		switch (this->Kamera) {
		case AnimKamera::Domyslna:
			this->Urzadzenie->SetTransform(D3DTS_VIEW, &this->mcObraz);
			break;

		case AnimKamera::WysrodkujNaCiele: {
			D3DXMATRIXA16 mc;
			D3DXMatrixLookAtLH(&mc, &this->PatrzZ, &this->KameraPatrzNa, &this->Gora);

			this->Urzadzenie->SetTransform(D3DTS_VIEW, &mc);
			break;
		}

		case AnimKamera::WKierunkuPredkosci:{
			D3DXMATRIXA16 mc;
			D3DXMatrixLookAtLH(&mc, &this->KameraPatrzZ, &this->KameraPatrzNa, &this->Gora);

			this->Urzadzenie->SetTransform(D3DTS_VIEW, &mc);
			break;
		}

		default:
			this->Urzadzenie->SetTransform(D3DTS_VIEW, &this->mcObraz);
		}

		this->Urzadzenie->SetTransform(D3DTS_WORLD, &this->mcBezPrzeksztalcen);

		this->Urzadzenie->SetRenderState(D3DRS_LIGHTING, FALSE);
		this->Urzadzenie->SetStreamSource(0, this->WierzcholkiKolor, 0, sizeof(VERTEX_DIFF));
		this->Urzadzenie->SetFVF(D3DFVF_CUSTOMVERTEX_DIFF);

		//Uklad wspolrzednych
		D3DXMATRIXA16 Wspolrzedne;
		D3DXMATRIXA16 Obrot;
		D3DXMATRIXA16 Skalowanie;
		D3DXMATRIXA16 Przesuniecie;

		D3DXMatrixScaling(&Wspolrzedne, this->Ustawienia.JednIlosc, this->Ustawienia.JednIlosc, this->Ustawienia.JednIlosc);
		this->Urzadzenie->SetTransform(D3DTS_WORLD, &Wspolrzedne);
		this->Urzadzenie->DrawPrimitive(D3DPT_LINELIST, 0, 1);
		this->Urzadzenie->DrawPrimitive(D3DPT_LINELIST, 2, 1);
		this->Urzadzenie->DrawPrimitive(D3DPT_LINELIST, 4, 1);

		//X
		D3DXMatrixRotationZ(&Obrot, PI / 2.0);
		D3DXMatrixScaling(&Skalowanie, PODZIALKA_DLUGOSC, 1, 1);
		Wspolrzedne = Skalowanie * Obrot;

		for (double i = 1; i <= this->Ustawienia.JednIlosc; i += 1) {
			D3DXMatrixTranslation(&Przesuniecie, i, 0, 0);
			this->Urzadzenie->SetTransform(D3DTS_WORLD, &(Wspolrzedne*Przesuniecie));
			this->Urzadzenie->DrawPrimitive(D3DPT_LINELIST, 0, 1);
		}

		//Y
		D3DXMatrixRotationZ(&Obrot, 3.0f * PI / 2.0f);
		D3DXMatrixScaling(&Skalowanie, 1, PODZIALKA_DLUGOSC, 1);
		Wspolrzedne = Skalowanie * Obrot;

		for (double i = 1; i <= this->Ustawienia.JednIlosc; i += 1) {
			D3DXMatrixTranslation(&Przesuniecie, 0, i, 0);
			this->Urzadzenie->SetTransform(D3DTS_WORLD, &(Wspolrzedne*Przesuniecie));
			this->Urzadzenie->DrawPrimitive(D3DPT_LINELIST, 2, 1);
		}

		//Z
		D3DXMatrixRotationX(&Obrot, 3.0f * PI / 2.0f);
		D3DXMatrixScaling(&Skalowanie, 1, 1, PODZIALKA_DLUGOSC);
		Wspolrzedne = Skalowanie * Obrot;

		for (double i = 1; i <= this->Ustawienia.JednIlosc; i += 1) {
			D3DXMatrixTranslation(&Przesuniecie, 0, 0, i);
			this->Urzadzenie->SetTransform(D3DTS_WORLD, &(Wspolrzedne*Przesuniecie));
			this->Urzadzenie->DrawPrimitive(D3DPT_LINELIST, 4, 1);
		}

		this->Urzadzenie->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		this->Urzadzenie->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		this->Urzadzenie->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		this->Urzadzenie->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		this->Urzadzenie->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		this->Urzadzenie->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

		this->Urzadzenie->SetRenderState(D3DRS_LIGHTING, TRUE);
		this->Urzadzenie->SetStreamSource(0, this->WierzcholkiSwiatlo, 0, sizeof(VERTEX_LIGHT));
		this->Urzadzenie->SetFVF(D3DFVF_CUSTOMVERTEX_LIGHT);

		D3DXVECTOR3 vecDir;
		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(D3DLIGHT9));
		light.Type = D3DLIGHT_DIRECTIONAL;
		light.Diffuse.r = 1.0;
		light.Diffuse.g = 1.0;
		light.Diffuse.b = 1.0;
		
		vecDir = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);
		D3DXVec3Normalize(&vecDir, &vecDir);
		light.Direction = vecDir;
		light.Range = 1000.0f;
		this->Urzadzenie->SetLight(0, &light);
		this->Urzadzenie->LightEnable(0, TRUE);
		
		this->Urzadzenie->SetRenderState(D3DRS_AMBIENT, 0x00909090);		
		this->Urzadzenie->SetTransform(D3DTS_WORLD, &this->mcBezPrzeksztalcen);

		//Kula
		D3DXMATRIXA16 a;
		D3DXMATRIXA16 b;
		double r;

		DaneCiala* CialoD;
		Punkt* Cialo;

#ifndef czajniczek
		int tex_id;
#endif

		this->Urzadzenie->SetMaterial(&this->mtrBialy);

		for (int i = 0; i < this->DaneGr.PobierzIlosc(); i++) {
			CialoD = &this->DaneGr.PobierzWskDaneCial()[i];
			if (CialoD->puste) continue;
			Cialo = &this->DaneGr.PobierzWskCiala()[i];

			//Kolor
#ifndef czajniczek
			tex_id = i % this->ile_tekstur;

			if (this->Tekstury[tex_id]) {

				this->Urzadzenie->SetMaterial(&this->mtrBialy);
				this->Urzadzenie->SetTexture(0, this->Tekstury[tex_id]);

			} else {
#endif

				this->Urzadzenie->SetTexture(0, NULL);

				switch (this->DaneGr.PobierzWskDaneCial()[i].kolor) {

				case AnimKolor::Niebieski:
					this->Urzadzenie->SetMaterial(&this->mtrNiebieski);
					break;

				case AnimKolor::Zielony:
					this->Urzadzenie->SetMaterial(&this->mtrZielony);
					break;

				case AnimKolor::Czerwony:
					this->Urzadzenie->SetMaterial(&this->mtrCzerwony);
					break;

				default:
					this->Urzadzenie->SetMaterial(&this->mtrNiebieski);
					break;

				}

#ifndef czajniczek
			}
#endif

			r = CialoD->promien;
			D3DXMatrixScaling(&a, r, r, r);
			D3DXMatrixTranslation(&b, Cialo->x, Cialo->y, Cialo->z);

			this->Urzadzenie->SetTransform(D3DTS_WORLD, &(a*b));

#ifndef czajniczek

			for (int i = 0; i < ILOSC_WIERSZY; i++) {
				this->Urzadzenie->DrawPrimitive(D3DPT_TRIANGLESTRIP, i*(2*TROJKATY_WIERSZ+2) + TROJKATY_PRZESUNIECIE, 2 * TROJKATY_WIERSZ);
			}
#else
			this->Czajniczek->DrawSubset(0);
#endif
			
		}

		//Opis cial
		float wsp = 0.5;
		this->Urzadzenie->SetTexture(0, NULL);
		this->Urzadzenie->SetMaterial(&this->mtrPurpurowy);	

		for (int i = 0; i < this->DaneGr.PobierzIlosc(); i++) {
			CialoD = &this->DaneGr.PobierzWskDaneCial()[i];
			if (CialoD->puste) continue;

			Punkt* Cialo;
			Cialo = &this->DaneGr.PobierzWskCiala()[i];

			r = CialoD->promien;
			D3DXMatrixTranslation(&b, Cialo->x, Cialo->y + r + 0.6, Cialo->z);

			if (!CialoD->nazwa_m) {
				this->UtworzTekst3D(&CialoD->nazwa_m, NAZWA_CZCIONKI, CialoD->nazwa.c_str(), ROZMIAR_CZCIONKI_OPIS, false, false);
			}

			this->Urzadzenie->SetTransform(D3DTS_WORLD, &(this->mcSkaluj1*b));
			CialoD->nazwa_m->DrawSubset(0);
		}

		//Opisy osi
		if (this->PodzialkaOpis) {
			this->Urzadzenie->SetMaterial(&this->mtrCzarny);

			D3DXMATRIXA16 SkalujTekst;
			D3DXMatrixScaling(&SkalujTekst, wsp, wsp, wsp);

			D3DXMatrixTranslation(&Przesuniecie, this->Ustawienia.JednIlosc + 0.2, -wsp, 0);
			this->Urzadzenie->SetTransform(D3DTS_WORLD, &(SkalujTekst*Przesuniecie));
			this->PodzialkaOpis[0]->DrawSubset(0);

			for (int i = 3; i < this->IloscOpisowPodzialki; i++) {
				D3DXMatrixTranslation(&Przesuniecie, (i-2)*this->Ustawienia.UstJednPodzialka, -wsp, 0);
				this->Urzadzenie->SetTransform(D3DTS_WORLD, &(SkalujTekst*Przesuniecie));
				this->PodzialkaOpis[i]->DrawSubset(0);
			}

			//Y
			D3DXMatrixTranslation(&Przesuniecie, -2*wsp-0.3, this->Ustawienia.JednIlosc + 0.2, 0.0);
			this->Urzadzenie->SetTransform(D3DTS_WORLD, &(SkalujTekst*Przesuniecie));
			this->PodzialkaOpis[1]->DrawSubset(0);

			for (int i = 3; i < this->IloscOpisowPodzialki; i++) {
				D3DXMatrixTranslation(&Przesuniecie, -wsp-0.3f, (i-2)*this->Ustawienia.UstJednPodzialka, 0.1f);
				this->Urzadzenie->SetTransform(D3DTS_WORLD, &(SkalujTekst*Przesuniecie));
				this->PodzialkaOpis[i]->DrawSubset(0);
			}

			//Z
			D3DXMatrixTranslation(&Przesuniecie, 0, -wsp, this->Ustawienia.JednIlosc + 0.2);
			D3DXMatrixRotationY(&Obrot, 3.0f * PI / 2.0f);

			this->Urzadzenie->SetTransform(D3DTS_WORLD, &(SkalujTekst*Obrot*Przesuniecie));
			this->PodzialkaOpis[2]->DrawSubset(0);

			for (int i = 3; i < this->IloscOpisowPodzialki; i++) {
				D3DXMatrixTranslation(&Przesuniecie, 0, -wsp, (i - 2)*this->Ustawienia.UstJednPodzialka);
				this->Urzadzenie->SetTransform(D3DTS_WORLD, &(SkalujTekst*Obrot*Przesuniecie));
				this->PodzialkaOpis[i]->DrawSubset(0);
			}
		}


		this->Urzadzenie->EndScene();
	}

	this->Urzadzenie->Present(NULL, NULL, NULL, NULL);

	LeaveCriticalSection(&this->DostepDoUrzadzenia);
}

void Animacja3D::UtworzMacierze(){
	D3DXMatrixLookAtLH(&this->mcObrazKierunek, &this->PatrzZ, &this->PatrzNa, &this->Gora);
	D3DXMatrixRotationX(&this->mcObrazObrotX, this->ObrazObrotX);
	D3DXMatrixRotationY(&this->mcObrazObrotY, this->ObrazObrotY);
	D3DXMatrixRotationZ(&this->mcObrazObrotZ, this->ObrazObrotZ);
	this->mcObraz = this->mcObrazObrotX * this->mcObrazObrotY * this->mcObrazObrotZ * this->mcObrazKierunek;

	D3DXMATRIXA16 macierz1;
	D3DXMATRIXA16 macierz2;
	D3DXMATRIXA16 macierz3;
	D3DXMATRIXA16 macierz4;
	D3DXMATRIXA16 macierz5;
	D3DXMatrixRotationX(&macierz1, 0);
	D3DXMatrixRotationY(&macierz2, 0);
	D3DXMatrixRotationZ(&macierz3, 0);
	D3DXMatrixTranslation(&macierz4, 0, 0, 0);
	D3DXMatrixScaling(&macierz5, 1, 1, 1);
	this->mcBezPrzeksztalcen = macierz1 * macierz2 * macierz3 * macierz4 * macierz5;

	D3DXMatrixScaling(&this->mcSkaluj1, 1, 1, 1);
}

void Animacja3D::UtworzMaterialy(){
	//Cialo zwykle
	ZeroMemory(&this->mtrNiebieski, sizeof(D3DMATERIAL9));
	mtrNiebieski.Diffuse.r = mtrNiebieski.Ambient.r = 0.21094f;
	mtrNiebieski.Diffuse.g = mtrNiebieski.Ambient.g = 0.19922f;
	mtrNiebieski.Diffuse.b = mtrNiebieski.Ambient.b = 1.0f;
	mtrNiebieski.Diffuse.a = mtrNiebieski.Ambient.a = 1.0f;

	//Cialo zaznaczone
	ZeroMemory(&this->mtrZielony, sizeof(D3DMATERIAL9));
	mtrZielony.Diffuse.r = mtrZielony.Ambient.r = 0.0f;
	mtrZielony.Diffuse.g = mtrZielony.Ambient.g = 1.0f;
	mtrZielony.Diffuse.b = mtrZielony.Ambient.b = 0.28125f;
	mtrZielony.Diffuse.a = mtrZielony.Ambient.a = 1.0f;

	//Cialo wybuchajace
	ZeroMemory(&this->mtrCzerwony, sizeof(D3DMATERIAL9));
	mtrCzerwony.Diffuse.r = mtrCzerwony.Ambient.r = 1.0f;
	mtrCzerwony.Diffuse.g = mtrCzerwony.Ambient.g = 0.19922f;
	mtrCzerwony.Diffuse.b = mtrCzerwony.Ambient.b = 0.19922f;
	mtrCzerwony.Diffuse.a = mtrCzerwony.Ambient.a = 1.0f;

	//Cialo z tekstura
	ZeroMemory(&this->mtrBialy, sizeof(D3DMATERIAL9));
	mtrBialy.Diffuse.r = mtrBialy.Ambient.r = 1.0f;
	mtrBialy.Diffuse.g = mtrBialy.Ambient.g = 1.0f;
	mtrBialy.Diffuse.b = mtrBialy.Ambient.b = 1.0f;
	mtrBialy.Diffuse.a = mtrBialy.Ambient.a = 1.0f;

	//Opis ciala
	ZeroMemory(&this->mtrPurpurowy, sizeof(D3DMATERIAL9));
	mtrPurpurowy.Diffuse.r = mtrPurpurowy.Ambient.r = 0.9098f;
	mtrPurpurowy.Diffuse.g = mtrPurpurowy.Ambient.g = 0.0666667f;
	mtrPurpurowy.Diffuse.b = mtrPurpurowy.Ambient.b = 0.2352941f;
	mtrPurpurowy.Diffuse.a = mtrPurpurowy.Ambient.a = 1.0f;

	//Opis osi
	ZeroMemory(&this->mtrCzarny, sizeof(D3DMATERIAL9));
	mtrCzarny.Diffuse.r = mtrCzarny.Ambient.r = 0.0f;
	mtrCzarny.Diffuse.g = mtrCzarny.Ambient.g = 0.0f;
	mtrCzarny.Diffuse.b = mtrCzarny.Ambient.b = 0.0f;
	mtrCzarny.Diffuse.a = mtrCzarny.Ambient.a = 1.0f;
}

void Animacja3D::UtworzWierzcholki() {

	this->VertDiff[0] = { 0.0f, 0.0f, 0.0f, 0xffff0000 };
	this->VertDiff[1] = { 1.0f, 0.0f, 0.0f, 0xffff0000 };

	this->VertDiff[2] = { 0.0f, 0.0f, 0.0f, 0xff00ff00 };
	this->VertDiff[3] = { 0.0f, 1.0f, 0.0f, 0xff00ff00 };

	this->VertDiff[4] = { 0.0f, 0.0f, 0.0f, 0xff0000ff };
	this->VertDiff[5] = { 0.0f, 0.0f, 1.0f, 0xff0000ff };


	this->VertLight[0] = { {-0.5, -0.5, 0.0}, {}, 0.0, 1.0 };
	this->VertLight[1] = { {-0.5, 0.5, 0.0}, {0.0, 0.0, 0.0}, 0.0, 0.0 };
	this->VertLight[2] = { {0.5, -0.5, 0.0}, {}, 1.0, 1.0 };
	this->VertLight[3] = { {0.5, 0.5, 0.0}, {}, 1.0, 0.0 };

	int ix = TROJKATY_PRZESUNIECIE;
	float x, y1, y2, z, r1, r2;
	float kat1, kat2;
	y1 = -1.0;
	y2 = 1.0;
	r1 = 1;
	r2 = 1;
	float wiersze = (float)ILOSC_WIERSZY;
	float trojkaty = (float)TROJKATY_WIERSZ;

	//Kula
	for (int i = 1; i < ILOSC_WIERSZY+1; i++) {
		y1 = sinf(((float)(i - 1) / wiersze) * PI - PI / 2.0);
		y2 = sinf(((float)i / wiersze) * PI - PI / 2.0);
		r1 = cosf(((float)(i - 1) / wiersze) * PI - PI / 2.0);
		r2 = cosf(((float)i / wiersze) * PI - PI / 2.0);

		for (int j = 0; j < TROJKATY_WIERSZ; j++) {
			kat1 = ((float)j / trojkaty) * 2.0 * PI;
			kat2 = kat1;

			if (i % 2 == 0) kat1 -= PI / trojkaty; else kat2 += PI / trojkaty;

			x = r1 * cosf(kat1);
			z = r1 * sinf(kat1);

			VertLight[ix].position = D3DXVECTOR3(x, y1, z);
			VertLight[ix].normal = D3DXVECTOR3(x, y1, z);

			VertLight[ix].tx = 2 * kat1 / (2*PI);
			VertLight[ix].ty = 1 - (y1+1)/2;

			ix++;

			x = r2 * cosf(kat2);
			z = r2 * sinf(kat2);

			VertLight[ix].position = D3DXVECTOR3(x, y2, z);
			VertLight[ix].normal = D3DXVECTOR3(x, y2, z);

			VertLight[ix].tx = 2 * kat2 / (2 * PI);
			VertLight[ix].ty = 1 - (y2 + 1) / 2;

			ix++;
		}

		VertLight[ix].position = VertLight[ix - 2 * TROJKATY_WIERSZ].position;
		VertLight[ix].normal = VertLight[ix -   2 * TROJKATY_WIERSZ].normal;
		VertLight[ix].tx =  VertLight[ix - 2 * TROJKATY_WIERSZ].tx;
		VertLight[ix].ty =  VertLight[ix - 2 * TROJKATY_WIERSZ].ty;

		ix++;


		VertLight[ix].position = VertLight[ix -  2 * TROJKATY_WIERSZ].position;
		VertLight[ix].normal = VertLight[ix -  2 * TROJKATY_WIERSZ].normal;
		VertLight[ix].tx = VertLight[ix - 2 * TROJKATY_WIERSZ].tx;
		VertLight[ix].ty = VertLight[ix - 2 * TROJKATY_WIERSZ].ty;

		ix++;
	}

}

void Animacja3D::UtworzTekstury() {
	char tekst[30];

	this->ile_tekstur = 0;

	for (int i = 0; i < ILOSC_TEKSTUR; i++) {
		sprintf_s(tekst, "tex\\%d.bmp", i);
		if (D3DXCreateTextureFromFile(this->Urzadzenie, tekst, &this->Tekstury[i]) == D3D_OK) {
			this->ile_tekstur = i;
		} else {
			this->Tekstury[i] = NULL;
		}
	}

	this->ile_tekstur++;
}

void Animacja3D::CzyscOpisyPodzialki() {
	if (this->PodzialkaOpis) {
		for (int i = 0; i < this->IloscOpisowPodzialki; i++) {
			this->PodzialkaOpis[i]->Release();
		}

		delete[] this->PodzialkaOpis;
	}

	this->PodzialkaOpis = NULL;
	this->IloscOpisowPodzialki = 0;
}

void Animacja3D::UtworzOpisyPodzialki() {
	if (this->Ustawienia.UstJednPodzialka == 0) return;

	if (this->Ustawienia.JednIlosc < PODZIALKA_PRZYTNIJ) {
		this->IloscOpisowPodzialki = 3;
	} else {
		this->IloscOpisowPodzialki = (this->Ustawienia.JednIlosc - PODZIALKA_PRZYTNIJ) / this->Ustawienia.UstJednPodzialka + 3;
	}
	
	this->PodzialkaOpis = new LPD3DXMESH[this->IloscOpisowPodzialki];

	UtworzTekst3D(&this->PodzialkaOpis[0], NAZWA_CZCIONKI, ("X [" + this->Ustawienia.JednNazwa + "]").c_str(), ROZMIAR_CZCIONKI_PODZIALKA, false, false);
	UtworzTekst3D(&this->PodzialkaOpis[1], NAZWA_CZCIONKI, ("Y [" + this->Ustawienia.JednNazwa + "]").c_str(), ROZMIAR_CZCIONKI_PODZIALKA, false, false);
	UtworzTekst3D(&this->PodzialkaOpis[2], NAZWA_CZCIONKI, ("Z [" + this->Ustawienia.JednNazwa + "]").c_str(), ROZMIAR_CZCIONKI_PODZIALKA, false, false);

	int ix = 3;

	char* t = new char[DLUGOSC_TEKSTU];

	for (int i = 3; i < this->IloscOpisowPodzialki; i++) {
		ftoa(t, this->Ustawienia.Jednostka0 * this->Ustawienia.UstJednPodzialka * (i-2));
		UtworzTekst3D(&this->PodzialkaOpis[i], NAZWA_CZCIONKI, t, ROZMIAR_CZCIONKI_PODZIALKA, false, false);
	}

	delete[] t;
}

void Animacja3D::DodajWierzcholki(){
	void* vert;
	
	//Wierzcholki z ustalonym kolorem (uklad wspolrzednych)
	this->Urzadzenie->CreateVertexBuffer(LICZBA_WIERZCHOLKOW_KOLOR * sizeof(VERTEX_DIFF), 0, D3DFVF_CUSTOMVERTEX_DIFF, D3DPOOL_DEFAULT, &this->WierzcholkiKolor, NULL);

	this->WierzcholkiKolor->Lock(0, sizeof(VertDiff), (void**)&vert, 0);
	memcpy(vert, this->VertDiff, sizeof(this->VertDiff));
	this->WierzcholkiKolor->Unlock();

	//Wierzcholki oswietlane
	this->Urzadzenie->CreateVertexBuffer(LICZBA_WIERZCHOLKOW_SWIATLO * sizeof(VERTEX_LIGHT), 0, D3DFVF_CUSTOMVERTEX_LIGHT, D3DPOOL_DEFAULT, &this->WierzcholkiSwiatlo, NULL);

	this->WierzcholkiSwiatlo->Lock(0, sizeof(VertLight), (void**)&vert, 0);
	memcpy(vert, this->VertLight, sizeof(this->VertLight));
	this->WierzcholkiSwiatlo->Unlock();
}

int Animacja3D::PobierzAntyaliasing(bool PelnyEkran) {
	for (int i = 16; i >= 0; i--) {
		if (this->D3DApi->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, this->DaneEkranu.Format, PelnyEkran, (D3DMULTISAMPLE_TYPE)i, NULL) == D3D_OK) return i;
	}

	return D3DMULTISAMPLE_TYPE::D3DMULTISAMPLE_NONE;
}

void Animacja3D::PrzesunObraz(float x, float y, float z) {
	this->PatrzNa.x += x;
	this->PatrzNa.y += y;
	this->PatrzNa.z += z;

	this->PatrzZ.x += x;
	this->PatrzZ.y += y;
	this->PatrzZ.z += z;

	D3DXMatrixLookAtLH(&this->mcObrazKierunek, &this->PatrzZ, &this->PatrzNa, &this->Gora);
	this->mcObraz = this->mcObrazObrotX * this->mcObrazObrotY * this->mcObrazObrotZ * this->mcObrazKierunek;
	this->Rysuj();
}

void Animacja3D::ObrocObrazX(float alfa) {
	this->ObrazObrotX += alfa;
	D3DXMatrixRotationX(&this->mcObrazObrotX, this->ObrazObrotX);

	this->mcObraz = this->mcObrazObrotX * this->mcObrazObrotY * this->mcObrazObrotZ * this->mcObrazKierunek;
	this->Rysuj();
}

void Animacja3D::ObrocObrazY(float alfa) {
	this->ObrazObrotY += alfa;
	D3DXMatrixRotationY(&this->mcObrazObrotY, this->ObrazObrotY);

	this->mcObraz = this->mcObrazObrotX * this->mcObrazObrotY * this->mcObrazObrotZ * this->mcObrazKierunek;
	this->Rysuj();
}

void Animacja3D::ObrocObrazZ(float alfa) {
	this->ObrazObrotZ += alfa;
	D3DXMatrixRotationZ(&this->mcObrazObrotZ, this->ObrazObrotZ);

	this->mcObraz = this->mcObrazObrotX * this->mcObrazObrotY * this->mcObrazObrotZ * this->mcObrazKierunek;
	this->Rysuj();
}

void Animacja3D::ResetujKamere(){
	this->PatrzNa.x = 0;
	this->PatrzNa.y = 0;
	this->PatrzNa.z = 0;

	this->PatrzZ.x = OKO_X;
	this->PatrzZ.y = OKO_Y;
	this->PatrzZ.z = OKO_Z;

	this->ObrazObrotX = 0;
	this->ObrazObrotY = 0;
	this->ObrazObrotZ = 0;

	D3DXMatrixLookAtLH(&this->mcObrazKierunek, &this->PatrzZ, &this->PatrzNa, &this->Gora);
	D3DXMatrixRotationX(&this->mcObrazObrotX, this->ObrazObrotX);
	D3DXMatrixRotationY(&this->mcObrazObrotY, this->ObrazObrotY);
	D3DXMatrixRotationZ(&this->mcObrazObrotZ, this->ObrazObrotZ);

	this->mcObraz = this->mcObrazObrotX * this->mcObrazObrotY * this->mcObrazObrotZ * this->mcObrazKierunek;
	this->Kamera = AnimKamera::Domyslna;
	this->Rysuj();
}

LRESULT WINAPI wpwndAnimacja(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch (msg) {
	case WM_KEYDOWN: {

		if (wParam == VK_F5 || wParam == VK_ESCAPE) {
			Animacja->ZamknijPelnyEkran();
			return 0;
		}

		Animacja->PrzetworzKlawisz(wParam);

		return 0;
	}

	case WM_DESTROY:
		Animacja->ZamknijPelnyEkran();
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}