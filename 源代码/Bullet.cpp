#include"Bullet.h"
#include"EnemyTank.h"
#include"Explode.h"

extern EnemyTank *enetank;
extern Explode *explode;
class GroundObjetc;


BULLET::BULLET(LPDIRECT3DDEVICE9 _Device,CamerClass * camera)
{
	_camera=camera;
	Device=_Device;
}
//==============================================================================
//这个函数用来加载子弹模型的x文件。
//==============================================================================
bool BULLET::InitBullet ()
{
	InitXfile(L"333.x"); //初始化子弹x文件
	return true;
}

//==============================================================================
//这个resetbullet函数用来初始化一颗子弹的状态，用于在addBullet函数中向容器尾部加入一个新子弹
//==============================================================================
void BULLET::resetbullet (Bullet *bullet)
{
	_camera->getposition(&(bullet->_position ));//获取当前摄像机的位置
	//设定子弹的正确位置
	bullet->_position.z+=0.0f;
	bullet->_position.y+=-6.5f;
	bullet->_position.x+=0.0f;
	//以摄像机的观察向量为子弹的速度方向
	_camera->getlook (&(bullet->_speed));
	bullet->_age=0;
	//寿命2秒
	bullet->_lifetime=1;
	//活着
	bullet->_alive=true;
}
//=============================================================
//增加新的子弹的函数
//=============================================================
void BULLET::addbullet ()
{
	Bullet bullet;
	resetbullet(&bullet);
	_allbuttet.push_back (bullet);
}
//============================================================
//更新子弹函数，判断是否已经死亡，更新位置
//============================================================
void BULLET::Update (float timeDelta)
{
	std::list<Bullet>::iterator i;
	for(i=_allbuttet.begin ();i!=_allbuttet.end ();i++)
	{
			//更新每个子弹的位置
			i->_position.z+=timeDelta*500*i->_speed.z;
			i->_position.y+=timeDelta*500*i->_speed.y;
			i->_position.x+=timeDelta*500*i->_speed.x;
			//更新子弹的年龄
			i->_age += timeDelta;
		if(i->_age > i->_lifetime ) 
		{
			i->_alive = false;
		}
		//如果子弹击中了坦克，那么子弹也会死亡
		if(enetank->Collisiontest(i->_position))
		{
			i->_alive=false;
			//初始化一个爆炸位置
			EXPLODEA explode;
			explode._position=i->_position;
			explode.sign=0;

			_allbound.push_back (explode);//把产生子弹碰撞置加入爆炸位置链表
			//在这里调用
		}
	}
	removedead ();
}

//=========================================================
//渲染子弹函数
//=========================================================

void BULLET::render ()
{
	std::list <Bullet>::iterator i;
	if(!_allbuttet.empty ())
	{
		for(i=_allbuttet.begin();i!=_allbuttet.end();i++)
		{
			if(i->_alive==true)
				{
					D3DXMATRIX Move;
					D3DXMatrixTranslation(&Move,i->_position.x,i->_position.y -30,i->_position.z);//指定子弹的初始位置 
					D3DXMATRIX matscal;
					//缩放子弹
					D3DXMatrixScaling(&matscal,0.150f,0.150f,0.150f);
					Move=matscal*Move;
					Device->SetTransform (D3DTS_WORLD,&Move);
					RenderXfile ();
				}
		}
	}
}
//==============================================================================
//函数removedead，移除列表中死亡的子弹
//==============================================================================
void BULLET::removedead ()
{
	std::list<Bullet>::iterator i;
	i=_allbuttet.begin();
	while(i!=_allbuttet.end ())
	{
		if(i->_age>i->_lifetime)
			i=_allbuttet.erase (i);
		else
			i++;
	}
}


void BULLET ::renderbound(D3DXMATRIX *V,float timeDelta)
{
	std::list <EXPLODEA>::iterator i;
	D3DXMATRIX pos;
	if(!_allbound.empty())
	{
		i=_allbound.begin ();
		while(i!=_allbound.end ())
		{	
			D3DXMatrixTranslation(&pos,i->_position.x ,i->_position.y-100 ,i->_position.z);
			Device->SetTransform (D3DTS_WORLD,&pos);
			if(!explode->RanderExplpde(V,pos,timeDelta,&(i->sign)))
				i=_allbound.erase (i);
			else
				i++;
				
		}
	}
}
