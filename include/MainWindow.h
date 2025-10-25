#pragma once
#include "Windows.h"
#include <memory>
#include "BlockScheduler.h"
const wchar_t MainWindow_NAME[] = L"Fractall Application";

class BlockScheduler;

class MainWindow {
public:
	MainWindow(HINSTANCE hInstance, int nCmdShow);
	~MainWindow() = default;
	HWND CreateButtonDEF(
		LPCWSTR text,
		int x,
		int y,
		int width,
		int height,
		int id
		);

	HWND GetHwnd() const { return hwnd; }
	void CreateMandelbrott();
	void CreateJulia();
	void ResetFractals();
	void CreateCoordsStatic(HINSTANCE hInstance);
	HWND CreateTrackbar(int x, int y, int width, int id, const wchar_t* label);
	void SavePicture(HBITMAP BitMap, const char* Name);
	void UpdateColorPhases(int sliderID, double& phaseVar);
private:
	HWND hwnd;
	HWND hwndMandelbrot;
	HWND hwndJulia;
	HWND hwndCoordsStatic;
	HWND hRedSlider;
	HWND hBlueSlider;
	HWND hGreenSlider;
	double redPhase;
	double bluePhase;
	double greenPhase;
	static LRESULT CALLBACK StaticWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void Registed(HINSTANCE hInstance);
	
	void Create(HINSTANCE hInstance, int nCmdShow);
	
	
};
class Mandelbrot {
public:
	Mandelbrot(HINSTANCE hInstance, int nCmdShow);
	~Mandelbrot() = default;
	HWND GetHwnd() const { return hWnd; }
	static void Registed(HINSTANCE hInstance);
private:
	HWND hWnd;
	
	static LRESULT CALLBACK MandelbrotProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
};
class MandelbrottRender {
public:
	void createBitmap(int w, int h);
	HBITMAP GetBitmap() const { return hBitmap; };
	void SetPixel(int x, int y, COLORREF color);
	void SetColorPhase(double r, double b, double g);
	void RenderFromPixels(int startX, int startY, int endX, int endY);
	void Draw();
	double PixelToMathY(int pixelY);
	double PixelToMathX(int pixelX);
	COLORREF CalculateMandelbrot(int x,int y);
	void RenderWorker(BlockScheduler& sheduler);
private:
	double redPhase;
	double bluePhase;
	double greenPhase;
	HBITMAP hBitmap;
	BYTE* pixels;
	int width, height;
	double viewLeft = -2.0, viewRight = 2.0, viewTop = -2.0, viewBottom = 2.0;
};
class Julia {
public:
	Julia(HINSTANCE hInstance, int nCmdShow);
	~Julia() = default;
	HWND GetHwnd() const { return hWnd; }
	static void Registed(HINSTANCE hInstance);
	
private:
	HWND hWnd;
	static LRESULT CALLBACK JuliaProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
class JuliaRender {
public:
	void createBitmap(int w, int h);
	HBITMAP GetBitmap() const { return hBitmap; };
	void RenderFromPixels(int startX, int startY, int endX, int endY);
	void SetColorPhase(double r, double b, double g);
	void SetPixel(int x, int y, COLORREF color);
	void Draw();
	COLORREF CalculateJulia(int x, int y);
    double PixelToMathY(int pixelY);
	double PixelToMathX(int pixelX);
	void RenderWorker(BlockScheduler& sñheduler);

private:
	double redPhase;
	double bluePhase;
	double greenPhase;
	HBITMAP hBitmap;
	BYTE* pixels;
	int width, height;
	double viewLeft = -2.0, viewRight = 2.0, viewTop = -2.0, viewBottom = 2.0;
};

