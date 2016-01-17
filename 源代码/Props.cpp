#include"Props.h"
#include<math.h>

//*****************初始化道具函数*********************
bool Props::initProps()
{
	Prop1=new XFileMesh (Device);
	Prop1->InitXfile (L"shose.x");//初始化道具模型

	//创建道具模型材质
	D3DXCreateTextureFromFile(Device,L"花.bmp",&propstex[0]);

	for(int i=1;i<=1;i++)  //向道具链表中加入道具 （这里暂时只加一个）
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

//****************更新道具函数******************************
void Props::upDateProps (float timeDelta)
{
	std::list <PROPS>::iterator i;
	for (i=_allprops.begin ();i!=_allprops.end ();i++)
	{
		i->_timeHappen+=timeDelta;
		//如果道具到了该出现的时间，重新设置它的状态和随机位置
		if(i->_timeHappen>i->_timeDelta&&i->_isalive==false)
		{
			i->_position.x=d3d::GetRandomFloat (-400,400);
			i->_position.y=d3d::GetRandomFloat (50,400);
			i->_position.z=d3d::GetRandomFloat (-400,400);
			i->_isalive=true;
		}
	}
}

//**************渲染道具函数*************************
void Props::RenderProps ()
{
	std ::list <PROPS >::iterator i;
	for(i=_allprops.begin ();i!=_allprops.end ();i++)
	{
		if(i->_isalive==true)
		{
			//设置世界变换
			D3DXMATRIX position;
			D3DXMatrixTranslation(&position,i->_position.x ,i->_position.y ,i->_position.z);
			Device->SetTransform (D3DTS_WORLD,&position);
			Device->SetTransform (D3DTS_WORLD,&position);
			//绘制道具
			if(i->_id==1)  
			{
				Device->SetTexture (0,propstex[0]);
				Prop1->RenderXfile ();
			}
		}
	}
}

//**********碰撞函数******蛇与道具的碰撞*********************
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