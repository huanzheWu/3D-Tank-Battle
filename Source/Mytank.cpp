#include"Mytank.h"
Tank::Tank(LPDIRECT3DDEVICE9 _Device)
{
	Device=_Device;
	D3DXMatrixIdentity(&matHero);
}

void Tank::SetTank(CamerClass* Camera,DInputClass * DInput)
{

		static float j=0;
		D3DXMATRIX matscal;
		D3DXMatrixScaling(&matscal,0.01f,0.01f,0.01f);//��ģ����С
		D3DXVECTOR3 tankposition;//̹�˵�λ��
		D3DXMATRIX setrightpos; //��ת��ȷ�ķ���
		D3DXMatrixRotationY(&setrightpos,D3DX_PI); //��ת180�ȣ��������ͬ��
		(*Camera).getposition (&tankposition);//��ȡ�������λ��
		DInput->GetInput();  

		//if(DInput->IsKeyDown(DIK_A))
		//	j+=0.006;
		//if(DInput->IsKeyDown(DIK_D))
		//	j-=0.006;
		j+=DInput->MouseDX()* 0.006f;
		//����ƶ�����������Ӿ�
		(*Camera).yaw(DInput->MouseDX()* 0.006f); 
	/*	(*Camera).yaw(j);  */


		D3DXMATRIX temp;
		D3DXMatrixRotationY(&temp,j);
		D3DXMatrixTranslation(&matHero,tankposition.x ,tankposition.y-100.0f,tankposition.z);  //�ƶ�
		matHero=matscal*temp*setrightpos*matHero;

		Device->SetTransform(D3DTS_WORLD, &matHero);//����ģ�͵��������Ϊ������׼�� 
	
}