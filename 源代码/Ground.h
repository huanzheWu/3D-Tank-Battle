#pragma once
#include"XFileMesh.h"//����x�ļ�ͷ�ļ�
#include<list>

class Ground
{
	private :
		LPDIRECT3DDEVICE9 Device;//D3D�豸ָ��
		LPDIRECT3DTEXTURE9 Tex[1];//����ӿ�
		std::list <D3DXVECTOR3> _allWalls;//Χǽ������
		XFileMesh *Wall; //Χǽ��X�ļ�ָ��
		D3DXMATRIX WallPos;//ǽ�ڵ�λ�þ���
		D3DXMATRIX WallSca;//ǽ�ڵ����ž���
	public:
		Ground (LPDIRECT3DDEVICE9 _Device){Device=_Device;};//���캯��
		~Ground();//��������
		bool initObject ();//��ʼ������
		bool createWall(D3DXVECTOR3 centre);
		void renderGround ();

};