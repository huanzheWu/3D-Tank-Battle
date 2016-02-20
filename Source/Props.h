#include"d3dUtility.h"
#include<list>
#include"BillBoard.h"

//���߹��ܺ궨��
#define ADDBODY		1
#define SPEEDUP		2
#define SPEEDDOWN	3
#define CUTDOWN		4
#define ADDSCORE	5
#define NOADDSCORE  6
#define EXPAND		7
#define DEAD		8


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

	private:   //���ݳ�Ա
		ID3DXFont *PFont ; //����ӿ�ָ��
	 	PDIRECT3DDEVICE9 Device;//D3D�豸�ӿ�ָ��
		IDirect3DTexture9 * prop5tex[8]; //����5�Ĳ���ָ������
		IDirect3DTexture9 * prop6tex[8]; //����6�Ĳ���ָ������
		IDirect3DTexture9 * prop7tex[8]; //�����ߵĲ���ָ������
		
		LPDIRECT3DVERTEXBUFFER9 Vb;
		std::list <PROPS> _allprops;//��������
		BillBoard *prop1;//��һ�ֵ��ߣ��ù��������ʾ��(�ӳ�����)
		BillBoard *prop2;//�ڶ��ֵ��ߣ����٣�
		BillBoard *prop3;//�����ֵ��ߣ����٣�
		BillBoard *prop4;//�����ֵ��ߣ���ȥ���ȣ�
		float prop5TimeLeft,prop6TimeLeft,prop7TimeLeft;
		float prop5TimeExit,prop6TimeExit,prop7TimeExit;
		bool  isaddscore;
		int propscore;  //���ߵ÷�

		//�����������Ķ����ʽ
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

	private://�ڲ����ú���
		void RenderTime (int time,RECT rect );
		void RenderDynaminc(IDirect3DTexture9 * proptex[8],D3DXMATRIX positon,D3DXMATRIX V);
	public:
		bool initProps();//��ʼ�����ߺ���
		void upDateProps(float timeDelta);//���µ��ߺ���
		void RenderProps();//���Ƶ��ߺ���
		int Collision(D3DXVECTOR3 position);//��ײ����
		int getpropscore (){return propscore;};
};