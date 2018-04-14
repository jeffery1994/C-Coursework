#pragma once
#ifndef PRILIB_H
#define PRILIB_H
#include <iostream>
#include <Windows.h>

using namespace std;

//Set UI title
void My_SetTitle(char* title);

//Set UI color
void My_SetColor(int forecolor, int backcolor);

//
void My_SetPosition(int x, int y);

#endif // !PrisonerLib_H_INCLUDED;
