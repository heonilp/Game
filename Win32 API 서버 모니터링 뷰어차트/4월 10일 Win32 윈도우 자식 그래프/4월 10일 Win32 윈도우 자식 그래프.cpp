// 4�� 10�� Win32 ������ �ڽ� �׷���.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//
#include "stdafx.h"
#include "4�� 10�� Win32 ������ �ڽ� �׷���.h"
#include "gs_Gragh.h"
#include "hique.h"
#include <MMSystem.h>
//extern const int size = 30;
#define MAX_LOADSTRING 100
#pragma comment(lib,"Winmm.lib")
#define  WM_ALERT1 WM_USER+1
#define  WM_ALERT2 WM_USER+2
#define  WM_ALERT3 WM_USER+3
#define  WM_ALERT4 WM_USER+4
gs_Gragh *p1;
gs_Gragh *p2;
gs_Gragh *p3;
gs_Gragh *p4;
// ���� ����:
HINSTANCE childhInst;
HINSTANCE hInst;
COLORREF background_color = RGB(255, 255, 255);

WNDCLASSEX wcex;
// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void aram(HWND hWnd, WPARAM wParam);
void hOnTimer(HWND hWnd, WPARAM wParam);
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	HACCEL hAccelTable;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY410WIN32, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY410WIN32));

	// �⺻ �޽��� �����Դϴ�.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
//	COLORREF color;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY410WIN32));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	//color = RGB(255, 255, 255);
	//wcex.hbrBackground = (HBRUSH)CreateSolidBrush(color);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_MY410WIN32);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	HBRUSH brush;
	//HDC hMemDC;
	switch (message)
	{
		// RGB(rand() % 256, rand() % 256, rand() % 256)
	case WM_CREATE:
		
		p1 = new gs_Gragh(childhInst, hWnd, RGB(192, 192, 192), 0, 0, 300, 200, TEXT("1"), 2);
		p2 = new gs_Gragh(childhInst, hWnd, RGB(255, 182, 193), 300, 0, 500, 200, TEXT("2"),1);
		p3 = new gs_Gragh(childhInst, hWnd, RGB(214, 214, 214), 0, 200, 800, 300, TEXT("3"),3);
		//gs_Gragh::gs_Gragh(HWND hWnd, HINSTANCE hInstance, COLORREF color, int x, int y, int nWidth, int nHeight, LPCWSTR className)
	
		SetTimer(hWnd, 1, 250, NULL);
		SetTimer(hWnd, 2, 30, NULL);
		SetTimer(hWnd, 3, 90, NULL);
	//	SetTimer(hWnd, 4, 150, NULL);
		break;

	case WM_TIMER:
		hOnTimer(hWnd, wParam);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rt);
		brush = CreateSolidBrush(background_color);
		SelectObject(hdc, brush);
		Rectangle(hdc, rt.left, rt.top, rt.right, rt.bottom);
		EndPaint(hWnd, &ps);
		DeleteObject(brush);
		break;

	case WM_ALERT1://����//"��Ŷ ���۷�";
		background_color = RGB(255, 0, 0);
		InvalidateRect(hWnd, NULL, FALSE);
		//  Ÿ�̸Ӹ� �����ϰ� �մϴ�. WM_TIMER: 1000�� ���ϴ�.
		SetTimer(hWnd, 1000, 100, NULL);
		break;

	case WM_ALERT2: //�Ķ�//"���� �̿��� ��";
		background_color = RGB(0, 0, 255);
		InvalidateRect(hWnd, NULL, FALSE);
	
		SetTimer(hWnd, 1000, 100, NULL);
		break;

	case WM_ALERT3://���//"�޸� ����";
		background_color = RGB(255, 255, 0);
		InvalidateRect(hWnd, NULL, FALSE);
		
		SetTimer(hWnd, 1000, 100, NULL);
		break;

	case WM_COMMAND:
		
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �޴� ������ ���� �м��մϴ�.
		switch (wmId)
		{
			return FALSE;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
	
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


void hOnTimer(HWND hWnd, WPARAM wParam)
{
	//HBRUSH MyBrush, OldBrush;
	HFONT font, oldfont;
	HPEN MyPen, OldPen;
	//wchar_t str[] = L"��Ʈ Test 1234";
	//CreatePen
	//--------------------------------------------------------
	//�ڽ� �������� �̸��� �������ݴϴ�.
	//--------------------------------------------------------
	WCHAR str1[] = L"CPU ����";
	WCHAR str2[] = L"��Ŷ ���۷�";//L"���� �̿��� ��";
	WCHAR str3[] = L"���� �̿��� ��";//L"�޸� ����";

	switch (wParam)
	{
		//--------------------------------------------------------
		//�ٽ� ȭ���� ������� �ٲ��ݴϴ�.
		//--------------------------------------------------------

	case 1000:
		background_color = RGB(255, 255, 255);
		KillTimer(hWnd, 1000);
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case 1:
		//--------------------------------------------------------
		//�����͸� �־��ݴϴ�. �׸��� �˶��Լ��� ȣ���մϴ�.
		//--------------------------------------------------------
		p1->SetintData();
		aram(hWnd, wParam);
		Rectangle(p1->hMemDC, 0, 0, 384, 20);
		//--------------------------------------------------------
		//�׸���� ������ �׷��ݴϴ�.
		//--------------------------------------------------------
		for (int i = 39; i <= 400; i = i + 39)
		{
			MyPen = CreatePen(PS_DOT, 1, RGB(255, 255, 255));
			OldPen = (HPEN)SelectObject(p1->hMemDC, MyPen);
			MoveToEx(p1->hMemDC, i, 20, NULL);
			LineTo(p1->hMemDC, i, 160);
			SelectObject(p1->hMemDC, OldPen);
			DeleteObject(MyPen);
		}
		MyPen = CreatePen(PS_DOT, 1, RGB(255, 255, 255));
		OldPen = (HPEN)SelectObject(p1->hMemDC, MyPen);
		MoveToEx(p1->hMemDC, 0, 123, NULL);
		LineTo(p1->hMemDC, 383, 123);
		MoveToEx(p1->hMemDC, 0, 85, NULL);
		LineTo(p1->hMemDC, 383, 85);
		MoveToEx(p1->hMemDC, 0, 50, NULL);
		LineTo(p1->hMemDC, 383, 50);
		SelectObject(p1->hMemDC, OldPen);
		DeleteObject(MyPen);
		//--------------------------------------------------------
		//y�� ��ġ�� ��Ÿ���ϴ�.
		//--------------------------------------------------------
		TextOut(p1->hMemDC, 1, 147, L"0", 1);
		TextOut(p1->hMemDC, 1, 115, L"15", 2);
		TextOut(p1->hMemDC, 1, 77, L"30", 2);
		TextOut(p1->hMemDC, 1, 43, L"60", 2);
		//--------------------------------------------------------
		//����� ���� �߽��ϴ�.
		//--------------------------------------------------------
		MoveToEx(p1->hMemDC, 0, 0, NULL);
		LineTo(p1->hMemDC, 0, 200);
		MoveToEx(p1->hMemDC, 383, 0, NULL);
		LineTo(p1->hMemDC, 383, 160);
		MoveToEx(p1->hMemDC, 0, 160, NULL);
		LineTo(p1->hMemDC, 383, 160);
		font = CreateFont(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("�������"));
		oldfont = (HFONT)SelectObject(p1->hMemDC, font);
		TextOut(p1->hMemDC, 0, 0, str1, wcslen(str1));
		SelectObject(p1->hMemDC, oldfont);
		SetBkMode(p1->hMemDC, NULL);
		DeleteObject(font);
		break;
	case 2:

		//--------------------------------------------------------
		//�����͸� �־��ݴϴ�. �׸��� �˶��Լ��� ȣ���մϴ�.
		//--------------------------------------------------------
		MyPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		OldPen = (HPEN)SelectObject(p2->hMemDC, MyPen);
		p2->SetintData();
		SelectObject(p2->hMemDC, OldPen);
		DeleteObject(MyPen);
		aram(hWnd, wParam);
		Rectangle(p2->hMemDC, 0, 0, 484, 20);
		//--------------------------------------------------------
		//�׸���� ������ �׷��ݴϴ�.
		//--------------------------------------------------------
		for (int i = 39; i <= 500; i = i + 39)
		{
			MyPen = CreatePen(PS_DOT, 1, RGB(255, 255, 255));
			OldPen = (HPEN)SelectObject(p2->hMemDC, MyPen);
			MoveToEx(p2->hMemDC, i, 20, NULL);
			LineTo(p2->hMemDC, i, 160);
			SelectObject(p2->hMemDC, OldPen);
			DeleteObject(MyPen);
		}
		MyPen = CreatePen(PS_DOT, 1, RGB(255, 255, 255));
		OldPen = (HPEN)SelectObject(p2->hMemDC, MyPen);
		MoveToEx(p2->hMemDC, 0, 123, NULL);
		LineTo(p2->hMemDC, 483, 123);
		MoveToEx(p2->hMemDC, 0, 85, NULL);
		LineTo(p2->hMemDC, 483, 85);
		MoveToEx(p2->hMemDC, 0, 50, NULL);
		LineTo(p2->hMemDC, 483, 50);
		SelectObject(p2->hMemDC, OldPen);
		DeleteObject(MyPen);
		//--------------------------------------------------------
		//y�� ��ġ�� ��Ÿ���ϴ�.
		//--------------------------------------------------------
		TextOut(p2->hMemDC, 1, 147, L"0", 1);
		TextOut(p2->hMemDC, 1, 115, L"30", 2);
		TextOut(p2->hMemDC, 1, 77, L"60", 2);
		TextOut(p2->hMemDC, 1, 43, L"90", 2);
		//--------------------------------------------------------
		//����� ���� �߽��ϴ�.
		//--------------------------------------------------------
		MoveToEx(p2->hMemDC, 0, 0, NULL);
		LineTo(p2->hMemDC, 0, 200);
		MoveToEx(p2->hMemDC, 483, 0, NULL);
		LineTo(p2->hMemDC, 483, 160);
		MoveToEx(p2->hMemDC, 0, 160, NULL);
		LineTo(p2->hMemDC, 483, 160);
		font = CreateFont(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("�������"));
		oldfont = (HFONT)SelectObject(p2->hMemDC, font);
		TextOut(p2->hMemDC, 0, 0, str2, wcslen(str2));
		SelectObject(p2->hMemDC, oldfont);
		SetBkMode(p2->hMemDC, NULL);
		DeleteObject(font);
		break;
	case 3:
		//--------------------------------------------------------
		//�����͸� �־��ݴϴ�. �׸��� �˶��Լ��� ȣ���մϴ�.
		//--------------------------------------------------------
		MyPen = CreatePen(PS_SOLID, 1, RGB(0, 0,80));
		OldPen = (HPEN)SelectObject(p3->hMemDC, MyPen);
		//p3->Data = rand() % 90;
		p3->SetintData();
		SelectObject(p3->hMemDC, OldPen);
		DeleteObject(MyPen);
		aram(hWnd, wParam);
		Rectangle(p3->hMemDC, 0, 0, 784, 20);
		//--------------------------------------------------------
		//�׸���� ������ �׷��ݴϴ�.
		//--------------------------------------------------------
		for (int i = 39; i <= 670; i = i + 39)
		{
			MyPen = CreatePen(PS_DOT, 1, RGB(255, 255, 255));
			OldPen = (HPEN)SelectObject(p3->hMemDC, MyPen);
			MoveToEx(p3->hMemDC, i, 20, NULL);
			LineTo(p3->hMemDC, i, 260);
			SelectObject(p3->hMemDC, OldPen);
			DeleteObject(MyPen);
		}
		MyPen = CreatePen(PS_DOT, 1, RGB(255, 255, 255));
		OldPen = (HPEN)SelectObject(p3->hMemDC, MyPen);
		MoveToEx(p3->hMemDC, 0, 123, NULL);
		LineTo(p3->hMemDC, 667, 123);
		MoveToEx(p3->hMemDC, 0, 85, NULL);
		LineTo(p3->hMemDC, 667, 85);
		MoveToEx(p3->hMemDC, 0, 50, NULL);
		LineTo(p3->hMemDC, 667, 50);
		SelectObject(p3->hMemDC, OldPen);
		DeleteObject(MyPen);
		//--------------------------------------------------------
		//y�� ��ġ�� ��Ÿ���ϴ�.
		//--------------------------------------------------------
		//TextOut(p3->hMemDC, 1, 147, L"0", 1);
		//TextOut(p3->hMemDC, 1, 115, L"20", 2);
		//TextOut(p3->hMemDC, 1, 77, L"50", 2);
		//TextOut(p3->hMemDC, 1, 43, L"80", 2);
		//--------------------------------------------------------
		//����� ���� �߽��ϴ�.
		//--------------------------------------------------------
		MoveToEx(p3->hMemDC, 0, 0, NULL);
		LineTo(p3->hMemDC, 0, 260);
		MoveToEx(p3->hMemDC, 783, 0, NULL);
		LineTo(p3->hMemDC, 783, 260);
		MoveToEx(p3->hMemDC, 0, 260, NULL);
		LineTo(p3->hMemDC, 783, 260);

		font = CreateFont(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("�������"));
		oldfont = (HFONT)SelectObject(p3->hMemDC, font);
		TextOut(p3->hMemDC, 0, 0, str3, wcslen(str3));
		SelectObject(p3->hMemDC, oldfont);
		SetBkMode(p3->hMemDC, NULL);
		break;
	}
}


void aram(HWND hWnd, WPARAM wParam)
{
	//--------------------------------------------------------
	//�˸��Լ� - �溸���� ������ �� ���� ���� �޽��� ����
	//--------------------------------------------------------
	if (p1->GetintData() >= 99)
	{
		SendMessage(hWnd, WM_ALERT1, wParam, 0);
		PlaySound(L".\\WavFiles\\test.wav", NULL, SND_ASYNC);
	}

	if (p2->GetintData() >= 99)
	{
		SendMessage(hWnd, WM_ALERT2, wParam, 0);
		PlaySound(L".\\WavFiles\\test.wav", NULL, SND_ASYNC);
	}

	if (p3->GetintData() >= 99)
	{
		SendMessage(hWnd, WM_ALERT3, wParam, 0);
		PlaySound(L".\\WavFiles\\test.wav", NULL, SND_ASYNC);
	}
}