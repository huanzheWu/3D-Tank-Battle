#include"Mytank.h"

#define D3DFVF_GUI (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)//Ѫ���������ʽ

Tank::Tank(LPDIRECT3DDEVICE9 _Device)
{
	Device=_Device;
	D3DXMatrixIdentity(&matHero);
	blood =12;
	
		//���ļ�������Ѫ����ͼ
		D3DXCreateTextureFromFile(Device,L"x0.png", &Tex[0]);
		D3DXCreateTextureFromFile(Device,L"x1.png", &Tex[1]);
		D3DXCreateTextureFromFile(Device,L"x2.png", &Tex[2]);
		D3DXCreateTextureFromFile(Device,L"x3.png", &Tex[3]);
		D3DXCreateTextureFromFile(Device,L"x4.png", &Tex[4]);
		D3DXCreateTextureFromFile(Device,L"x5.png", &Tex[5]);
		D3DXCreateTextureFromFile(Device,L"x6.png", &Tex[6]);
		D3DXCreateTextureFromFile(Device,L"x7.png", &Tex[7]);
		D3DXCreateTextureFromFile(Device,L"x8.png", &Tex[8]);
		D3DXCreateTextureFromFile(Device,L"x9.png",&Tex[9]);
		D3DXCreateTextureFromFile(Device,L"x10.png",&Tex[10]);
		D3DXCreateTextureFromFile(Device,L"x11.png",&Tex[11]);

		//�������㻺��
		Device->CreateVertexBuffer (
		4*sizeof(GUIVERTEX1),
		0,
		D3DFVF_GUI,
		D3DPOOL_MANAGED,
		&Vb,
		0
		);

		unsigned long white = D3DCOLOR_XRGB(255,255,255);
		//��ȡѪ���ĳ����
		D3DSURFACE_DESC desc;
		Tex[11]->GetLevelDesc(0, &desc);
		float w = (float)desc.Width;
		float h = (float)desc.Height;
		
		GUIVERTEX1 * Vertex;
		Vb->Lock (0,0,(void **)&Vertex,0);
		Vertex[0]=GUIVERTEX1(w+10,10,0,1,white,1,0);
		Vertex[1]=GUIVERTEX1(w+10,10+h,0,1,white,1,1);
		Vertex[2]=GUIVERTEX1(10,10,0,1,white,0,0);
		Vertex[3]=GUIVERTEX1(10,10+h,0,1,white,0,1);
		Vb->Unlock ();

}

void Tank::SetTank(CamerClass* Camera,DInputClass * DInput)
{

		static float j=0;
		D3DXMATRIX matscal;
		D3DXMatrixScaling(&matscal,0.01f,0.01f,0.01f);//��ģ����С
		D3DXMATRIX setrightpos; //��ת��ȷ�ķ���
		D3DXMatrixRotationY(&setrightpos,D3DX_PI); //��ת180�ȣ��������ͬ��
		(*Camera).getposition (&tankposition);//��ȡ�������λ��
		DInput->GetInput();  

		j+=DInput->MouseDX()* 0.006f;
		//����ƶ�����������Ӿ�
		(*Camera).yaw(DInput->MouseDX()* 0.006f); 

		D3DXMATRIX temp;
		D3DXMatrixRotationY(&temp,j);
		D3DXMatrixTranslation(&matHero,tankposition.x ,tankposition.y-100.0f,tankposition.z);  //�ƶ�
		matHero=matscal*temp*setrightpos*matHero;

		Device->SetTransform(D3DTS_WORLD, &matHero);//����ģ�͵��������Ϊ������׼�� 

}
//
//�ҷ�̹������ߵ���ײ����
//
bool Tank::collisiontest(D3DXVECTOR3 _position)
{
		double distance;
		distance=sqrt(pow((tankposition.x-_position.x),2)+pow((tankposition.y -_position.y),2)+pow((tankposition.z-_position.z),2));
		if(distance<100)
			return true;
		return false;
}

void Tank::rendblood()
{
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		Device->SetTexture(0,Tex[blood-1]);
		Device->SetStreamSource(0, Vb, 0, sizeof(GUIVERTEX1));
		Device->SetFVF(D3DFVF_GUI);
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);


}

void Tank::calcublood(int i)
{
	blood -=i;
	if(blood <1)
		blood =1;
}