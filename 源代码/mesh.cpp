#include"d3dUtility.h"
LPD3DXMESH Mesh=NULL;//�������
IDirect3DTexture9 *meshtexture[3];//�����������������
struct CUSTOMVERTEX
{
	float _x,_y,_z;
	float _u,_v;
	CUSTOMVERTEX(float x,float y,float z ,float u,float v)
	{
		_x=x,_y=y,_z=z,_u=u,_v=v;
	}
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_TEX1)
void InitMesh(LPDIRECT3DDEVICE9 Device)
{
	D3DXCreateMeshFVF(12,24,D3DXMESH_MANAGED,D3DFVF_CUSTOMVERTEX,Device,&Mesh);//��������ģ��
	//�����������
	CUSTOMVERTEX *pVertex=NULL;
	Mesh->LockVertexBuffer(0,(void **)&pVertex);
		/*���涥��*/ 
   pVertex[0] = CUSTOMVERTEX(-1.0f,  1.0f, -1.0f,0.0f, 0.0f);  
   pVertex[1] = CUSTOMVERTEX( 1.0f,  1.0f, -1.0f,  1.0f, 0.0f);  
   pVertex[2] = CUSTOMVERTEX( 1.0f, -1.0f, -1.0f, 1.0f, 1.0f);  
   pVertex[3] = CUSTOMVERTEX(-1.0f, -1.0f, -1.0f,0.0f, 1.0f);  
    /*���涥��*/  
   pVertex[4] = CUSTOMVERTEX( 1.0f,  1.0f, 1.0f, 0.0f, 0.0f);  
   pVertex[5] = CUSTOMVERTEX(-1.0f,  1.0f, 1.0f,  1.0f, 0.0f);  
   pVertex[6] = CUSTOMVERTEX(-1.0f, -1.0f, 1.0f,1.0f, 1.0f);  
   pVertex[7] = CUSTOMVERTEX( 1.0f, -1.0f, 1.0f, 0.0f, 1.0f);  
    /*���涥��*/ 
   pVertex[8]  = CUSTOMVERTEX(-1.0f, 1.0f,  1.0f,0.0f, 0.0f);  
   pVertex[9]  = CUSTOMVERTEX( 1.0f, 1.0f,  1.0f,  1.0f, 0.0f);  
   pVertex[1] = CUSTOMVERTEX( 1.0f, 1.0f, -1.0f, 1.0f,1.0f);  
   pVertex[11] = CUSTOMVERTEX(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f);  
   /*���涥��*/ 
   pVertex[12] = CUSTOMVERTEX(-1.0f, -1.0f, -1.0f,0.0f, 0.0f);  
   pVertex[13] = CUSTOMVERTEX( 1.0f, -1.0f, -1.0f,   1.0f, 0.0f);  
   pVertex[14] = CUSTOMVERTEX( 1.0f, -1.0f,  1.0f, 1.0f, 1.0f);  
   pVertex[15] = CUSTOMVERTEX(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f);  
  /*���涥��*/
   pVertex[16] = CUSTOMVERTEX(-1.0f,  1.0f,  1.0f,0.0f, 0.0f);  
   pVertex[17] = CUSTOMVERTEX(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f);  
   pVertex[18] = CUSTOMVERTEX(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f);  
   pVertex[19] = CUSTOMVERTEX(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f);  
  /*���涥��*/  
   pVertex[20] = CUSTOMVERTEX( 1.0f,  1.0f, -1.0f,0.0f,0.0f);  
   pVertex[21] = CUSTOMVERTEX( 1.0f,  1.0f,  1.0f, 1.0f, 0.0f);  
   pVertex[22] = CUSTOMVERTEX( 1.0f, -1.0f,  1.0f,1.0f, 1.0f);  
   pVertex[23] = CUSTOMVERTEX( 1.0f, -1.0f, -1.0f, 0.0f, 1.0f); 

   Mesh->UnlockVertexBuffer();
   
   WORD *pIndices=NULL;
   Mesh->LockIndexBuffer (0,(void **)&pIndices);
    pIndices[0] = 0; pIndices[1] = 1; pIndices[2] = 2;  
    pIndices[3] = 0; pIndices[4] = 2; pIndices[5] = 3;  
    /*������������ */ 
    pIndices[6] = 4; pIndices[7]  = 5; pIndices[8]  = 6;  
    pIndices[9] = 4; pIndices[10] = 6; pIndices[11] = 7;  
    /*������������ */ 
    pIndices[12] = 8; pIndices[13] =  9; pIndices[14] = 10;  
    pIndices[15] = 8; pIndices[16] = 10; pIndices[17] = 11;  
    /*������������ */ 
    pIndices[18] = 12; pIndices[19] = 13; pIndices[20] = 14;  
    pIndices[21] = 12; pIndices[22] = 14; pIndices[23] = 15;  
    /*������������ */ 
    pIndices[24] = 16; pIndices[25] = 17; pIndices[26] = 18;  
    pIndices[27] = 16; pIndices[28] = 18; pIndices[29] = 19;  
    /*������������ */  
    pIndices[30] = 20; pIndices[31] = 21; pIndices[32] = 22;  
    pIndices[33] = 20; pIndices[34] = 22; pIndices[35] = 23; 

	Mesh->UnlockIndexBuffer();

	//������Ա�����
	DWORD *PAttrib=NULL;
		Mesh->LockAttributeBuffer (0,&PAttrib);
		for(int a=0;a<4;++a)
			PAttrib[a]=0;
		for(int b=4;b<8;++b)
			PAttrib[b]=1;
		for(int c=8;c<12;c++)
			PAttrib[c]=2;
		Mesh->UnlockAttributeBuffer ();

	//��������
		D3DXCreateTextureFromFile(Device,L"mesh1.jpg",&meshtexture[0]);
		D3DXCreateTextureFromFile(Device,L"mesh2.jpg",&meshtexture[1]);
		D3DXCreateTextureFromFile(Device,L"mesh3.jpg",&meshtexture[2]);
}

void RenderMesh(LPDIRECT3DDEVICE9 Device)
{
	D3DXMATRIX world;
	D3DXMatrixTranslation(&world,5.0f,0.0f,0.0f);
	Device->SetTransform (D3DTS_WORLD,&world);	

	for(int i=0;i<3;++i)
	{
		Device->SetTexture (0,meshtexture[i]);
		Mesh->DrawSubset (i);
	}
}

void ReleaseMesh()
{
	Mesh->Release ();
	for(int i=0;i<3;i++)
	{
		meshtexture[i]->Release ();
	}
}