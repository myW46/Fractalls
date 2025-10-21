#include "MainWindow.h"
#include "ConstDefine.h"
#include <windowsx.h>
#include <shobjidl.h>     
#include <objbase.h> 
#include <string>
#include <shlwapi.h>      
#pragma comment(lib, "shlwapi.lib")  

const wchar_t Name[] = L"Application";
const wchar_t Name_Mandelbrott[] = L"Mandelbrot";
const wchar_t Name_Julia[] = L"Julia";

HWND hwndCoordsStatic;
POINT g_dragStart, g_staticStart;
bool g_isDragging = false;
static WNDPROC g_oldStaticProc = nullptr;
int g_mouseStartX, g_mouseStartY;
int g_mouseEndX, g_mouseEndY;
bool g_isSelecting = false;
int g_CurrentX = 0;
int g_CurrentY = 0;


LRESULT CALLBACK CoordsStaticProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_LBUTTONDOWN: {
        SetCapture(hWnd);
        g_isDragging = true;
        g_dragStart.x = GET_X_LPARAM(lParam);
        g_dragStart.y = GET_Y_LPARAM(lParam);
        RECT rc;
        GetWindowRect(hWnd, &rc);
        POINT top = { rc.left, rc.top };
        ScreenToClient(GetParent(hWnd), &top);
        g_staticStart = top;
        return 0;
    }
    case WM_MOUSEMOVE: {
        if (g_isDragging && GetCapture() == hWnd) {
          
            RECT oldRect;
            GetWindowRect(hWnd, &oldRect);

            POINT pt;
            GetCursorPos(&pt);
            ScreenToClient(GetParent(hWnd), &pt);
            int newX = (pt.x - g_dragStart.x);
            int newY = (pt.y - g_dragStart.y);

            
            SetWindowPos(hWnd, NULL, newX, newY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

            MapWindowPoints(NULL, GetParent(hWnd), (POINT*)&oldRect, 2);
            InvalidateRect(GetParent(hWnd), &oldRect, TRUE);
            RedrawWindow(GetParent(hWnd), &oldRect, NULL,
                RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);

            return 0;
        }
        break;
    }
    case WM_LBUTTONUP: {
        if (g_isDragging) {
            g_isDragging = false;
            ReleaseCapture();
            return 0;
        }
        break;
    }
    }
    if (g_oldStaticProc) return CallWindowProc(g_oldStaticProc, hWnd, uMsg, wParam, lParam);
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

MainWindow::MainWindow(HINSTANCE hInstance, int nCmdShow) {
    Registed(hInstance);
    Create(hInstance, nCmdShow);
}

Mandelbrot::Mandelbrot(HINSTANCE hInstance, int nCmdShow) {
    Registed(hInstance);
}

Julia::Julia(HINSTANCE hInstance, int nCmdShow) {
    Registed(hInstance);
}

void Mandelbrot::Registed(HINSTANCE hInstance) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = MandelbrotProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = Name_Mandelbrott;
    RegisterClass(&wc);
}

void Julia::Registed(HINSTANCE hInstance) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = JuliaProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = Name_Julia;
    RegisterClass(&wc);
}

void MainWindow::Registed(HINSTANCE hInstance) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = StaticWindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = Name;
    RegisterClass(&wc);
}

void MainWindow::Create(HINSTANCE hInstance, int nCmdShow) {
    hwnd = CreateWindowEx(
        0,
        Name,
        L"Application",
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT, 1920, 1080,
        NULL, NULL, hInstance, this
    );
    if (!hwnd) return;
    hwndCoordsStatic = CreateWindow(
        L"STATIC",
        L"Координаты: X: 0, Y: 0",
        WS_CHILD | WS_VISIBLE | SS_NOTIFY ,
        10, 10, 220, 30,
        hwnd,
        (HMENU)ID_COORDS_STATIC,
        hInstance,
        NULL
    );
    if (hwndCoordsStatic) {
        g_oldStaticProc = (WNDPROC)SetWindowLongPtr(hwndCoordsStatic, GWLP_WNDPROC, (LONG_PTR)CoordsStaticProc);
    }
    hRedSlider = CreateTrackbar(250, 100, 200, ID_RED_SLIDER, L"Red Phase");
hGreenSlider = CreateTrackbar(250, 150, 200, ID_GREEN_SLIDER, L"Green Phase");  
hBlueSlider = CreateTrackbar(250, 200, 200, ID_BLUE_SLIDER, L"Blue Phase");
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
}

void MainWindow::CreateMandelbrott() {
    HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
    static MandelbrottRender renderer;
    renderer.createBitmap(800, 800);
    renderer.Draw();
    HWND hwndMandelbrott = CreateWindowEx(
        0,
        Name_Mandelbrott,
        L"Mandelbrot",
        WS_VISIBLE | WS_CHILD | WS_BORDER,
        600, 0, 800, 800,
        hwnd, NULL, hInstance, NULL
    );
    if (hwndMandelbrott) SetWindowLongPtr(hwndMandelbrott, GWLP_USERDATA, (LONG_PTR)&renderer);
}

void MainWindow::CreateJulia() {
    HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
    static JuliaRender renderer;
    renderer.createBitmap(800, 800);
    renderer.Draw();
    HWND hwndJulia = CreateWindowEx(
        0,
        Name_Julia,
        L"Julia",
        WS_VISIBLE | WS_CHILD | WS_BORDER,
        600, 0, 800, 800,
        hwnd, NULL, hInstance, NULL
    );
    if (hwndJulia) SetWindowLongPtr(hwndJulia, GWLP_USERDATA, (LONG_PTR)&renderer);
}

LRESULT CALLBACK MainWindow::StaticWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    MainWindow* pThis;
    if (uMsg == WM_NCCREATE) {
        pThis = static_cast<MainWindow*>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
        pThis->hwnd = hwnd;
    }
    else {
        pThis = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }
    if (pThis) return pThis->WindowProc(uMsg, wParam, lParam);
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK Mandelbrot::MandelbrotProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    
    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        MandelbrottRender* renderer = (MandelbrottRender*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
        if (renderer) {
            HDC hdcMem = CreateCompatibleDC(hdc);
            SelectObject(hdcMem, renderer->GetBitmap());
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);
            BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, hdcMem, 0, 0, SRCCOPY);
            DeleteDC(hdcMem);
            
        }

        if (g_isSelecting) {

            HPEN hRedPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
            HPEN hOldPen = (HPEN)SelectObject(hdc, hRedPen);
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
           
            
            Rectangle(hdc,
                min(g_mouseStartX, g_CurrentX), min(g_mouseStartY, g_CurrentY),
                max(g_mouseStartX, g_CurrentX), max(g_mouseStartY, g_CurrentY)
            );
            SelectObject(hdc, hOldPen);
            SelectObject(hdc, hOldBrush);
            DeleteObject(hRedPen);
        }
        EndPaint(hWnd, &ps);
        
            return 0;
    }
    case WM_MOUSEMOVE: {
         g_CurrentX = GET_X_LPARAM(lParam);
         g_CurrentY = GET_Y_LPARAM(lParam);
        HWND hParent = GetParent(hWnd);
        if (hParent) SendMessage(hParent, WM_UPDATE_COORDS, (WPARAM)g_CurrentX, (LPARAM)g_CurrentY);
        InvalidateRect(hWnd, NULL, TRUE);
        break;

    }
    case WM_LBUTTONDOWN: {
        g_mouseStartX = LOWORD(lParam);
        g_mouseStartY = HIWORD(lParam);
        g_isSelecting = true;
        SetCapture(hWnd);

        break;
    }
    case WM_LBUTTONUP: {
        
        if (g_isSelecting) {
            g_mouseEndX = LOWORD(lParam);
            g_mouseEndY = HIWORD(lParam);
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);


            bool startPointValid = (g_mouseStartX >= 0 && g_mouseStartX < clientRect.right &&
                g_mouseStartY >= 0 && g_mouseStartY < clientRect.bottom);
            bool endPointValid = (g_mouseEndX >= 0 && g_mouseEndX < clientRect.right &&
                g_mouseEndY >= 0 && g_mouseEndY < clientRect.bottom);
            g_isSelecting = false;
            if (startPointValid && endPointValid) {
                MandelbrottRender* renderer = (MandelbrottRender*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
                if (renderer) {
                    renderer->RenderFromPixels(g_mouseStartX, g_mouseStartY, g_mouseEndX, g_mouseEndY);
                    InvalidateRect(hWnd, NULL, TRUE);
                }
            }
            ReleaseCapture();
        }
        break;
    }

    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    
    }
    return 0;
}

LRESULT CALLBACK Julia::JuliaProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        JuliaRender* renderer = (JuliaRender*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
        if (renderer) {
            HDC hdcMem = CreateCompatibleDC(hdc);
            SelectObject(hdcMem, renderer->GetBitmap());
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);
            BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, hdcMem, 0, 0, SRCCOPY);
            DeleteDC(hdcMem);
        }
        if (g_isSelecting) {

            HPEN hRedPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
            HPEN hOldPen = (HPEN)SelectObject(hdc, hRedPen);
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));


            Rectangle(hdc,
                min(g_mouseStartX, g_CurrentX), min(g_mouseStartY, g_CurrentY),
                max(g_mouseStartX, g_CurrentX), max(g_mouseStartY, g_CurrentY)
            );
            SelectObject(hdc, hOldPen);
            SelectObject(hdc, hOldBrush);
            DeleteObject(hRedPen);
        }
        EndPaint(hWnd, &ps);

      

        return 0;
    }
    case WM_MOUSEMOVE: {
        g_CurrentX = GET_X_LPARAM(lParam);  
        g_CurrentY = GET_Y_LPARAM(lParam);
        HWND hParent = GetParent(hWnd);
        if (hParent) SendMessage(hParent, WM_UPDATE_COORDS, (WPARAM)g_CurrentX, (LPARAM)g_CurrentY);
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
    
    case WM_LBUTTONDOWN: {
        g_mouseStartX = LOWORD(lParam);
        g_mouseStartY = HIWORD(lParam);
        g_isSelecting = true;
        SetCapture(hWnd);

        break;
    }
    case WM_LBUTTONUP: {

        if (g_isSelecting) {
            g_mouseEndX = LOWORD(lParam);
            g_mouseEndY = HIWORD(lParam);
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);


            bool startPointValid = (g_mouseStartX >= 0 && g_mouseStartX < clientRect.right &&
                g_mouseStartY >= 0 && g_mouseStartY < clientRect.bottom);
            bool endPointValid = (g_mouseEndX >= 0 && g_mouseEndX < clientRect.right &&
                g_mouseEndY >= 0 && g_mouseEndY < clientRect.bottom);
            g_isSelecting = false;
            if (startPointValid && endPointValid) {
                JuliaRender* renderer = (JuliaRender*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
                if (renderer) {
                    renderer->RenderFromPixels(g_mouseStartX, g_mouseStartY, g_mouseEndX, g_mouseEndY);
                    InvalidateRect(hWnd, NULL, TRUE);
                }
            }
            ReleaseCapture();
        }
        break;
    }
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}

LRESULT MainWindow::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        return 0;
    case WM_DRAWITEM: {
        LPDRAWITEMSTRUCT dis = (LPDRAWITEMSTRUCT)lParam;

        if (dis->CtlID == ID_COORDS_BUTTON) {
            HBRUSH hBrush = GetSysColorBrush(COLOR_BTNFACE);
            FillRect(dis->hDC, &dis->rcItem, hBrush);

            SetBkMode(dis->hDC, TRANSPARENT);
            wchar_t text[100];
            GetWindowText(dis->hwndItem, text, 100);
            DrawText(dis->hDC, text, -1, &dis->rcItem,
                DT_LEFT | DT_VCENTER | DT_SINGLELINE);

            return TRUE;
        }
        break;
    }
    case WM_ERASEBKGND: {
        HDC hdc = (HDC)wParam;
        RECT rc;
        GetClientRect(hwnd, &rc);
        HBRUSH hBrush = GetSysColorBrush(COLOR_BTNFACE);
        FillRect(hdc, &rc, hBrush);
        return 1;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_BUTTON_JULIA_DRAW:
            ResetFractals();
            CreateJulia();
            break;
        case ID_BUTTON_MANDELBROTT_DRAW:
            ResetFractals();
            CreateMandelbrott();
            break;
        
        case ID_BUTTON_RESET:

            ResetFractals();
            break;
        case ID_BUTTON_SAVE_FILE: {
            HWND hMandelbrot = FindWindowEx(hwnd, NULL, Name_Mandelbrott, NULL);
            HWND hJulia = FindWindowEx(hwnd, NULL, Name_Julia, NULL);
            if (hMandelbrot) {

                MandelbrottRender* renderer = (MandelbrottRender*)GetWindowLongPtr(hMandelbrot, GWLP_USERDATA);
                SavePicture(renderer->GetBitmap(), "mandelbrot.bmp");
            }
            else if (hJulia) {

                JuliaRender* renderer = (JuliaRender*)GetWindowLongPtr(hJulia, GWLP_USERDATA);

                SavePicture(renderer->GetBitmap(), "julia.bmp");

            }

            break;
        }
        }
        return 0;
    case WM_UPDATE_COORDS: {
        int x = (int)wParam;
        int y = (int)lParam;
        wchar_t buffer[128];
        swprintf(buffer, 128, L"Координаты: X: %d, Y: %d", x, y);
        SetWindowText(hwndCoordsStatic, buffer);
        return 0;
    }
    case WM_HSCROLL: {
        HWND hSlider = (HWND)lParam;
        int phase = (int)SendMessage(hSlider, TBM_GETPOS, 0, 0);
        if (hSlider == hRedSlider) {
            UpdateColorPhases(ID_RED_SLIDER, redPhase);
        }
        else if (hSlider == hBlueSlider) {
            UpdateColorPhases(ID_BLUE_SLIDER, bluePhase);
        }
        else if (hSlider == hGreenSlider) {
            UpdateColorPhases(ID_GREEN_SLIDER, greenPhase);
        }
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

HWND MainWindow::CreateButtonDEF(LPCWSTR text, int x, int y, int width, int height, int id) {
    HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
    HWND hw = CreateWindow(
        L"BUTTON",
        text,
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        x, y, width, height,
        hwnd,
        (HMENU)(INT_PTR)id,
        hInstance,
        NULL
    );
    return hw;
}
void MainWindow::ResetFractals() {
   
        HWND hMandelbrot = FindWindowEx(hwnd, NULL, Name_Mandelbrott, NULL);
        if (hMandelbrot) {
            DestroyWindow(hMandelbrot);
        }
        HWND hJulia = FindWindowEx(hwnd, NULL, Name_Julia, NULL);
        if (hJulia) {
            DestroyWindow(hJulia);
        }
        g_isSelecting = false;
        g_mouseStartX = 0;
        g_mouseStartY = 0;
        g_mouseEndX = 0;
        g_mouseEndY = 0;
        g_CurrentX = 0;
        g_CurrentY = 0;

        SetWindowText(hwndCoordsStatic, L"Координаты: X: 0, Y: 0");
    }

void MainWindow::SavePicture(HBITMAP BitMap  , const char* Name) {

    IFileSaveDialog* p_file_save;
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, (void**)&p_file_save);
    COMDLG_FILTERSPEC fileTypes[] = {
        {L"Bitmap Images", L"*.bmp"},
        {L"All Files", L"*.*"}
    };
    p_file_save->SetFileTypes(2, fileTypes);
    p_file_save->SetDefaultExtension(L"bmp");
    if (SUCCEEDED(p_file_save->Show(hwnd))) {
        IShellItem* p_Item;
        p_file_save->GetResult(&p_Item);
        PWSTR file_path;
        p_Item->GetDisplayName(SIGDN_FILESYSPATH, &file_path);


        BITMAP bm;
        GetObject(BitMap, sizeof(BITMAP), &bm);

        int width = bm.bmWidth;
        int height = bm.bmHeight;
        BITMAPFILEHEADER btf = {};
        btf.bfType = 0x4D42;
        btf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (width * height * 4);
        btf.bfReserved1 = 0;
        btf.bfReserved2 = 0;
        btf.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


        BITMAPINFOHEADER bts = { 0 };
        bts.biSize = sizeof(BITMAPINFOHEADER);
        bts.biWidth = width;
        bts.biHeight = height;
        bts.biPlanes = 1;
        bts.biBitCount = 32;
        bts.biCompression = BI_RGB;
        HANDLE hFile = CreateFile(file_path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        WriteFile(hFile, &btf, sizeof(btf), NULL, NULL);
        WriteFile(hFile, &bts, sizeof(bts), NULL, NULL);

        HDC hdc = GetDC(NULL);
        void* pixels = malloc(width*height*4);
        GetDIBits(hdc, BitMap, 0, height, pixels, (BITMAPINFO*)&bts, DIB_RGB_COLORS);
        WriteFile(hFile, pixels, width * height * 4, NULL, NULL);
        free(pixels);
        ReleaseDC(NULL, hdc);
        CloseHandle(hFile);


        CoTaskMemFree(file_path);  
        p_Item->Release();
    }
    p_file_save->Release();  
    CoUninitialize();   

    MessageBox(hwnd, L"Файл сохранен!", L"Успех", MB_OK);
}
void MandelbrottRender::SetColorPhase(double r, double b, double g) {
    redPhase = r;
    bluePhase = b;
    greenPhase = g;
};
void JuliaRender::SetColorPhase(double r, double b, double g) {
    redPhase = r;
    bluePhase = b;
    greenPhase = g;
};

HWND MainWindow::CreateTrackbar(int x, int y, int width, int id, const wchar_t* label) {
    HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
    CreateWindow(L"STATIC",
        label,
        WS_CHILD | WS_VISIBLE | SS_NOTIFY,
        1920-x, y - 19, 50, 20,
        hwnd,
        NULL,
        hInstance,
        NULL);
    HWND hSlider = CreateWindow(TRACKBAR_CLASS,
        L"",
        WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_BOTH,
        1920-x, y, width, 30,
        hwnd, (HMENU)(INT_PTR)id, hInstance, NULL);

    SendMessage(hSlider, TBM_SETRANGE, TRUE, MAKELONG(-157, 157));
    SendMessage(hSlider, TBM_SETPOS, TRUE, 0);
    SendMessage(hSlider, TBM_SETPAGESIZE, 0, 10);
    return hSlider;

}

void MainWindow::UpdateColorPhases(int sliderID, double& phaseVar) {
    HWND hSlider = GetDlgItem(hwnd, sliderID);
    int value = SendMessage(hSlider, TBM_GETPOS, 0, 0);
    phaseVar = value / 100.0;

    
    HWND hMandelbrot = FindWindowEx(hwnd, NULL, Name_Mandelbrott, NULL);
    HWND hJulia = FindWindowEx(hwnd, NULL, Name_Julia, NULL);

    if (hMandelbrot) {
        MandelbrottRender* renderer = (MandelbrottRender*)GetWindowLongPtr(hMandelbrot, GWLP_USERDATA);
        if (renderer) {
            renderer->SetColorPhase(redPhase, greenPhase, bluePhase);
            renderer->Draw();
            InvalidateRect(hMandelbrot, NULL, TRUE);
        }
        return;
    }

   
    if (hJulia ) {
        JuliaRender* renderer = (JuliaRender*)GetWindowLongPtr(hJulia, GWLP_USERDATA);
        if (renderer) {
            renderer->SetColorPhase(redPhase, greenPhase, bluePhase);
            renderer->Draw();
            InvalidateRect(hJulia, NULL, TRUE);
        }
        return;
    }
}