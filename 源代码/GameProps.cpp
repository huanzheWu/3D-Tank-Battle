#include"GameProps.h"
#include"XFileMesh.h"
#include"Explode.h"

#define D3DFVF_TIME (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)//ʱ��������ʽ

extern Tank *mytank;//����melon�е�̹��ʵ����������չ��������ҷ�̹�˺͵�����ײ�������裩
extern EnemyTank *enetank;//����melon�еĵ���̹��ʵ����������չ��������ɻ��ӵ���з�̹����ײ�������裩
extern Explode *explode;//�ѱ�ըʵ����������չ������

GameProps::GameProps (LPDIRECT3DDEVICE9 _Device)
{
	Device=_Device;
	time=60;
}
//====================================================================
//��ʼ����Ϸ���ߺ���
//���ֵ��ߵ�xģ�Ͷ�Ӧ������һ���г�ʼ��
//====================================================================

void GameProps::InitProps()
{
	//Ϊ3�����ߴ���3��XFileMesh�ӿ�
	PPops1 =new XFileMesh (Device);
	PPops2	=new XFileMesh (Device);
	PPops3 =new XFileMesh(Device);
	//�����ɻ�x�ļ��ӿ�
	PEffect1=new XFileMesh(Device);
	PBullet=new XFileMesh(Device);

	//��ʼ��3�����ߵ�x�ļ�
	PPops1->InitXfile(L"m18nade.x"); //����ʱ���е㳤
	PPops2->InitXfile(L"shose.x");
	PPops3->InitXfile(L"medkit.x");
	PBullet->InitXfile (L"444.x"); //����ʱ���е㳤

	//��ʼ���ɻ�x�ļ�
	PEffect1->InitXfile(L"dropship.x");


	//������������
	for(int i=1;i<4;i++)
	{
		PROPS props;
		props.ID=i; 
		props._isalive=false;
		props._happenTime=0; 
		props._timeDelta=3;
		_allProps.push_back (props);
	}
	//
	//
	//
	//
	wchar_t a[61][10]={{},
		{L"1.png"},{L"2.png"},{L"3.png"},{L"4.png"},{L"5.png"},{L"6.png"},{L"7.png"},{L"8.png"},{L"9.png"},
		{L"10.png"},{L"11.png"},{L"12.png"},{L"13.png"},{L"14.png"},{L"15.png"},{L"16.png"},{L"17.png"},{L"18.png"},{L"19.png"},
		{L"20.png"},{L"21.png"},{L"22.png"},{L"23.png"},{L"24.png"},{L"25.png"},{L"26.png"},{L"27.png"},{L"28.png"},
		{L"29.png"},{L"30.png"},{L"31.png"},{L"32.png"},{L"33.png"},{L"34.png"},{L"35.png"},{L"36.png"},{L"37.png"},{L"38.png"},
		{L"39.png"},{L"40.png"},{L"41.png"},{L"42.png"},{L"43.png"},{L"44.png"},{L"45.png"},{L"46.png"},{L"47.png"},{L"48.png"},
		{L"49.png"},{L"50.png"},{L"51.png"},{L"52.png"},{L"53.png"},{L"54.png"},{L"55.png"},{L"56.png"},{L"57.png"},{L"58.png"},
		{L"59.png"},{L"60.png"}
	};
	for(int i=1;i<=60;i++)
	{
		D3DXCreateTextureFromFile(Device,a[i], &TimeTex[i]);//��������
	}
		unsigned long white = D3DCOLOR_XRGB(255,255,255);
		//��ȡѪ���ĳ����
		D3DSURFACE_DESC desc;
		TimeTex[10]->GetLevelDesc(0, &desc);
		float w = (float)desc.Width;
		float h = (float)desc.Height;

		Device->CreateVertexBuffer (
		4*sizeof(TIME),
		0,
		D3DFVF_TIME,
		D3DPOOL_MANAGED,
		&Vb,
		0
		);

		TIME * Vertex;
		Vb->Lock(0,0,(void **)&Vertex,0);
		Vertex[0]=TIME(w+600,10,0,1,white,1,0);
		Vertex[1]=TIME(w+600,10+h,0,1,white,1,1);
		Vertex[2]=TIME(600,10,0,1,white,0,0);
		Vertex[3]=TIME(600,10+h,0,1,white,0,1);
		Vb->Unlock ();

		sign=0;//���ٵ��߳�ʼ��Ϊ0����ʾ��û���Ե�
		sign3=0; 
		speeduptime=10;//����ʱ��Ϊ10s��
}

//================================================================================
//���µ��ߺ���
//����ֻ�뵽���µ��ߵ�����״̬������λ��
//
//================================================================================
void GameProps::upDateProps(float timeDelta)
{
	std::list <PROPS>::iterator i;
	for(i=_allProps.begin();i!=_allProps.end ();i++)
	{
		i->_happenTime+=timeDelta;
		if(i->_happenTime>i->_timeDelta&&i->_isalive==false) //ֻ�е�ʱ�䵽�ˣ����ҵ���������ʱ���ȥ����������������һ��ʱ�����߻��ƶ�������
				{
					i->_isalive=true; //���߳���ʱ�䵽��
					//���������һ�����ֵ�λ��
					i->_position.x=d3d::GetRandomFloat (-1000,1000);
					i->_position.y =-100.0f;
					i->_position.z=d3d::GetRandomFloat (-1000,1000);
				}
		
	}
}

//==========================================================================
//��Ⱦ���ߺ���
//�����ϵ��һ�����⣺ʲôʱ���Ҫ��Ⱦ����Ⱦ�����״̬��ζ�
//��ν����ߺ�����x�ļ���ϵ����
//==========================================================================
void GameProps::RenderProps ()
{
	D3DXMATRIX position;//���ߵĳ���λ��
	D3DXMATRIX scaling;
	D3DXMatrixScaling(&scaling,1,1,1);
	std::list <PROPS>::iterator i;
	for(i=_allProps.begin();i!=_allProps.end ();i++)
	{
		if(i->_isalive==true)
		{
			//�����������
			D3DXMatrixTranslation(&position,i->_position.x,i->_position.y,i->_position.z);
			position=scaling*position;
			Device->SetTransform (D3DTS_WORLD,&position);
			
			if(i->ID==1)
				PPops1->RenderXfile ();
			if(i->ID==2)
				PPops2->RenderXfile();
			if(i->ID==3)
				PPops3->RenderXfile();
		}
	}
}
//===============================================================================
//������ײ����Լ�������ص�Ч������
//===============================================================================
void GameProps::Effect(float timeDelta)
{
	std::list <PROPS>::iterator i;
	for(i=_allProps.begin();i!=_allProps.end ();i++)
	{
		if(mytank->collisiontest(i->_position)&&i->_isalive==true)
		{
			i->_happenTime =0.0f;//���߳���ʱ�����㡣//��һ��Ҫ�Ƶ����궫����ʱ���ʹ��
			i->_isalive=false;

			if(i->ID==1)//����һ
				{
					addplane (10);//��ɻ��������10�ܷɻ�
				}
			if (i->ID==2)
				{
					sign=1;//���ٵ��߱�ǣ�����Ƿ񱻳Ե�
					if(time>=50)
						time=60;
					else
					time+=10;
				}
			if(i->ID==3)
				sign3=1;
		}
	}
}
//==============================================================================
//����һЧ����Ⱦ����
//==============================================================================
void GameProps::effect1(D3DXMATRIX *V,float timeDalte)
{
	//�����10���ɻ�����ô���ڷɻ������������10��Ԫ��
	std::list <PLANE>::iterator i;
	D3DXMATRIX matposition;
	D3DXMATRIX matscaling;
	D3DXMatrixScaling(&matscaling,3.0f,3.0f,3.0f);
	if(!_allplane.empty ())
	{
		for(i=_allplane.begin();i!=_allplane.end ();i++)
		{
			if(i->_isalive==true) //����ɻ�����
			{
				D3DXMatrixTranslation(&matposition,i->_position.x,i->_position.y,i->_position.z); //���÷ɻ����ֵ�λ��
				matposition=matscaling*matposition;
				Device->SetTransform (D3DTS_WORLD,&matposition);
				PEffect1->RenderXfile ();//��Ⱦ�ɻ�
			}
		}
	}

	//
	//
	//��Ⱦ�ӵ�
	std::list <PROBULLET> ::iterator  j;
	if(!_allbullet.empty())
	{
		for(j=_allbullet.begin();j!=_allbullet.end ();j++)
		{
			if (j->_isalive==true)
			{
				D3DXMATRIX setposition,scaling,rotation;
				D3DXMatrixScaling(&scaling,0.01,0.01,0.01);
				D3DXMatrixTranslation(&setposition,j->_position.x,j->_position.y,j->_position.z);
				D3DXMatrixRotationZ(&rotation,D3DX_PI/2.0f);
				setposition=scaling*rotation*setposition;
				Device->SetTransform (D3DTS_WORLD,&setposition);
				PBullet->RenderXfile();//��Ⱦ�ӵ�
			}
		}
	}
	//
	//
	//
	updateplane (V,timeDalte);//���·ɻ����ӵ�״̬
	removedead ();//�Ƴ������ɻ����ӵ�
}

//=======================================================================
//�Ƴ��Ѿ������ķɻ�
//=======================================================================
void GameProps::removedead ()
{
	//���б����Ƴ������ķɻ�
	std::list <PLANE>::iterator i;
	i=_allplane.begin();
	while(i!=_allplane.end ())
	{
		if(i->_isalive==false)
		i=_allplane.erase (i);//����ɻ����������б����Ƴ�
		else
			i++;
	}

	//���б����Ƴ��������ӵ�
	std::list <PROBULLET>::iterator j;
	j=_allbullet.begin();
	while(j!=_allbullet.end ())
	{
		if(j->_isalive==false)
			j=_allbullet.erase (j);
		else 
			j++;
	}
}

//========================================================================
//���·ɻ����ӵ���״̬����
//========================================================================
void GameProps::updateplane (D3DXMATRIX * V,float timeDalte)
{
	//���·ɻ�״̬
	std::list <PLANE>::iterator i;
	for(i=_allplane.begin();i!=_allplane.end ();i++)
	{
		i->_keeptime+=timeDalte;

		//���·ɻ�λ�ã�ʵ����ֻ��Ҫ����z�Ϳ����ˣ�
		i->_position.x+=500*timeDalte*(i->_look.x);
		i->_position.y+=500*timeDalte*(i->_look.y);
		i->_position.z+=500*timeDalte*(i->_look.z);

		//
		static float time=1.0f;
		time+=timeDalte;
		if((int)time%2==0)
		{
			addbullet(i->_position);
			time=1;
		}
		//

		//�ɻ��ɵ�ʱ�乻���ˣ�״̬��Ϊ����
		if(i->_keeptime>i->_lifetime)
			i->_isalive=false;
	}

	//�����ӵ�����
	std::list<PROBULLET> ::iterator j;
	for(j=_allbullet.begin();j!=_allbullet.end ();j++)
	{
		if(j->_isalive=true)
		{
			//�����ӵ���λ��
			j->_position.x+=300*timeDalte*(j->_speed.x);
			j->_position.y +=300*timeDalte*(j->_speed.y);
			j->_position.z +=300*timeDalte*(j->_speed.z);
		}
		j->_happentime+=timeDalte;
		if(j->_happentime>j->_lifetime)
			{
				j->_isalive=false;
				
				//��ʼ��һ��λ�ò��������ը������
				EXPLODE explode;
				explode._position=j->_position;
				explode.sign=0;
				_allbound.push_back(explode);

			}
		if(enetank->Collisiontest (j->_position))
			{
				j->_isalive=false;
				//��ʼ��һ��λ�ò��������ը������
				EXPLODE explode;
				explode._position=j->_position;
				explode.sign=0;
				_allbound.push_back(explode);
			}
	}
}

//==========================================================================
//���������������ʼ��һ���·ɻ�
//==========================================================================
void GameProps::resetplane (PLANE *plane)
{
	plane->_position.x=d3d::GetRandomFloat (-2000,2000);
	plane->_position.y=300;
	plane->_position.z=d3d::GetRandomFloat (2000,2500);
	plane->_isalive=true;
	plane->_keeptime=0;
	plane->_look.x=0.0f,plane->_look.y =0.0f,plane->_look.z=-1.0f;
	plane->_lifetime=10;
}
//============================================================================
//��ӷɻ�����
//============================================================================
void GameProps::addplane (int numofplane)
{
	for(int i=0;i<numofplane;i++)
	{
		PLANE plane;
		resetplane(&plane);//��ʼ��һ�ܷɻ�
		_allplane.push_back (plane);//������µķɻ�����ɻ�����
	}
}

//============================================================================
//��ʼ���ӵ�����
//============================================================================
void GameProps::resetbullet(D3DXVECTOR3 position,PROBULLET* bullet)
{
	bullet->_happentime=0;
	bullet->_isalive=true;
	bullet->_lifetime=1.5;
	bullet->_speed.x=0.0f,bullet->_speed.y=-1,bullet->_speed.z=0.0f;
	bullet->_position.x=position.x,bullet->_position.y=position.y ,bullet->_position.z =position.z;
}
//===========================================================================
//����ӵ�����
//===========================================================================
void GameProps::addbullet(D3DXVECTOR3 position)
{
	PROBULLET bullet;
	resetbullet (position,&bullet);
	_allbullet.push_back(bullet);
}

//=======================================================================
//���ٵ��ߺ���
//=======================================================================
bool GameProps::Effect2(float timeDelta)
{
	static float overtime=0;
	overtime+=timeDelta;
	if(overtime>speeduptime)
	{
		sign=0;
		overtime=0;
	}
	if(sign==1)
	{
		return true;
	}
	return false;
}

//==========================================================================
//���߱�ը����
//==========================================================================
void GameProps::renderbound (D3DXMATRIX *V,float timeDelta)
{
	std::list <EXPLODE>::iterator i;
	D3DXMATRIX matposition;
	if(!_allbound.empty ())
	{
		i=_allbound.begin();
		while(i!=_allbound.end ())
		{
			D3DXMatrixTranslation(&matposition,i->_position.x,i->_position.y,i->_position.z);
			Device->SetTransform (D3DTS_WORLD,&matposition);//ָ����ը�ص�
			explode->RanderExplpde(V,matposition,timeDelta,&(i->sign));
			if(i->sign>24)
				i=_allbound.erase (i);
			else 
				i++;
		}
	}
}

//=====================================================
//�������������Ƿ񱻳Ե�
//=====================================================
bool GameProps::Effect3()
{
	if(sign3==1)
		{
			sign3=0;
			return true;
	}
	return false;
}

//=======================================================
//����ʱ�亯��
//����������˻���ʱ���⣬�䷵��ֵ������ʱ���Ƿ��ù⣨��Ϸ�ǲ���Ӧ�ý�����
//
//=======================================================

bool GameProps::rendertime (float timeDelta)
{

	time-=timeDelta;
	if(time<=1)
	{
		return true;//��ʾ��ϷӦ�ý�����
	}
	else
	{
	//�����ں�
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	Device->SetTexture(0,TimeTex[(int)time]);
	Device->SetStreamSource(0, Vb, 0, sizeof(TIME));
	Device->SetFVF(D3DFVF_TIME);
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	
	//�ر��ں�
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
	return false ;//��ʾʱ�仹ʣ��
	}
	
}