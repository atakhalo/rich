#include "DirectWrite.h"

DirectWrite::DirectWrite()
{
	mpD2DFactory = nullptr;
	mpColorBrush = nullptr;
	mpDWriteFactory = nullptr;
	mpTextFormat = nullptr;
	mpSwapChain = nullptr;
	mpRenderTarget = nullptr;
	HDC screen = GetDC(0);
	mDpiScaleX = GetDeviceCaps(screen, LOGPIXELSX) / 96.0f;
	mDpiScaleY = GetDeviceCaps(screen, LOGPIXELSY) / 96.0f;
	ReleaseDC(nullptr, screen);
}

DirectWrite::~DirectWrite()
{
	ReleaseCOM(mpRenderTarget);
	ReleaseCOM(mpColorBrush);
	ReleaseCOM(mpD2DFactory);
	ReleaseCOM(mpDWriteFactory);
	ReleaseCOM(mpTextFormat);
}

void DirectWrite::GetSwapChain(IDXGISwapChain * pSwapChain)
{
	mpSwapChain = pSwapChain;
}

void DirectWrite::Draw(wchar_t * text, D2D1::ColorF color, XMFLOAT2 &leftUp, XMFLOAT2 &rightDown)
{
	unsigned lengthWchar = wcslen(text);
	RECT rc;
	ReleaseCOM(mpColorBrush);
	mpRenderTarget->CreateSolidColorBrush(
		color,&mpColorBrush);
	GetClientRect(mHwnd, &rc);
	float width = (rc.right - rc.left)*0.5f;
	float height = (rc.bottom - rc.top)*0.5f;
	// Create a D2D rect that is the same size as the window.
	D2D1_RECT_F layoutRect = D2D1::RectF(
		static_cast<FLOAT>(leftUp.x + 1.0f) *width,
		static_cast<FLOAT>(-leftUp.y + 1.0f) *height,
		static_cast<FLOAT>(rightDown.x + 1.0f)*width,
		static_cast<FLOAT>(-rightDown.y + 1.0f)*height
	);

	// Use the DrawText method of the D2D render target interface to draw.
	mpRenderTarget->BeginDraw();
	mpRenderTarget->SetTransform(D2D1::IdentityMatrix());
  	mpRenderTarget->DrawText(
		text,        // The string to render.
		lengthWchar,    // The string's length.
		mpTextFormat,    // The text format.
		layoutRect,       // The region of the window where the text will be rendered.
	 mpColorBrush // The brush used to draw the text.
	);
	mpRenderTarget->EndDraw();
}

bool DirectWrite::SetTextAlignment()
{

	if (mpTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool DirectWrite::SetParagraphAlignment()
{
	if (mpTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool DirectWrite::SetFont(wchar_t * wFont, FLOAT  fontSize, DWRITE_FONT_WEIGHT fontWeight,
	IDWriteFontCollection * pFontCollection, DWRITE_FONT_STYLE  fontStyle,
	DWRITE_FONT_STRETCH  fontStretch, wchar_t * localeName)
{	
	bool fontSet = mpDWriteFactory->CreateTextFormat(
		wFont,                // Font family name.
		pFontCollection,                 // Font collection (NULL sets it to use the system font collection).
		fontWeight,
		fontStyle,
		fontStretch,
		fontSize,
		localeName,
		&mpTextFormat
	);
	return fontSet;
}

bool DirectWrite::SetDigxBuff()
{
	if (mpSwapChain!=nullptr)
	{
		IDXGISurface *dxgiBackbuffer;
		HR(mpSwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackbuffer)));
		D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
		HR(mpD2DFactory->CreateDxgiSurfaceRenderTarget(dxgiBackbuffer, props, &mpRenderTarget));
		ReleaseCOM(dxgiBackbuffer);
	}
	return true;
}



HRESULT DirectWrite::Initialize(IDXGISwapChain * swapChain,HWND hwnd)
{
	mHwnd = hwnd;
	mpSwapChain = swapChain;

	//1: Create 2D Factory
	D2D1_FACTORY_OPTIONS options;
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
	HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, &mpD2DFactory));
	HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&mpDWriteFactory)));

	//2: Set up the D2D render target by getting the back buffer's interface of IDXGISurface
	IDXGISurface *dxgiBackbuffer;
	HR(mpSwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackbuffer)));

	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
	HR(mpD2DFactory->CreateDxgiSurfaceRenderTarget(dxgiBackbuffer, props, &mpRenderTarget));


	ReleaseCOM(dxgiBackbuffer);			//Should do this or next time the IDXGISwapChain::ResizeBuffers will fail

	return S_OK;
}

void DirectWrite::PrepareResize()
{
	ReleaseCOM(mpRenderTarget);
}

wchar_t * DirectWrite::CharToWchar(char * chars)
{
	int length_chars = MultiByteToWideChar(CP_ACP, 0, chars, strlen(chars), NULL, 0);
	wchar_t * wchar = new wchar_t[length_chars + 1];
	ZeroMemory(wchar, length_chars + 1);
	MultiByteToWideChar(CP_ACP, 0, chars, strlen(chars), wchar, length_chars);
	return wchar;
}
