#pragma once
#include"d3dUtility.h"
class XFileMesh{
public:
	XFileMesh(LPDIRECT3DDEVICE9 _Device);
	XFileMesh();
	~XFileMesh();

	private:

	LPDIRECT3DDEVICE9 Device;
	LPD3DXBUFFER  ppAdjacency;
	LPD3DXBUFFER  ppMaterials;
	DWORD pNumMaterials;
	LPD3DXMESH SourceMesh;
	LPD3DXPMESH PMesh;//���н�������
	D3DMATERIAL9*       g_pMaterials   ; // ����Ĳ�����Ϣ  
	LPDIRECT3DTEXTURE9* g_pTextures    ; // �����������Ϣ 

	public:
		void InitXfile(LPCTSTR pFilename);//��ʼ��������
		void RenderXfile();	//��Ⱦ����
		
};