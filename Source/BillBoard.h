//==========================================================================
//������
//��������Ա������InitBoard(wchar_t *filename)��RanderBorad(D3DXMATRIX *view,D3DXMATRIX matNPC)
//ʹ��RanderBoardʱӦ�ô���������������ó���ȡ���任�����Լ�һ������ָ�������λ��
//==========================================================================
//��ѧ���棺������ת����ΪʲôҪȡ��:
//��������Ĺ۲������͹���ƽ��ķ�������֪
//��������������ת�������෴��
//==========================================================================
//����һ��ʼ�������ﱳ���޷����������⣺
//��Ҫ�����ںϵ�����Ӧ���Ȼ��Ƴ��������ںϹ�ʽֻ��DestPixel�ǵ�ǰ���ں�̨����
//�е�������ɫֵ
//==========================================================================
//2014.8.6
//�汾1.0
//==========================================================================
#pragma once
#include"d3dUtility.h"
class BillBoard
{
	private :
		LPDIRECT3DVERTEXBUFFER9 Vb;
		LPDIRECT3DTEXTURE9 Tex;
		LPDIRECT3DDEVICE9 Device;
		D3DXMATRIX MatNPC;
		struct vertex
		{
			vertex(){}
			vertex(float x,float y,float z,float u,float v)
			{
				_x=x;_y=y,_z=z;
				_u=u;_v=v;
			}
			float _x,_y, _z;
			float _u,_v;
			static const DWORD FVF;
		};

	public:

		BillBoard(LPDIRECT3DDEVICE9 dvice);
		~BillBoard();
		void InitBoard(wchar_t *filename);
		void RanderBorad(D3DXMATRIX *view,D3DXMATRIX matNPC);
};