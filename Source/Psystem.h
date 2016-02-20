//=========================================================================
//							����ϵͳ����
//
//=========================================================================


#pragma once
#include"d3dUtility.h"
#include<list>

//���ӽṹ
struct Particle
{
	D3DXVECTOR3 _position;
	D3DCOLOR    _color;
};


//��������
struct Attribute
{
	D3DXVECTOR3 _position;
	D3DXVECTOR3 _velocity;//���ӵ��ٶ�
	D3DXVECTOR3 _acceleration ;//���ٶ�
	float _lifeTime;
	float _color;
	float _colorFade;
	bool _isAlive;
	float _age;
};
class PSystem
{
	public:
		PSystem();
		virtual ~PSystem();

		virtual bool init( IDirect3DDevice9* device, wchar_t * texFileName); //�������㻺�� ��������
		virtual void reset();//�����趨ϵͳ�����ӵ�����

		virtual void resetParticle (Attribute * attribute)=0; //����������������ֵ
		virtual void addParticle();//Ϊϵͳ����һ������

		virtual void update(float timeDelta)=0;//��ϵͳ���������ӽ��и���

		//����ǰ�к�
		virtual void preRender ();
		virtual void render ();
		virtual void postRender ();

		bool isEmpty();//��⵱ǰϵͳ�Ƿ�������
		bool isDead();//������������Ƿ�����

		protected:
			virtual void removeDeadParticles(); //�����Ա�����������Ƴ��������ӡ�
		protected:
			IDirect3DDevice9* _device;
			D3DXVECTOR3 _origin;//ϵͳ����Դ
			float _emitRate;
			float _size;//ϵͳ���������ӵĳߴ�
			IDirect3DTexture9* _tex;
			IDirect3DVertexBuffer9* _vb;
			std::list<Attribute>  _particles;
			int _maxParticles;
			d3d::BoundingBox  _boundingBox;//�����

			DWORD _vbSize;
			DWORD _vbOffset;
			DWORD _vbBatchSize;
};