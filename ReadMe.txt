========================================================================
    APPLICAZIONE WIN32: cenni preliminari sul progetto 
    func
========================================================================

La creazione guidata applicazione ha creato questa applicazione 
func.

Questo file contiene un riepilogo del contenuto di ciascun file che fa parte
dell'applicazione func.


func.vcxproj
    File di progetto principale per i progetti VC++ generati tramite una 
    creazione guidata applicazione.
    Contiene informazioni sulla versione di Visual C++ che ha generato il file e
    informazioni sulle piattaforme, le configurazioni e le caratteristiche del 
    progetto selezionate con la Creazione guidata applicazione.

func.vcxproj.filters
    File dei filtri per i progetti VC++ generati tramite una Creazione guidata 
    applicazione. 
    Contiene informazioni sull'associazione tra i file del progetto e i filtri. 
    Tale associazione viene utilizzata nell'IDE per la visualizzazione di
    raggruppamenti di file con estensioni simili in un nodo specifico, ad 
    esempio: i file con estensione cpp sono associati al filtro "File di 
    origine".

func.cpp
    File di origine principale dell'applicazione.

/////////////////////////////////////////////////////////////////////////////
La creazione guidata applicazione ha creato le seguenti risorse:

func.rc
    Elenco di tutte le risorse Microsoft Windows utilizzate dal
    programma. Include le icone, le bitmap e i cursori memorizzati
    nella sottodirectory RES. Questo file può essere modificato direttamente
    in Microsoft Visual C++.

Resource.h
    File di intestazione standard che definisce i nuovi ID risorse.
    Tale file viene letto e aggiornato da Microsoft Visual C++.

func.ico
    File di icona utilizzato come icona dell'applicazione (32x32).
    Tale icona è inclusa dal file di risorse principale func.rc.

small.ico
    File di icona contenente una versione di dimensioni inferiori (16x16)
    dell'icona dell'applicazione. Tale icona è inclusa dal file di risorse
    principale func.rc.

/////////////////////////////////////////////////////////////////////////////
Altri file standard:

StdAfx.h, StdAfx.cpp
    Tali file vengono utilizzati per compilare un file di intestazione
    precompilato denominato func.pch e un file dei tipi 
    precompilato denominato StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Altre note:

La creazione guidata applicazione utilizza i commenti "TODO:" per indicare le
parti del codice sorgente da aggiungere o personalizzare.

/////////////////////////////////////////////////////////////////////////////

 case WM_VSCROLL:
            {
            switch (LOWORD(wParam))
            {
            case SB_LINEUP:
                 VscrollPos -= 1;
                 break;
            case SB_LINEDOWN:
                 VscrollPos += 1;
                 break;
            case SB_PAGEUP:
                 VscrollPos -= cyClient / cyChar;
                 break;
            case SB_PAGEDOWN:
                 VscrollPos += cyClient / cyChar;
                 break;
            case SB_THUMBPOSITION:
                 VscrollPos = HIWORD(wParam);
                 break;
            default:
                 break;
            }

            VscrollPos = max(0, min(VscrollPos, LINES - 1));

            if (VscrollPos != GetScrollPos(hwnd, SB_VERT)) {
                SetScrollPos(hwnd, SB_VERT, VscrollPos, TRUE);
                InvalidateRect(hwnd, NULL, TRUE);
            }
            break;