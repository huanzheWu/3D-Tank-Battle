#include"GroundObject.h"
#include"Explode.h"
const DWORD GroundObject::vertex::FVF =D3DFVF_XYZ|D3DFVF_TEX1;//�����������ʽ

extern Explode *explode;//�ѱ�ըʵ����������չ������
//���캯������ȡD3D�豸ָ��
GroundObject::GroundObject(LPDIRECT3DDEVICE9 device)
{
	Device=device;
}

//��ʼ�������
void GroundObject::InitObject(wchar_t * filename,wchar_t *wallname,int numofboard,int numwall)
{
	//�������Ҫ����壬��ôҲ����ȥ�������桢��������ӿ�
	if(numofboard!=0)
	{
		Device->CreateVertexBuffer (
			4*sizeof(vertex),
			0,
			GroundObject::vertex::FVF,
			D3DPOOL_MANAGED,
			&Vb,
			0
			);
		vertex *BoardVertex=NULL;
		Vb->Lock (0,0,(void **)&BoardVertex,0);
		BoardVertex[0]=vertex(-100.0f,300.0f,0.0f,0.0f,0.0f);//a
		BoardVertex[1]=vertex(100.0f,300.0f,0.0f,1.0f,0.0f);//b
		BoardVertex[2]=vertex(-100.0f,0.0f,0.0f,0.0f,1.0f);//c
		BoardVertex[3]=vertex(100.0f,0.0f,0.0f,1.0f,1.0f);//d
		Vb->Unlock ();
		D3DXCreateTextureFromFile(Device,filename, &Tex);
		Device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );  
		Device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );  
	
		//�����������������й�����λ����Ϣ
		for(int i=0;i<numofboard ;i++)
		{
			BILLBOARD board;
			board._position.x =d3d::GetRandomFloat(-5000,5000);
			board._position.y=-100;
			board._position.z=d3d::GetRandomFloat(-5000,5000);
			_allBillBoard.push_back (board);//�����������һ������棻
		}
	}
	//�������ǽ�ڵ�λ��
	for(int i=0;i<numwall;i++)
	{
		WALL wall;
		wall._position.x=d3d::GetRandomFloat(-5000,5000);
		wall._position.z=d3d::GetRandomFloat(-5000,5000);
		wall._position.y=0;
		_allWall.push_back (wall);
	}
	InitXfile(wallname);//��ʼ��ǽ�ڵ�x�ļ�
}

//��Ⱦ�����
void GroundObject::RanderObject(D3DXMATRIX *View)
{
	
	//����Χǽ
	std::list <WALL>::iterator j;
	for(j=_allWall.begin();j!=_allWall.end ();j++)
	{
		D3DXMatrixTranslation(&MatWall,j->_position.x,j->_position.y-160,j->_position.z);
		D3DXMATRIX rotation,scaling;
		D3DXMatrixRotationY(&rotation,D3DX_PI/2.0f);//��ת90��
		D3DXMatrixScaling(&scaling,3.0f,3.0f,3.0f);
		MatWall=scaling*rotation*MatWall;
		Device->SetTransform (D3DTS_WORLD,&MatWall);
		RenderXfile();
	}

	//���û�й���壬�򲻱ؽ����������
	if(!_allWall.empty ())
	{
		Device->SetRenderState (D3DRS_ALPHABLENDENABLE,true);//����alpha�ں�
		//�����ں�ϵ��
		Device->SetRenderState (D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
		Device->SetRenderState (D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
		D3DXMATRIX matBillboard;  
		D3DXMatrixIdentity(&matBillboard);  
		matBillboard._11 =(*View)._11;  
		matBillboard._13 =(*View)._13;  
		matBillboard._31 =(*View)._31;  
		matBillboard._33 =(*View)._33;
		D3DXMatrixInverse(&matBillboard, NULL, &matBillboard);

		//������ľ
		std::list <BILLBOARD>::iterator i;
		for(i=_allBillBoard.begin ();i!=_allBillBoard.end ();i++)
		{
			//MatNPC=matNPC; 
			D3DXMatrixTranslation(&MatNPC ,i->_position.x,i->_position.y-20,i->_position.z);
			MatNPC =  matBillboard*MatNPC  ;  
			Device->SetTransform(D3DTS_WORLD, &MatNPC ); 
			//��ʼ����
			Device->SetTexture(0, Tex);  
			Device->SetStreamSource(0, Vb , 0, sizeof(vertex) );  
			Device->SetFVF(GroundObject::vertex::FVF);  
			Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2 );  
		}
	}
	Device->SetRenderState (D3DRS_ALPHABLENDENABLE,false);
}

//===================================================================================
//̹�˺͵������壨��ľ��ǽ�ڣ���ײ���
//��Ϊ����������ľ����ǽ����ײ����ײ���Ϊ�涼����ת̹�ˣ�����������ײ����д��һ��
//���ӵ���ǽ�ڵ���ײ����ǣ������ײ���ӵ�״̬Ϊ����������Ҫ��дһ����ײ����
//====================================================================================
bool GroundObject::BCollisiontest(D3DXVECTOR3 _position)
{
	double distance;
	if(!_allBillBoard.empty())
	{
		//̹������ľ����ײ���
		std::list<BILLBOARD>::iterator i;
		for(i=_allBillBoard.begin();i!=_allBillBoard.end ();i++)
		{
			distance=sqrt(pow((i->_position.x-_position.x),2)+pow((i->_position.y-_position.y),2)+pow((i->_position.z-_position.z),2));
			if(distance<5)
				{
					//��ʼ��һ����ըλ��
					EXPLODEB explode;
					explode._position=i->_position;
					explode.sign=0;
					explode._position.y=i->_position.y -50;
					_allbound.push_back (explode);//��ըλ���б��м���һ��Ԫ��
					return true;
				}
		}
	}

	//̹����ǽ�ڵ���ײ���
	double range;
	if(!_allBillBoard.empty())//���������ǿյģ���ô֤����ʹ�õ��Ƿ���x�ļ�����ʱ������ײ��ΧӦ�ô�һ��
		range=50.0f;
	else 
		range=20.0f;
	std::list <WALL>::iterator j;
	for(j=_allWall.begin ();j!=_allWall.end ();j++)
	{
		distance=sqrt(pow((j->_position.x-_position.x),2)+pow((j->_position.y-_position.y),2)+pow((j->_position.z-_position.z),2));
		if(distance<range)
			{
					EXPLODEB explode;
					explode._position=j->_position;
					explode._position.y=j->_position.y -50;
					explode.sign=0;
					_allbound.push_back (explode);//��ըλ���б��м���һ��Ԫ��
				return true;
			}	
	}
	return false;
}

//=========================================================================================================================
//�ӵ�������������ײ����
//
//=========================================================================================================================
bool GroundObject::BTCollisiontest(D3DXVECTOR3 _position)
{
	double distance;
	std::list <WALL>::iterator i;
	for(i=_allWall.begin();i!=_allWall.end ();i++)
	{
		distance=sqrt(pow((i->_position.x-_position.x),2)+pow((i->_position.y-_position.y),2)+pow((i->_position.z-_position.z),2));
		if (distance<300)
			return true;
	}
	return false;
}


//==================================================================================================
//��Ⱦ�ӵ������������ײʱ�ı�ըЧ���ĺ���
//==================================================================================================
void GroundObject::renderbound (D3DXMATRIX *V,float timeDelta)
{
	std::list <EXPLODEB>::iterator i;
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

//��������
GroundObject::~GroundObject()
{
	if(!_allBillBoard.empty ())
		{
			Vb->Release ();
			Tex->Release ();
		}
}




