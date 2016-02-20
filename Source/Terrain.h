#pragma once
#include"d3dUtility.h"
#include<iostream>
#include<string>
#include<vector>
using namespace std;
class Terrain{
public:
	Terrain(LPDIRECT3DDEVICE9 Device,string heightmap,int numversperrow,int numverspercol,int cellspacingm,float heightscale);
	~Terrain();
private:
	IDirect3DDevice9 * _device;
	IDirect3DTexture9 *_tex;
	IDirect3DVertexBuffer9 *_ver;
	IDirect3DIndexBuffer9 *_ind;
	std::vector <int> _heightmap;

	int _numVersPerRow;
	int _numVersPerCol;

	int _cellSpacing;
	int _numCellsPerRow;
	int _numCellsPerCol;

	int _width;
	int _depth;

	int _numVertices;
	int _numTriangles;
	
	float _heightScale;

	struct TerrainVertex
	{
		TerrainVertex(){}
		TerrainVertex(float x,float y,float z,float u,float v)
		{
			_x=x;
			_y=y;
			_z=z;
			_u=u;
			_v=v;
		}
		float _x,_y,_z;
		float _u,_v;
		static const DWORD FVF; 
	};
	void readRawFile(std::string  fileName);//��ȡ������ͼ
	void computeVertices();//���㶥�㻺��
	void computerIndices();//������������
	void  clean();
public:
	void  Terrain::loadTexture(wchar_t *fileName);
	bool  draw(D3DXMATRIX* world, bool drawTris);
	
};