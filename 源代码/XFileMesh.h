#pragma once
#include"d3dUtility.h"

//外接体结构
	struct BOUNDINGBOX
	{
		D3DXVECTOR3 vMin;
		D3DXVECTOR3 vMax;
	};

class XFileMesh{
public:
	XFileMesh(){};
	XFileMesh(LPDIRECT3DDEVICE9 _Device);
	virtual ~XFileMesh();

	protected:
	
	BOUNDINGBOX BoxSphere;//外接体结构
	LPD3DXMESH BoxMesh; //外接体的网格
	LPDIRECT3DDEVICE9 Device;
	LPD3DXBUFFER  ppAdjacency;
	LPD3DXBUFFER  ppMaterials;
	DWORD pNumMaterials;
	LPD3DXMESH SourceMesh;
	LPD3DXPMESH PMesh;//进行渐进网格
	D3DMATERIAL9*       g_pMaterials   ; // 网格的材质信息  
	LPDIRECT3DTEXTURE9* g_pTextures    ; // 网格的纹理信息 

	public:
		virtual void InitXfile(LPCTSTR pFilename);//初始化网格函数
		virtual void RenderXfile();	//渲染函数
		
};