#include "complex.h"
#include "AppManager.h"
#include "MainWindow.h"
#include "ConstDefine.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR , int nCmdShow) {
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	MainWindow win(hInstance, nCmdShow);
	MSG msg = {};
	
	win.CreateButtonDEF(L"draw Mandelbrot", 23, 230, 200, 40, ID_BUTTON_MANDELBROTT_DRAW);
	win.CreateButtonDEF(L"draw Julia", 250, 230, 200, 40, ID_BUTTON_JULIA_DRAW);
	win.CreateButtonDEF(L"reset", 23, 280,428, 40, ID_BUTTON_RESET);
	Mandelbrot::Registed(hInstance);
	Julia::Registed(hInstance);
	while (GetMessage(&msg, nullptr, 0, 0)) {
		
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}