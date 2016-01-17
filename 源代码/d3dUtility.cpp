#include "d3dUtility.h"

#define  MAX_RANGE_LIGHT  10000.0f; 


bool d3d::InitD3D(
	HINSTANCE hInstance,
	int width,
	int height,
	bool windowed,
	D3DDEVTYPE deviceType,
	IDirect3DDevice9** device,HWND *hWnd)
{
	//******************************************************************
	// Create the main application window.
	//����һ������
	//******************************************************************
	WNDCLASS wc;//����һ������
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)d3d::WndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon=(HICON)::LoadImage(NULL,L"iconi.ico",IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE); 
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = L"̹�������ս";

	if( !RegisterClass(&wc) )  //ע��һ������ʵ��
	{
		::MessageBox(0, L"RegisterClass() - FAILED", 0, 0);
		return false;
	}
		
	HWND hwnd = 0;

	//����һ������
	hwnd = ::CreateWindow(
		L"̹�������ս",
		L"̹�������ս",  
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width, 
		height,
		0 /*parent hwnd*/,
		0 /* menu */, 
		hInstance,
		0 /*extra*/); 

	*hWnd=hwnd;//��������ھ������ȥ������ʼ��DInputʹ��

	if( !hwnd )
	{
		::MessageBox(0, L"CreateWindow() - FAILED", 0, 0);
		return false;
	}

	::ShowWindow(hwnd, SW_SHOW);  //��ʾ����
	::UpdateWindow(hwnd);		//���´���

	//**************************************************************
	// Init D3D: 
	//��ʼ��D3D
	//**************************************************************
	HRESULT hr = 0;
	// Step 1: Create the IDirect3D9 object.��ȡ�ӿ�IDirect3D9ָ��

	IDirect3D9* d3d9 = 0;//
    d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

    if( !d3d9 )
	{
		::MessageBox(0, L"Direct3DCreate9() - FAILED", 0, 0);
		return false;
	}
	// Step 2: Check for hardware vp.
	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);

	int vp = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// Step 3: Fill out the D3DPRESENT_PARAMETERS structure.
 
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth            = width;
	d3dpp.BackBufferHeight           = height;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = hwnd;
	d3dpp.Windowed                   = true;//windowed
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = 0;//D3DPRESENT_RATE_DEFAULT
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	// Step 4: Create the device.
	hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT, // primary adapter
		deviceType,         // device type
		hwnd,               // window associated with device
		vp,                 // vertex processing
	    &d3dpp,             // present parameters
	    device);            // return created device

	if( FAILED(hr) )
	{
		// try again using a 16-bit depth buffer
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		
		hr = d3d9->CreateDevice(
			D3DADAPTER_DEFAULT,
			deviceType,
			hwnd,
			vp,
			&d3dpp,
			device);
		if( FAILED(hr) )
		{
			d3d9->Release(); // done with d3d9 object
			::MessageBox(0, L"CreateDevice() - FAILED", 0, 0);
			return false;
		}
	}
	d3d9->Release(); // done with d3d9 object
	return true;
}

//**********************************************
//��Ϣѭ��
//**********************************************
int d3d::EnterMsgLoop( bool (*ptr_display)(float timeDelta) )
{
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));
	static float lastTime = (float)timeGetTime();  //tiemGetTime()��������ϵͳʱ��
	while(msg.message != WM_QUIT)
	{
		if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))//PM_REMOVE ����Ϣ������ɾ����Ϣ
		{
			::TranslateMessage(&msg);//�����ⰴ��ת��Ϊ�ַ���Ϣ����������Ϣ���С�
			::DispatchMessage(&msg);//����һ����Ϣ�����ڹ���
		}
		else
        {	
			float currTime  = (float)timeGetTime();
			float timeDelta = (currTime - lastTime)*0.001f;
			ptr_display(timeDelta); //������Ⱦ����
			lastTime = currTime;
        }
    }
    return msg.wParam;
}


D3DLIGHT9 d3d::InitDirectionalight ( D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 Light;
	::ZeroMemory(&Light,sizeof(Light));
	Light.Type=D3DLIGHT_DIRECTIONAL;
	Light.Direction=*direction;
	Light.Ambient=*color*0.4f;
	Light.Diffuse=*color;
	Light.Specular=*color * 0.6f;
	return Light;
}

D3DLIGHT9 d3d::InitPointlight(D3DXVECTOR3 *Position,D3DXCOLOR *Color)
{
	D3DLIGHT9 Light;
	::ZeroMemory(&Light,sizeof(Light));
	Light.Ambient=*Color*0.4f;
	Light.Diffuse=*Color;
	Light.Specular=*Color*0.6f;
	Light.Falloff=1.0f;
	Light.Range =MAX_RANGE_LIGHT;
	Light.Position=*Position;
	Light.Attenuation0 = 1.0f;
	Light.Attenuation1 = 0.0f;
	Light.Attenuation2 = 0.0f;
	Light.Type=D3DLIGHT_POINT;
	return Light;

}
D3DLIGHT9 d3d::InitSpotlight(D3DXVECTOR3 *position,D3DXVECTOR3 *direction,D3DXCOLOR *color)
{
	D3DLIGHT9 Light;
	::ZeroMemory(&Light,sizeof(Light));
	Light.Ambient=*color*0.4f;
	Light.Specular  = *color * 0.6f;
	Light.Diffuse=*color;
	Light.Direction=*direction;
	Light.Range=MAX_RANGE_LIGHT;
	Light.Position=*position;
	Light.Falloff=1.0f;
	Light.Type=D3DLIGHT_SPOT;
	Light.Attenuation0 = 1.0f;
	Light.Attenuation1 = 0.0f;
	Light.Attenuation2 = 0.0f;
	Light.Theta        = 0.4f;
	Light.Phi          = 0.9f;
	return Light;
}
D3DMATERIAL9 d3d::IintMetirial(D3DXCOLOR *a,D3DXCOLOR *b,D3DXCOLOR *c,D3DXCOLOR *d,float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient=*a;
	mtrl.Diffuse=*b;
	mtrl.Emissive=*c;
	mtrl.Power=p;
	mtrl.Specular=*d;
	return mtrl;
}

DWORD d3d::FtoDw(float f)
{
	return *(( DWORD * ) &f ) ;
}
float d3d::GetRandomFloat(float lowBound,float highBound)
{
	if(lowBound>highBound)
		return lowBound;
	float f=(rand()%10000)*0.0001f;
	return (f*(highBound-lowBound))+lowBound;
}
void d3d::GetRandomVector(D3DXVECTOR3* out,D3DXVECTOR3 * min,D3DXVECTOR3 * max)
{
	out->x =GetRandomFloat(min->x ,max->x);
	out->y =GetRandomFloat(min->y ,max->y);
	out->z =GetRandomFloat(min->z,max->z );
}
d3d::BoundingBox::BoundingBox()
{
	_min.x = d3d::INFINITY;
	_min.y = d3d::INFINITY;
	_min.z = d3d::INFINITY;

	_max.x = -d3d::INFINITY;
	_max.y = -d3d::INFINITY;
	_max.z = -d3d::INFINITY;
}

bool d3d::BoundingBox::isPointInside(D3DXVECTOR3& p)
{
	if( p.x >= _min.x && p.y >= _min.y && p.z >= _min.z &&
		p.x <= _max.x && p.y <= _max.y && p.z <= _max.z )
	{
		return true;
	}
	else
	{
		return false;
	}
}