#pragma once
#include"d3dUtility.h"

//�����ṹ
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
	
	BOUNDINGBOX BoxSphere;//�����ṹ
	LPD3DXMESH BoxMesh; //����������
	LPDIRECT3DDEVICE9 Device;
	LPD3DXBUFFER  ppAdjacency;
	LPD3DXBUFFER  ppMaterials;
	DWORD pNumMaterials;
	LPD3DXMESH SourceMesh;
	LPD3DXPMESH PMesh;//���н�������
	D3DMATERIAL9*       g_pMaterials   ; // ����Ĳ�����Ϣ  
	LPDIRECT3DTEXTURE9* g_pTextures    ; // �����������Ϣ 

	public:
		virtual void InitXfile(LPCTSTR pFilename);//��ʼ��������
		virtual void RenderXfile();	//��Ⱦ����
		
};