#include"Props.h"
#include<math.h>
#include<stdlib.h>
#include"CameraClass.h"
extern CamerClass Camera;

//�����������Ķ�������ʽ
const DWORD Props::vertex::FVF =D3DFVF_XYZ|D3DFVF_TEX1;

//*****************��ʼ�����ߺ���*********************
bool Props::initProps()
{
	//��������
	D3DXCreateFont (Device,25,12,1000,0,true,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,0,L"����",&PFont);

	//��ʼ����������һ
	prop1=new BillBoard(Device);
	prop1->InitBoard(L"RedApple_archigraphs.png");
//
	//��ʼ���������߶�
	prop2=new BillBoard(Device);
	prop2->InitBoard (L"RedDaisy_archigraphs.png");

	//��ʼ������������
	prop3=new BillBoard(Device);
	prop3->InitBoard (L"WhiteDaisy_archigraphs.png");

	//��ʼ������������
	prop4=new BillBoard(Device);
	prop4->InitBoard (L"cock.png");

	//�������������ͼ�ļ�������
	wchar_t Nameprop5tex[8][13]=
	    {
			{ L"prop51.bmp" } ,{ L"prop52.bmp" },{ L"prop53.bmp" },{ L"prop54.bmp "},
			{ L"prop55.bmp" } ,{ L"prop56.bmp" },{ L"prop57.bmp" },{ L"prop58.bmp" },
	    };

	//��������������ͼ�ļ�������
	wchar_t Nameprop6tex[8][13]=
		{
			{ L"prop71.bmp" } ,{ L"prop72.bmp" },{ L"prop73.bmp" },{ L"prop74.bmp "},
			{ L"prop75.bmp" } ,{ L"prop76.bmp" },{ L"prop77.bmp" },{ L"prop78.bmp" },
		};

	wchar_t Nameprop7tex[8][13]=
	    {
			{ L"prop61.bmp" } ,{ L"prop62.bmp" },{ L"prop63.bmp" },{ L"prop64.bmp "},
			{ L"prop65.bmp" } ,{ L"prop66.bmp" },{ L"prop67.bmp" },{ L"prop68.bmp" },
		};

	//�������5/6�Ķ�̬������ͼ�ļ�
	for(int i=0 ; i < 8 ; i++)
	{
		D3DXCreateTextureFromFile( Device, Nameprop5tex[i], &prop5tex[i]);
		D3DXCreateTextureFromFile( Device ,Nameprop6tex[i],&prop6tex[i]);
		D3DXCreateTextureFromFile( Device,Nameprop7tex[i],&prop7tex[i]);//����7��������ͼ
	}
	
	
	//��̬������ߵĶ��㻺�洴��
	Device->CreateVertexBuffer (
		4*sizeof(vertex),
		0,
		Props::vertex::FVF,
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

	for(int i=1;i<=7;i++)  //����������м������ 
	{
		PROPS prop;
		prop._isalive=true;
		prop._id=i; 
		if(i==1)
			prop._timeDelta=2;//������ĵ���û2�����һ��
		if(i==2)
			prop._timeDelta=d3d::GetRandomFloat (10,20); //���ٵ���ÿ10��20�����һ��
		if(i==3)
			prop._timeDelta=d3d::GetRandomFloat (10,20); //���ٵ���ÿ10-20�����һ��
		if(i==4)
			prop._timeDelta=d3d::GetRandomFloat (90,100); //���ٽڵ����ÿ25-30�����һ��
		if(i==5)
			prop._timeDelta=d3d::GetRandomFloat	(50,60);
		if(i==6)
			prop._timeDelta=d3d::GetRandomFloat (100,120);
		if(i==7)
			prop._timeDelta=d3d::GetRandomFloat (12,15);//��׽����ÿ30�����һ��

		prop._timeHappen=0;
		prop._position.x =d3d::GetRandomFloat (-400,400);
		prop._position.y =d3d::GetRandomFloat (50,400);
		prop._position.z =d3d::GetRandomFloat (0,400);
		_allprops.push_back (prop); 
	}

	prop5TimeLeft=30;  //����5��ʱ
	prop5TimeExit=0;   //����5���ֵ�ʱ��	

	prop7TimeLeft = 10;
	prop7TimeExit = 0;

	isaddscore=true;

	propscore  =0; //���ߵ÷�Ϊ0��

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
			i->_position.z=d3d::GetRandomFloat (-200,400);
			i->_isalive=true;
			if(i->_id==5)
				isaddscore=false;
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
			D3DXMATRIX position,scaling,V;
			D3DXMatrixScaling(&scaling,0.5f,0.3f,0.5f);
			D3DXMatrixTranslation(&position,i->_position.x ,i->_position.y ,i->_position.z);
			position=scaling*position;
			Camera.SetViewMatrix(&V);
			if(i->_id==1)
			{
				prop1->RanderBorad ( &V,position);
			}
			if(i->_id==2)
			{
				prop2->RanderBorad (&V,position);
			}
			if(i->_id==3)
			{
				prop3->RanderBorad(&V,position);
			}
			if(i->_id==4)
			{
				D3DXMatrixScaling(&scaling,1.0f,1.0f,1.0f);
				position=scaling*position;
				prop4->RanderBorad (&V,position);
			}
			switch (i->_id)
			{
				case 5:
					{
						prop5TimeExit +=0.1;
						if(prop5TimeLeft-(int)prop5TimeExit >0)
						{
							//ָ��ʱ����ֵ�λ��
							RECT rect;
							rect.left=400,rect.top=50,rect.right=800,rect.bottom=600;
							//��Ⱦʱ��
							RenderTime(prop5TimeLeft-(int) prop5TimeExit,rect );
							//��Ⱦ��̬����
							RenderDynaminc(prop5tex,position,V);
						}
						else //�����ʾʱ�䵽�� û�Ե��Ǹ�����
						{
							isaddscore=false;	//��ʾû�гԵ��Ǹ�����
							i->_isalive=false;	//������ʧ
							i->_timeHappen=0;	//���߳���ʱ������
							prop5TimeExit=0;	//����5ʱ���������
						}
						break;
					}
				case 6:
					{
						RenderDynaminc(prop6tex,position,V);
						break;
					}
				case 7:
					{
						if(prop7TimeLeft-prop7TimeExit>0)
						{
							RenderDynaminc (prop7tex,position,V);
							prop7TimeExit += 0.1;
						}
						else 
						{
							prop7TimeExit =0;
							i->_isalive=false;
							i->_timeHappen=0;
						}
						break;
					}
			}

		}
	}
}


//******************************��Ⱦ��̬����ĺ���***********************************************************************************************
void Props::RenderDynaminc (IDirect3DTexture9 * proptex[8],D3DXMATRIX position,D3DXMATRIX V)
{
	D3DXMATRIX scaling;
	Device->SetRenderState (D3DRS_ALPHABLENDENABLE,true);//����alpha�ں�
	//�����ں�ϵ��
	Device->SetRenderState (D3DRS_SRCBLEND,D3DBLEND_ONE);
	Device->SetRenderState (D3DRS_DESTBLEND,D3DBLEND_ONE);
	D3DXMATRIX matBillboard;  
	D3DXMatrixIdentity(&matBillboard);  
	matBillboard._11 =	V._11;  
	matBillboard._13 =	V._13;  
	matBillboard._31 =	V._31;  
	matBillboard._33 =	V._33;
	D3DXMatrixInverse(&matBillboard, NULL, &matBillboard);  
	/*D3DXMatrixScaling(&scaling,10.0f,10.0f,10.0f);*/
	position =  /*scaling*/matBillboard*position  ;  
	Device->SetTransform(D3DTS_WORLD, &position ); 
	//��ʼ����
	static float time =0;
	static float timechance=0.3;
	static int j=0;
	time+=0.1;
	if(time >timechance)
	{
		time = 0;
		j ++;
	}
	Device->SetTexture(0,proptex[j]);
	Device->SetStreamSource(0, Vb , 0, sizeof(vertex) );  
	Device->SetFVF(Props::vertex::FVF);  
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2 );  
	Device->SetRenderState (D3DRS_ALPHABLENDENABLE,false);
	if(j==7)
	{
		j=0;
		//break;
	}
}


//**********��ײ����******������ߵ���ײ*********************
int Props::Collision (D3DXVECTOR3 position)
{
	std ::list <PROPS>::iterator  i;
	double distance=50.0f;
	for(i=_allprops.begin ();i!=_allprops.end ();i++)
	{
		if(sqrt(    pow(i->_position.x-position.x,2)   +  pow(i->_position.y-position.y,2)
			+pow(i->_position.z-position.z,2)  )  <distance&&i->_isalive==true)
		{
			propscore ++;//���ߵ÷ּ�1

			i->_isalive=false;
			i->_timeHappen=0;
			if(i->_id==1)
				return ADDBODY;		//���ӽڵ�
			if(i->_id==2)
				return SPEEDUP;		//����
			if(i->_id==3)
				return SPEEDDOWN;	//����
			if(i->_id==4)
				return CUTDOWN;		//�ж����峤��
			if(i->_id==5)			//���ӷ���
			{
				prop5TimeExit=0;
				return ADDSCORE;
			}
			if(i->_id==6)
			{
				return EXPAND;
			}
			if(i->_id==7)
			{
				return DEAD;
				//�ߵ�״̬��Ϊ����
			}
		}
		if(isaddscore==false)
		{
			isaddscore=true;
			return NOADDSCORE;
		}

	}

	return 0;
}

//---------------------------------------------------------------------------
//���ã��ṩ����5����ʱ����Ⱦ����
//���ͣ�Props����ڲ����ú���
//��ע����RenderProps()��������
//ʱ�䣺2014.8.26.
//---------------------------------------------------------------------------
void Props::RenderTime(int time,RECT rect )
{
	char b[20]={"Time left :"};
	char c[5];
	wchar_t * a;
	itoa(time,c,10);    //��intת����Ϊchar*
	for(int i=0,j=11;;i++)
	{
		b[j++]=c[i];
		if(c[i]=='\0')
			{
				b[j]='\0';
				break;
			}	
	}
	//��char* �͵�bת��Ϊwchar_t*��a
	DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, b, -1, NULL, 0);
	a = new wchar_t[dwNum]; 
	MultiByteToWideChar (CP_ACP, 0, b,(strlen(b) + 1), a, dwNum); //��char�͵�bת��Ϊwchar_t ��

		//����һ������
		//��������
	PFont->DrawText(NULL,a,-1,&rect,DT_LEFT|DT_NOCLIP|DT_SINGLELINE,D3DCOLOR_XRGB(255,255,255));

	delete []a ;
}