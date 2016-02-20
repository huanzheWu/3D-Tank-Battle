#include"XFileMesh.h"
#include<d3dx9.h>
//���캯��
XFileMesh::XFileMesh(LPDIRECT3DDEVICE9 _Device)
{
	Device=_Device;

}
//��ʼ������
void XFileMesh::InitXfile(LPCTSTR pFilename)
{
	/*������֮һ��ͨ��.X�ļ���������ģ��*/
	D3DXLoadMeshFromX(
		pFilename,
		D3DXMESH_MANAGED,
		Device,
		&ppAdjacency,
		&ppMaterials,
		NULL,
		&pNumMaterials,//���������еĲ�����Ŀ
		&SourceMesh//�����������Ĳ�������XFile�������ݵ�ID3DXMesh����
		);

	//��������XFile���Ƿ��ж��㷨����Ϣ��
	if(!SourceMesh->GetFVF ()&D3DFVF_NORMAL)
	{
		ID3DXMesh *temp=0;
		SourceMesh->CloneMeshFVF(
			D3DXMESH_MANAGED,
			SourceMesh->GetFVF ()|D3DFVF_NORMAL,
			Device,
			&temp);
		SourceMesh->Release ();
		SourceMesh=temp;
	}

	//������֮����������ʺ�����
	D3DXMATERIAL *pMtrls=(D3DXMATERIAL *)  ppMaterials->GetBufferPointer(); 

	g_pMaterials= new D3DMATERIAL9[ pNumMaterials];  
    g_pTextures  = new LPDIRECT3DTEXTURE9[ pNumMaterials];                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         

	//��ȡ����Ͳ�������
	for(DWORD i=0; i<pNumMaterials; i++)  
      {  
           //��ȡ���ʣ�������һ�»��������ɫֵ  
           g_pMaterials[i]= pMtrls[i].MatD3D;  
           g_pMaterials[i].Ambient= g_pMaterials[i].Diffuse;  
           //����һ���������  
           g_pTextures[i]  = NULL;  
           D3DXCreateTextureFromFileA(Device,pMtrls[i].pTextureFilename, &g_pTextures[i]);  
      }  

	//���������Ż�
		SourceMesh->OptimizeInplace(
		D3DXMESHOPT_COMPACT |D3DXMESHOPT_VERTEXCACHE|D3DXMESHOPT_ATTRSORT ,
		(DWORD *)ppAdjacency->GetBufferPointer (),
		(DWORD *)ppAdjacency->GetBufferPointer (),
		NULL,
		NULL
		);
	ppAdjacency->Release ();
    ppMaterials->Release ();
}

//��Ⱦ����
void XFileMesh::RenderXfile()
{
	 for(DWORD i = 0; i < pNumMaterials; i++)  
      {  
			Device->SetMaterial(&g_pMaterials[i]);  
		///*	Device->SetTexture(0,g_pTextures[i]);  */
			SourceMesh->DrawSubset(i);  
      }  
}
//��������
XFileMesh::~XFileMesh()
{
	SourceMesh->Release ();
}
