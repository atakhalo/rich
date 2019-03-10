#include "Terrain.h"


Terrain::Terrain() : 
	m_QuadPatchVB(0), 
	m_QuadPatchIB(0), 
	m_LayerMapArraySRV(0), 
	m_BlendMapSRV(0), 
	m_HeightMapSRV(0),
	m_NumPatchVertices(0),
	m_NumPatchQuadFaces(0),
	m_NumPatchVertRows(0),
	m_NumPatchVertCols(0)
{
	XMStoreFloat4x4(&m_World, XMMatrixIdentity());

	m_Mat.Ambient  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Mat.Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Mat.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 64.0f);
	m_Mat.Reflect  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
}

Terrain::~Terrain()
{
	ReleaseCOM(m_QuadPatchVB);
	ReleaseCOM(m_QuadPatchIB);
	ReleaseCOM(m_LayerMapArraySRV);
	ReleaseCOM(m_BlendMapSRV);
	ReleaseCOM(m_HeightMapSRV);
}
//计算地图在3D中的大小
float Terrain::GetWidth()const
{
	// Total terrain width.
	return (mInfo.HeightmapWidth-1)*mInfo.CellSpacing;
}
//计算地图在3D中的大小
float Terrain::GetDepth()const
{
	// Total terrain depth.
	return (mInfo.HeightmapHeight-1)*mInfo.CellSpacing;
}
//获取地形上某个点的高度
float Terrain::GetHeight(float x, float z)const
{
	//转换到格子坐标系
	// Transform from terrain local space to "cell" space.
	float c = (x + 0.5f*GetWidth()) /  mInfo.CellSpacing;
	float d = (z - 0.5f*GetDepth()) / -mInfo.CellSpacing;
	//找到所在格子
	// Get the row and column we are in.
	int row = (int)floorf(d);
	int col = (int)floorf(c);

	//判断位于格子的哪个三角形中(格子由两个三角形组成)
	// Grab the heights of the cell we are in.
	// A*--*B
	//  | /|
	//  |/ |
	// C*--*D
	//四个顶点
	float A = m_Heightmap[row*mInfo.HeightmapWidth + col];
	float B = m_Heightmap[row*mInfo.HeightmapWidth + col + 1];
	float C = m_Heightmap[(row+1)*mInfo.HeightmapWidth + col];
	float D = m_Heightmap[(row+1)*mInfo.HeightmapWidth + col + 1];

	// Where we are relative to the cell.
	float s = c - (float)col;
	float t = d - (float)row;

	// If upper triangle ABC.
	//用向量算出y坐标
	if( s + t <= 1.0f)
	{
		float uy = B - A;
		float vy = C - A;
		return A + s*uy + t*vy;
	}
	else // lower triangle DCB.
	{
		float uy = C - D;
		float vy = B - D;
		return D + (1.0f-s)*uy + (1.0f-t)*vy;
	}
}

XMMATRIX Terrain::GetWorld()const
{
	return XMLoadFloat4x4(&m_World);
}

void Terrain::SetWorld(CXMMATRIX M)
{
	XMStoreFloat4x4(&m_World, M);
}

void Terrain::Init(ID3D11Device* device, ID3D11DeviceContext* dc, const InitInfo& initInfo)
{
	mInfo = initInfo;

	// Divide heightmap into patches such that each patch has CellsPerPatch.
	//行数和列数
	m_NumPatchVertRows = ((mInfo.HeightmapHeight-1) / CellsPerPatch) + 1;
	m_NumPatchVertCols = ((mInfo.HeightmapWidth-1) / CellsPerPatch) + 1;
	//顶点数和面数
	m_NumPatchVertices  = m_NumPatchVertRows*m_NumPatchVertCols;
	m_NumPatchQuadFaces = (m_NumPatchVertRows-1)*(m_NumPatchVertCols-1);

	LoadHeightmap();
	Smooth();
	CalcAllPatchBoundsY();

	BuildQuadPatchVB(device);
	BuildQuadPatchIB(device);
	BuildHeightmapSRV(device);

	std::vector<std::wstring> layerFilenames;
	//根据贴图个数载入纹理  注：此处不在每个case后加break是为了叠加
	switch (mInfo.texCount)
	{
	case 5:
		layerFilenames.push_back(mInfo.LayerMapFilename4);
	case 4:
		layerFilenames.push_back(mInfo.LayerMapFilename3);
	case 3:
		layerFilenames.push_back(mInfo.LayerMapFilename2);
	case 2:
		layerFilenames.push_back(mInfo.LayerMapFilename1);
	case 1:
		layerFilenames.push_back(mInfo.LayerMapFilename0);
		break;
	default:break;
	}
	m_LayerMapArraySRV = d3dHelper::CreateTexture2DArraySRV(device, dc, layerFilenames);

	HR(D3DX11CreateShaderResourceViewFromFile(device, 
		mInfo.BlendMapFilename.c_str(), 0, 0, &m_BlendMapSRV, 0));
}

void Terrain::Draw(ID3D11DeviceContext* dc, XMFLOAT4X4 ViewProj, XMFLOAT3 position, DirectionalLight lights[3])
{
	//设置曲面细分的元素,地形为四边形，用D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	dc->IASetInputLayout(InputLayouts::Terrain);

	UINT stride = sizeof(Vertex::Terrain);
    UINT offset = 0;
    dc->IASetVertexBuffers(0, 1, &m_QuadPatchVB, &stride, &offset);
	dc->IASetIndexBuffer(m_QuadPatchIB, DXGI_FORMAT_R16_UINT, 0);

	XMMATRIX viewProj = XMLoadFloat4x4(&ViewProj);
	//XMMATRIX world  = XMLoadFloat4x4(&m_World) *  XMLoadFloat4x4(&translation);
	//XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
	//XMMATRIX worldViewProj = world*viewProj;

	XMFLOAT4 worldPlanes[6];
	ExtractFrustumPlanes(worldPlanes, viewProj);

	// Set per frame constants.
	Effects::TerrainFX->SetViewProj(viewProj);
	Effects::TerrainFX->SetEyePosW(position);
	Effects::TerrainFX->SetDirLights(lights);
	//Effects::TerrainFX->SetFogColor(Colors::Silver);   //移植到player.cpp
	//Effects::TerrainFX->SetFogStart(15.0f);
	//Effects::TerrainFX->SetFogRange(175.0f);
	Effects::TerrainFX->SetMinDist(20.0f);
	Effects::TerrainFX->SetMaxDist(500.0f);
	Effects::TerrainFX->SetMinTess(0.0f);
	Effects::TerrainFX->SetMaxTess(6.0f);
	Effects::TerrainFX->SetTexelCellSpaceU(1.0f / mInfo.HeightmapWidth);
	Effects::TerrainFX->SetTexelCellSpaceV(1.0f / mInfo.HeightmapHeight);
	Effects::TerrainFX->SetWorldCellSpace(mInfo.CellSpacing);
	Effects::TerrainFX->SetWorldFrustumPlanes(worldPlanes);
	
	Effects::TerrainFX->SetLayerMapArray(m_LayerMapArraySRV);
	Effects::TerrainFX->SetBlendMap(m_BlendMapSRV);
	Effects::TerrainFX->SetHeightMap(m_HeightMapSRV);

	Effects::TerrainFX->SetMaterial(m_Mat);

	ID3DX11EffectTechnique* tech = Effects::TerrainFX->Light1Tech;
    D3DX11_TECHNIQUE_DESC techDesc;
    tech->GetDesc( &techDesc );

    for(UINT i = 0; i < techDesc.Passes; ++i)
    {
        ID3DX11EffectPass* pass = tech->GetPassByIndex(i);
		pass->Apply(0, dc);

		dc->DrawIndexed(m_NumPatchQuadFaces*4, 0, 0);
	}	

	// FX sets tessellation stages, but it does not disable them.  So do that here
	// to turn off tessellation.
	//关闭曲面细分
	dc->HSSetShader(0, 0, 0);
	dc->DSSetShader(0, 0, 0);
}

//此版本不改变灯光，可由用户自己手动管理灯光
void Terrain::Draw(ID3D11DeviceContext* dc, XMFLOAT4X4 ViewProj, XMFLOAT3 position)
{
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	dc->IASetInputLayout(InputLayouts::Terrain);

	UINT stride = sizeof(Vertex::Terrain);
	UINT offset = 0;
	dc->IASetVertexBuffers(0, 1, &m_QuadPatchVB, &stride, &offset);
	dc->IASetIndexBuffer(m_QuadPatchIB, DXGI_FORMAT_R16_UINT, 0);

	XMMATRIX viewProj = XMLoadFloat4x4(&ViewProj);
	XMMATRIX world = XMLoadFloat4x4(&m_World);
	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
	XMMATRIX worldViewProj = world*viewProj;

	XMFLOAT4 worldPlanes[6];
	ExtractFrustumPlanes(worldPlanes, viewProj);

	// Set per frame constants.
	Effects::TerrainFX->SetViewProj(viewProj);
	Effects::TerrainFX->SetEyePosW(position);
	//Effects::TerrainFX->SetFogColor(Colors::Silver);				//移植到player.cpp
	//Effects::TerrainFX->SetFogStart(15.0f);
	//Effects::TerrainFX->SetFogRange(175.0f);
	Effects::TerrainFX->SetMinDist(20.0f);
	Effects::TerrainFX->SetMaxDist(500.0f);
	Effects::TerrainFX->SetMinTess(0.0f);
	Effects::TerrainFX->SetMaxTess(6.0f);
	Effects::TerrainFX->SetTexelCellSpaceU(1.0f / mInfo.HeightmapWidth);
	Effects::TerrainFX->SetTexelCellSpaceV(1.0f / mInfo.HeightmapHeight);
	Effects::TerrainFX->SetWorldCellSpace(mInfo.CellSpacing);
	Effects::TerrainFX->SetWorldFrustumPlanes(worldPlanes);

	Effects::TerrainFX->SetLayerMapArray(m_LayerMapArraySRV);
	Effects::TerrainFX->SetBlendMap(m_BlendMapSRV);
	Effects::TerrainFX->SetHeightMap(m_HeightMapSRV);

	Effects::TerrainFX->SetMaterial(m_Mat);

	ID3DX11EffectTechnique* tech = Effects::TerrainFX->Light1FogTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	tech->GetDesc(&techDesc);

	for (UINT i = 0; i < techDesc.Passes; ++i)
	{
		ID3DX11EffectPass* pass = tech->GetPassByIndex(i);
		pass->Apply(0, dc);

		dc->DrawIndexed(m_NumPatchQuadFaces * 4, 0, 0);
	}

	// FX sets tessellation stages, but it does not disable them.  So do that here
	// to turn off tessellation.
	dc->HSSetShader(0, 0, 0);
	dc->DSSetShader(0, 0, 0);
}

//读取高度图，因高度图为8-bit 的raw，所以可以用char直接读取
void Terrain::LoadHeightmap()
{
	// A height for each vertex
	std::vector<unsigned char> in( mInfo.HeightmapWidth * mInfo.HeightmapHeight );

	// Open the file.
	std::ifstream inFile;
	inFile.open(mInfo.HeightMapFilename.c_str(), std::ios_base::binary);

	if(inFile)
	{
		// Read the RAW bytes.
		inFile.read((char*)&in[0], (std::streamsize)in.size());

		// Done with file.
		inFile.close();
	}

	// Copy the array data into a float array and scale it.
	//将高度放大，不然最高只有255
	m_Heightmap.resize(mInfo.HeightmapHeight * mInfo.HeightmapWidth, 0);
	for(UINT i = 0; i < mInfo.HeightmapHeight * mInfo.HeightmapWidth; ++i)
	{
		m_Heightmap[i] = (in[i] / 255.0f)*mInfo.HeightScale;
	}
}

//使地形平滑过渡
void Terrain::Smooth()
{
	std::vector<float> dest( m_Heightmap.size() );

	for(UINT i = 0; i < mInfo.HeightmapHeight; ++i)
	{
		for(UINT j = 0; j < mInfo.HeightmapWidth; ++j)
		{
			dest[i*mInfo.HeightmapWidth+j] = Average(i,j);
		}
	}

	// Replace the old heightmap with the filtered one.
	m_Heightmap = dest;
}
//判断是否在区域内,只有区域内的点才会用于平均
bool Terrain::InBounds(int i, int j)
{
	// True if ij are valid indices; false otherwise.
	return 
		i >= 0 && i < (int)mInfo.HeightmapHeight && 
		j >= 0 && j < (int)mInfo.HeightmapWidth;
}
//计算该点及周围八个点的平均高度
float Terrain::Average(int i, int j)
{
	// Function computes the average height of the ij element.
	// It averages itself with its eight neighbor pixels.  Note
	// that if a pixel is missing neighbor, we just don't include it
	// in the average--that is, edge pixels don't have a neighbor pixel.
	//
	// ----------
	// | 1| 2| 3|
	// ----------
	// |4 |ij| 6|
	// ----------
	// | 7| 8| 9|
	// ----------

	float avg = 0.0f;
	float num = 0.0f;

	// Use int to allow negatives.  If we use UINT, @ i=0, m=i-1=UINT_MAX
	// and no iterations of the outer for loop occur.
	for(int m = i-1; m <= i+1; ++m)
	{
		for(int n = j-1; n <= j+1; ++n)
		{
			if( InBounds(m,n) )
			{
				avg += m_Heightmap[m*mInfo.HeightmapWidth + n];
				num += 1.0f;
			}
		}
	}

	return avg / num;
}

//得到每个格子的最大高度和最小高度
void Terrain::CalcAllPatchBoundsY()
{
	m_PatchBoundsY.resize(m_NumPatchQuadFaces);

	// For each patch
	for(UINT i = 0; i < m_NumPatchVertRows-1; ++i)
	{
		for(UINT j = 0; j < m_NumPatchVertCols-1; ++j)
		{
			CalcPatchBoundsY(i, j);
		}
	}
}
//计算一个格子的最大高度和最小高度
void Terrain::CalcPatchBoundsY(UINT i, UINT j)
{
	// Scan the heightmap values this patch covers and compute the min/max height.

	UINT x0 = j*CellsPerPatch;
	UINT x1 = (j+1)*CellsPerPatch;

	UINT y0 = i*CellsPerPatch;
	UINT y1 = (i+1)*CellsPerPatch;

	float minY = +MathHelper::Infinity;
	float maxY = -MathHelper::Infinity;
	for(UINT y = y0; y <= y1; ++y)
	{
		for(UINT x = x0; x <= x1; ++x)
		{
			UINT k = y*mInfo.HeightmapWidth + x;
			minY = MathHelper::Min(minY, m_Heightmap[k]);
			maxY = MathHelper::Max(maxY, m_Heightmap[k]);
		}
	}

	UINT patchID = i*(m_NumPatchVertCols-1)+j;
	m_PatchBoundsY[patchID] = XMFLOAT2(minY, maxY);
}

//建立顶点缓存，以格子为单位
void Terrain::BuildQuadPatchVB(ID3D11Device* device)
{
	std::vector<Vertex::Terrain> patchVertices(m_NumPatchVertRows*m_NumPatchVertCols);

	float halfWidth = 0.5f*GetWidth();
	float halfDepth = 0.5f*GetDepth();

	//一个格子的长宽
	float patchWidth = GetWidth() / (m_NumPatchVertCols-1);
	float patchDepth = GetDepth() / (m_NumPatchVertRows-1);
	float du = 1.0f / (m_NumPatchVertCols-1);
	float dv = 1.0f / (m_NumPatchVertRows-1);

	for(UINT i = 0; i < m_NumPatchVertRows; ++i)
	{
		float z = halfDepth - i*patchDepth;
		for(UINT j = 0; j < m_NumPatchVertCols; ++j)
		{
			float x = -halfWidth + j*patchWidth;

			patchVertices[i*m_NumPatchVertCols+j].Pos = XMFLOAT3(x, 0.0f, z);

			// Stretch texture over grid.
			patchVertices[i*m_NumPatchVertCols+j].Tex.x = j*du;
			patchVertices[i*m_NumPatchVertCols+j].Tex.y = i*dv;
		}
	}

	//创建包围盒
	// Store axis-aligned bounding box y-bounds in upper-left patch corner.
	for(UINT i = 0; i < m_NumPatchVertRows-1; ++i)
	{
		for(UINT j = 0; j < m_NumPatchVertCols-1; ++j)
		{
			UINT patchID = i*(m_NumPatchVertCols-1)+j;
			patchVertices[i*m_NumPatchVertCols+j].BoundsY = m_PatchBoundsY[patchID];
		}
	}

    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::Terrain) * patchVertices.size();
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &patchVertices[0];
    HR(device->CreateBuffer(&vbd, &vinitData, &m_QuadPatchVB));
}

void Terrain::BuildQuadPatchIB(ID3D11Device* device)
{
	std::vector<USHORT> indices(m_NumPatchQuadFaces*4); // 4 indices per quad face

	// Iterate over each quad and compute indices.
	int k = 0;
	for(UINT i = 0; i < m_NumPatchVertRows-1; ++i)
	{
		for(UINT j = 0; j < m_NumPatchVertCols-1; ++j)
		{
			// Top row of 2x2 quad patch
			indices[k]   = i*m_NumPatchVertCols+j;
			indices[k+1] = i*m_NumPatchVertCols+j+1;

			// Bottom row of 2x2 quad patch
			indices[k+2] = (i+1)*m_NumPatchVertCols+j;
			indices[k+3] = (i+1)*m_NumPatchVertCols+j+1;

			k += 4; // next quad
		}
	}

	D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(USHORT) * indices.size();
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &indices[0];
    HR(device->CreateBuffer(&ibd, &iinitData, &m_QuadPatchIB));
}

void Terrain::BuildHeightmapSRV(ID3D11Device* device)
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = mInfo.HeightmapWidth;
	texDesc.Height = mInfo.HeightmapHeight;
    texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format    = DXGI_FORMAT_R16_FLOAT;
	texDesc.SampleDesc.Count   = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	// HALF is defined in xnamath.h, for storing 16-bit float.
	std::vector<HALF> hmap(m_Heightmap.size());
	std::transform(m_Heightmap.begin(), m_Heightmap.end(), hmap.begin(), XMConvertFloatToHalf);
	
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &hmap[0];
    data.SysMemPitch = mInfo.HeightmapWidth*sizeof(HALF);
    data.SysMemSlicePitch = 0;

	ID3D11Texture2D* hmapTex = 0;
	HR(device->CreateTexture2D(&texDesc, &data, &hmapTex));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	HR(device->CreateShaderResourceView(hmapTex, &srvDesc, &m_HeightMapSRV));

	// SRV saves reference.
	ReleaseCOM(hmapTex);
}