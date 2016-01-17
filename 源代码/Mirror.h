//======================================================================================
//                                      镜像类说明
//这个类含有3个函数。
//InitMirror(int length)函数包含了对镜子顶点结构描述、顶点索引创建、纹理贴图的载入等等
//D3DXMATRIX PreSet(D3DXMATRIX moddleplace)函数中首先画了镜子，接着是系列的模版缓存操作。
//需要传入一个矩阵确定模型位置，返回一个矩阵给调用函数用于主调中确定镜像位置
//void LastSet();函数是恢复一些缓存设置
//=======================================================================================
//使用该类时需要传入一个Device
//初始化镜子需要传入镜子大小参数
//======================================================================================
//缺点：没办法指定镜子位置
//======================================================================================
//2014.8.06
//======================================================================================

#pragma once
#include"d3dUtility.h"
class Mirror{
public:
	Mirror(LPDIRECT3DDEVICE9 device)
	{
		Device=device;
		MirrorLength=1000;
	}
	~Mirror();
private:
		LPDIRECT3DDEVICE9 Device;
		int MirrorLength;
		IDirect3DVertexBuffer9 *Vb;
		IDirect3DTexture9 *Mirrortex;
		D3DXMATRIX ModdlePlace;
		//定义顶点格式

	struct MirrorVertex
	{
		MirrorVertex(float x,float y,float z,float nx,float ny,float nz,float u,float v)
		{
			_x=x,_y=y,_z=z;
			_nx=nx,_ny=ny,_nz=nz;
			_u=u,_v=v;
		}
		float _x,_y,_z;
		float _nx, _ny, _nz;
		float _u,_v;
		static const DWORD FVF_Mirror;
	};


public:
		void InitMirror(int length);
		D3DXMATRIX PreSet(D3DXMATRIX moddleplace);
		void LastSet();
};