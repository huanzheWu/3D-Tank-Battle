#include"skyboxclass.h"
#include"d3dUtility.h"

//===========================================================================
//���캯������ʼ��
//===========================================================================
SkyBox::SkyBox( LPDIRECT3DDEVICE9  pDevice )
{
	//��������������ֵ
	m_pVertexBuffer=NULL;
	Device=pDevice;
	for(int i=0; i<6; i++)
		Texture[i] = NULL; 
	m_Length = 0.0f;
}

void SkyBox::LoadSkyTextureFromFile(wchar_t *pFrontTextureFile, wchar_t *pBackTextureFile,wchar_t *pLeftTextureFile, wchar_t *pRightTextureFile,wchar_t *pTopTextureFile,wchar_t *pGroundTextureFile)
{
	//���ļ�������������
	D3DXCreateTextureFromFile( Device , pFrontTextureFile, &Texture[0] );  //ǰ��
	D3DXCreateTextureFromFile( Device , pBackTextureFile,  &Texture[1] );  //����
	D3DXCreateTextureFromFile( Device , pLeftTextureFile,  &Texture[2] );  //����
	D3DXCreateTextureFromFile( Device , pRightTextureFile, &Texture[3] );  //����
	D3DXCreateTextureFromFile( Device , pTopTextureFile,   &Texture[4] );  //����
	D3DXCreateTextureFromFile( Device , pGroundTextureFile,   &Texture[5] );  //����


}

//===============================================================================
//��ʼ����պк���
//================================================================================
void SkyBox::InitSkyBox(float length)
{

	m_Length=length;
	Device->CreateVertexBuffer (
	24*sizeof(SKYBOXVERTEX),
		0,
		SkyBoxFVF,
		D3DPOOL_MANAGED,
		&m_pVertexBuffer,
		0);
	//ǰ��
	SKYBOXVERTEX vertices[] =
    {
		//ǰ����ĸ�����
		{ -m_Length/2, 0.0f,    m_Length/2, 0.0f, 1.0f, },//A
		{ -m_Length/2, m_Length/2,   m_Length/2, 0.0f, 0.0f, },//B
		{  m_Length/2, 0.0f,    m_Length/2, 1.0f, 1.0f, },//C
		{  m_Length/2, m_Length/2,   m_Length/2, 1.0f, 0.0f, },//D

		//������ĸ�����
		{  m_Length/2, 0.0f,   -m_Length/2, 0.0f, 1.0f, },//E
		{  m_Length/2, m_Length/2,  -m_Length/2, 0.0f, 0.0f, },//F
		{ -m_Length/2, 0.0f,   -m_Length/2, 1.0f, 1.0f, },//G
		{ -m_Length/2, m_Length/2,  -m_Length/2, 1.0f, 0.0f, },//H

		//������ĸ�����
		{ -m_Length/2, 0.0f,   -m_Length/2, 0.0f, 1.0f, },
		{ -m_Length/2, m_Length/2,  -m_Length/2, 0.0f, 0.0f, },
		{ -m_Length/2, 0.0f,    m_Length/2, 1.0f, 1.0f, },
		{ -m_Length/2, m_Length/2,   m_Length/2, 1.0f, 0.0f, },

		//������ĸ�����
		{ m_Length/2, 0.0f,   m_Length/2, 0.0f, 1.0f, },
		{ m_Length/2, m_Length/2,  m_Length/2, 0.0f, 0.0f, },
		{ m_Length/2, 0.0f,  -m_Length/2, 1.0f, 1.0f, },
		{ m_Length/2, m_Length/2, -m_Length/2, 1.0f, 0.0f, },

		//������ĸ�����
		{  m_Length/2, m_Length/2, -m_Length/2, 1.0f, 0.0f, },//f
		{  m_Length/2, m_Length/2,  m_Length/2, 1.0f, 1.0f, },//d
		{ -m_Length/2, m_Length/2, -m_Length/2, 0.0f, 0.0f, },//h
		{ -m_Length/2, m_Length/2,  m_Length/2, 0.0f, 1.0f, }, //b

		

		//������ĸ�����
		{ -m_Length/2, 0.0f,    -m_Length/2, 0.0f, 1.0f, },
		{ -m_Length/2, 0.0f,   m_Length/2, 0.0f, 0.0f, },
		{ m_Length/2, 0.0f, - m_Length/2, 1.0f, 1.0f, },
		{ m_Length/2, 0.0f,   m_Length/2, 1.0f, 0.0f, }

    };
	//׼����䶥������
    void* pVertices;
	//2.����
    m_pVertexBuffer->Lock( 0, 0, (void**)&pVertices, 0 );
	//3.���ʡ��ѽṹ���е�����ֱ�ӿ������㻺������
    memcpy( pVertices, vertices, sizeof(vertices) );
	//4.����
	m_pVertexBuffer->Unlock();
}
//============================================================================================
//��Ⱦ��պк���
//============================================================================================
void SkyBox::RenderSkyBox()
{
	Device->SetRenderState(D3DRS_LIGHTING,false);
	Device->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);  //��������ɫ��ϵĵ�һ����������ɫֵ�������  
    Device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );   //������ɫ��ϵĵ�һ��������ֵ��ȡ������ɫֵ  

	//���������ȡѰַģʽ
	Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	D3DXMATRIX matSky,matTransSky,matRotSky;  
	D3DXMatrixTranslation(&matTransSky,0.0f,-15000.0f,0.0f);  
    D3DXMatrixRotationY(&matRotSky, -0.000005f*timeGetTime());   //��ת�������, ��ģ���Ʋ��˶�Ч��  
    matSky=matTransSky*matRotSky;  

	Device->SetTransform (D3DTS_WORLD,&matSky);

	Device->SetStreamSource(0,m_pVertexBuffer, 0, sizeof(SKYBOXVERTEX));    //�Ѱ����ļ�������Ϣ�Ķ��㻺�����Ⱦ��ˮ�������

	Device->SetFVF(SkyBoxFVF);  //����FVF�����ʽ  
    //һ��forѭ������5������Ƴ���  
    for(int i =0; i<6; i++)  
    {  
        Device->SetTexture(0, Texture[i]);  
       Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, i*4, 2);  
    }  
	Device->SetRenderState (D3DRS_LIGHTING,true);
}

//=======================================================================
//��������
//=======================================================================
SkyBox::~SkyBox(void)  
{  
	m_pVertexBuffer->Release ();
    for(int i=0; i<5; i++)  
    {  
		Texture[i]->Release ();
    }  
}