#include "stdafx.h"
#include "4월 10일 Win32 윈도우 자식 그래프.h"
#include "gs_Gragh.h"

//구조체로 정보를 저장함.
miniGragh childgragh[5];
gs_Gragh *Gpoint;


gs_Gragh::gs_Gragh(HINSTANCE hInstance, HWND hWnd, COLORREF color, int x, int y, int nWidth, int nHeight, LPCWSTR className, int type)
{

	Type = type;
	//그래프의 타입을 정해줍니다.

	static int icount;
	//DWORD
	//COLORREF
	//LoadString(hInst, IDC_MY410WIN32, WindowClassc, MAX);

	WNDCLASSEX wcex;
	HDC hdc;
	m_color = color;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)CreateSolidBrush(color);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = className;
	wcex.hIconSm = NULL;
	RegisterClassEx(&wcex);
	//||CAPTION| WS_CLIPSIBLINGS | WS_CAPTION | WS_CLIPSIBLINGS
	hWndChild = CreateWindow(className, NULL, WS_CHILD | WS_VISIBLE | WS_CAPTION | WS_CLIPSIBLINGS, x, y, nWidth, nHeight, hWnd, NULL, hInstance, NULL);
	//이름 //이름 // 스타일            // 좌표             // 부모 , 메뉴 인스턴스 lpParam
	childgragh[icount].pointer = this;
	childgragh[icount].hWnd = hWndChild;
	icount++;


	GetClientRect(hWnd, &crt);

	hdc = GetDC(hWnd);

	if (hBit == NULL) {
		hBit = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
	}

	hMemDC = CreateCompatibleDC(hdc);

	OldBit = (HBITMAP)SelectObject(hMemDC, hBit);

	ReleaseDC(hWnd, hdc);
	
}


gs_Gragh* gs_Gragh::point(HWND hWnd)
{
	for (int icount = 0; icount < 5; icount++)
	{
		if (childgragh[icount].hWnd == hWnd)
		{
			return childgragh[icount].pointer;
		}
	}
	return NULL;
}



LRESULT CALLBACK gs_Gragh::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	PAINTSTRUCT ps;
	HDC hdc;

	Gpoint = point(hWnd);
	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		// TODO: 여기에 그리기 코드를 추가합니다.
		GetClientRect(hWnd, &Gpoint->crt);
		Gpoint->OldBit = (HBITMAP)SelectObject(Gpoint->hMemDC, Gpoint->hBit);
		BitBlt(hdc, 0, 0, Gpoint->crt.right, Gpoint->crt.bottom, Gpoint->hMemDC, 0, 0, SRCCOPY);
		SelectObject(Gpoint->hMemDC, Gpoint->OldBit);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		if (Gpoint->hBit)
		{
			DeleteObject(Gpoint->hBit);
		}
		SelectObject(Gpoint->hMemDC, Gpoint->OldBit);
		DeleteObject(Gpoint->OldBit);
		DeleteDC(Gpoint->hMemDC);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


int gs_Gragh::GetintData()
{
	Data = rand() % 100;
	return Data;
}

void  gs_Gragh::SetintData()
{
	GetintData();
	///Gpoint = point(hWnd);
	if (QUE.Put(Data) == FALSE)
	{
		QUE.Get(NULL);
		QUE.Put(Data);
	}
	if (Type == 1)
	{
		onPaint(hWndChild);
	}

	if (Type == 2)
	{
		RectPaint(hWndChild);

	}

	if (Type == 3)
	{
		MutiPaint(hWndChild);
	}
}


void gs_Gragh::onPaint(HWND hWnd){

	FillRect(hMemDC, &crt, (HBRUSH)CreateSolidBrush(m_color));
	
	int which = 0;
	int iXcount = 0;
	
	//선 그래프

	which = QUE.read;
	MoveToEx(hMemDC, 0, QUE.queue[QUE.read] + 40, NULL);
	while (which != QUE.write)
	{
		LineTo(hMemDC, iXcount, QUE.queue[which] + 40);
		iXcount = iXcount + 5;
		which = (which + 1) % QUE.size;
	}

	InvalidateRect(hWnd, NULL, FALSE);
	
}



//--
//2
//--
void gs_Gragh::RectPaint(HWND hWnd){
	HPEN MyPen, OldPen;
	FillRect(hMemDC, &crt, (HBRUSH)CreateSolidBrush(m_color));

	MyPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	OldPen = (HPEN)SelectObject(hMemDC, MyPen);
	Rectangle(hMemDC, 100, 162, 200, QUE.TailGet());
	SelectObject(hMemDC, OldPen);
	DeleteObject(MyPen);

	InvalidateRect(hWnd, NULL, FALSE);

}


//--
//3
//--
void gs_Gragh::MutiPaint(HWND hWnd){

	FillRect(hMemDC, &crt, (HBRUSH)CreateSolidBrush(m_color));

	int which = 0;
	int iXcount = 0;

	//선 그래프

	which = QUE.read;
	MoveToEx(hMemDC, 0, QUE.queue[QUE.read] + 40, NULL);
	while (which != QUE.write)
	{
		LineTo(hMemDC, iXcount, QUE.queue[which] + 40);
		iXcount = iXcount + 5;
		which = (which + 1) % QUE.size;
	}

	InvalidateRect(hWnd, NULL, FALSE);

}