#include"Terrain.h"
#include <fstream>
const DWORD Terrain::TerrainVertex::FVF = D3DFVF_XYZ | D3DFVF_TEX1;
//���캯��
Terrain::Terrain(LPDIRECT3DDEVICE9 Device,string heightmapname,int numversperrow,int numverspercol,int cellspacing,float heightscale)
{
	_device=Device;//�豸ָ��
	_numVersPerRow=numversperrow;//ÿ�ж�����
	_numVersPerCol=numverspercol;//ÿ�ж�����
	_cellSpacing=cellspacing;//���
	_heightScale=heightscale;//����
	_numCellsPerRow=numversperrow-1;//ÿ�и�����
	_numCellsPerCol=numverspercol-1;//ÿ�и�����
	_width=cellspacing*_numCellsPerCol;//���
	_depth=cellspacing*_numCellsPerRow;//���
	_numVertices=numverspercol*numversperrow;//��������
	_numTriangles=_numCellsPerRow*_numCellsPerCol*2;//��������
	readRawFile(heightmapname);
	for(int i = 0; i < _heightmap.size(); i++)
		_heightmap[i] *= heightscale;
	computeVertices();
	computerIndices();
}

void Terrain::readRawFile (string fileName)
{
	vector<BYTE> in(_numVertices);
	ifstream inFile (fileName.c_str(),ios_base::binary);
	inFile.read ((char*)&in[0],in.size ());
	inFile.close ();
	_heightmap.resize (_numVertices);
	for(int i=0;i<in.size ();i++)
		_heightmap[i]=in[i];
}


//���㻺��ļ���
void Terrain::computeVertices()
{
	int startx=-_width/2;
	int endx=_width/2;
	int startz=_depth/2;
	int endz=-_depth/2;

	float uCoordIncrementSize=1.0f/(float)_numCellsPerRow;
	float vCoordIncrementSize=1.0f/(float)_numCellsPerCol;

	_device->CreateVertexBuffer (   //�������㻺��
		_numVertices*sizeof(TerrainVertex),
		D3DUSAGE_WRITEONLY,
		TerrainVertex::FVF,
		D3DPOOL_MANAGED,
		&_ver,
		0);

	TerrainVertex* v = 0;
	_ver->Lock(0, 0, (void**)&v, 0);
	int i = 0;
	for(int z = startz; z >= endz; z -= _cellSpacing)
	{
		int j = 0;
		for(int x = startx; x <= endx; x += _cellSpacing)
		{
			int index = i *_numVersPerRow + j;//index���������±�
			v[index] = TerrainVertex(
				(float)x,
				(float)_heightmap[index],
				(float)z,
				(float)j * uCoordIncrementSize,
				(float)i * vCoordIncrementSize);

			j++; // next column
		}
		i++; // next row
	}
	_ver->Unlock ();
}

//��������ļ���
void Terrain:: computerIndices()
{
	_device->CreateIndexBuffer (
		_numTriangles*3*sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&_ind,
		0
		);

	WORD* indices=NULL;

	_ind->Lock (0,0,(void **)&indices,0);

	int baseIndex=0;
	for(int i=0;i<_numCellsPerCol;i++)
	{
		for(int j=0;j<_numCellsPerRow;j++)
		{
			indices[baseIndex]     =   i   * _numVersPerRow + j;
			indices[baseIndex + 1] =   i   * _numVersPerRow + j + 1;
			indices[baseIndex + 2] = (i+1) * _numVersPerRow + j;

			indices[baseIndex + 3] = (i+1) * _numVersPerRow + j;
			indices[baseIndex + 4] =   i   * _numVersPerRow + j + 1;
			indices[baseIndex + 5] = (i+1) * _numVersPerRow + j + 1;
			// next quad
			baseIndex += 6;
		}
	}
	_ind->Unlock ();
}


void Terrain::loadTexture (wchar_t *fileName)
{
	 D3DXCreateTextureFromFile(_device,fileName,&_tex);
}

bool Terrain:: draw(D3DXMATRIX* world, bool drawTris)
{
	
	HRESULT hr = 0;
	if( _device )
	{
		_device->SetTransform(D3DTS_WORLD, world);
		_device->SetStreamSource(0, _ver, 0, sizeof(TerrainVertex));//P����78
		_device->SetFVF(TerrainVertex::FVF);
		_device->SetIndices(_ind);
		_device->SetTexture(0, _tex);
		_device->SetRenderState(D3DRS_LIGHTING, false);
		hr =_device->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,
			0,
			0,
			_numVertices,
			0,
			_numTriangles);
		_device->SetRenderState(D3DRS_LIGHTING, true);
		if( drawTris )
		{
			_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			hr =_device->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST,
				0,
				0,
				_numVertices,
				0,
				_numTriangles);
			_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
		if(FAILED(hr))
			return false;
	}
	return true;
}

void Terrain::clean()
{
	_tex->Release ();
	_ver->Release ();
	_ind->Release ();
}