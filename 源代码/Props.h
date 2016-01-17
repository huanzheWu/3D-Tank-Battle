#include"d3dUtility.h"
#include"XFileMesh.h"
#include<list>

//道具功能宏定义
#define SPEEDUP 2
#define ADDBODY 1

//道具结构体
struct PROPS
{
	int _id;//标记道具
	D3DXVECTOR3 _position;//位置
	float _timeDelta;//周期
	float _timeHappen;//消失的时间
	bool _isalive;//状态
};

class Props 
{
	public:
		Props(PDIRECT3DDEVICE9 _Device){Device=_Device;};//构造函数
		~Props();//析构函数

	private:
		PDIRECT3DDEVICE9 Device;//D3D设备接口指针
		IDirect3DTexture9 * propstex[10]; //材质指针数组
		std::list <PROPS> _allprops;//道具链表
		XFileMesh *Prop1;//第一种道具 （增加长度）
	public:
		bool initProps();//初始化道具函数
		void upDateProps(float timeDelta);//更新道具函数
		void RenderProps();//绘制道具函数
		int Collision(D3DXVECTOR3 position);//碰撞函数
};