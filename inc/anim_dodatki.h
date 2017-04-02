#pragma once
#ifndef anim_dodatki_h
#define anim_dodatki_h

#include <Windows.h>
#include "animacja.h"
#include "dodatki.h"
#include "mainwnd.h"
#include "dane.h"

const float KAT_OBROTU = 2.0f * PI / 72.0f;		//5 stopni

void PrzetworzKlawisz(WPARAM wParam);
void RysujCiala();

#endif