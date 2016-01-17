#pragma once
#include"d3dUtility.h"
//#pragma comment(lib, "strmiids.lib")
//#pragma comment(lib,"dxguid.lib")
struct SKYBOXVERTEX
{
	float	x,y,z;
	float	u,v;
};
#define SkyBoxFVF (D3DFVF_XYZ|D3DFVF_TEX1)

class SkyBox{
	private:
		float m_Length;
		LPDIRECT3DDEVICE9				Device; //D3D�豸����
		LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer ;  //���㻺��
		LPDIRECT3DTEXTURE9			Texture[6];	//6������ӿڶ���
	public:
		SkyBox( LPDIRECT3DDEVICE9  pDevice );      //���캯��  
		virtual ~SkyBox(void);    //��������
	public:
		void LoadSkyTextureFromFile(wchar_t *pFrontTextureFile, wchar_t *pBackTextureFile,wchar_t *pLeftTextureFile, wchar_t *pRightTextureFile,wchar_t *pTopTextureFile,wchar_t *pGroundTextureFile);//���ļ���������պ���ͼ
		void InitSkyBox(float length);//��ʼ����պ�
		void RenderSkyBox();//��Ⱦ��պ�
};