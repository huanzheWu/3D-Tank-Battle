#include"Ground.h"

//********初始化函数***************************
bool Ground::initObject ()
{
	//创建围墙的纹理
	D3DXCreateTextureFromFile (Device,L"maoshe1.bmp",&Tex[0]);
	Wall=new XFileMesh(Device);
	//初始化围墙模型
	Wall->InitXfile (L"shu.x");
	//设置墙壁的缩放矩阵
	D3DXMatrixScaling(&WallSca,1000.0f,1000.0f,1000.0f);

	return true;
}

//***********创建围墙函数***********************
bool Ground ::createWall (D3DXVECTOR3 centre)
{
	float i;
	D3DXVECTOR3 position;
	for (i=centre.x -500;i<=centre.x+500;i+=100)  //前
	{
		position.y =0.0f,position.z =500.0f,position.x=i;
		_allWalls.push_back (position);
	}
	for (i=centre.z-500;i<=centre.z +500;i+=100)//左
	{
		position.x=-500.0f,position.y=0.0f,position.z =i;
		_allWalls.push_back (position);
	}
	for(i=centre.z -500;i<=centre.z+500;i+=100 )//右
	{
		position.x =500.0f,position.y =0.0f,position.z =i;
		_allWalls.push_back (position);
	}
	for (i=centre.x -500;i<=centre.x+500;i+=100)//后
	{
		position.y =0.0f,position.z =-500.0f,position.x=i;
		_allWalls.push_back (position);
	}

	return true ;
}
//***********渲染地面物体函数*******************
void Ground::renderGround ()
{
	std::list <D3DXVECTOR3 >::iterator i;
	for(i=_allWalls.begin ();i!=_allWalls.end ();i++)
	{
		//设置世界矩阵，把围墙放置在适当的位置
		
		D3DXMatrixTranslation(&WallPos,i->x ,i->y ,i->z);
		WallPos=WallSca*WallPos;
		Device->SetTransform (D3DTS_WORLD,&WallPos);
		//设置纹理
		Device->SetTexture (0,Tex[0]);
		//绘制围墙x文件
		Wall->RenderXfile ();
	}
}
