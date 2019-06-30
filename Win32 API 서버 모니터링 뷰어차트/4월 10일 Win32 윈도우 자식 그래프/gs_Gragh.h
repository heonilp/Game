#ifndef _GS_GRAGH_H__
#define _GS_GRAGH_H__
#include "hique.h"

class gs_Gragh
{
private:
	CQue <int> QUE;
	int Data;
public:
	int Type;
	HWND hWndChild;
	RECT crt;
	HBITMAP OldBit, hBit;
	HDC hMemDC;
	COLORREF m_color;
	//	CreateWindow(L"listbox", L"Click Me", WS_CHILD | WS_VISIBLE | WS_TILEDWINDOW,
	//	10, 50, 100, 500, hWnd, (HMENU)0, hInst, NULL);
	gs_Gragh(HINSTANCE hInstance, HWND hWnd, COLORREF color, int x, int y, int nWidth, int nHeight, LPCWSTR className, int type);
	
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static gs_Gragh* point(HWND hWnd);

	void onTimer(HWND hWnd);
	void onPaint(HWND hWnd);
	void RectPaint(HWND hWnd);
	void MutiPaint(HWND hWnd);
	void SetintData();
	int GetintData();
};
struct miniGragh
{
	HWND hWnd;
	gs_Gragh *pointer;
};
/*
struct st_Gragh_Type
{
	ULONG64 ServerID;
	int Type; //->어떤 데이터;
	WCHAR szname[20];

};

*/

#endif