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
//����������������ӵ�ģ�͵�x�ļ���
//==============================================================================
bool BULLET::InitBullet ()
{
	InitXfile(L"333.x"); //��ʼ���ӵ�x�ļ�
	return true;
}

//==============================================================================
//���resetbullet����������ʼ��һ���ӵ���״̬��������addBullet������������β������һ�����ӵ�
//==============================================================================
void BULLET::resetbullet (Bullet *bullet)
{
	_camera->getposition(&(bullet->_position ));//��ȡ��ǰ�������λ��
	//�趨�ӵ�����ȷλ��
	bullet->_position.z+=0.0f;
	bullet->_position.y+=-6.5f;
	bullet->_position.x+=0.0f;
	//��������Ĺ۲�����Ϊ�ӵ����ٶȷ���
	_camera->getlook (&(bullet->_speed));
	bullet->_age=0;
	//����2��
	bullet->_lifetime=1;
	//����
	bullet->_alive=true;
}
//=============================================================
//�����µ��ӵ��ĺ���
//=============================================================
void BULLET::addbullet ()
{
	Bullet bullet;
	resetbullet(&bullet);
	_allbuttet.push_back (bullet);
}
//============================================================
//�����ӵ��������ж��Ƿ��Ѿ�����������λ��
//============================================================
void BULLET::Update (float timeDelta)
{
	std::list<Bullet>::iterator i;
	for(i=_allbuttet.begin ();i!=_allbuttet.end ();i++)
	{
			//����ÿ���ӵ���λ��
			i->_position.z+=timeDelta*500*i->_speed.z;
			i->_position.y+=timeDelta*500*i->_speed.y;
			i->_position.x+=timeDelta*500*i->_speed.x;
			//�����ӵ�������
			i->_age += timeDelta;
		if(i->_age > i->_lifetime ) 
		{
			i->_alive = false;
		}
		//����ӵ�������̹�ˣ���ô�ӵ�Ҳ������
		if(enetank->Collisiontest(i->_position))
		{
			i->_alive=false;
			//��ʼ��һ����ըλ��
			EXPLODEA explode;
			explode._position=i->_position;
			explode.sign=0;

			_allbound.push_back (explode);//�Ѳ����ӵ���ײ�ü��뱬ըλ������
			//���������
		}
	}
	removedead ();
}

//=========================================================
//��Ⱦ�ӵ�����
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
					D3DXMatrixTranslation(&Move,i->_position.x,i->_position.y -30,i->_position.z);//ָ���ӵ��ĳ�ʼλ�� 
					D3DXMATRIX matscal;
					//�����ӵ�
					D3DXMatrixScaling(&matscal,0.150f,0.150f,0.150f);
					Move=matscal*Move;
					Device->SetTransform (D3DTS_WORLD,&Move);
					RenderXfile ();
				}
		}
	}
}
//==============================================================================
//����removedead���Ƴ��б����������ӵ�
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
