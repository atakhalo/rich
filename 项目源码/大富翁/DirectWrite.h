//ver 1.0	Create by ChitoseRuri
#ifndef DIRECT_WRITE_H
#define DIRECT_WRITE_H

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <string.h>
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
#include "d3dUtil.h"

class DirectWrite
{
public:
	DirectWrite();
	~DirectWrite();
	void GetSwapChain(IDXGISwapChain* swapChain);
	void Draw(wchar_t * text, D2D1::ColorF color, XMFLOAT2 &leftUp, XMFLOAT2 &rightDown);
	bool SetTextAlignment();//设置文本对齐方式
	bool SetParagraphAlignment();//设置段落对齐方式
	bool SetFont(wchar_t * wFont = L"Gabriola", FLOAT  fontSize = 72.0f, DWRITE_FONT_WEIGHT fontWeight = DWRITE_FONT_WEIGHT_REGULAR, 
		IDWriteFontCollection * pFontCollection = nullptr,	DWRITE_FONT_STYLE  fontStyle = DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH  fontStretch = DWRITE_FONT_STRETCH_NORMAL,wchar_t * localeName = L"en-us");	//设置字体
	bool SetDigxBuff();
	HRESULT Initialize(IDXGISwapChain * swapChain, HWND hwnd);
	void PrepareResize();

private:
	wchar_t * CharToWchar(char * chars);

private:
	HWND mHwnd;
	// Direct2D
	ID2D1Factory* mpD2DFactory;
	ID2D1RenderTarget * mpRenderTarget;
	ID2D1SolidColorBrush* mpColorBrush;
		
	// DirectWrite
	IDWriteFactory* mpDWriteFactory;
	IDWriteTextFormat* mpTextFormat;
	IDXGISwapChain* mpSwapChain;

	float mDpiScaleX;
	float mDpiScaleY;
};
#endif // !DIRECT_WRITE_H

