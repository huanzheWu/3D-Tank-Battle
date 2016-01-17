#include"d3dUtility.h"
#include "DirectInputClass.h"
//-----------------------------------------------------------------------------
// ����: ���캯��
//-----------------------------------------------------------------------------
DInputClass::DInputClass()
{
	m_pDirectInput = NULL;
	m_KeyboardDevice = NULL;
	ZeroMemory(m_keyBuffer,sizeof(char)*256);
	m_MouseDevice= NULL;
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));
}


//-----------------------------------------------------------------------------
// ��������DInputClass::Init(��
// ����: ��ʼ��DirectInput���̼���������豸
//-----------------------------------------------------------------------------
HRESULT DInputClass::Init( HWND hWnd,HINSTANCE hInstance,DWORD keyboardCoopFlags, DWORD mouseCoopFlags )
{
	HRESULT hr;
	//��ʼ��һ��IDirectInput8�ӿڶ���
	HR(DirectInput8Create( hInstance, DIRECTINPUT_VERSION, 
		IID_IDirectInput8,(void**)&m_pDirectInput,NULL ));

	//���м����豸�ĳ�ʼ��
	HR( m_pDirectInput->CreateDevice( GUID_SysKeyboard, &m_KeyboardDevice, NULL )); //�����豸
	HR( m_KeyboardDevice->SetCooperativeLevel( hWnd, keyboardCoopFlags));//�����豸��Э������
	HR( m_KeyboardDevice->SetDataFormat( &c_dfDIKeyboard ));//�������ݸ�ʽ
	HR( m_KeyboardDevice->Acquire( ));//��ȡȨ��
	HR( m_KeyboardDevice->Poll( ));//��ȡ��ѯ

	//��������豸�ĳ�ʼ��
	HR( m_pDirectInput->CreateDevice( GUID_SysMouse, &m_MouseDevice, NULL ));
	HR( m_MouseDevice->SetCooperativeLevel( hWnd,mouseCoopFlags));
	HR( m_MouseDevice->SetDataFormat( &c_dfDIMouse ));
	HR( m_MouseDevice->Acquire( ));
	HR( m_KeyboardDevice->Poll( ));

	return S_OK;
}

//-----------------------------------------------------------------------------
// ��������DInputClass::GetInput()
// ����: ���ڻ�ȡ������Ϣ�ĺ���
//-----------------------------------------------------------------------------
void DInputClass::GetInput()
{
	HRESULT hr = m_KeyboardDevice->GetDeviceState(sizeof(m_keyBuffer), (void*)&m_keyBuffer); 

	//��ȡ����������Ϣ
	if(hr)
	{
		m_KeyboardDevice->Acquire();  
		m_KeyboardDevice->GetDeviceState( sizeof(m_keyBuffer),(LPVOID)m_keyBuffer );
	}

	hr = m_MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (void*)&m_MouseState); 
	//��ȡ���������Ϣ
	if(hr)
	{
		m_MouseDevice->Acquire();
		m_MouseDevice->GetDeviceState( sizeof(DIMOUSESTATE), (void*)&m_MouseState);
	}
}

//-----------------------------------------------------------------------------
// ��������DInputClass::IsKeyDown()
// ����: �жϼ�����ĳ�����Ƿ���
//-----------------------------------------------------------------------------
bool DInputClass::IsKeyDown(int iKey)
{
	if(m_keyBuffer[iKey] & 0x80)
		return true;
	else
		return false;
}


//-----------------------------------------------------------------------------
// ��������DInputClass::IsMouseButtonDown()
// ����: �ж������ĳ���Ƿ���
//-----------------------------------------------------------------------------
bool DInputClass::IsMouseButtonDown(int button)
{
	return (m_MouseState.rgbButtons[button] & 0x80) != 0;
}

//-----------------------------------------------------------------------------
// ��������DInputClass::MouseDX
// ����: �������ָ���X������ֵ
//-----------------------------------------------------------------------------
float DInputClass::MouseDX()
{
	return (float)m_MouseState.lX;
}

//-----------------------------------------------------------------------------
// ��������DInputClass::MouseDY
// ����: �������ָ���Y������ֵ
//-----------------------------------------------------------------------------
float DInputClass::MouseDY()
{
	return (float)m_MouseState.lY;
}

//-----------------------------------------------------------------------------
// ��������DInputClass::MouseDZ
// ����: �������ָ���Z������ֵ�����֣�
//-----------------------------------------------------------------------------
float DInputClass::MouseDZ()
{
	return (float)m_MouseState.lZ;
}




//-----------------------------------------------------------------------------
// ����: ��������
//-----------------------------------------------------------------------------
DInputClass::~DInputClass(void)
{
	if(m_KeyboardDevice != NULL)
		m_KeyboardDevice->Unacquire();
	if(m_MouseDevice != NULL)
		m_MouseDevice->Unacquire();
	m_KeyboardDevice->Release ();
	m_MouseDevice->Release ();
	m_pDirectInput->Release ();
}
