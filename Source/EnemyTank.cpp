#include<math.h>
#include"EnemyTank.h"

extern GroundObject * groundobject ;//�ѵ�������ʵ������������չ�������У���Ϊ��Ҫ���ʸ�ʵ���еĳ�Ա

EnemyTank::EnemyTank (LPDIRECT3DDEVICE9 _Device)
{
	Device=_Device;
	deadline=200.0;//�趨̹�˵ı�������Χ
}
bool EnemyTank::InitTank ()
{
	InitXfile(L"M3FRVT1.x");//ָ���з�̹��x�ļ�ģ��
	return true;
}
//============================================================
//���������������̹�˵ĳ�ʼ��Ϣ
//============================================================
void EnemyTank::resetTank (ENEMYTANK *etank)
{
	//λ�����
	etank->_position.x=d3d::GetRandomFloat(-1000,1000);
	etank->_position.y =00.0f;
	etank->_position.z=d3d::GetRandomFloat (2000,4000);
	etank->_isalive=true;
	D3DXMatrixIdentity(&(etank->_rotation));
	//̹�˵�look������Ҫ����ģ�ͣ��۲�һ��ʼ̹���ǳ����ıߣ���������
	//���̹�˵�look��������crood������������ת�����������������ȼ�������lookΪ��0��0��-1��
	etank->_look.x=0.0f,etank->_look.y=0.0f,etank->_look .z=-1.0f;
	//��2-5��֮�䣬�������̹�˵�����ʱ��
	etank->_walktime=d3d::GetRandomFloat (3.0f,6.0f);
	//״̬����ʱ��Ϊ0	
	etank->_keeptime =0;
	//�ӵ�����ʱ����Ϊ1-3��֮�����
	etank->_launchtime=d3d::GetRandomFloat (1.0f,3.0f);
	//����һ�η����ʱ��������Ϊ��
	etank->_unlaunchtime =0;
	//һ��ʼ�������ӵ�
	etank->_launch =false;
}
void EnemyTank::addTank()
{
	ENEMYTANK enemytank;
	resetTank(&enemytank);//������̹�˵���Ϣ
	_allenetank.push_back (enemytank);//�������м����µ�̹��
}



void EnemyTank::upDate (float timeDelta)
{
	//���Զ���һ��ʱ�䣬��̹���������Ƕ�ʱ���ת�䡣
	std::list <ENEMYTANK>::iterator i;
	if(!_allenetank.empty())
	{
		for(i=_allenetank.begin();i!=_allenetank.end ();i++)
		{
			//���Ź۲���������
			i->_position.x+=200*timeDelta*(i->_look.x);
			i->_position.y+=200*timeDelta*(i->_look.y);
			i->_position.z+=200*timeDelta*(i->_look.z);
			i->_keeptime+=timeDelta;
			i->_unlaunchtime+=timeDelta;

			static int enble1=0;

			if(i->_keeptime>i->_walktime||i->_position.z<=-10000||i->_position .z>=10000)
			{
				if(i->_keeptime>i->_walktime)
					enble1=1;
				//��ת�������תҲ��Ҫ�������,�Ƕ��Ƿ�Ҫ���������ʱ�ȶ�Ϊ�����һ�ԣ�
				//��ת����	
				angle=d3d::GetRandomFloat (-0.25,0.25);//�������0.1-1��18�ȵ�PI��
				D3DXMatrixRotationY(&(i->_rotation),D3DX_PI*angle);
				i->_look .x=0.0f,i->_look .y =0.0f,i->_look .z=-1.0f;

				if(i->_position.z<=-10000||i->_position .z>=10000)
				{
					i->_position.x=d3d::GetRandomFloat(-10000,10000);
					i->_position.y =00.0f;
					i->_position.z=d3d::GetRandomFloat (4000,8000);
				}

				//�ı�̹�˵Ĺ۲������ķ���
				D3DXVec3TransformCoord(&(i->_look),&(i->_look),&(i->_rotation));
 				i->_keeptime =0;//״̬ʱ������
			}


			if(groundobject->BCollisiontest(i->_position)&&enble1==1)//���̹�˺͹������ײ
			{
				D3DXMatrixRotationY(&(i->_rotation),D3DX_PI*angle);
				enble1=0;	
				D3DXVec3TransformCoord(&(i->_look),&(i->_look),&(i->_rotation));
			}

		

			if(i->_unlaunchtime>i->_launchtime)
			{
				//�����ӵ����ʱ���㹻�������Է����ӵ���
				i->_launch =true;
			}
		}	
	}
	removedeadtank();
}



void EnemyTank::removedeadtank()
{
	std::list <ENEMYTANK>::iterator i;
	i=_allenetank.begin();
	while (i!=_allenetank.end ())
	{
		if(i->_isalive==false)
			i=_allenetank.erase (i);
		else 
			i++;
	}
}

void EnemyTank::Render ()
{
	std::list <ENEMYTANK>::iterator i;
	for(i=_allenetank.begin();i!=_allenetank.end ();i++)
	{
		if(i->_isalive)
		{
			//ȡ�õ�ǰ̹��λ��
			D3DXMATRIX move;
			D3DXMatrixTranslation(&move,i->_position.x,i->_position.y-100,i->_position.z);
			//����̹�˴�С
			D3DXMATRIX matscal;
			D3DXMatrixScaling(&matscal,0.015f,0.015f,0.015f);
			move=matscal*(i->_rotation)*move;
			Device->SetTransform (D3DTS_WORLD,&move);
			RenderXfile ();
		
			if(i->_launch)
			{
				//�����ӵ���
				//�������У��ӵ�ֻ�ᱻ��Ⱦһ��
				//Ӧ���ǻش����ʱ���̹�˵�λ�úͷ��򣬶���display��������Ⱦ��Щ�Ѿ������ȥ���ӵ�
				//��������Ӵ���addbullet���ǵ�
				addEbullet (i->_position ,i->_look );
				i->_launch=false ;
				i->_unlaunchtime=0;
			}
		}
	}
}


//========================================================================
//��ʼ���ӵ�������ָ���ļ���
//========================================================================

void EnemyTank::InitEbullet()
{
	/*������֮һ��ͨ��.X�ļ���������ģ��*/
	D3DXLoadMeshFromX(
		L"airplane.x",
		D3DXMESH_MANAGED,
		Device,
		&eppAdjacency,
		&eppMaterials,
		NULL,
		&epNumMaterials,//���������еĲ�����Ŀ
		&Bulletmesh//�����������Ĳ�������XFile�������ݵ�ID3DXMesh����
		);
	//��������XFile���Ƿ��ж��㷨����Ϣ��
	if(!Bulletmesh->GetFVF ()&D3DFVF_NORMAL)
	{
		ID3DXMesh *temp=0;
		Bulletmesh->CloneMeshFVF(
			D3DXMESH_MANAGED,
			Bulletmesh->GetFVF ()|D3DFVF_NORMAL,
			Device,
			&temp);
		Bulletmesh->Release ();
		Bulletmesh=temp;
	}

	//������֮����������ʺ�����
	D3DXMATERIAL *pMtrls=(D3DXMATERIAL *)eppMaterials->GetBufferPointer(); 

	eg_pMaterials= new D3DMATERIAL9[epNumMaterials];  
    eg_pTextures  = new LPDIRECT3DTEXTURE9[ epNumMaterials];                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         

	//��ȡ����Ͳ�������
	for(DWORD i=0; i<epNumMaterials; i++)  
      {  
           //��ȡ���ʣ�������һ�»��������ɫֵ  
           eg_pMaterials[i]= pMtrls[i].MatD3D;  
           eg_pMaterials[i].Ambient= eg_pMaterials[i].Diffuse;  
           //����һ���������  
           eg_pTextures[i]  = NULL;  
           D3DXCreateTextureFromFileA(Device,pMtrls[i].pTextureFilename, &eg_pTextures[i]);  
      }  
	//���������Ż�
		Bulletmesh->OptimizeInplace(
		D3DXMESHOPT_COMPACT |D3DXMESHOPT_VERTEXCACHE|D3DXMESHOPT_ATTRSORT ,
		(DWORD *)eppAdjacency->GetBufferPointer (),
		(DWORD *)eppAdjacency->GetBufferPointer (),
		NULL,
		NULL
		);
		eppAdjacency->Release ();
		eppMaterials->Release ();
}
//========================================================================
//��ʼ��һ�����ӵ��ĺ���,��ΪaddEbullet�����ĵ��ú���
//========================================================================
void EnemyTank::resetEbullet (Ebullet *ebullet,D3DXVECTOR3 position,D3DXVECTOR3 look)
{
	ebullet->_age=0;//�������ӵ�
	ebullet->_alive=true;//���
	ebullet->_lifetime=10;//����1����
	ebullet->_position =position; //���ﵽʱ����Ҫ���޸Ĳ��� ��ʹ���ӵ��ķ�����ȷ
	ebullet->_speed=look;

}

//========================================================================
//�����ӵ���������Ϊrender�ĵ��ú���
//========================================================================
void EnemyTank::addEbullet (D3DXVECTOR3 position,D3DXVECTOR3 look)
{
	Ebullet ebullet;//���ӵ�
	resetEbullet(&ebullet,position,look);//�����ӵ���λ�úͷ���
	_Ebullet.push_back (ebullet);//�����ӵ�����
}

//=========================================================================
//�����ӵ�����
//=========================================================================
void EnemyTank::UpDateBullet(float timeDelta)
{
	std::list <Ebullet>::iterator i;
	for(i=_Ebullet.begin();i!=_Ebullet.end ();i++)
	{
		//�����ӵ���λ��
		i->_position .x +=1000*timeDelta*(i->_speed .x);
		i->_position .y +=1000*timeDelta*(i->_speed .y);
		i->_position .z +=1000*timeDelta*(i->_speed .z);
		i->_age +=timeDelta;
		if(i->_age >i->_lifetime )
			i->_alive=false;
	}
	removedead ();//�����Ƴ������ӵ��ĺ���
}


void EnemyTank::removedead ()
{
	std::list <Ebullet>::iterator i;
	i=_Ebullet.begin();
	while (i!=_Ebullet.end ())
	{
		if(i->_alive==false)
			i=_Ebullet.erase (i);
		else 
			i++;
	}
}


void EnemyTank::RenderBullet()
{
	std::list <Ebullet>::iterator j;
	if(!_Ebullet.empty ())//�ж��Ƿ����ӵ����еĻ�������Ⱦ
	{
		for(j=_Ebullet.begin();j!=_Ebullet.end ();j++)
		{
			if(j->_alive=true)
			{
				D3DXMATRIX MaxLarght;
				D3DXMatrixScaling (&MaxLarght,10.0f,10.0f,10.0f);
					D3DXMATRIX Move;
					D3DXMatrixTranslation(&Move,j->_position.x,j->_position.y -30,j->_position.z);//ָ���ӵ��ĳ�ʼλ�� 
					Move=MaxLarght*Move;
					Device->SetTransform (D3DTS_WORLD,&Move);
				//RenderXfile
				for(DWORD i = 0; i < epNumMaterials; i++)  
				{  
					Device->SetMaterial(&eg_pMaterials[i]);  
					Device->SetTexture(0,eg_pTextures[i]);  
					Bulletmesh->DrawSubset(i);  
				 } 
			}
		}
	}
}
//==============================================================================================
//��ײ���Ժ���
//==============================================================================================

bool  EnemyTank::Collisiontest(D3DXVECTOR3 _position)
{
	double distance;
	std::list<ENEMYTANK>::iterator i;
	for(i=_allenetank.begin();i!=_allenetank.end ();i++)
	{
		distance=sqrt(pow((i->_position.x-_position.x),2)+pow((i->_position.y-_position.y),2)+pow((i->_position.z-_position.z),2));
		if(distance<deadline)
			{
				i->_isalive =false;
				return true;
			}
	}
	return false;
}




EnemyTank::~EnemyTank()
{

}
