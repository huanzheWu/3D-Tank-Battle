#pragma once
#include<list>
#include"XFileMesh.h"
#include"d3dUtility.h"
#include"DirectInputClass.h"
#include"Bullet.h"
#include"GroundObject.h"
#include"GameProps.h"
class GameProps;
//�з�̹�˵��ӵ�
class BULLET;
struct Ebullet
{
		float  _age;//����
		float _lifetime;//����
		bool _alive;//�Ƿ����
		D3DXVECTOR3 _position;//�ӵ�λ��
		D3DXVECTOR3 _speed;//�ӵ����ٶȣ�����
		enum Whos{my,enemy};//˭���ӵ�
};
struct ENEMYTANK
{
	D3DXVECTOR3 _position;//̹�˵�λ��
	D3DXVECTOR3 _look;//̹�˹۲�����
	bool _isalive;//̹������״̬
	float _keeptime;//̹�˱�����������״̬��ʱ��
	D3DXMATRIX  _rotation;//̹�˵���ת�Ƕ�
	float _walktime; //��ʾ̹��ֱ�����ߵ�ʱ��Σ�
	float _launchtime;//��÷���һ���ӵ�
	float _unlaunchtime;//�����ӵ���ʱ����
	bool _launch;//�Ƿ����ӵ�
};

class EnemyTank :public XFileMesh 
{

	friend BULLET;//��ΪҪ�����ӵ��������ʳ�Ա����Collisiontest(D3DXVECTOR3 _position)��
	friend GameProps;//����Ϸ��������Ϊ��Ԫ����Ϊ��Ҫ��Ϸ�����зɻ����ӵ���Ҫ����EnemyTank����ײ������
	public:
		EnemyTank(LPDIRECT3DDEVICE9 _Device);
		~EnemyTank();//�����￼������������ôд����ȥ�޸�MyTank��

		//=======================================================
		//̹����غ���
		//=======================================================
		bool InitTank(); //��ʼ��̹��
		void Render();//��Ⱦ̹��
		void upDate(float timeDelta);//����̹��״̬
		void resetTank(ENEMYTANK *etank);//����̹�˳�ʼ״̬
		void addTank();//����һ��̹��
		void removedeadtank();
		//=======================================================
		//�ӵ���غ���
		//=======================================================
		void InitEbullet ();	//��ʼ���ӵ����� ---������ʹ�ú��ҷ�̹�˲�һ�����ӵ�
		void resetEbullet(Ebullet *ebullet,D3DXVECTOR3 position,D3DXVECTOR3 look);	//ָ��һ�����ӵ���״̬������Ҫ����i����ȡtrue��̹�ˣ�ָ���ӵ���λ�úͷ����ڲ����ã�
		void addEbullet (D3DXVECTOR3 position,D3DXVECTOR3 look);		//���ж�trueʱ���������ӵ��������һ���ӵ����ڲ����ã�
		void UpDateBullet(float timeDelta);	//�����ӵ�״̬��ÿ���ӵ���resetʱ��Ҫ����̹�˵�ʱ�ķ���
		void RenderBullet();	//��Ⱦ�ӵ�
		void removedead();		//�Ƴ��������ӵ����ڲ����ã�
		bool Collisiontest(D3DXVECTOR3 _position);
		int EB_MTCollisiontest(D3DXVECTOR3 _position);//�з��ӵ����ҷ�̹�˵���ײ����(�������˼���)
		void DrawDead();//����������̹�˵���Ŀ
		int getNumOfDead(){return numOfDead;};
		int getTankNum(){return numOfTank;};//��ȡ��ǰ̹������
		//
		//��ײ����
		//
		
	private :
		
		int numOfTank;//̹������
	    std::list<ENEMYTANK> _allenetank; //̹������
		std::list <Ebullet> _Ebullet;//�з�̹���ӵ�
		float angle;//̹����ת�ĽǶȣ�����趨����
		LPD3DXMESH Bulletmesh;
		LPD3DXBUFFER  eppAdjacency;
		LPD3DXBUFFER  eppMaterials;
		DWORD epNumMaterials;
		LPD3DXMESH eSourceMesh;
		LPD3DXPMESH ePMesh;//���н�������
		D3DMATERIAL9*       eg_pMaterials   ; // ����Ĳ�����Ϣ  
		LPDIRECT3DTEXTURE9* eg_pTextures    ; // �����������Ϣ 
		double deadline;//��������
		D3DXMATRIX rotation;//̹������ΧǽʱҪ��ת
		ID3DXFont *Texdeadnum;//2D�ı��ӿ�
		int numOfDead;//������̹�˵���Ŀ
};