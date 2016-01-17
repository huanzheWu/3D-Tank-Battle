#include"Props.h"
#include<math.h>

//*****************��ʼ�����ߺ���*********************
bool Props::initProps()
{
	Prop1=new XFileMesh (Device);
	Prop1->InitXfile (L"shose.x");//��ʼ������ģ��

	//��������ģ�Ͳ���
	D3DXCreateTextureFromFile(Device,L"��.bmp",&propstex[0]);

	for(int i=1;i<=1;i++)  //����������м������ ��������ʱֻ��һ����
	{
		PROPS prop;
		prop._isalive=true;
		prop._id=i;
		prop._timeDelta=2;
		prop._timeHappen=0;
		prop._position.x =d3d::GetRandomFloat (-100,100);
		prop._position.y =d3d::GetRandomFloat (-100,100);
		prop._position.z =d3d::GetRandomFloat (-100,100);
		_allprops.push_back (prop);
	}
	return true ;
}

//****************���µ��ߺ���******************************
void Props::upDateProps (float timeDelta)
{
	std::list <PROPS>::iterator i;
	for (i=_allprops.begin ();i!=_allprops.end ();i++)
	{
		i->_timeHappen+=timeDelta;
		//������ߵ��˸ó��ֵ�ʱ�䣬������������״̬�����λ��
		if(i->_timeHappen>i->_timeDelta&&i->_isalive==false)
		{
			i->_position.x=d3d::GetRandomFloat (-400,400);
			i->_position.y=d3d::GetRandomFloat (50,400);
			i->_position.z=d3d::GetRandomFloat (-400,400);
			i->_isalive=true;
		}
	}
}

//**************��Ⱦ���ߺ���*************************
void Props::RenderProps ()
{
	std ::list <PROPS >::iterator i;
	for(i=_allprops.begin ();i!=_allprops.end ();i++)
	{
		if(i->_isalive==true)
		{
			//��������任
			D3DXMATRIX position;
			D3DXMatrixTranslation(&position,i->_position.x ,i->_position.y ,i->_position.z);
			Device->SetTransform (D3DTS_WORLD,&position);
			Device->SetTransform (D3DTS_WORLD,&position);
			//���Ƶ���
			if(i->_id==1)  
			{
				Device->SetTexture (0,propstex[0]);
				Prop1->RenderXfile ();
			}
		}
	}
}

//**********��ײ����******������ߵ���ײ*********************
int Props::Collision (D3DXVECTOR3 position)
{
	std ::list <PROPS>::iterator  i;
	double distance=100.0f;
	for(i=_allprops.begin ();i!=_allprops.end ();i++)
	{
		if(sqrt(    pow(i->_position.x-position.x,2)   +  pow(i->_position.y-position.y,2)
			+pow(i->_position.z-position.z,2)  )  <distance&&i->_isalive==true)
		{
			i->_isalive=false;
			i->_timeHappen=0;
			if(i->_id==1)
				return ADDBODY;
		}
	}
}