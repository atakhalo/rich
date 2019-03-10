#include "XUtility.h"

void GetPictureInfo(LPCWSTR fileName, LONG * pOutWidth, LONG * pOutHeight)
{
	//��ȡλͼ�ߴ�
	FILE* fp = _wfopen(fileName, L"rb");
	if (!fp)
	{
		MessageBox(0, (std::wstring(fileName) + L"��ʧ��!").c_str(), L"����", MB_OK);
		assert(fp != NULL);
	}

	unsigned char buf[4];
	unsigned char BMPHeader[] = { 0x42,0x4d,0xf6,0xb8,0x59 };				//BMPͷ�ļ�
	unsigned char PNGHeader[] = {0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A};  //PNGͷ�ļ�

	unsigned seek_pos = 0;
	fread_s(buf, 4, sizeof(char), 4, fp);
	if (!memcmp(buf, PNGHeader, 4))
	{

		fseek(fp, 16, SEEK_SET);
		fread_s(buf, 4, sizeof(char), 4, fp);
		*pOutWidth = BYTE_TO_UINT(buf[0], buf[1], buf[2], buf[3]);
		fread_s(buf, 4, sizeof(char), 4, fp);
		*pOutHeight = BYTE_TO_UINT(buf[0], buf[1], buf[2], buf[3]);
	}
	else if (!memcmp(buf, BMPHeader, 4))
	{
		fseek(fp, 18, SEEK_SET);
		fread_s(buf, 4, sizeof(char), 4, fp);
		*pOutWidth = BYTE_TO_UINT(buf[3], buf[2], buf[1], buf[0]);
		fread_s(buf, 4, sizeof(char), 4, fp);
		*pOutHeight = BYTE_TO_UINT(buf[3], buf[2], buf[1], buf[0]);
	}

	fclose(fp);
}

void BuildFX(ID3D11Device* dev, LPCWSTR fileName, ID3DX11Effect** ppEffect)
{
	DWORD shaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;

	HRESULT hr = D3DX11CompileFromFile(fileName, 0, 0, 0, "fx_5_0", shaderFlags,
		0, 0, &compiledShader, &compilationMsgs, 0);

	// compilationMsgs���Ի�ȡ������ߴ�����Ϣ
	if (compilationMsgs != 0)
	{
		MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, 0);
		ReleaseCOM(compilationMsgs);
	}

	// ���û�б�����Ϣ��ȷ������û�д���
	if (FAILED(hr))
	{
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DX11CompileFromFile", true);
	}

	HR(D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(),
		0, dev, ppEffect));

	// ������ɫ����ɣ������ͷ���
	ReleaseCOM(compiledShader);
}