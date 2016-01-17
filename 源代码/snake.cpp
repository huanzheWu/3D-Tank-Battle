#include"Snake.h"
#include"Props.h"
extern Props *props;
//=======================================================================
//��ʼ��������������������������ͷ��ģ�ͣ�ָ����Ϸ��ʼʱ��ͷ��λ�ú�
//���߷��򣬲�����ͷ����ڵ�������
//=======================================================================
bool Snake::initSnake()
{
	Psnake=new XFileMesh(Device);
	Psnake->InitXfile (L"CAICHONG.x");//��ʼ����ͷģ��

	//��������
	D3DXCreateTextureFromFile(Device,L"1.jpg",&tex);
	D3DXVECTOR3 position;
	position.x =0.0f,position.y=0.0f,position.z =0.0f;//�Ȱ���ͷ��λ�÷���ԭ��

	D3DXVECTOR3 direction;
	direction.x =0.0f,direction.y=0.0f,direction.z=1.0f;//��ͷ��z������������
	
	SNAKE snake;//����һ���ڵ�ʵ��
	snake.direction=direction;
	snake.position=position;

	_allSnakeNode.push_back (snake);//����ͷ����ڵ�������

	return true;
}
//****************���º���*********************
void Snake::upDateSanke (float timeDelta,DInputClass* DInput)
{
	DInput->GetInput ();
	std::list <SNAKE>::iterator node;
	std::list <TURNING>::iterator turning;

	//*********��̬����time��������ͷ�Զ�����*********
	static float time ;
	time+=timeDelta;
	static int signofaddturning=0;

	//********��Ӧ�����ı��ߵ����߷���*****************
	node=_allSnakeNode.begin ();
	if(DInput->IsKeyDown(DIK_C))
		{node->direction.x=-1.0f;node->direction.y =0.0f;node->direction.z=0.0f;signofaddturning=1;}
	if(DInput->IsKeyDown(DIK_B))
		{node->direction.x=1.0f;node->direction.y =0.0f;node->direction.z=0.0f;signofaddturning=1;}
	if(DInput->IsKeyDown(DIK_V))
		{node->direction.x=0.0f;node->direction.y =-1.0f;node->direction.z=0.0f;signofaddturning=1;}
	if(DInput->IsKeyDown(DIK_F))
		{node->direction.x=0.0f;node->direction.y =1.0f;node->direction.z=0.0f;signofaddturning=1;}
	if(DInput->IsKeyDown (DIK_N))
		{node->direction.x =0.0f;node->direction .y=0.0f;node->direction.z =1.0f;signofaddturning=1;}
	if(DInput->IsKeyDown (DIK_M))
		{node->direction .x=0.0f;node->direction.y =0.0f;node->direction.z=-1.0f;signofaddturning=1;}

	if(signofaddturning ==1)//�����ת��
	{
		if(_allTurning.empty ()) //��������ǿյģ���ֱ�Ӱ�ת�����ӵ�������
				
			{
				if(_allSnakeNode.size ()>1)
				addTurning(node->position,node->direction);
			}	
		else  
			{
				turning=--_allTurning.end ();
				if(node->direction!=turning->direction&&_allSnakeNode.size ()>1) //��������ǿյģ�Ϊ�˱����ظ����ͬһ�����ת�۵㣬��һ��������
				{															//��ֻ����ͷ��ʱ�򣬲������ת�۵�
					addTurning(node->position,node->direction);//��ת�����������һ��Ԫ��
					signofaddturning=0;
				}
			}
	}
	
	//**********������ͷ����ߵ���ײ����*****************
	if(ADDBODY==props->Collision(node->position))  //����Ե������ӳ��ȵĵ���
	{
		node=--_allSnakeNode.end ();//��ȡ���һ������
		addSnake (node->position,node->direction);
	}

	if(time>0.1)	//ÿ��ǰ��һ��
	{
		time =0; //�Ƚ�ʱ����0��
		for(node=_allSnakeNode.begin();node!=_allSnakeNode.end ();node++)
		{
			node->position.x +=10*node->direction.x;
			node->position.y +=10*node->direction.y;
			node->position.z +=10*node->direction.z;
		}
	}	

	//************����ÿ���ڵ���ת����ת��
	node=_allSnakeNode.begin ();
	std::list <SNAKE>::iterator lastNode;
	lastNode=--_allSnakeNode.end ();//lastNode�������������һ��Ԫ��

	while(node!=_allSnakeNode.end ())
	{
		turning=_allTurning.begin ();
		while (turning!=_allTurning.end ())
		{
			if(node->position==turning->position)
				node->direction=turning->direction;
			if (turning->position==lastNode->position)//����ߵ����һ���ڵ�Ҳ�߹���ת�۵㣬��ô��ת�۵����ϢӦ��ɾ��
				{
					lastNode->direction=turning->direction;
					turning=_allTurning.erase (turning);	
				}	
			else
				turning++;
		}
		node++;
	}


}


//*****************************��Ⱦ����******************************************
void Snake::RenderSnake ()
{
	D3DXMATRIX rotation;
	D3DXMatrixRotationY(&rotation,0);
	std::list <SNAKE>::iterator node;
	node=_allSnakeNode.begin ();


	//��������任����ʹ��ͷ�ĳ�����ȷ
	if(node->direction.x ==-1)
		D3DXMatrixRotationY(&rotation,D3DX_PI/2.0f);
	else if(node->direction.x ==1)
		D3DXMatrixRotationY(&rotation,-D3DX_PI/2.0f);
	else if(node->direction.y ==1)
		D3DXMatrixRotationX(&rotation,D3DX_PI/2.0f);
	else if(node->direction .y ==-1)
		D3DXMatrixRotationX(&rotation,-D3DX_PI/2.0f);
	else if (node->direction.z==1)
		D3DXMatrixRotationY(&rotation,-D3DX_PI);
	//��������
	for(node=_allSnakeNode.begin ();node!=_allSnakeNode.end ();node++)
	{
		D3DXMATRIX position;
		D3DXMatrixTranslation(&position,node->position.x,node->position.y,node->position.z);
		position=rotation*position;
		Device->SetTransform (D3DTS_WORLD,&position);
		Device->SetTexture (0,tex);
		Psnake->RenderXfile ();
	}
}

//********�ڲ����ú���*********����һ������****************
void Snake::addSnake (D3DXVECTOR3 position,D3DXVECTOR3 direction)
{
	SNAKE snake;
	snake.direction=direction;
	snake.position.x =position.x-50*direction.x ,snake.position.y =position.y -50*direction.y ,snake.position.z=position.z -50*direction.z;
	_allSnakeNode.push_back (snake);
}

//*******�ڲ����ú���*********����һ��ת�۵�***************
void Snake::addTurning(D3DXVECTOR3 position,D3DXVECTOR3 direction)
{
	TURNING turn ;
	turn.position=position;
	turn.direction=direction;
	_allTurning.push_back (turn);//����һ������Ԫ��
}

//**************��ȡ��ͷλ�úͷ���************************
void Snake::getHeadPos (D3DXVECTOR3 * position)
{
	std::list<SNAKE>::iterator i;
	i=_allSnakeNode.begin ();
	*position=i->position;
	(*position).y +=30;
}

Snake::~Snake()//��������
{
	delete Psnake;
}