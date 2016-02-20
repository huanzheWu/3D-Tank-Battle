#include"Mirror.h"
const DWORD Mirror::MirrorVertex::FVF_Mirror = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

void Mirror::InitMirror(int length)
{
	
	Device->CreateVertexBuffer (
		6*sizeof(MirrorVertex),
		0,
		MirrorVertex::FVF_Mirror,
		D3DPOOL_MANAGED,
		&Vb,
		0
		);
	MirrorVertex *mirrorVertex=NULL;
	Vb->Lock (0,0,(void **)&mirrorVertex,0);
	mirrorVertex[0]=MirrorVertex(0,0.0f,1000.0f,-1.0f,0.0f,0.0f,0.0f,0.0f);//a
	mirrorVertex[1]=MirrorVertex(0,1000,1000,-1.0f,0.0f,0.0f,1.0f,1.0f);//c
	mirrorVertex[2]=MirrorVertex(0,1000,-1000,-1.0f,0.0f,0.0f,1.0f,0.0f);//b

	mirrorVertex[3]=MirrorVertex(0,0.0f,1000,-1.0f,0.0f,0.0f,1.0f,1.0f);//a
	mirrorVertex[4]=MirrorVertex(0,1000,-1000,-1.0f,0.0f,0.0f,0.0f,1.0f);//b
	mirrorVertex[5]=MirrorVertex(0,0.0f,-1000,-1.0f,0.0f,0.0f,1.0f,0.0f);//d

	Vb->Unlock ();
	D3DXCreateTextureFromFile(Device, L"ice.bmp", &Mirrortex);
}
D3DXMATRIX Mirror::PreSet(D3DXMATRIX moddleplace)
{
	
		ModdlePlace=moddleplace;
		//��Ⱦ����
		D3DXMATRIX I;
		D3DXMatrixTranslation(&I,101.0f,-200.0f,0.0f);
		Device->SetTransform(D3DTS_WORLD, &I);
		Device->SetStreamSource (0,Vb,0,sizeof(MirrorVertex));
		Device->SetFVF (MirrorVertex::FVF_Mirror);
		Device->SetTexture  (0,Mirrortex);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);

		Device->SetRenderState (D3DRS_STENCILENABLE,true);//����ģ�滺��
		Device->SetRenderState (D3DRS_STENCILFUNC,D3DCMP_ALWAYS);//�������ǳɹ�
		Device->SetRenderState (D3DRS_STENCILREF,0x1);//�ο�ֵ
		Device->SetRenderState (D3DRS_STENCILMASK,0xffffffff);
		Device->SetRenderState (D3DRS_STENCILWRITEMASK,0xffffffff);
		Device->SetRenderState (D3DRS_STENCILZFAIL,D3DSTENCILOP_KEEP);
		Device->SetRenderState (D3DRS_STENCILFAIL,D3DSTENCILOP_KEEP);
		Device->SetRenderState (D3DRS_STENCILPASS,D3DSTENCILOP_REPLACE);//��ģ��ο�ֵ���ģ�滺���ж�Ӧ��ֵ

		Device->SetRenderState (D3DRS_ZWRITEENABLE,false);//�ر���Ȼ��棬��ֹ��z�������д����

		Device->SetRenderState (D3DRS_ALPHABLENDENABLE,true);//�����ں����㣬��ֹ�����̨�����еı仯
		//�����ں����ӣ���������ʹ�ú�̨���治�����仯
		Device->SetRenderState (D3DRS_SRCBLEND,D3DBLEND_ZERO);
		Device->SetRenderState (D3DRS_DESTBLEND,D3DBLEND_ONE);
		
		//Ȼ��ʼ��������
		Device->SetTexture  (0,Mirrortex);
		D3DXMatrixTranslation(&I,100.0f,-200.0f,0.0f);
		Device->SetTransform(D3DTS_WORLD, &I);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);

		//����ӳ��ֻ��ƥ�����д�루ֻ���ھ�����������ʾ��
		Device->SetRenderState (D3DRS_STENCILFUNC,D3DCMP_EQUAL);
		Device->SetRenderState (D3DRS_STENCILPASS,D3DSTENCILOP_KEEP);

		//������Ȼ��棬��Ϊ������ȴ��ھ������
		Device->Clear (0,0,D3DCLEAR_ZBUFFER,0,1.0f,0);
		//�����ںϣ�ʹ��ӳ���ھ����г���
		Device->SetRenderState (D3DRS_SRCBLEND,D3DBLEND_DESTCOLOR);
		Device->SetRenderState (D3DRS_DESTBLEND,D3DBLEND_ZERO);
		Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

		//��λӳ��λ��
		D3DXMATRIX W,R;
		D3DXPLANE planeXZ(1.0f,0.0f,0.0f,-100.0f);//xzƽ��
		D3DXMatrixReflect(&R,&planeXZ);
		W=R*ModdlePlace;

		return W;

}
void Mirror::LastSet ()
{

		Device->SetRenderState (D3DRS_ALPHABLENDENABLE,false);
		Device->SetRenderState (D3DRS_STENCILENABLE,false);
		Device->SetRenderState (D3DRS_ZWRITEENABLE,true);
		Device->SetRenderState (D3DRS_CULLMODE,D3DCULL_CCW);
}
Mirror::~Mirror()
{
	Vb->Release ();
	Mirrortex->Release ();
}