#include"d3dUtility.h"
#include"XFileMesh.h"
#include<list>

//���߹��ܺ궨��
#define SPEEDUP 2
#define ADDBODY 1

//���߽ṹ��
struct PROPS
{
	int _id;//��ǵ���
	D3DXVECTOR3 _position;//λ��
	float _timeDelta;//����
	float _timeHappen;//��ʧ��ʱ��
	bool _isalive;//״̬
};

class Props 
{
	public:
		Props(PDIRECT3DDEVICE9 _Device){Device=_Device;};//���캯��
		~Props();//��������

	private:
		PDIRECT3DDEVICE9 Device;//D3D�豸�ӿ�ָ��
		IDirect3DTexture9 * propstex[10]; //����ָ������
		std::list <PROPS> _allprops;//��������
		XFileMesh *Prop1;//��һ�ֵ��� �����ӳ��ȣ�
	public:
		bool initProps();//��ʼ�����ߺ���
		void upDateProps(float timeDelta);//���µ��ߺ���
		void RenderProps();//���Ƶ��ߺ���
		int Collision(D3DXVECTOR3 position);//��ײ����
};