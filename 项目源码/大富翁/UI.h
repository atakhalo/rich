//Made by ChitoseRuri
//verson 1.0����ʱû�з���bug��
//�ⲿ����"DirectWrite.h"��"Sprite2D.h"
//�ؼ���ImageBox��TextBox��Button
//�����ؼ����UI�����඼����ʹ�ã�����Ӧ�����ĺ����ŵ���Ӧλ��

#ifndef UI_H_
#define UI_H_

#include "Sprite2D.h"
#include "DirectWrite.h"

namespace DX11UI
{
	enum ButtonStatus { BUTTON_OUT, BUTTON_ON, BUTTON_DOWN };

	class Control																																//�ؼ�����
	{
	public :
		Control();
		~Control();
		static void GetDevice(ID3D11Device * pDevice, ID3DX11Effect *pFX);
		//��ȡ����
		static void GetHwnd(HWND hwnd);
		//��ʼ���ؼ�
		virtual void SetControl(XMFLOAT2 size, unsigned order) = NULL;	
		//��ʼ���ؼ�
		virtual void SetControl(float x, float y, unsigned order) = NULL;
		//����D3D:ReSize֮ǰ	
		virtual void BeforeReSize() = NULL;		
		//����D3D:ReSize֮��
		virtual void AfterReSize() = NULL;					
		//�ؼ��ĸ���
		virtual void UpData() = NULL;	
		//���ƿؼ�
		virtual void Draw() = NULL;	
		//�ƶ��ؼ�
		virtual void Move(float x, float y) = NULL;		
		//�ؼ�����
		virtual void Visible();						
		//�ؼ�������
		virtual void Invisible();		

	protected:
		//�ؼ���2��״̬
		enum ControlStatus {VISIBLE, INVISIBLE };					
		static ID3D11Device * m_pD3dDevice;															
		static ID3DX11Effect *m_pFX;
		static ID3D11BlendState* m_pAlphaToCoverageBS;
		static HWND m_Hwnd;
		int m_ControlStatus;																											//��ǿؼ���״̬
		XMFLOAT2 m_Size;
		unsigned order;
	};	



	class ImageBox :public Control
	{
	public:
		ImageBox();
		~ImageBox();
		//��ʼ���ؼ�
		static void Setup(ID3D11DeviceContext * d3DDeviceContext, ID3D11InputLayout * pInputLayout, ID3D11BlendState* pBlendState, HWND hwnd);
		//����λ��
		void SetControl(XMFLOAT2 size, unsigned order) override;
		//����λ��
		void SetControl(float x, float y, unsigned order) override;
		//����ͼƬ
		void SetImage(ID3D11ShaderResourceView * pTexture);																			
		//����D3D:ReSize֮ǰ	
		void BeforeReSize() override;												
		//����D3D:ReSize֮��
		void AfterReSize() override;			
		//�ؼ��ĸ���
		void UpData() override;		
		//�ƶ��ؼ�
		void Move(float x, float y) override;					
		//�ؼ���ת
		void Rotate(float radian);		
		//�ؼ���Բ��ת
		void RotateElliptical(float radian);
		//���ƿؼ�
		void Draw() override;																										

	private:
		Sprite2D m_Image;																											//ͼƬ�ؼ�
	};



	class TextBox :public Control
	{
	public:
		TextBox();
		~TextBox();
		//���뽻�����ʹ���
		static void Setup(IDXGISwapChain* pSwapChain, HWND hwnd);						
		//����λ��
		void SetControl(XMFLOAT2 size, unsigned order)override;			
		//����λ��
		void SetControl(float x, float y, unsigned order) override;
		//��������
		void SetFont(wchar_t * wFont, FLOAT  fontSize);														
		//����Ҫ��ʾ���ֺ���ɫ
		void SetWords(const wchar_t * text, int color);																		
		//����D3D:ReSize֮ǰ	
		void BeforeReSize() override;																							
		//����D3D:ReSize֮��
		void AfterReSize() override;																							
		//�ؼ��ĸ���
		void UpData() override;																										
		//�ƶ��ؼ�
		void Move(float x, float y) override;																				
		//���ƿؼ�
		void Draw() override;																											

	private:
		static IDXGISwapChain * m_pSwapChain;
		DirectWrite m_Text;
		XMFLOAT2 m_LeftUp;
		XMFLOAT2 m_RightDown;
		wchar_t  m_pText[2048];
		int m_Color;
	};



	class Button:public Control
	{
	public:
		Button();
		~Button();
		//��ʼ���ؼ�
		static void Setup(IDXGISwapChain* pSwapChain, HWND hwnd, ID3D11DeviceContext * d3DDeviceContext,
			 ID3D11InputLayout * pInputLayout, ID3D11BlendState* pBlendState);
		//����λ��
		void SetControl(XMFLOAT2 size, unsigned order) override;
		//����λ��
		void SetControl(float x, float y, unsigned order) override;
		void SetPicture_All(ID3D11ShaderResourceView * pTexture);
		void SetFont_All(wchar_t * wFont, FLOAT  fontSize);
		void SetWords_All(const wchar_t * text, int color);
		//����ͼƬ
		void SetPicture_ButtonOut(ID3D11ShaderResourceView * pTexture);
		//��������
		void SetFont_ButtonOut(wchar_t * wFont, FLOAT  fontSize);							
		//����Ҫ��ʾ���ֺ���ɫ
		void SetWords_ButtonOut(const wchar_t * text, int color);
		//����ͼƬ
		void SetPicture_ButtonOn(ID3D11ShaderResourceView * pTexture);
		//��������
		void SetFont_ButtonOn(wchar_t * wFont, FLOAT  fontSize);									
		//����Ҫ��ʾ���ֺ���ɫ
		void SetWords_ButtonOn(const wchar_t * text, int color);
		//����ͼƬ
		void SetPicture_ButtonDown(ID3D11ShaderResourceView * pTexture);																	
		//��������
		void SetFont_ButtonDown(wchar_t * wFont, FLOAT  fontSize);							
		//����Ҫ��ʾ���ֺ���ɫ
		void SetWords_ButtonDown(const wchar_t * text, int color);
		//����D3D:ReSize֮ǰ	
		void BeforeReSize() override;																							
		//����D3D:ReSize֮��
		void AfterReSize() override;																								
		//�ؼ��ĸ���
		void UpData() override;																										
		//�ж�buttonʧ�����
		void ButtonCheck(const int &x, const int &y);
		//��갴��ʱ
		void ButtonDown();																											
		//���ſ�ʱ
		bool ButtonUp();																													
		//�ƶ��ؼ�
		void Move(float x, float y) override;	
		//��Բ��תbutton�����ͼƬ��buttonλ�ò��䣩
		void RotateElliptical(float radian);
		//���ƿؼ�
		void Draw() override;																											
		//��������
		void Enable();																														
		//������������ͣ����OUT����̬
		void Disable();				
		//�ؼ�����
		void Visible() override;
		//�ؼ�������
		void Invisible() override;

	private:
		void ReSetConButtonSize();
		static RECT m_Rect;
		ImageBox m_ButtonOut_Image;																					//��겻�ڸİ�ť��
		TextBox m_ButtonOut_Text;
		ImageBox m_ButtonOn_Image;																						//����ڰ�ť��
		TextBox m_ButtonOn_Text;
		ImageBox m_ButtonDown_Image;																				//��갴�°�ť
		TextBox m_ButtonDown_Text;
		XMFLOAT2 m_LeftUp;
		XMFLOAT2 m_RightDown;
		float m_ButtonLeft;
		float m_ButtonTop;
		float m_ButtonRight;
		float m_ButtonDown;
		int m_ButtonStatus;																											//button��״̬
		bool m_IsEnable;																												//button�ܲ��ܱ���
	};		

	class UI
	{
	public:
		//����һ��ҳ����ֿؼ�������
		UI();
		~UI();
		static void InitAll(ID3D11Device * pDevice, ID3DX11Effect *pFX, IDXGISwapChain* pSwapChain, HWND hwnd, ID3D11DeviceContext * d3DDeviceContext,
			 ID3D11InputLayout * pInputLayout,ID3D11BlendState* pBlendState);
		//ȷ����ǰ������ֿؼ�������
		void Init(int num_Textbox, int num_Picture, int num_Button);
		//���ر�����Button���±�
		int ButtonCheck();
		//ʹ����Button�����
		void ResetButtonCheck();
		//����Updata����
		void Updata();
		//����d3dResizeǰ��
		void BeforeReSize();
		//����d3dResize����
		void AfterResize(HWND hwnd);
		//�������������µĵط�
		void MouseLeftDown();
		//�����������ſ��ط�
		void MouseLeftUp();
		//����
		void Draw();
		//��ȡ�ؼ����鳤��
		int Button_Size();
		int TextureBox_Size();
		int ImageBox_Size();

	public:
		TextBox* m_TextBox;
		ImageBox* m_ImageBox;
		Button* m_Button;

	private:
		static HWND m_Hwnd;
		int m_ButtonClick;
		int m_NumText, m_NumImage, m_NumButton;
	};
}
#endif // !UI_H_