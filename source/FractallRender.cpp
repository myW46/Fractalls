#include "MainWindow.h"
#include "complex.h"

void MandelbrottRender::createBitmap(int w, int h){
	width = w;
	height = h;
	HDC hdcScreen = GetDC(NULL);
	BITMAPINFO bmi={0};
	
	bmi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = -h;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	
	
	hBitmap = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS,(void**)&pixels, NULL, 0);
	ReleaseDC(NULL, hdcScreen);

	viewLeft = -2.0;
	viewRight = 2.0;
	viewTop = -2.0;
	viewBottom =2.0;
}
void JuliaRender::createBitmap(int w, int h) {
	width = w;
	height = h;
	HDC hdcScreen = GetDC(NULL);
	BITMAPINFO bmi = { 0 };

	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = -h;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;


	hBitmap = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
	ReleaseDC(NULL, hdcScreen);
	viewLeft = -2.0;
	viewRight = 2.0;
	viewTop = -2.0;
	viewBottom = 2.0;
}
void MandelbrottRender::Draw() {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			COLORREF color = CalculateMandelbrot(x, y);
			SetPixel(x, y, color);
		
		}
	}
}
void MandelbrottRender::SetPixel(int x, int y, COLORREF color) {
	int index = (y*width  + x) * 4;
	pixels[index + 0] = GetBValue(color);
	pixels[index + 1] = GetGValue(color);
	pixels[index + 2] = GetRValue(color);
	pixels[index + 3] = 255;
}
COLORREF MandelbrottRender::CalculateMandelbrot(int x, int y) {
	double cx = viewLeft + (x / (double)width) * (viewRight - viewLeft);
	double cy = viewTop + (y / (double)height) * (viewBottom - viewTop);
	Complex c(cx, cy);
	Complex z(0.0, 0.0);
	int it = 0;
	int maxiter = 1000;
	while (z.ABS() <  4.0 && it < maxiter) {
		z = z*z+c;
		it++;
	}
	if (it == maxiter) {
		return RGB(0,0,0);
	}
	else {
		int r = (it * 5) % 255;
		int g = (it * 7) % 255;
		int b = (it * 11) % 255;
		return RGB(r, g, b);
	}

}
void MandelbrottRender::RenderFromPixels(int startX, int startY, int endX, int endY) {
	double mathStartX = PixelToMathX(min(startX, endX));
	double mathEndX = PixelToMathX(max(startX, endX));
	double mathStartY = PixelToMathY(min(startY, endY));
	double mathEndY = PixelToMathY(max(startY, endY));
	
	viewLeft = mathStartX;
	viewRight = mathEndX;
	viewTop = mathStartY;
	viewBottom = mathEndY;
	Draw();

}
void JuliaRender::Draw() {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			COLORREF color = CalculateJulia(x, y);
			SetPixel(x, y, color);

		}
	}

}

COLORREF JuliaRender::CalculateJulia(int x, int y) {
	double cx = viewLeft + (x / (double)width) * (viewRight - viewLeft);
	double cy = viewTop + (y / (double)height) * (viewBottom - viewTop);
	Complex z(cx, cy);
	Complex c(0.0+0.549, 0.0+0.0);
	int it = 0;
	int maxiter = 1000;
	while (z.ABS() < 4.0 && it < maxiter) {
		z = z * z*z*z*z+ c;
		it++;
	}
	if (it == maxiter) {
		return RGB(0, 0, 0);
	}
	else {
		int r = (it * 5) % 255;
		int g = (it * 7) % 255;
		int b = (it * 11) % 255;
		return RGB(r, g, b);
	}

}
void JuliaRender::SetPixel(int x, int y, COLORREF color) {
	int index = (y * width + x) * 4;
	pixels[index + 0] = GetBValue(color);
	pixels[index + 1] = GetGValue(color);
	pixels[index + 2] = GetRValue(color);
	pixels[index + 3] = 255;
}
double MandelbrottRender::PixelToMathX(int pixelX) {

	return viewLeft + (pixelX / (double)width) * (viewRight - viewLeft);
}

double MandelbrottRender::PixelToMathY(int pixelY) {
	return viewTop + (pixelY / (double)height) * (viewBottom - viewTop);
}
void JuliaRender::RenderFromPixels(int startX, int startY, int endX, int endY) {
	double mathStartX = PixelToMathX(min(startX, endX));
	double mathEndX = PixelToMathX(max(startX, endX));
	double mathStartY = PixelToMathY(min(startY, endY));
	double mathEndY = PixelToMathY(max(startY, endY));
	
	viewLeft = mathStartX;
	viewRight = mathEndX;
	viewTop = mathStartY;
	viewBottom = mathEndY;
	Draw();

}

double JuliaRender::PixelToMathX(int pixelX) {

	return viewLeft + (pixelX / (double)width) * (viewRight - viewLeft);
}

double JuliaRender::PixelToMathY(int pixelY) {
	return viewTop + (pixelY / (double)height) * (viewBottom - viewTop);
}