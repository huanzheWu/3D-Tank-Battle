#include"Snake.h"
#include"Props.h"
extern Props *props;
//=======================================================================
//初始化函数，在这个函数里，创建了蛇头的模型，指定游戏开始时蛇头的位置和
//行走方向，并将蛇头加入节点链表中
//=======================================================================
bool Snake::initSnake()
{
	Psnake=new XFileMesh(Device);
	Psnake->InitXfile (L"CAICHONG.x");//初始化蛇头模型

	//创建纹理
	D3DXCreateTextureFromFile(Device,L"1.jpg",&tex);
	D3DXVECTOR3 position;
	position.x =0.0f,position.y=0.0f,position.z =0.0f;//先把蛇头的位置放在原点

	D3DXVECTOR3 direction;
	direction.x =0.0f,direction.y=0.0f,direction.z=1.0f;//蛇头向z轴正方向行走
	
	SNAKE snake;//定义一个节点实例
	snake.direction=direction;
	snake.position=position;

	_allSnakeNode.push_back (snake);//将蛇头加入节点链表中

	return true;
}
//****************更新函数*********************
void Snake::upDateSanke (float timeDelta,DInputClass* DInput)
{
	DInput->GetInput ();
	std::list <SNAKE>::iterator node;
	std::list <TURNING>::iterator turning;

	//*********静态变量time，控制蛇头自动行走*********
	static float time ;
	time+=timeDelta;
	static int signofaddturning=0;

	//********对应按键改变蛇的行走方向*****************
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

	if(signofaddturning ==1)//如果有转向
	{
		if(_allTurning.empty ()) //如果链表是空的，则直接把转向点添加到链表中
				
			{
				if(_allSnakeNode.size ()>1)
				addTurning(node->position,node->direction);
			}	
		else  
			{
				turning=--_allTurning.end ();
				if(node->direction!=turning->direction&&_allSnakeNode.size ()>1) //如果链表不是空的，为了避免重复添加同一方向的转折点，用一个条件来
				{															//且只有蛇头的时候，不必添加转折点
					addTurning(node->position,node->direction);//向转向链表中添加一个元素
					signofaddturning=0;
				}
			}
	}
	
	//**********进行蛇头与道具的碰撞函数*****************
	if(ADDBODY==props->Collision(node->position))  //如果吃到了增加长度的道具
	{
		node=--_allSnakeNode.end ();//获取最后一颗身体
		addSnake (node->position,node->direction);
	}

	if(time>0.1)	//每秒前进一次
	{
		time =0; //先将时间置0；
		for(node=_allSnakeNode.begin();node!=_allSnakeNode.end ();node++)
		{
			node->position.x +=10*node->direction.x;
			node->position.y +=10*node->direction.y;
			node->position.z +=10*node->direction.z;
		}
	}	

	//************更新每个节点在转向点的转向
	node=_allSnakeNode.begin ();
	std::list <SNAKE>::iterator lastNode;
	lastNode=--_allSnakeNode.end ();//lastNode保存链表中最后一个元素

	while(node!=_allSnakeNode.end ())
	{
		turning=_allTurning.begin ();
		while (turning!=_allTurning.end ())
		{
			if(node->position==turning->position)
				node->direction=turning->direction;
			if (turning->position==lastNode->position)//如果蛇的最后一个节点也走过了转折点，那么该转折点的信息应该删除
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


//*****************************渲染函数******************************************
void Snake::RenderSnake ()
{
	D3DXMATRIX rotation;
	D3DXMatrixRotationY(&rotation,0);
	std::list <SNAKE>::iterator node;
	node=_allSnakeNode.begin ();


	//设置世界变换矩阵，使蛇头的朝向正确
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
	//绘制蛇身
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

//********内部调用函数*********增加一节身体****************
void Snake::addSnake (D3DXVECTOR3 position,D3DXVECTOR3 direction)
{
	SNAKE snake;
	snake.direction=direction;
	snake.position.x =position.x-50*direction.x ,snake.position.y =position.y -50*direction.y ,snake.position.z=position.z -50*direction.z;
	_allSnakeNode.push_back (snake);
}

//*******内部调用函数*********增加一个转折点***************
void Snake::addTurning(D3DXVECTOR3 position,D3DXVECTOR3 direction)
{
	TURNING turn ;
	turn.position=position;
	turn.direction=direction;
	_allTurning.push_back (turn);//增加一个链表元素
}

//**************获取蛇头位置和方向************************
void Snake::getHeadPos (D3DXVECTOR3 * position)
{
	std::list<SNAKE>::iterator i;
	i=_allSnakeNode.begin ();
	*position=i->position;
	(*position).y +=30;
}

Snake::~Snake()//析构函数
{
	delete Psnake;
}