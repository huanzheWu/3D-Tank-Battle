//===========================================
//头文件
//===========================================
#include "d3dUtility.h"
#include"Snow.h"
#include"Terrain.h"
#include"CameraClass.h"
#include"skyboxclass.h"
#include <tchar.h>
#include"GroundObject.h"
#include"Bullet.h"
#include"Mytank.h"
#include"DirectInputClass.h"
#include"EnemyTank.h"
#include"GameProps.h"
#include <ctime>
#include"Explode.h"
#include"D3DGUI.h"
using namespace d3d;

#define WINDOW_WIDTH	800						//为窗口宽度定义的宏，以方便在此处修改窗口宽度
#define WINDOW_HEIGHT	600						//为窗口高度定义的宏，以方便在此处修改窗口高度

//=========================================
//全局变量声明部分
//==========================================
LPDIRECT3DDEVICE9 Device = 0;//		D3D设备接口
D3DXMATRIX World;          //定义了一个矩阵
CamerClass Camera(CamerClass::land);
SkyBox* skybox=NULL;
Terrain *terrain=NULL;
GroundObject * groundobject=NULL;
GroundObject * groundobject1=NULL;
Snow * snow=NULL;
DInputClass *DInput=NULL;
Tank *mytank=NULL;
BULLET *bullet=NULL;
EnemyTank *enetank=NULL;
GameProps *props;
Explode *explode=NULL;
bool												g_LMBDown = false;      // GUI中的鼠标状态信息，鼠标左键是否按下的标识
int												g_MouseX = 0, g_MouseY = 0;      //存储鼠标坐标的两个变量
float timedelta;
//
 //创建四个GUI类对象，分别代表四个页面
D3DGUIClass		*g_MainGUI = NULL;//主窗口
D3DGUIClass		*g_StartGUI = NULL; //游戏开始 窗口
D3DGUIClass		*g_LoadGUI = NULL; //游戏载入窗口
D3DGUIClass		*g_OptionGUI = NULL; //游戏设置窗口
int						g_MainGUIFontID = -1;						//  GUI中字体对象的ID
int						g_StartGUIFontID = -1;						//  GUI中字体对象的ID
int						g_LoadGUIFontID = -1;						//  GUI中字体对象的ID
int						g_OptionGUIFontID = -1;					//  GUI中字体对象的ID
int						g_currentGUI = GUI_MAIN_SCREEN;    //一个当前的GUI标识
//==========================================
//外部函数声明
//==========================================
extern void KeyboardAndMouse( CamerClass *Camera,float ,float  );
//==========================================
//全局函数声明部分
//==========================================
bool Setup();
void Cleanup();
bool Display(float timeDelta);
//
//
//
void	GUICallback(int id, int state); //GUI的回调函数
//
//
//
//==========================================
// WinMain 程序入口函数
//==========================================
int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	 
	HWND hwnd = 0;//用于传进去InitD3D中获取窗口句柄，作为初始化DInput的一个参数
	if(!d3d::InitD3D(hinstance,800,600, true, D3DDEVTYPE_HAL, &Device,&hwnd))
	{
		
		::MessageBox(0, L"InitD3D() - FAILED", 0, 0);
		return 0;
	}
	//把DInput放在这里的原因：因为初始化DInput需要窗口句柄和实例窗口句柄，为了不给SET UP增加参数，就在这里初始化了。
	DInput=new DInputClass ();
	DInput->Init(hwnd,hinstance,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if(!Setup())
	{
		::MessageBox(0, L"Setup() - FAILED", 0, 0);
		return 0;
	}
	d3d::EnterMsgLoop( Display );
	Cleanup();
	Device->Release();
	return 0;
}
//
// WndProc
// 窗口消息处理函数
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	 
	switch( msg )
	{
		case WM_DESTROY:
			::PostQuitMessage(0);
			break;
		case WM_KEYDOWN:
			{if( wParam == VK_ESCAPE )
			::DestroyWindow(hwnd);
		
			break;
		}
	//case WM_LBUTTONDOWN:
			//点击鼠标时的操作
		//
		//
		case WM_LBUTTONDOWN:
			g_LMBDown = true;
			break;

		case WM_LBUTTONUP:
			g_LMBDown = false;
			break;

		case WM_MOUSEMOVE:
			g_MouseX = LOWORD (lParam);
			g_MouseY = HIWORD (lParam);
			break;
		//
		//
		//
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}
//
//设置函数
//
bool Setup()                                                                                                                                                                                                                                                                                                                                                             
{
	PlaySound(L"menu.wav", NULL, SND_FILENAME | SND_ASYNC|SND_LOOP);
	srand((unsigned int)time(0));//设置随机种子

	// 创建一些GUI系统
	g_MainGUI = new D3DGUIClass(Device, WINDOW_WIDTH, WINDOW_HEIGHT); //主菜单页面
	g_StartGUI = new D3DGUIClass(Device, WINDOW_WIDTH, WINDOW_HEIGHT); //开始新游戏页面
	g_LoadGUI = new D3DGUIClass(Device, WINDOW_WIDTH, WINDOW_HEIGHT); //载入游戏页面
	g_OptionGUI = new D3DGUIClass(Device, WINDOW_WIDTH, WINDOW_HEIGHT); //设置页面

	// 给四个页面分别添加背景图
	if(!g_MainGUI->AddBackground(L"maingui.jpg")) return false;  
	if(!g_StartGUI->AddBackground(L"startgui.jpg")) return false;
	if(!g_LoadGUI->AddBackground(L"loadgui.jpg")) return false;
	if(!g_OptionGUI->AddBackground(L"optiongui.jpg")) return false;


	// 分别给四个页面添加字体
	if(!g_MainGUI->CreateTextFont(L"微软雅黑", 28, &g_MainGUIFontID)) return false;
	if(!g_StartGUI->CreateTextFont(L"微软雅黑", 38, &g_StartGUIFontID)) return false;
	if(!g_LoadGUI->CreateTextFont(L"微软雅黑", 38, &g_LoadGUIFontID)) return false;
	if(!g_OptionGUI->CreateTextFont(L"微软雅黑", 38, &g_OptionGUIFontID)) return false;

	//---------------------------------【主菜单main页面相关的页面布局代码】---------------------------------
	// 添加静态文本到页面中
	if(!g_MainGUI->AddStaticText(STATIC_TEXT_ID, L"Make By Me.",
		350, 535, D3DCOLOR_XRGB(55,155,255), g_MainGUIFontID)) return false;

	// 添加4个按钮，分别是开始游戏，载入进度，选项和退出游戏，每个按钮对应3幅图
	if(!g_MainGUI->AddButton(BUTTON_START_ID, 350, 340, L"startUp.png",
		L"StartOver.png", L"startDown.png")) return false;
	
	if(!g_MainGUI->AddButton(BUTTON_LOAD_ID, 350, 385, L"loadUp.png",
		L"loadOver.png", L"loadDown.png")) return false;

	if(!g_MainGUI->AddButton(BUTTON_OPTION_ID,350, 430, L"optionsUp.png",
		L"optionsOver.png", L"optionsDown.png")) return false;

	if(!g_MainGUI->AddButton(BUTTON_QUIT_ID, 350, 475, L"quitUp.png",
		L"quitOver.png", L"quitDown.png")) return false;


	//------------------------【开始新游戏start页面相关的页面布局代码】------------------------
	// 添加按钮到页面中
	if(!g_StartGUI->AddButton(BUTTON_LEVEL_1_ID,550, 380,  L"level1Up.png", L"level1Over.png",
		L"level1Down.png")) return false;


	if(!g_StartGUI->AddButton(BUTTON_BACK_ID, 10, 550, L"backUp.png", L"backOver.png",
		L"backDown.png")) return false;

	//------------------------【载入游戏load页面相关的页面布局代码】------------------------
	//添加静态文本到页面中
	if(!g_LoadGUI->AddStaticText(STATIC_TEXT_ID, L"载入游戏成功！请返回后开始游戏！",
		150, 20, D3DCOLOR_XRGB(180,180,150), g_LoadGUIFontID)) return false;
	// 添加按钮到页面中
	if(!g_LoadGUI->AddButton(BUTTON_BACK_ID, 10, 500, L"backUp.png", L"backOver.png",
		L"backDown.png")) return false;


	//------------------------【游戏设置option页面相关的页面布局代码】------------------------
	// 添加按钮到页面中
	if(!g_OptionGUI->AddButton(BUTTON_BACK_ID,50, 450, L"backUp.png", L"backOver.png",
		L"backDown.png")) return false;
	//添加静态文本到页面中
	 if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"操作提示：",
	 		180, 60, D3DCOLOR_XRGB(0,0,0), g_OptionGUIFontID)) return false;
	 if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"前进: W",
	 		240, 90, D3DCOLOR_XRGB(0,0,0), g_OptionGUIFontID)) return false;
	 if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"后退：S",
	 		240, 120, D3DCOLOR_XRGB(0,0,0), g_OptionGUIFontID)) return false;
	 if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"左移：A",
	 		240, 150, D3DCOLOR_XRGB(0,0,0), g_OptionGUIFontID)) return false;
	 if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"右移：D：",
	 		240, 180, D3DCOLOR_XRGB(0,0,0), g_OptionGUIFontID)) return false;
	 if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"移动鼠标：旋转视觉",
	 		240, 210, D3DCOLOR_XRGB(240,240,240), g_OptionGUIFontID)) return false;
	 if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"鼠标左键：发射子弹",
	 		240, 240, D3DCOLOR_XRGB(240,240,240), g_OptionGUIFontID)) return false;
	 if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"道具介绍：",
	 		180, 270, D3DCOLOR_XRGB(240,240,240), g_OptionGUIFontID)) return false;
	 if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"炸弹：召唤轰炸机支援。",
	 		240, 300, D3DCOLOR_XRGB(240,240,240), g_OptionGUIFontID)) return false;
	 if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"血包：增加生命。",
	 		240, 330, D3DCOLOR_XRGB(240,240,240), g_OptionGUIFontID)) return false;
	 if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"鞋子：加速，加时间",
	 		240, 360, D3DCOLOR_XRGB(240,240,240), g_OptionGUIFontID)) return false;
	 if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"游戏规则：",
	 		180, 390, D3DCOLOR_XRGB(240,240,240), g_OptionGUIFontID)) return false;
	  if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"在规定时间与生命内杀掉30个敌人则算胜利。",
	 		200, 420, D3DCOLOR_XRGB(240,240,240), g_OptionGUIFontID)) return false;
	return true;
}

bool Init ()
{	
	Device->SetRenderState(D3DRS_LIGHTING,true);
	Device->SetRenderState(D3DRS_NORMALIZENORMALS,true);

	skybox= new SkyBox(Device);
	skybox->LoadSkyTextureFromFile (L"frontsnow1.jpg",L"backsnow1.jpg",L"leftsnow1.jpg",L"rightsnow1.jpg",L"topsnow1.jpg",L"groundsnow1.jpg");//加载天空盒贴图

	terrain=new Terrain(Device,"heighmap.raw", 200, 200, 60,0.1f);//创建一个地形类
	terrain->loadTexture(L"ground.jpg");

	groundobject=new GroundObject(Device);
	groundobject->InitObject(L"tree.dds",L"028.x",50,20);//产生50个公告版

	groundobject1=new GroundObject(Device);
	groundobject1->InitObject(L"tree.dds",L"WoodCutter.x",0,2);

	//====================================================================
	//坦克类试验
	//====================================================================
	mytank=new Tank(Device);
	mytank->InitXfile(L"M4A375W_.x");

	//确定雪花区域
	d3d::BoundingBox boundingBox;
	boundingBox._min = D3DXVECTOR3(-5000.0f, -100.0f, -5000.0f);
	boundingBox._max = D3DXVECTOR3( 5000.0f,  300.0f,  5000.0f);
	snow = new Snow(&boundingBox, 2000);
	snow->init(Device, L"snowflake.dds");



	//子弹
	bullet=new BULLET(Device,&Camera);
	bullet->InitBullet ();

	//敌方坦克和子弹
	enetank=new EnemyTank(Device);
	enetank->InitTank ();
	enetank->InitEbullet();

	//初始化天空盒
	skybox->InitSkyBox(60000.f);

	//初始化道具类
	props=new GameProps(Device);
	props->InitProps ();

	//初始化爆炸效果
	explode=new Explode(Device);
	explode->InitExplode ();

	//设置投影变换
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
	D3DX_PI*0.5f,
	800.0/600.0,
	1.0f,
	100000.0f
		);
	Device->SetTransform (D3DTS_PROJECTION,&proj);
	//设置材质
	D3DMATERIAL9 mtrl;
	::ZeroMemory(&mtrl,sizeof(mtrl));
	D3DXCOLOR colorA=d3d::BABYBLUE;
	D3DXCOLOR colorB=d3d::RED ;
	D3DXCOLOR colorC=d3d::BLUE;
	D3DXCOLOR colorD=d3d::GREEN ;
	mtrl=d3d::IintMetirial(&colorA,&colorB,&colorC,&colorD,1.0f);
	Device->SetMaterial (&mtrl);
	//设置灯光
	D3DLIGHT9 light;
	::ZeroMemory(&light,sizeof(light));
	D3DXCOLOR LightColor=d3d::WHILT ;
	D3DXVECTOR3 LightPosition (0.0f,0.0f,-500.0f);
	light=d3d::InitPointlight(&LightPosition,&LightColor);
	Device->SetLight (0,&light);	
	//开启灯光，关闭背面消隐
	Device->LightEnable (0,true);
	Device->SetRenderState (D3DRS_CULLMODE,D3DCULL_NONE);
	//设置纹理过滤方式
	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);  
   	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);  
    Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);  
	return true;
}

//渲染函数
bool Display(float timeDelta)
{
	timedelta=timeDelta;
	if( Device ) 
	{ 
		//清空后台
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(100,155,255), 1.0f, 0);
		//开始绘制
		Device->BeginScene ();
		if(g_currentGUI == GUI_MAIN_SCREEN)
			ProcessGUI(g_MainGUI, g_LMBDown, g_MouseX,g_MouseY, GUICallback);
		else if(g_currentGUI == GUI_START_SCREEN)
			ProcessGUI(g_StartGUI, g_LMBDown, g_MouseX,g_MouseY, GUICallback);
		else if(g_currentGUI == GUI_LOAD_SCREEN)
			ProcessGUI(g_LoadGUI, g_LMBDown, g_MouseX,g_MouseY, GUICallback);
		else if(g_currentGUI == GUI_OPTION_SCREEN)
			ProcessGUI(g_OptionGUI, g_LMBDown, g_MouseX,g_MouseY, GUICallback);
		else
			ProcessGUI(g_MainGUI, g_LMBDown, g_MouseX,g_MouseY, GUICallback);
		Device->EndScene ();
		Device->Present(0, 0, 0, 0);//提交后台缓存示翻转）
	}
	return true;
}


void render (float timeDelta)
{
		static int i=0;
		//设置取景变换
		D3DXMATRIX V;
		Camera.SetViewMatrix(&V);
		Device->SetTransform  (D3DTS_VIEW,&V);
		if(props->Effect2(timeDelta))
			KeyboardAndMouse(&Camera ,timeDelta,3.0f);
		else
			KeyboardAndMouse(&Camera ,timeDelta,1.0f);//处理键盘按键信息	
		D3DXMATRIX TerrainWorld;
		D3DXMatrixTranslation(&TerrainWorld,0.0f,-160.0f,0.0f);
		//更新雪花、粒子枪、烟花
		snow->update(timeDelta);
		props->upDateProps (timeDelta);//更新道具类


		//定义一个向量tankposition，获取摄像机当前位置（也即是坦克当前位置）。

		D3DXVECTOR3 tankposition;
		Camera.getposition(&tankposition);
		mytank->calcublood (enetank->EB_MTCollisiontest(tankposition));//把该位置传给敌人子弹与我方坦克的碰撞函数，进行碰撞检测，该函数放回碰撞次数																//作为我方坦克计算血量的实参
		
		//开启深度缓存
		Device->SetRenderState (D3DRS_ZENABLE,true);
		Device->SetRenderState (D3DRS_ZFUNC,D3DCMP_LESS);
		Device->SetRenderState (D3DRS_ZWRITEENABLE,true);

		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(100,155,255), 1.0f, 0);

		//开始绘制
		Device->BeginScene ();


		//渲染天空盒
		skybox->RenderSkyBox();
		//渲染地形
		terrain->draw (&TerrainWorld,false);


		//渲染我方坦克
		mytank->SetTank (&Camera,DInput);
		mytank->RenderXfile ();
		mytank->rendblood ();//渲染坦克血条

		
		//更新子弹
		bullet->Update(timeDelta);
		//更新敌方坦克
		enetank->upDate (timeDelta);
		//更新敌方坦克子弹
		enetank->UpDateBullet(timeDelta);

		//鼠标左键释放子弹
		if (DInput->IsMouseButtonDown(0))
			 {
				 bullet->addbullet ();	 
			 }
		//鼠标右键添加敌方坦克
		if(enetank->getTankNum()<6)
			enetank->addTank();

		
		//绘制我方子弹
		bullet->render ();
		//绘制敌方坦克
		enetank->Render ();

		//绘制敌方坦克子弹
		enetank->RenderBullet();
		
		
		/*渲染地上物体*/
		groundobject1->RanderObject(&V);
		groundobject->RanderObject(&V);

		props->RenderProps ();//渲染道具类
		props->Effect(timeDelta);
		props->effect1(&V,timeDelta);
		if(props->Effect3())//如果补血道具被吃到
			mytank->setblood ();//重新设置血量

		bullet->renderbound(&V,timeDelta);//绘制我方子弹打中敌方坦克的爆炸效果
		props->renderbound (&V,timeDelta);//绘制道具飞机子弹爆炸效果；
		groundobject->renderbound(&V,timeDelta);

		if(props->rendertime(timeDelta)||mytank->getblood ()==1)//绘制时间 !rendertime表示游戏时间还有剩余
			{
				if(enetank->getNumOfDead ()>=30)
					g_StartGUI->AddButton(BUTTON_BACK_ID, 350, 300, L"win.png", L"win.png",
					L"win.png");
				if(mytank->getblood ()==1||enetank->getNumOfDead()<30)
					g_StartGUI->AddButton(BUTTON_BACK_ID, 350, 300, L"fail.png", L"fail.png",
					L"fail.png");
			}

		//绘制雪花
		snow->render();

		enetank->DrawDead ();
		Device->EndScene ();
		Device->Present(0, 0, 0, 0);//提交后台缓存示翻转）

				//关闭深度缓存
		Device->SetRenderState (D3DRS_ZENABLE,false);
		Device->SetRenderState (D3DRS_ZFUNC,D3DCMP_LESS);
		Device->SetRenderState (D3DRS_ZWRITEENABLE,false);


}

void Cleanup()
{
	delete skybox;
	delete terrain;
	delete groundobject;
	delete groundobject1;
	delete snow;
	delete bullet;
	delete enetank;
	delete DInput;
	//delete explode;
	//delete props;
}
//-----------------------------------【GUICallback( )函数】---------------------------------------
//	描述：GUI系统的回调函数，填写按钮按下后的相关处理代码
//--------------------------------------------------------------------------------------------------
void GUICallback(int id, int state)
{
	switch(id)
	{
	case BUTTON_START_ID:   //start game开始游戏按钮
		if(state == UGP_BUTTON_DOWN)
			{
				g_currentGUI = GUI_START_SCREEN;
				
			}
		break;

	case BUTTON_LOAD_ID:  //load game载入游戏按钮
		if(state == UGP_BUTTON_DOWN)
			{
				g_currentGUI = GUI_LOAD_SCREEN;
				Init();
			}	
		break;

	case BUTTON_OPTION_ID: //option设置按钮
		if(state == UGP_BUTTON_DOWN)
			g_currentGUI = GUI_OPTION_SCREEN;
		break;

	case BUTTON_BACK_ID: //back返回按钮
		if(state == UGP_BUTTON_DOWN)
			g_currentGUI = GUI_MAIN_SCREEN;
		break;

	case BUTTON_QUIT_ID://quit退出按钮
		if(state == UGP_BUTTON_DOWN)
			PostQuitMessage(0);
		break;

	case BUTTON_LEVEL_1_ID:
		render (timedelta);//start game开始游戏页面中，Level1按钮
		break;
	}
}