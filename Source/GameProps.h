#pragma once
#include"d3dUtility.h"
#include"XFileMesh.h"
#include<list>
#include"Mytank.h"
#include"EnemyTank.h"

//ʱ��Ķ����ʽ
struct TIME
{
	TIME(){}
	TIME(float _x,float _y,float _z,float _rhw,unsigned long _color,float _tu,float _tv)
	{
		x=_x,y=_y,z=_z,rhw=_rhw,color=_color,tu=_tu,tv=_tv;
	}
	float x, y, z, rhw;
	unsigned long color;
	float tu, tv;
};

//��ը�ṹ��
struct EXPLODE
{
	D3DXVECTOR3 _position;
	int sign;//��ǵ�ǰ��Ӧ�Ĺ����Ӧ�û�����һ��DDS�ļ�
};
struct PROBULLET //�ӵ��ṹ��
{
	D3DXVECTOR3 _position;//λ��
	D3DXVECTOR3 _speed;//�ٶȷ���
	float _isalive;//�Ƿ����
	float _happentime;//�Ѿ�����ʱ��
	float _lifetime;//�ӵ����ʱ��
};

struct PROPS  //���߽ṹ��
{
	int ID;//�������һ�ֵ���
	D3DXVECTOR3 _position; //���߳��ֵ�λ��
	float _timeDelta; //���߳��ֵļ��ʱ��
	float _happenTime; //���߸ձ��Ե���ʱ��
	wchar_t _fileName; //����x�ļ�����
	bool _isalive; //�Ƿ���ţ���û���Ե���
};


struct PLANE//�ɻ��ṹ��
{
	D3DXVECTOR3 _position;//�ɻ���λ��
	D3DXVECTOR3 _look;//�ɻ���ǰ������
	float _lifetime;//�ɻ����������Ʒ��еľ��룩
	float _keeptime;//���ֵ�ʱ��
	bool _isalive;//�Ƿ����
};

class GameProps //�̳�XFileMesh��
{
	private:
		float time;
		std::list <PLANE> _allplane;//�ɻ�����
		std::list<PROPS> _allProps;//��������
		std::list <EXPLODE> _allbound;
		std::list<PROBULLET> _allbullet;//�ӵ�����
		XFileMesh *PPops1; //����һ��x�ļ�ָ��
		XFileMesh *PPops2; //���߶���x�ļ�ָ��
		XFileMesh *PPops3;//��������x�ļ�ָ��
		XFileMesh *PEffect1; //��һ�ֵ��߶�Ӧ��Ч�����ɻ���ը����x�ļ�ģ��
		XFileMesh *PBullet ;//�ӵ�x�ļ�ָ��
		LPDIRECT3DDEVICE9 Device;

		LPDIRECT3DVERTEXBUFFER9 Vb;//���㻺��
		LPDIRECT3DTEXTURE9 TimeTex[61];//����ӿڣ�60��������ʱ�䣩

		int sign;//һ����ǣ���Ǽ��ٵ����Ƿ񱻳Ե�
		int sign3;//��ǵ����������Ƿ񱻳Ե�

		float speeduptime;//���ٵ�ʱ��

	public:	
		GameProps(LPDIRECT3DDEVICE9 Device);
		~GameProps();
		void InitProps();//��ʼ������
		void RenderProps();//��Ⱦ���ߺ���
		bool Collision(D3DXVECTOR3 position);  //���̹���Ƿ�Ե��˵��ߣ���==��ײ��
		void upDateProps(float timeDelta); //���µ���״̬
		void effect1(D3DXMATRIX *V,float timeDelta);//Ч��һ����Ӧ����һ��
		void Effect (float timeDelta);
		bool Effect2(float timeDelta);//�жϼ��ٵ����Ƿ񱻳Ե��ĺ���
		void updateplane (D3DXMATRIX * V,float timeDelte);//���·ɻ�״̬�������ӵ���
		void resetplane(PLANE *plane);
		void addplane(int numofpalne);//��ӷɻ�����
		void removedead();//�Ƴ������ķɻ����ӵ�
		void addbullet(D3DXVECTOR3 position);//����ӵ�����//��������ӵ�ʱ��Ҫ֪���ӵ��ĳ�ʼλ�ã�
		void resetbullet(D3DXVECTOR3 position,PROBULLET* bullet);//��ʼ��һ���ӵ�
		bool Effect3();
		void renderbound(D3DXMATRIX *V,float timeDelta);//���Ʊ�ը����
		bool rendertime(float timeDelta);//����ʱ�亯��

};
