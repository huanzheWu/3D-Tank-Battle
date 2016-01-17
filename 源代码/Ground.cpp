#include"Ground.h"

//********��ʼ������***************************
bool Ground::initObject ()
{
	//����Χǽ������
	D3DXCreateTextureFromFile (Device,L"maoshe1.bmp",&Tex[0]);
	Wall=new XFileMesh(Device);
	//��ʼ��Χǽģ��
	Wall->InitXfile (L"shu.x");
	//����ǽ�ڵ����ž���
	D3DXMatrixScaling(&WallSca,1000.0f,1000.0f,1000.0f);

	return true;
}

//***********����Χǽ����***********************
bool Ground ::createWall (D3DXVECTOR3 centre)
{
	float i;
	D3DXVECTOR3 position;
	for (i=centre.x -500;i<=centre.x+500;i+=100)  //ǰ
	{
		position.y =0.0f,position.z =500.0f,position.x=i;
		_allWalls.push_back (position);
	}
	for (i=centre.z-500;i<=centre.z +500;i+=100)//��
	{
		position.x=-500.0f,position.y=0.0f,position.z =i;
		_allWalls.push_back (position);
	}
	for(i=centre.z -500;i<=centre.z+500;i+=100 )//��
	{
		position.x =500.0f,position.y =0.0f,position.z =i;
		_allWalls.push_back (position);
	}
	for (i=centre.x -500;i<=centre.x+500;i+=100)//��
	{
		position.y =0.0f,position.z =-500.0f,position.x=i;
		_allWalls.push_back (position);
	}

	return true ;
}
//***********��Ⱦ�������庯��*******************
void Ground::renderGround ()
{
	std::list <D3DXVECTOR3 >::iterator i;
	for(i=_allWalls.begin ();i!=_allWalls.end ();i++)
	{
		//����������󣬰�Χǽ�������ʵ���λ��
		
		D3DXMatrixTranslation(&WallPos,i->x ,i->y ,i->z);
		WallPos=WallSca*WallPos;
		Device->SetTransform (D3DTS_WORLD,&WallPos);
		//��������
		Device->SetTexture (0,Tex[0]);
		//����Χǽx�ļ�
		Wall->RenderXfile ();
	}
}
