#include"d3dUtility.h"
#include"CameraClass.h"
#include"DirectInputClass.h"
extern DInputClass *DInput;//将DInput的作用域拓展到这里
void KeyboardAndMouse( CamerClass *Camera,float timeDelta,float multiple )
{

	DInput->GetInput();//获取控制权
	 float movez=0.0f,movex=0.0f,movey=0.0f;

		if( ::GetAsyncKeyState(VK_UP) & 0x8000f )
			{
				movey+=10;
				(*Camera).fly(movey);
			}
		if( ::GetAsyncKeyState(VK_DOWN) & 0x8000f )
			{
				movey+=10;
				(*Camera).fly(-movey);
			}	
		if( ::GetAsyncKeyState(VK_F1) & 0x8000f )
		{
			D3DXVECTOR3 position(0.0f,0.0f,-3.0f);
			(*Camera).setposition(&position);
		}
		if(DInput->IsKeyDown(DIK_W) )
		{
			movez+=5 *multiple;
			(*Camera).walk (movez);
		}
		if( DInput->IsKeyDown(DIK_S))
			{
				movez+=5* multiple;
				(*Camera).walk (-movez);
			}
			if( DInput->IsKeyDown(DIK_A) )
			{
				movex+=10* multiple;
				(*Camera).strafe (-movex);
			}
		if( DInput->IsKeyDown(DIK_D) )
			{
				movex+=10* multiple;
				(*Camera).strafe (movex);
			}
		/*if(::GetAsyncKeyState ('W')&0x8000f)
		{
			(*Camera).pitch(D3DX_PI*timeDelta);
		}
		if(::GetAsyncKeyState ('S')&0x8000f)
		{
			(*Camera).pitch(-D3DX_PI*timeDelta);
		}*/
		//if(::GetAsyncKeyState ('A')&0x8000f)	
		//	(*Camera).yaw(-D3DX_PI*timeDelta*00.5);	
		//if(::GetAsyncKeyState ('D')&0x8000f)
		//	(*Camera).yaw(D3DX_PI*timeDelta*00.5);
}