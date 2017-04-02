//#define laptop

//#define czajniczek
#pragma warning( disable : 4244 )	//konwersja double->float

#include <Windows.h>
#include <mmsystem.h>
#include <math.h>
#include "dane.h"
#include "dodatki.h"
#include "fizyka1.h"
#include "fizyka.h"
#include "mainwnd.h"

#pragma once
#ifndef animacja_h
#define animacja_h

const float OKO_X = 5.0f;
const float OKO_Y = 5.0f;
const float OKO_Z = -20.0f;

const float PI = 3.1415f;
const float PODZIALKA_DLUGOSC = 0.25f;

const int ILOSC_WIERSZY = 20;
const int TROJKATY_WIERSZ = 20;
const int TROJKATY_PRZESUNIECIE = 9;

const int ILOSC_TEKSTUR = 10;
const int ROZMIAR_CZCIONKI_OPIS = 10;
const int ROZMIAR_CZCIONKI_PODZIALKA = 1;
const double PODZIALKA_PRZYTNIJ = 0.5;

const int LICZBA_WIERZCHOLKOW_SWIATLO = TROJKATY_PRZESUNIECIE + ILOSC_WIERSZY * 2 * (2 + TROJKATY_WIERSZ);
const int LICZBA_WIERZCHOLKOW_KOLOR = 6;

const LPCSTR NWNDANIMACJA = "wndAnimacja";
const LPCSTR TWNDANIMACJA = "Fizyka 3D";

const int OKNO_SZER = 600;
const int OKNO_WYS = 600;

const LPCSTR NAZWA_CZCIONKI = "Arial";

#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )

#ifndef laptop
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#endif
#ifdef laptop
#include <C:\Program Files (x86)\Microsoft DirectX SDK (August 2008)\Include\d3dx9.h>
#pragma comment(lib, "C:\\Program Files (x86)\\Microsoft DirectX SDK (August 2008)\\Lib\\x64\\d3d9.lib")
#pragma comment(lib, "C:\\Program Files (x86)\\Microsoft DirectX SDK (August 2008)\\Lib\\x64\\d3dx9.lib")
#endif

LRESULT WINAPI wpwndAnimacja(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//Wierzcholki
struct VERTEX_DIFF{
	FLOAT x, y, z;
	DWORD color;
};

struct VERTEX_LIGHT {
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	FLOAT tx;
	FLOAT ty;
};

#define D3DFVF_CUSTOMVERTEX_DIFF (D3DFVF_XYZ|D3DFVF_DIFFUSE)
#define D3DFVF_CUSTOMVERTEX_LIGHT (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

enum AnimKolor { Niebieski, Zielony, Czerwony };
enum AnimKamera { Domyslna, WysrodkujNaCiele, WKierunkuPredkosci };

struct UstawieniaSceny {
	double JednIlosc;
	string JednNazwa;

	double Jednostka0;
	double UstJednPodzialka;
};

struct DaneCiala {
	bool puste;
	double promien;
	AnimKolor kolor;
	string nazwa;
	LPD3DXMESH nazwa_m = NULL;
};

class DaneGrafiki {
private:
	Punkt* Ciala = NULL;
	DaneCiala* DaneCial = NULL;
	int Ilosc = 0;
	void Czysc();

public:
	~DaneGrafiki();
	int PobierzIlosc() { return this->Ilosc; };
	inline Punkt* PobierzWskCiala() { return this->Ciala; };
	inline DaneCiala* PobierzWskDaneCial() { return this->DaneCial; };
	void UstawIlosc(int ilosc);
};

class Animacja3D {
private:
	LPDIRECT3D9             D3DApi = NULL;
	LPDIRECT3DDEVICE9       Urzadzenie = NULL;
	LPDIRECT3DVERTEXBUFFER9 WierzcholkiKolor = NULL;
	LPDIRECT3DVERTEXBUFFER9 WierzcholkiSwiatlo = NULL;
	LPDIRECT3DTEXTURE9		Tekstury[ILOSC_TEKSTUR] = { 0 };
	LPD3DXMESH*				PodzialkaOpis = NULL;

	int IloscOpisowPodzialki = 0;

	HWND OknoGlowne;
	HWND OknoAnimacja;
	HINSTANCE Instancja;
	UstawieniaSceny Ustawienia;

	CRITICAL_SECTION DostepDoUrzadzenia;

	bool OknoZminimalizowane = false;

	//Wierzcholki
	VERTEX_DIFF VertDiff[LICZBA_WIERZCHOLKOW_KOLOR];
	VERTEX_LIGHT VertLight[LICZBA_WIERZCHOLKOW_SWIATLO];

#ifdef czajniczek
	LPD3DXMESH Czajniczek = NULL;
#endif

	void Czysc();
	void CzyscUrzadzenie();
	void PobierzDaneEkranu();
	void RejestrujOknoAnimacji();
	void UtworzOknoAnimacji();
	void UtworzUrzadzenieOkno();
	void UtworzUrzadzeniePelnyEkran();
	void UtworzMacierze();
	void UtworzMaterialy();
	void UtworzWierzcholki();
	void UtworzTekstury();
	void CzyscOpisyPodzialki();
	void UtworzOpisyPodzialki();
	void DodajWierzcholki();
	void UtworzTekst3D(LPD3DXMESH* ppMesh, LPCSTR pstrFont, LPCSTR tekst, DWORD dwSize, BOOL bBold, BOOL bItalic);

	int PobierzAntyaliasing(bool PelnyEkran);

	int ile_tekstur = 1;

	int AntyAlOkno;
	int AntyAlPelnyEkran;

	int okno_szer = OKNO_SZER;
	int okno_wys = OKNO_WYS;

	D3DXVECTOR3 Gora = { 0.0f, 1.0f, 0.0f };
	D3DXVECTOR3 PatrzNa;
	D3DXVECTOR3 PatrzZ;
	float ObrazObrotX = 0;
	float ObrazObrotY = 0;
	float ObrazObrotZ = 0;
	bool PelnyEkran = false;

	D3DDISPLAYMODE DaneEkranu;

	D3DMATERIAL9 mtrNiebieski;
	D3DMATERIAL9 mtrZielony;
	D3DMATERIAL9 mtrCzerwony;
	D3DMATERIAL9 mtrBialy;
	D3DMATERIAL9 mtrPurpurowy;
	D3DMATERIAL9 mtrCzarny;

	D3DXMATRIXA16 mcProjekcja;
	D3DXMATRIXA16 mcObraz;
	D3DXMATRIXA16 mcObrazKierunek;
	D3DXMATRIXA16 mcObrazObrotX;
	D3DXMATRIXA16 mcObrazObrotY;
	D3DXMATRIXA16 mcObrazObrotZ;
	D3DXMATRIXA16 mcBezPrzeksztalcen;
	D3DXMATRIXA16 mcPerspektywa;
	D3DXMATRIXA16 mcSkaluj1;

public:
	Animacja3D();
	~Animacja3D();
	HRESULT Inicjuj(HWND hWnd, HINSTANCE hInstance, void(*PrzetworzKlawisz_)(WPARAM wParam));
	void Rysuj();

	DaneGrafiki DaneGr;
	AnimKamera Kamera = AnimKamera::Domyslna;
	D3DXVECTOR3 KameraPatrzNa{ 0.0, 0.0, 0.0 };
	D3DXVECTOR3 KameraPatrzZ{ 0.0, 0.0, 0.0 };
	
	void(*PrzetworzKlawisz)(WPARAM wParam);
	void PrzesunObraz(float x, float y, float z);
	void ObrocObrazX(float alfa);
	void ObrocObrazY(float alfa);
	void ObrocObrazZ(float alfa);
	void ResetujKamere();
	void UstawPelnyEkran(bool PelnyEkran);
	bool CzyPelnyEkran() { return this->PelnyEkran; }
	HWND PobierzOknoAnimacji() { return this->OknoAnimacja; }
	void UstawRozmiarObrazu(int width, int height);
	void ZamknijPelnyEkran();
	void UstawUstawienia(UstawieniaSceny ust);
	D3DXVECTOR3 PobierzKatyObrotu() { return D3DXVECTOR3(this->ObrazObrotX, this->ObrazObrotY, this->ObrazObrotZ); }
	D3DXVECTOR3 PobierzKamerePatrzNa() { return this->PatrzNa; }
	D3DXVECTOR3 PobierzKamerePatrzZ() { return this->PatrzZ; }
	void UstawKamere(D3DXVECTOR3& Obroty, D3DXVECTOR3& PatrzNa, D3DXVECTOR3& PatrzZ);
};

#endif