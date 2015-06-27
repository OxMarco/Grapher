#include "stdafx.h"
#include "func.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <CommDlg.h>
#include <WindowsX.h>
#include "graph.h"
#include <sstream>
using namespace std;
#define MAX_LOADSTRING 100
#define M_PI  3.141592653589793238462643
// Variabili globali:
HINSTANCE hInst;								// istanza corrente
TCHAR szTitle[MAX_LOADSTRING];					// Testo della barra del titolo
TCHAR szWindowClass[MAX_LOADSTRING];			// nome della classe di finestre principale

// Dichiarazioni con prototipo delle funzioni incluse in questo modulo di codice:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DlgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK riepilogo(HWND hWndD, UINT msg, WPARAM wParam, LPARAM lParam);

int test_inputbox2(wstring text);
wstring test_inputbox(bool bMultiLine = false);
inline void plot(HWND hWnd, HDC console, wstring str, COLORREF rgb);
inline void assi(HDC console);
void limiti(HDC console);
void apri(HWND hWnd, wstring file);
void salva(wstring file); 
void dialog(BOOL apr);
wstring dialog2();
int bitmap(HWND hWnd, vector<wstring> str, string nome, int Max, int Min, int org, int org2, int scl);
inline void scale(HDC console, HWND hWnd, int spazio);
inline void griglia(HDC console, HWND hWnd, int spazio);

int MIN_DOM = 0;
int MAX_DOM = 360;
vector<wstring> str;
COLORREF rgb[6] = { RGB(0, 170, 5), RGB(255, 0, 0), RGB(0, 0, 255), RGB(255, 105, 180), RGB(255, 148, 0), RGB(205, 127, 50) };
int Ymax = 0, Ymin = 0, org = 1580, org2 = 1348;
bool grl = FALSE;
wstring sft;
HPEN hPenOld, hLinePen;

//funzioni esterne
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	HACCEL hAccelTable;
	
	// Inizializzare le stringhe globali
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_FUNC, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Eseguire l'inizializzazione dall'applicazione:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FUNC));

	// Ciclo di messaggi principale:
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

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FUNC));
	wcex.hCursor		= LoadCursor(NULL, IDC_CROSS);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_FUNC);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Memorizzare l'handle di istanza nella variabile globale

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, SW_SHOWMAXIMIZED);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent, iPosX, iPosY;
	PAINTSTRUCT ps;
	HDC hdc;
	wstring stringa;
	POINT pp;
	char *nome = (char*)malloc(401);
	static int VscrollPos, HscrollPos, spazio = 1;
    TEXTMETRIC tm;
	UINT uMessage, wScrollNotify;
	HBRUSH hOldBrush, brush;

	switch (message)
	{ 
	case WM_CONTEXTMENU:
		{
			HMENU hMenu = CreatePopupMenu();	
	        AppendMenu(hMenu, MFT_STRING, IDM_UNDO, L"&Annulla" );
			AppendMenu(hMenu, MFT_STRING, IDM_GRAPH, L"&Nuovo" );
			AppendMenu(hMenu, MFT_STRING, IDM_FNZ, L"&Riepilogo" );
			AppendMenu(hMenu, MFT_SEPARATOR, 0, NULL);
			AppendMenu(hMenu, MFT_STRING, IDM_EXIT, L"&Esci" );
			GetCursorPos(&pp);
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pp.x, pp.y, 0, hWnd, 0);
		    DestroyMenu(hMenu);
		}
		break;

	case WM_LBUTTONDOWN:
		GetCursorPos(&pp);
		iPosX = pp.x - org;
		iPosY = (pp.y - org2 - 40 )/57;
		wchar_t coord[20];
		wsprintf(coord, TEXT("(%i , %i)"), iPosX, -iPosY);
		MessageBox(hWnd, coord, TEXT("Coordinate"), MB_OK | MB_ICONASTERISK);
		
		break;

	case WM_PARSERERR:
		MessageBox(hWnd, TEXT("Carattere non valido. Per ulteriori informazioni leggere il file error.log"), TEXT("Errore Parser"),                 MB_OK | MB_ICONERROR);
		break;

	case WM_ERR:
		MessageBox(hWnd, (LPCWSTR)wParam, TEXT("Errore"), MB_ICONERROR | MB_OK );
		if(lParam == 1)
		exit(1);
		break;

	case WM_CREATE:
          hdc = GetDC(hWnd);
            GetTextMetrics(hdc, &tm);
               SetScrollRange(hWnd, SB_VERT, 0, 2000, FALSE); //barra verticale
               SetScrollPos(hWnd, SB_VERT, 1348, TRUE);
			SetScrollRange(hWnd, SB_HORZ, 0, 3000, FALSE); //barra orizzzontale
            SetScrollPos(hWnd, SB_HORZ, 1582, TRUE);
		 VscrollPos = org2;
		 HscrollPos = org;
		break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Analizzare le selezioni di menu:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;

		case IDM_ORG:
			org = 1582;
			org2 = 1348;
			 SetScrollPos(hWnd, SB_VERT, 1348, TRUE);
			 SetScrollPos(hWnd, SB_HORZ, 1582, TRUE);
			break;

		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;

		case IDM_SAVE:
		dialog(FALSE);
		break;

		case IDM_BMP:
		sft = dialog2();
		if(sft == L"0")
			break;
		wcstombs(nome, sft.c_str(), 400);
		if(bitmap(hWnd, str, nome, MAX_DOM, MIN_DOM,org2, org, 1))
			SendMessage(hWnd, WM_ERR, (WPARAM)L"Errore Salvataggio", 0);
		sft.clear();
		break;

		case IDM_APRI:
			if(str.size() != 0)
				if(MessageBox(hWnd, TEXT("Salvare la sessione attuale?"), TEXT("Avviso"), MB_YESNO | MB_ICONQUESTION) == IDYES)
					dialog(FALSE);

		str.clear();
        dialog(TRUE);
		InvalidateRect(hWnd, 0, TRUE);
		break;

		case IDM_GRAPH:
			if(str.size() == 5){
				SendMessage(hWnd, WM_ERR, (WPARAM) L"Numero Massimo Grafici Raggiunto", 0);
				break;
			}
			stringa = test_inputbox();
			str.push_back(stringa);
			stringa.clear();
			InvalidateRect(hWnd, 0, TRUE);
			break;
			
		case IDM_SCALA:
				spazio = test_inputbox2(L"Modifica la scala numerica dell' asse");
				  if(spazio > 8 || spazio <= 0){
			       spazio = 1;
				   SendMessage(hWnd, WM_ERR, (WPARAM)"Il valore inserito supera i limiti di rappresentabilità(1 e 8)", 0);
				  }
			 if(spazio != 1){
				CheckMenuItem(GetMenu(hWnd), IDM_SCALA, MF_CHECKED);
			} else {
				CheckMenuItem(GetMenu(hWnd), IDM_SCALA, MF_UNCHECKED);               
			}
			InvalidateRect(hWnd, 0, TRUE);
			break;

		case IDM_VAL:
			if(!grl){
			CheckMenuItem(GetMenu(hWnd), IDM_VAL, MF_CHECKED);
            grl = TRUE;
			} else {
			CheckMenuItem(GetMenu(hWnd), IDM_VAL, MF_UNCHECKED);
            grl = FALSE;
			}
			InvalidateRect(hWnd, 0, TRUE);
			break;

		case IDM_CLEAR:
			spazio = 1;
			str.clear();
			Ymax = Ymin = 0;
			InvalidateRect(hWnd, 0, TRUE);
			break;

		case IDM_FNZ:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_PRIMARYFAMILY_DLG),
	          hWnd, reinterpret_cast<DLGPROC>(riepilogo));
			InvalidateRect(hWnd, 0, TRUE);
			break;

		case IDM_DOM:
			MIN_DOM = test_inputbox2(L"Inserire nuovo valore minimo dominio(gradi)");
			MAX_DOM = test_inputbox2(L"Inserire nuovo valore massimo dominio(gradi)");
			if(MAX_DOM > 1000 || MAX_DOM < -1000){
				SendMessage(hWnd, WM_ERR,(WPARAM) L"Il valore inserito supera i limiti di rappresentabilità[-1000, 1000]", 0);
				MAX_DOM = 360;
			} else if (MIN_DOM > 1000 || MIN_DOM < -1000) {
			    SendMessage(hWnd, WM_ERR,(WPARAM) L"Il valore inserito supera i limiti di rappresentabilità[-1000, 1000]", 0);
				MIN_DOM = 0;
		    } else if(MIN_DOM > MAX_DOM) {
				int temp = MIN_DOM;
				MIN_DOM = MAX_DOM;
				MAX_DOM = temp;
			} 
			InvalidateRect(hWnd, 0, TRUE);
			break;

		case IDM_UNDO:
			if(str.size() == 0){
				SendMessage(hWnd, WM_ERR, (WPARAM)L"Impossibile annullare", 0);
				break;
			}
		    str.pop_back();
			spazio = 1;
			Ymax = Ymin = 0;
			InvalidateRect(hWnd, 0, TRUE);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_PAINT:

		hdc = BeginPaint(hWnd, &ps);

			    EnableWindow(hWnd, TRUE);
				
				SetMapMode(hdc, MM_TEXT);

			if(!spazio)
		      spazio = 1;
			else if(spazio > 8)
              spazio = 8;

		if(grl){
	      hLinePen = CreatePen(PS_SOLID, 0, RGB(210,210,210));
          hPenOld = (HPEN)SelectObject(hdc, hLinePen);	
              griglia(hdc, hWnd, spazio*2);  	
	      SelectObject(hdc, hPenOld);
          DeleteObject(hLinePen);	
		}

		 assi(hdc);

		 	for(unsigned int i = 0; i < str.size(); i++ ){		
			brush = CreateSolidBrush(rgb[i]);
			hOldBrush = (HBRUSH)SelectObject(hdc, brush);
		  plot(hWnd, hdc, str[i], rgb[i]);
		}
     SelectObject(hdc, hOldBrush);
     DeleteObject(brush);

	 	scale(hdc, hWnd, spazio);

          EndPaint (hWnd, &ps) ;
		break;

	case WM_VSCROLL:
            switch (LOWORD(wParam))
            {
			case SB_TOP:
				 VscrollPos = 1000;
                 break;
			case SB_BOTTOM:
				 VscrollPos = -1000;
                 break;
            case SB_LINEUP:
                 VscrollPos -= 5;
                 break;
            case SB_LINEDOWN:
                 VscrollPos += 5;
                 break;
            case SB_PAGEUP:
                 VscrollPos -= 90;
                 break;
            case SB_PAGEDOWN:
                 VscrollPos += 90;
                 break;
            case SB_THUMBPOSITION:
                 VscrollPos = HIWORD(wParam);
                 break;
            default:
                 break;
            }

            VscrollPos = max(0, min(VscrollPos, 2000));

            if (VscrollPos != GetScrollPos(hWnd, SB_VERT)) {
                SetScrollPos(hWnd, SB_VERT, VscrollPos, TRUE);
                InvalidateRect(hWnd, NULL, TRUE);
            }
			org2 = VscrollPos - 1000;
            break;

	case WM_HSCROLL:
            switch (LOWORD(wParam))
            {
			case SB_TOP:
				 HscrollPos = 1000;
                 break;
			case SB_BOTTOM:
				 HscrollPos = -1000;
                 break;
            case SB_LINEUP:
                 HscrollPos -= 5;
                 break;
            case SB_LINEDOWN:
                 HscrollPos += 5;
                 break;
            case SB_PAGEUP:
                 HscrollPos -= 70;
                 break;
            case SB_PAGEDOWN:
                 HscrollPos += 70;
                 break;
            case SB_THUMBPOSITION:
                 HscrollPos = HIWORD(wParam);
                 break;
            default:
                 break;
            }

            HscrollPos = max(0, min(HscrollPos, 3000));

            if (HscrollPos != GetScrollPos(hWnd, SB_HORZ)) {
                SetScrollPos(hWnd, SB_HORZ, HscrollPos, TRUE);
                InvalidateRect(hWnd, NULL, TRUE);
            }
			org = HscrollPos - 1000;
            break;

	case WM_KEYDOWN:
      switch (wParam) 
      {
        case VK_UP:
            wScrollNotify = SB_LINEUP;
            uMessage = WM_VSCROLL;
            break;

        case VK_PRIOR:    //PAGEUP key
            wScrollNotify = SB_PAGEUP;
            uMessage = WM_VSCROLL;
            break;

        case VK_NEXT:     // PAGEDOWN key
            wScrollNotify = SB_PAGEDOWN;
            uMessage = WM_VSCROLL; 
            break;

        case VK_DOWN:
            wScrollNotify = SB_LINEDOWN;
            uMessage = WM_VSCROLL;
            break;

        case VK_HOME:
            wScrollNotify = SB_BOTTOM;
            uMessage = WM_HSCROLL;
            break;

        case VK_END:
            wScrollNotify = SB_TOP;
            uMessage = WM_HSCROLL;
            break;

        case VK_RIGHT:
            wScrollNotify = SB_LINEDOWN;
            uMessage = WM_HSCROLL;
            break;

        case VK_LEFT:
            wScrollNotify = SB_LINEUP;
            uMessage = WM_HSCROLL;
            break;

		case VK_ADD:
			spazio++;
			break;
			
		case VK_SUBTRACT:
			spazio--;
			break;

        default:
            wScrollNotify = 0xFFFF;
            break;
     }
     if (wScrollNotify != -1)
         SendMessage(hWnd, uMessage, MAKELONG(wScrollNotify, 0), 0L);
	  InvalidateRect(hWnd, 0, TRUE);
     break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
// Gestore dei messaggi della finestra Informazioni su.
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
LRESULT CALLBACK riepilogo(HWND hWndD, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND lbxPrimaryFamily;
    lbxPrimaryFamily = GetDlgItem(hWndD, IDC_PRIMARYFAMILY_LB);
	int iSelectedIndex;	
	wstring dtl;
	vector<wstring>::iterator j;
    switch(msg)
    {
    case WM_INITDIALOG:
	for(register unsigned int l = 0; l <str.size(); l++){
				sft = L"f(x) = " + str[l];
			SendMessage(lbxPrimaryFamily, LB_ADDSTRING, 0, (LPARAM)sft.c_str());
			}
        SendMessage(hWndD, WM_SETICON, IDI_FUNC, (LPARAM)LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION)));
	return TRUE;

  case WM_COMMAND:
	switch(LOWORD(wParam))
	{
		case IDOK:
	    EndDialog(hWndD, 0);
        break;

	case IDC_PRIMARYFAMILY_LB:
		{
		switch (HIWORD(wParam))
			case LBN_DBLCLK:
		// Getting the index of the item that was double-clicked and deleting it
		iSelectedIndex = ListBox_GetCurSel(lbxPrimaryFamily);
		ListBox_GetText(lbxPrimaryFamily,
			        iSelectedIndex,
			        sft.c_str());
		
		ListBox_DeleteString(lbxPrimaryFamily, iSelectedIndex);

		for(register unsigned int m = 0; m<str.size(); m++){    
			dtl = L"f(x) = " + str[m];
			j = str.begin()+m;
			     if( dtl == sft)
		             str.erase(j);
		}
		return TRUE;
	    }
	}
	return TRUE;

    case WM_CLOSE:
        PostQuitMessage(WM_QUIT);
        break;
    }

    return FALSE;
}
void plot(HWND hWnd, HDC console, wstring str, COLORREF rgb){

	inline double parser(HWND hWnd, float x, wstring str); //prototipo funzione esterna
	  wchar_t a[401];

	 for(register unsigned int k = 0; k<=str.length(); k++)
		 a[k] = str[k];
	 a[str.size()]=0; 

     int i = 0;
	while (a[i] != '\0')
	{
		if (a[i] >= 0x41 && a[i] <= 0x5A)
			(a[i] = a[i] ^ 0x20);
		i++;
	}

	Ymin = Ymax = parser(hWnd, MIN_DOM*M_PI/180, a);
	
	 register double y;
       for(register float x = MIN_DOM; x<MAX_DOM; x++){
		       y = parser(hWnd, x*M_PI/180, a);
			   y = (int) (-y*180/M_PI);
			 ShowCursor(TRUE);
			PatBlt(console,x+org,y+org2,1,1,PATCOPY);

		   Ymax = max(Ymax, y);
		   Ymin = min(Ymin, y); 
	   }  
}
void salva(wstring file){
		wofstream os(file, ios::binary);

		if (!os)
           {
           SendMessage(NULL, WM_ERR, (WPARAM) L"Impossibile salvare il file",0);
           return;
           }

            for(register unsigned int i=0; i<str.size(); i++)
             os<<str[i]<<";";
            
            os.close();
}
void apri(wstring file){

	    wstring lst;
	    wifstream  is(file, ios::binary);

           if (!is)
           {
           SendMessage(NULL, WM_ERR, (WPARAM)L"Impossibile aprire il file",0);
           return;
           }

	      while(!is.eof())
          {
            getline(is, lst, L';');
	        str.push_back(lst);
          }
		  str.pop_back();
            is.close();
}
void dialog(BOOL apr)
{

TCHAR szFilter[] = TEXT ("Graphics Data Files (*.dat)\0*.dat\0")  \
                               TEXT ("Tutti i file (*.*)\0*.*\0\0");
	TCHAR szDefExtention[] = TEXT("dat\0");

	static COSFDialog osfDlg;

	if(apr)
	{
		if(osfDlg.FileOpenDlg(szFilter, szDefExtention,TEXT("Apertura file dati"),  TRUE))
				apri(osfDlg.GetFileName());
		SetFocus(NULL);
	}
	else
	{
		if(osfDlg.FileSaveDlg(szFilter, szDefExtention,TEXT("Salvataggio file dati")))
		        salva(osfDlg.GetFileName());
		SetFocus(NULL);
	}
}
wstring dialog2()
{

TCHAR szFilter[] = TEXT ("BITMAP (*.bmp)\0*.bmp\0")  \
                               TEXT ("Tutti i file (*.*)\0*.*\0\0");
	TCHAR szDefExtention[] = TEXT("bmp\0");

	static COSFDialog osfDlg;

	SetFocus(NULL);

		if(osfDlg.FileSaveDlg(szFilter, szDefExtention,TEXT("Salvataggio file immagine"))){
		        return osfDlg.GetFileName();
		} else {
		return L"0"; //in caso di errore
		}
}
void assi(HDC console){   //assi cartesiani di riferimento
	MoveToEx(console, org, org2, NULL);
	LineTo(console, 1500, org2);
	MoveToEx(console,org, org2, NULL);
	LineTo(console, org, 1000);
	MoveToEx(console,org, org2, NULL);
	LineTo(console, org, -1000);
	MoveToEx(console, org, org2, NULL);
	LineTo(console, -1500, org2);
}
void scale(HDC console, HWND hWnd, int spazio) {
	RECT rect;
	GetWindowRect(hWnd, &rect);
  int width = rect.right - rect.left;
  int height = rect.bottom - rect.top;
  int count = 0;
  int step = MAX_DOM / spazio;
  wstringstream wss;

   for (register int i=org; i < width ; i += step) {
        MoveToEx(console, i, org2, NULL); 
        LineTo(console, i, org2+4);  
		wss.str(L"");
		wss<<count;
		TextOut(console, i-5, org2+6, wss.str().c_str(), wss.str().length());
		count += MAX_DOM / spazio;              //semiasse positivo delle ascisse
		MoveToEx(console, i, org2, NULL); 
        LineTo(console, i, org2-4); 
   }
      count = 0;
   for (int l=org; l >= 0; l -= step) {
        MoveToEx(console, l, org2, NULL); 
        LineTo(console, l,org2+4); 
		MoveToEx(console, l, org2, NULL);  //semiasse negativo delle ascisse
        LineTo(console, l, org2-4); 
		wss.str(L"");
		wss<<count;
		TextOut(console, l-6 ,org2+4, wss.str().c_str(), wss.str().length());
		count -= MAX_DOM / spazio; 
   }

      step = 57;           

      count = 0;
   for (register int j=org2; j < height ; j += step) {
        MoveToEx(console, org, j, NULL); 
        LineTo(console, org+4, j); 
		wss.str(L"");
		wss<<count;
		TextOut(console, org+6, j-8, wss.str().c_str(), wss.str().length());
	    count -= 1;                     //semiasse negativo delle ordinate
		MoveToEx(console, org, j, NULL); 
        LineTo(console, org-4, j); 
   }
   count = 0;
   for (int k=org2; k >= 0; k -= step){
        MoveToEx(console, org, k, NULL); 
        LineTo(console, org+4, k);    
		wss.str(L"");         //semiasse positivo delle ordinate
		wss<<count;
		TextOut(console, org+6, k-6, wss.str().c_str(), wss.str().length());
		count += 1;
		MoveToEx(console, org, k, NULL); 
        LineTo(console, org-4, k); 
   }
}
void griglia(HDC console, HWND hWnd, int tempo) {
		if(tempo > 13)
		tempo = 12;
	RECT rect;
   GetWindowRect(hWnd, &rect);
  int width = rect.right - rect.left;
  int height = rect.bottom - rect.top;
  int step = MAX_DOM / tempo;

   for (register int i=org; i < width ; i += step) {
        MoveToEx(console, i, org2, NULL); 
        LineTo(console, i, org2+1900);  //semiasse ascisse positivo
		MoveToEx(console, i, org2, NULL); 
        LineTo(console, i, org2-1900);
   }
   
   for (int l=org; l >= 0; l -= step) {
        MoveToEx(console, l, org2 , NULL);  // |   |
        LineTo(console, l, org2+1700);      // |-1_|_0   semiasse ascisse negativo
		MoveToEx(console, l, org2 , NULL); //  |   |
        LineTo(console, l, org2-1700);
   }

   step = 57/(tempo/2);

   for (register int j=org2; j < height ; j += step) {
        MoveToEx(console, org, j, NULL);            //  ________ 0|___________
        LineTo(console, org+2200, j);               //  ________-1|___________  semiasse ordinate negaivo
		MoveToEx(console, org, j, NULL); 
        LineTo(console, org-2000, j); 
   }

   for (int k=org2; k >= 0; k -= step){              // ________1|__________
        MoveToEx(console, org, k, NULL);             // ________0|__________  semiasse ordinate positivo
        LineTo(console, org+2200, k);                             
		MoveToEx(console, org, k, NULL); 
        LineTo(console, org-2000, k);
   }
   limiti(console);
}
void limiti(HDC hdc){
	for(register int v = 0; v < 1500; v = v+3) PatBlt(hdc, v, Ymax+org2, 1, 1, BLACKNESS);
	for(register int z = 0; z < 1500; z = z+3) PatBlt(hdc, z, Ymin+org2, 1, 1, BLACKNESS);
}