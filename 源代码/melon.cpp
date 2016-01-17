//===========================================
//ͷ�ļ�
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

#define WINDOW_WIDTH	800						//Ϊ���ڿ�ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڿ��
#define WINDOW_HEIGHT	600						//Ϊ���ڸ߶ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڸ߶�

//=========================================
//ȫ�ֱ�����������
//==========================================
LPDIRECT3DDEVICE9 Device = 0;//		D3D�豸�ӿ�
D3DXMATRIX World;          //������һ������
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
bool												g_LMBDown = false;      // GUI�е����״̬��Ϣ���������Ƿ��µı�ʶ
int												g_MouseX = 0, g_MouseY = 0;      //�洢����������������
float timedelta;
//
 //�����ĸ�GUI����󣬷ֱ�����ĸ�ҳ��
D3DGUIClass		*g_MainGUI = NULL;//������
D3DGUIClass		*g_StartGUI = NULL; //��Ϸ��ʼ ����
D3DGUIClass		*g_LoadGUI = NULL; //��Ϸ���봰��
D3DGUIClass		*g_OptionGUI = NULL; //��Ϸ���ô���
int						g_MainGUIFontID = -1;						//  GUI����������ID
int						g_StartGUIFontID = -1;						//  GUI����������ID
int						g_LoadGUIFontID = -1;						//  GUI����������ID
int						g_OptionGUIFontID = -1;					//  GUI����������ID
int						g_currentGUI = GUI_MAIN_SCREEN;    //һ����ǰ��GUI��ʶ
//==========================================
//�ⲿ��������
//==========================================
extern void KeyboardAndMouse( CamerClass *Camera,float ,float  );
//==========================================
//ȫ�ֺ�����������
//==========================================
bool Setup();
void Cleanup();
bool Display(float timeDelta);
//
//
//
void	GUICallback(int id, int state); //GUI�Ļص�����
//
//
//
//==========================================
// WinMain ������ں���
//==========================================
int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	 
	HWND hwnd = 0;//���ڴ���ȥInitD3D�л�ȡ���ھ������Ϊ��ʼ��DInput��һ������
	if(!d3d::InitD3D(hinstance,800,600, true, D3DDEVTYPE_HAL, &Device,&hwnd))
	{
		
		::MessageBox(0, L"InitD3D() - FAILED", 0, 0);
		return 0;
	}
	//��DInput���������ԭ����Ϊ��ʼ��DInput��Ҫ���ھ����ʵ�����ھ����Ϊ�˲���SET UP���Ӳ��������������ʼ���ˡ�
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
// ������Ϣ������
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
			//������ʱ�Ĳ���
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
//���ú���
//
bool Setup()                                                                                                                                                                                                                                                                                                                                                             
{
	PlaySound(L"menu.wav", NULL, SND_FILENAME | SND_ASYNC|SND_LOOP);
	srand((unsigned int)time(0));//�����������

	// ����һЩGUIϵͳ
	g_MainGUI = new D3DGUIClass(Device, WINDOW_WIDTH, WINDOW_HEIGHT); //���˵�ҳ��
	g_StartGUI = new D3DGUIClass(Device, WINDOW_WIDTH, WINDOW_HEIGHT); //��ʼ����Ϸҳ��
	g_LoadGUI = new D3DGUIClass(Device, WINDOW_WIDTH, WINDOW_HEIGHT); //������Ϸҳ��
	g_OptionGUI = new D3DGUIClass(Device, WINDOW_WIDTH, WINDOW_HEIGHT); //����ҳ��

	// ���ĸ�ҳ��ֱ���ӱ���ͼ
	if(!g_MainGUI->AddBackground(L"maingui.jpg")) return false;  
	if(!g_StartGUI->AddBackground(L"startgui.jpg")) return false;
	if(!g_LoadGUI->AddBackground(L"loadgui.jpg")) return false;
	if(!g_OptionGUI->AddBackground(L"optiongui.jpg")) return false;


	// �ֱ���ĸ�ҳ���������
	if(!g_MainGUI->CreateTextFont(L"΢���ź�", 28, &g_MainGUIFontID)) return false;
	if(!g_StartGUI->CreateTextFont(L"΢���ź�", 38, &g_StartGUIFontID)) return false;
	if(!g_LoadGUI->CreateTextFont(L"΢���ź�", 38, &g_LoadGUIFontID)) return false;
	if(!g_OptionGUI->CreateTextFont(L"΢���ź�", 38, &g_OptionGUIFontID)) return false;

	//---------------------------------�����˵�mainҳ����ص�ҳ�沼�ִ��롿---------------------------------
	// ��Ӿ�̬�ı���ҳ����
	if(!g_MainGUI->AddStaticText(STATIC_TEXT_ID, L"Make By Me.",
		350, 535, D3DCOLOR_XRGB(55,155,255), g_MainGUIFontID)) return false;

	// ���4����ť���ֱ��ǿ�ʼ��Ϸ��������ȣ�ѡ����˳���Ϸ��ÿ����ť��Ӧ3��ͼ
	if(!g_MainGUI->AddButton(BUTTON_START_ID, 350, 340, L"startUp.png",
		L"StartOver.png", L"startDown.png")) return false;
	
	if(!g_MainGUI->AddButton(BUTTON_LOAD_ID, 350, 385, L"loadUp.png",
		L"loadOver.png", L"loadDown.png")) return false;

	if(!g_MainGUI->AddButton(BUTTON_OPTION_ID,350, 430, L"optionsUp.png",
		L"optionsOver.png", L"optionsDown.png")) return false;

	if(!g_MainGUI->AddButton(BUTTON_QUIT_ID, 350, 475, L"quitUp.png",
		L"quitOver.png", L"quitDown.png")) return false;


	//------------------------����ʼ����Ϸstartҳ����ص�ҳ�沼�ִ��롿------------------------
	// ��Ӱ�ť��ҳ����
	if(!g_StartGUI->AddButton(BUTTON_LEVEL_1_ID,550, 380,  L"level1Up.png", L"level1Over.png",
		L"level1Down.png")) return false;


	if(!g_StartGUI->AddButton(BUTTON_BACK_ID, 10, 550, L"backUp.png", L"backOver.png",
		L"backDown.png")) return false;

	//------------------------��������Ϸloadҳ����ص�ҳ�沼�ִ��롿------------------------
	//��Ӿ�̬�ı���ҳ����
	if(!g_LoadGUI->AddStaticText(STATIC_TEXT_ID, L"������Ϸ�ɹ����뷵�غ�ʼ��Ϸ��",
		150, 20, D3DCOLOR_XRGB(180,180,150), g_LoadGUIFontID)) return false;
	// ��Ӱ�ť��ҳ����
	if(!g_LoadGUI->AddButton(BUTTON_BACK_ID, 10, 500, L"backUp.png", L"backOver.png",
		L"backDown.png")) return false;


	//------------------------����Ϸ����optionҳ����ص�ҳ�沼�ִ��롿------------------------
	// ��Ӱ�ť��ҳ����
	if(!g_OptionGUI->AddButton(BUTTON_BACK_ID,50, 450, L"backUp.png", L"backOver.png",
		L"backDown.png")) return false;
	//��Ӿ�̬�ı���ҳ����
	 if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"������ʾ��",
	 		180, 60, D3DCOLOR_XRGB(0,0,0), g_OptionGUIFontID)) return false;
	 if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"ǰ��: W",
	 		240, 90, D3DCOLOR_XRGB(0,0,0), g_OptionGUIFontID)) return false;
	 if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"���ˣ�S",
	 		240, 120, D3DCOLOR_XRGB(0,0,0), g_OptionGUIFontID)) return false;
	 if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"���ƣ�A",
	 		240, 150, D3DCOLOR_XRGB(0,0,0), g_OptionGUIFontID)) return false;
	 if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"���ƣ�D��",
	 		240, 180, D3DCOLOR_XRGB(0,0,0), g_OptionGUIFontID)) return false;
	 if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"�ƶ���꣺��ת�Ӿ�",
	 		240, 210, D3DCOLOR_XRGB(240,240,240), g_OptionGUIFontID)) return false;
	 if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"�������������ӵ�",
	 		240, 240, D3DCOLOR_XRGB(240,240,240), g_OptionGUIFontID)) return false;
	 if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"���߽��ܣ�",
	 		180, 270, D3DCOLOR_XRGB(240,240,240), g_OptionGUIFontID)) return false;
	 if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"ը�����ٻ���ը��֧Ԯ��",
	 		240, 300, D3DCOLOR_XRGB(240,240,240), g_OptionGUIFontID)) return false;
	 if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"Ѫ��������������",
	 		240, 330, D3DCOLOR_XRGB(240,240,240), g_OptionGUIFontID)) return false;
	 if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"Ь�ӣ����٣���ʱ��",
	 		240, 360, D3DCOLOR_XRGB(240,240,240), g_OptionGUIFontID)) return false;
	 if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"��Ϸ����",
	 		180, 390, D3DCOLOR_XRGB(240,240,240), g_OptionGUIFontID)) return false;
	  if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"�ڹ涨ʱ����������ɱ��30����������ʤ����",
	 		200, 420, D3DCOLOR_XRGB(240,240,240), g_OptionGUIFontID)) return false;
	return true;
}

bool Init ()
{	
	Device->SetRenderState(D3DRS_LIGHTING,true);
	Device->SetRenderState(D3DRS_NORMALIZENORMALS,true);

	skybox= new SkyBox(Device);
	skybox->LoadSkyTextureFromFile (L"frontsnow1.jpg",L"backsnow1.jpg",L"leftsnow1.jpg",L"rightsnow1.jpg",L"topsnow1.jpg",L"groundsnow1.jpg");//������պ���ͼ

	terrain=new Terrain(Device,"heighmap.raw", 200, 200, 60,0.1f);//����һ��������
	terrain->loadTexture(L"ground.jpg");

	groundobject=new GroundObject(Device);
	groundobject->InitObject(L"tree.dds",L"028.x",50,20);//����50�������

	groundobject1=new GroundObject(Device);
	groundobject1->InitObject(L"tree.dds",L"WoodCutter.x",0,2);

	//====================================================================
	//̹��������
	//====================================================================
	mytank=new Tank(Device);
	mytank->InitXfile(L"M4A375W_.x");

	//ȷ��ѩ������
	d3d::BoundingBox boundingBox;
	boundingBox._min = D3DXVECTOR3(-5000.0f, -100.0f, -5000.0f);
	boundingBox._max = D3DXVECTOR3( 5000.0f,  300.0f,  5000.0f);
	snow = new Snow(&boundingBox, 2000);
	snow->init(Device, L"snowflake.dds");



	//�ӵ�
	bullet=new BULLET(Device,&Camera);
	bullet->InitBullet ();

	//�з�̹�˺��ӵ�
	enetank=new EnemyTank(Device);
	enetank->InitTank ();
	enetank->InitEbullet();

	//��ʼ����պ�
	skybox->InitSkyBox(60000.f);

	//��ʼ��������
	props=new GameProps(Device);
	props->InitProps ();

	//��ʼ����ըЧ��
	explode=new Explode(Device);
	explode->InitExplode ();

	//����ͶӰ�任
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
	D3DX_PI*0.5f,
	800.0/600.0,
	1.0f,
	100000.0f
		);
	Device->SetTransform (D3DTS_PROJECTION,&proj);
	//���ò���
	D3DMATERIAL9 mtrl;
	::ZeroMemory(&mtrl,sizeof(mtrl));
	D3DXCOLOR colorA=d3d::BABYBLUE;
	D3DXCOLOR colorB=d3d::RED ;
	D3DXCOLOR colorC=d3d::BLUE;
	D3DXCOLOR colorD=d3d::GREEN ;
	mtrl=d3d::IintMetirial(&colorA,&colorB,&colorC,&colorD,1.0f);
	Device->SetMaterial (&mtrl);
	//���õƹ�
	D3DLIGHT9 light;
	::ZeroMemory(&light,sizeof(light));
	D3DXCOLOR LightColor=d3d::WHILT ;
	D3DXVECTOR3 LightPosition (0.0f,0.0f,-500.0f);
	light=d3d::InitPointlight(&LightPosition,&LightColor);
	Device->SetLight (0,&light);	
	//�����ƹ⣬�رձ�������
	Device->LightEnable (0,true);
	Device->SetRenderState (D3DRS_CULLMODE,D3DCULL_NONE);
	//����������˷�ʽ
	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);  
   	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);  
    Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);  
	return true;
}

//��Ⱦ����
bool Display(float timeDelta)
{
	timedelta=timeDelta;
	if( Device ) 
	{ 
		//��պ�̨
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(100,155,255), 1.0f, 0);
		//��ʼ����
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
		Device->Present(0, 0, 0, 0);//�ύ��̨����ʾ��ת��
	}
	return true;
}


void render (float timeDelta)
{
		static int i=0;
		//����ȡ���任
		D3DXMATRIX V;
		Camera.SetViewMatrix(&V);
		Device->SetTransform  (D3DTS_VIEW,&V);
		if(props->Effect2(timeDelta))
			KeyboardAndMouse(&Camera ,timeDelta,3.0f);
		else
			KeyboardAndMouse(&Camera ,timeDelta,1.0f);//������̰�����Ϣ	
		D3DXMATRIX TerrainWorld;
		D3DXMatrixTranslation(&TerrainWorld,0.0f,-160.0f,0.0f);
		//����ѩ��������ǹ���̻�
		snow->update(timeDelta);
		props->upDateProps (timeDelta);//���µ�����


		//����һ������tankposition����ȡ�������ǰλ�ã�Ҳ����̹�˵�ǰλ�ã���

		D3DXVECTOR3 tankposition;
		Camera.getposition(&tankposition);
		mytank->calcublood (enetank->EB_MTCollisiontest(tankposition));//�Ѹ�λ�ô��������ӵ����ҷ�̹�˵���ײ������������ײ��⣬�ú����Ż���ײ����																//��Ϊ�ҷ�̹�˼���Ѫ����ʵ��
		
		//������Ȼ���
		Device->SetRenderState (D3DRS_ZENABLE,true);
		Device->SetRenderState (D3DRS_ZFUNC,D3DCMP_LESS);
		Device->SetRenderState (D3DRS_ZWRITEENABLE,true);

		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(100,155,255), 1.0f, 0);

		//��ʼ����
		Device->BeginScene ();


		//��Ⱦ��պ�
		skybox->RenderSkyBox();
		//��Ⱦ����
		terrain->draw (&TerrainWorld,false);


		//��Ⱦ�ҷ�̹��
		mytank->SetTank (&Camera,DInput);
		mytank->RenderXfile ();
		mytank->rendblood ();//��Ⱦ̹��Ѫ��

		
		//�����ӵ�
		bullet->Update(timeDelta);
		//���µз�̹��
		enetank->upDate (timeDelta);
		//���µз�̹���ӵ�
		enetank->UpDateBullet(timeDelta);

		//�������ͷ��ӵ�
		if (DInput->IsMouseButtonDown(0))
			 {
				 bullet->addbullet ();	 
			 }
		//����Ҽ���ӵз�̹��
		if(enetank->getTankNum()<6)
			enetank->addTank();

		
		//�����ҷ��ӵ�
		bullet->render ();
		//���Ƶз�̹��
		enetank->Render ();

		//���Ƶз�̹���ӵ�
		enetank->RenderBullet();
		
		
		/*��Ⱦ��������*/
		groundobject1->RanderObject(&V);
		groundobject->RanderObject(&V);

		props->RenderProps ();//��Ⱦ������
		props->Effect(timeDelta);
		props->effect1(&V,timeDelta);
		if(props->Effect3())//�����Ѫ���߱��Ե�
			mytank->setblood ();//��������Ѫ��

		bullet->renderbound(&V,timeDelta);//�����ҷ��ӵ����ез�̹�˵ı�ըЧ��
		props->renderbound (&V,timeDelta);//���Ƶ��߷ɻ��ӵ���ըЧ����
		groundobject->renderbound(&V,timeDelta);

		if(props->rendertime(timeDelta)||mytank->getblood ()==1)//����ʱ�� !rendertime��ʾ��Ϸʱ�仹��ʣ��
			{
				if(enetank->getNumOfDead ()>=30)
					g_StartGUI->AddButton(BUTTON_BACK_ID, 350, 300, L"win.png", L"win.png",
					L"win.png");
				if(mytank->getblood ()==1||enetank->getNumOfDead()<30)
					g_StartGUI->AddButton(BUTTON_BACK_ID, 350, 300, L"fail.png", L"fail.png",
					L"fail.png");
			}

		//����ѩ��
		snow->render();

		enetank->DrawDead ();
		Device->EndScene ();
		Device->Present(0, 0, 0, 0);//�ύ��̨����ʾ��ת��

				//�ر���Ȼ���
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
//-----------------------------------��GUICallback( )������---------------------------------------
//	������GUIϵͳ�Ļص���������д��ť���º����ش������
//--------------------------------------------------------------------------------------------------
void GUICallback(int id, int state)
{
	switch(id)
	{
	case BUTTON_START_ID:   //start game��ʼ��Ϸ��ť
		if(state == UGP_BUTTON_DOWN)
			{
				g_currentGUI = GUI_START_SCREEN;
				
			}
		break;

	case BUTTON_LOAD_ID:  //load game������Ϸ��ť
		if(state == UGP_BUTTON_DOWN)
			{
				g_currentGUI = GUI_LOAD_SCREEN;
				Init();
			}	
		break;

	case BUTTON_OPTION_ID: //option���ð�ť
		if(state == UGP_BUTTON_DOWN)
			g_currentGUI = GUI_OPTION_SCREEN;
		break;

	case BUTTON_BACK_ID: //back���ذ�ť
		if(state == UGP_BUTTON_DOWN)
			g_currentGUI = GUI_MAIN_SCREEN;
		break;

	case BUTTON_QUIT_ID://quit�˳���ť
		if(state == UGP_BUTTON_DOWN)
			PostQuitMessage(0);
		break;

	case BUTTON_LEVEL_1_ID:
		render (timedelta);//start game��ʼ��Ϸҳ���У�Level1��ť
		break;
	}
}