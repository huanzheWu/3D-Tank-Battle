//*******************************************************
//ͷ�ļ�
//*******************************************************
#include "d3dUtility.h"
#include"Terrain.h"
#include"CameraClass.h"
#include"skyboxclass.h"
#include <tchar.h>
#include <Mmsystem.h>
#include"XFileMesh.h"
#include"Snake.h"
#include"Props.h"
#include"DirectInputClass.h"
#include"Ground.h"
#include"Snow.h"
#include"D3DGUI.h"
#include"GameUI.h"


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
//====================================================================
//ȫ�ֱ�����������
//====================================================================
LPDIRECT3DDEVICE9 Device = 0;			//		D3D�豸�ӿ�
D3DXMATRIX World;						//������һ������
CamerClass Camera(CamerClass::air);
SkyBox* skybox=NULL;
Terrain *terrain=NULL;
Snake *snake=NULL;
Snow * snow=NULL;
Props *props=NULL;						//������ָ��
DInputClass *DInput=NULL;				//Dinput��ָ��
IDirect3DTexture9 * SnakeBodyTex;
D3DMATERIAL9 mtrl;
Ground *groundobject=NULL;
GameUI *gameui=NULL;
GameUI *gameui2=NULL;
float gametime =0;


bool g_LMBDown = false;      // GUI�е����״̬��Ϣ���������Ƿ��µı�ʶ
int	 g_MouseX = 0, g_MouseY = 0;      //�洢����������������

float timedelta;
//
 //�����ĸ�GUI����󣬷ֱ�����ĸ�ҳ��
D3DGUIClass		*g_MainGUI		= NULL;//������
D3DGUIClass		*g_StartGUI		= NULL; //��Ϸ��ʼ����
D3DGUIClass		*g_ControlGUI	= NULL; //��Ϸ���봰��
D3DGUIClass		*g_StoreGUI		= NULL; //��Ϸ���ô���

int		g_MainGUIFontID		= -1;						//  GUI����������ID
int		g_StartGUIFontID	= -1;						//  GUI����������ID
int		g_ControlGUIFontID	= -1;						//  GUI����������ID
int		g_StoreGUIFontID	= -1;					//  GUI����������ID
int		g_currentGUI		= GUI_MAIN_SCREEN;    //һ����ǰ��GUI��ʶ

//=====================================
//�ⲿ��������
//=====================================
extern void KeyboardAndMouse( CamerClass *Camera,float  );
//===========================================
//ȫ�ֺ�����������
//===========================================
bool Setup();
void Cleanup();
bool Display(float timeDelta);
void ComputeNormal(D3DXVECTOR3 *P0,D3DXVECTOR3 *P1,D3DXVECTOR3 *P2,D3DXVECTOR3 *OUT);//�����淨����
void CalcuVertexVec(D3DXVECTOR3 *a,D3DXVECTOR3 *b,D3DXVECTOR3 *c,D3DXVECTOR3 *d,D3DXVECTOR3 *Res);
void	GUICallback(int id, int state); //GUI�Ļص�����
//===========================================
// WinMain ������ں���
//===========================================
int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	HWND hwnd = 0;

	if(!d3d::InitD3D(hinstance,800, 600, true, D3DDEVTYPE_HAL, &Device,&hwnd))
	{
		::MessageBox(0, L"InitD3D() - FAILED", 0, 0);
		return 0;
	}

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
		if( wParam == VK_ESCAPE )
			::DestroyWindow(hwnd);
		break;
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
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}
//
//���ú���
//
bool Setup()                                                                                                                                                                                                                                                                                                                                                             
{
	PlaySound(L"3.wav", NULL, SND_FILENAME | SND_ASYNC|SND_LOOP);
	//======================================================================================================================
	//GUIϵͳ��һЩ����
	//======================================================================================================================

	// ����һЩGUIϵͳ
	g_MainGUI = new D3DGUIClass(Device, WINDOW_WIDTH, WINDOW_HEIGHT); //���˵�ҳ��
	g_StartGUI = new D3DGUIClass(Device, WINDOW_WIDTH, WINDOW_HEIGHT); //��ʼ����Ϸҳ��
	g_ControlGUI  = new D3DGUIClass(Device, WINDOW_WIDTH, WINDOW_HEIGHT); //������Ϸҳ��
	g_StoreGUI  = new D3DGUIClass(Device, WINDOW_WIDTH, WINDOW_HEIGHT); //����ҳ��

	// ���ĸ�ҳ��ֱ���ӱ���ͼ
	if(!g_MainGUI->AddBackground(L"maingui.jpg")) return false;  
	if(!g_StartGUI->AddBackground (L"gameover.jpg")) return false ;
	if(!g_ControlGUI ->AddBackground(L"controlgui.jpg")) return false;
	if(!g_StoreGUI ->AddBackground(L"storegui.jpg")) return false;


	// �ֱ���ĸ�ҳ���������
	if(!g_MainGUI->CreateTextFont(L"΢���ź�", 28, &g_MainGUIFontID)) return false;
	if(!g_StartGUI->CreateTextFont(L"΢���ź�", 38, &g_StartGUIFontID)) return false;
	if(!g_ControlGUI ->CreateTextFont(L"΢���ź�", 38, &g_ControlGUIFontID)) return false;
	if(!g_StoreGUI ->CreateTextFont(L"΢���ź�", 38, &g_StoreGUIFontID)) return false;

	//---------------------------------�����˵�mainҳ����ص�ҳ�沼�ִ��롿----------------------

	// ���4����ť���ֱ��ǿ�ʼ��Ϸ��������ȣ�ѡ����˳���Ϸ��ÿ����ť��Ӧ3��ͼ
	if(!g_MainGUI->AddButton(BUTTON_START_ID, 0, 0, L"START_UP.png",
		L"START_OVER.png", L"START_DOWN.png")) return false;

	if(!g_MainGUI->AddButton(BUTTON_LOAD_ID, 0, 150, L"CONTROL_UP.png",
		L"CONTROL_OVER.png", L"CONTROL_DOWN.png")) return false;

	if(!g_MainGUI->AddButton(BUTTON_OPTION_ID,0, 330, L"STORE_UP.png",
		L"STORE_OVER.png", L"STORE_DOWN.png")) return false;

	if(!g_MainGUI->AddButton(BUTTON_QUIT_ID, 0, 475, L"THE_END_UP.png",
		L"THE_END_OVER.png", L"THE_END_DOWN.png"))return false ;

	//------------------------����ʼ����Ϸstartҳ����ص�ҳ�沼�ִ��롿------------------------

	if(!g_StartGUI->AddButton(BUTTON_LEVEL_1_ID,550, 1380,  L"level1Up.png", L"level1Over.png",
		L"level1Down.png"))return false;

	if(!g_StartGUI->AddButton(BUTTON_BACK_ID, 650, 500, L"backUp.png", L"backOver.png",
		L"backDown.png")) return false;

	//------------------------����Ϸ��������ҳ��ҳ����ص�ҳ�沼�ִ��롿-----------------------

	// ��Ӱ�ť��ҳ����
	if(!g_ControlGUI ->AddButton(BUTTON_BACK_ID, 650, 500, L"backUp.png", L"backOver.png",
		L"backDown.png")) return false;

	//------------------------����Ϸ����optionҳ����ص�ҳ�沼�ִ��롿------------------------
	// ��Ӱ�ť��ҳ����
	if(!g_StoreGUI ->AddButton(BUTTON_BACK_ID,650, 500, L"backUp.png", L"backOver.png",
		L"backDown.png")) return false;

	//�趨�ں�����
	Device->SetRenderState (D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	Device->SetRenderState (D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	//ָ��Alpha��Դ
	Device->SetTextureStageState (0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	Device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);

	Device->SetRenderState(D3DRS_LIGHTING,true);
	Device->SetRenderState(D3DRS_NORMALIZENORMALS,true);

	d3d::BoundingBox boundingBox;
	boundingBox._min = D3DXVECTOR3(-500.0f, 0.0f, -500.0f);
	boundingBox._max = D3DXVECTOR3( 500.0f,  300.0f,  500.0f);
	snow = new Snow(&boundingBox, 20);
	snow->init(Device, L"��4.png");

	skybox= new SkyBox(Device);
	skybox->LoadSkyTextureFromFile (L"frontsnow1.jpg",L"backsnow1.jpg",L"leftsnow1.jpg",L"rightsnow1.jpg",L"topsnow1.jpg",L"groundsnow1.jpg");//������պ���ͼ

	terrain=new Terrain(Device,"heighmap.raw", 200, 200, 60,0.0f);//����һ��������
	terrain->loadTexture(L"ground.jpg");

	snake=new Snake(Device);
	if(!snake->initSnake ())
		::MessageBox(0, L"InitSnake() - FAILED", 0, 0);

	props=new Props(Device);
	if(!props->initProps ())
		::MessageBox(0,L"InitProps()_FAILED" ,0,0);


	groundobject=new Ground (Device);
	groundobject->initObject ();
	D3DXVECTOR3 position;
	position.x =0.0f;position.y =0.0f;position.z =0.0f;//ָ��Χǽ����λ��
	groundobject->createBird (position);

	gameui=new GameUI(Device);//��Ϸ�����UI
	if(!gameui->initPicture  (L"����.png",10.0f,10.0f,5.0f))
		::MessageBox(0,L"InitUI()_FAILED" ,0,0);
	if(!gameui->initTex(L"����"))
		::MessageBox(0,L"InitFont()_FAILED" ,0,0);

	gameui2=new GameUI(Device);//��Ϸ�����UI
	if(!gameui2->initPicture  (L"EventObjectIcon4.dds",600.0f,10.0f,1.5f)) 
		::MessageBox(0,L"InitUI()_FAILED" ,0,0);

	//����ͶӰ�任
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
	D3DX_PI*0.5f,
	800.0/600.0,
	1.0f,
	30000.0f
		);
	Device->SetTransform (D3DTS_PROJECTION,&proj);

	D3DVIEWPORT9 VP={0,0,800,600,0,1};
	Device->SetViewport (&VP);


	//���ò���
	
	::ZeroMemory(&mtrl,sizeof(mtrl));
	D3DXCOLOR colorA=d3d::BLACK;
	D3DXCOLOR colorB=d3d::GREEN;
	D3DXCOLOR colorC=d3d::RED;
	D3DXCOLOR colorD=d3d::YELLOW;
	mtrl=d3d::IintMetirial(&colorA,&colorB,&colorC,&colorD,1.0f);
	Device->SetMaterial (&mtrl);

	//���õƹ�
	D3DLIGHT9 light;
	::ZeroMemory(&light,sizeof(light));
	D3DXCOLOR LightColor=d3d::GREEN  ;
	D3DXVECTOR3 LightPosition (0.0f,0.0f,0.0f);
	light=d3d::InitPointlight(&LightPosition,&LightColor);
	Device->SetLight (0,&light);	
	Device->LightEnable (0,true);
	Device->SetRenderState (D3DRS_CULLMODE,D3DCULL_NONE);
	skybox->InitSkyBox(20000.f);//��ʼ����պ�
	//����������˷�ʽ
	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);  
   	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);  
    Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);  

	D3DXCreateTextureFromFile(Device,L"ground.jpg",&SnakeBodyTex);

	return true;
}

//��Ⱦ����
//
//
bool Display(float timeDelta)
{
	timedelta=timeDelta;
	if( Device ) 
	{ 
		
		//*************��������*********************
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
		Device->BeginScene ();
		
		if(g_currentGUI == GUI_MAIN_SCREEN)
			ProcessGUI(g_MainGUI, g_LMBDown, g_MouseX,g_MouseY, GUICallback);
		else if(g_currentGUI == GUI_START_SCREEN)
			ProcessGUI(g_StartGUI, g_LMBDown, g_MouseX,g_MouseY, GUICallback);
		else if(g_currentGUI == GUI_LOAD_SCREEN)
			ProcessGUI(g_ControlGUI , g_LMBDown, g_MouseX,g_MouseY, GUICallback);
		else if(g_currentGUI == GUI_OPTION_SCREEN)
			ProcessGUI(g_StoreGUI , g_LMBDown, g_MouseX,g_MouseY, GUICallback);
		else
			ProcessGUI(g_MainGUI, g_LMBDown, g_MouseX,g_MouseY, GUICallback);

		Device->EndScene ();
		Device->Present(0, 0, 0, 0);//�ύ��̨����ʾ��ת��

	}
	return true;
}


void  render (float timeDelta)
{
		//����ȡ���任
	if (snake->ifDead ()==false)
	{
		gametime += timeDelta ;//������Ϸʱ��

		D3DXMATRIX V;
		Camera.SetViewMatrix(&V);
		Device->SetTransform  (D3DTS_VIEW,&V);
		KeyboardAndMouse(&Camera ,timeDelta);	
		D3DXMATRIX TerrainWorld;
		D3DXMatrixTranslation(&TerrainWorld,0.0f,00.0f,0.0f);

		//*************��������*********************
		
		props->upDateProps (timeDelta);
		groundobject->updateBird (timeDelta);
		snow->update(timeDelta);//������ɫ�Ļ�
		snake->changeWallRange (groundobject);
		D3DXVECTOR3 position;
		position.x =0.0f,position.y =0.0f,position.z =0.0f;
		groundobject->createWall (position);

		//ΪɶҪ���ߵĸ��º������������أ���Ϊ�����������Ⱦ����Ĳ���...
		snake->upDateSanke (timeDelta,DInput); 
		
		//������Ȼ���
		//������Ȼ���
		Device->SetRenderState (D3DRS_ZENABLE,true);
		Device->SetRenderState (D3DRS_ZFUNC,D3DCMP_LESS);
		Device->SetRenderState (D3DRS_ZWRITEENABLE,true);

		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(100,155,255), 1.0f, 0);
		
		//��ʼ����
		Device->BeginScene ();

		//��Ⱦ����
		terrain->draw (&TerrainWorld,false);

		//��Ⱦ��պ�
		skybox->RenderSkyBox();
	
	
		//��Ⱦ��������
		groundobject->renderGround ();
		
		//����ѩ��
		snow->render();
		gameui->RenderUI ();
		RECT rect;
		rect.left=120,rect.top=50,rect.right=800,rect.bottom=600;
		gameui->RenderFont(rect,snake->getScore());
		snake->Render();
		float i;
		groundobject->getWallRange(&i);
		if(snake->isDangerous (i))
			gameui2->RenderUI();

		//���Ƶ���
		props->RenderProps ();

		Device->EndScene ();
		Device->Present(0, 0, 0, 0);//�ύ��̨����ʾ��ת��
	}
	else 
	{
		//������Ϸ�÷�
		RECT rect;
		rect.left=280,rect.top=195,rect.right=800,rect.bottom=600;
		gameui->RenderFont (rect,snake->getScore());

		//����ʱ��
		rect.left=280,rect.top=140,rect.right=800,rect.bottom=600;
		gameui->RenderFont (rect,d3d::IntToWchart(gametime));

		//���Ƶ��ߵ÷�
		rect.left=280,rect.top=240,rect.right=800,rect.bottom=600;
		int propscore =props->getpropscore();
		gameui->RenderFont(rect ,d3d::IntToWchart(propscore));

		rect.left = 280,rect.top=422,rect.right=800,rect.bottom=600;
		int finnalscore=snake->getscore()+gametime +propscore*10;
		gameui->RenderFont (rect,d3d::IntToWchart(finnalscore));
		
	}
		//�ر���Ȼ���
		Device->SetRenderState (D3DRS_ZENABLE,false);
		Device->SetRenderState (D3DRS_ZFUNC,D3DCMP_LESS);
		Device->SetRenderState (D3DRS_ZWRITEENABLE,false);
}

void Cleanup()
{
		delete snake;
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
		render (timedelta); //start game��ʼ��Ϸҳ���У�Level1��ť
		break;
	}
}
