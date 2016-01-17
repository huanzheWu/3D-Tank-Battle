#include"BillBoard.h"
const DWORD BillBoard::vertex::FVF =D3DFVF_XYZ|D3DFVF_TEX1;

//构造函数
BillBoard::BillBoard(LPDIRECT3DDEVICE9 device)
{
	Device=device;
}
void BillBoard::InitBoard(wchar_t * filename)
{
	Device->CreateVertexBuffer (
		4*sizeof(vertex),
		0,
		BillBoard::vertex::FVF,
		D3DPOOL_MANAGED,
		&Vb,
		0
		);
	vertex *BoardVertex=NULL;
	Vb->Lock (0,0,(void **)&BoardVertex,0);
	BoardVertex[0]=vertex(-100.0f,300.0f,0.0f,0.0f,0.0f);//a
	BoardVertex[1]=vertex(100.0f,300.0f,0.0f,1.0f,0.0f);//b
	BoardVertex[2]=vertex(-100.0f,0.0f,0.0f,0.0f,1.0f);//c
	BoardVertex[3]=vertex(100.0f,0.0f,0.0f,1.0f,1.0f);//d
	Vb->Unlock ();
	D3DXCreateTextureFromFile(Device,filename, &Tex);
	Device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );  
	Device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );  
}

void BillBoard::RanderBorad(D3DXMATRIX *View,D3DXMATRIX matNPC)
{
	Device->SetRenderState (D3DRS_ALPHABLENDENABLE,true);//开启alpha融合
	//设置融合系数
	Device->SetRenderState (D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	Device->SetRenderState (D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	D3DXMATRIX matBillboard;  
	D3DXMatrixIdentity(&matBillboard);  
	matBillboard._11 =(*View)._11;  
	matBillboard._13 =(*View)._13;  
	matBillboard._31 =(*View)._31;  
	matBillboard._33 =(*View)._33;
	D3DXMatrixInverse(&matBillboard, NULL, &matBillboard);  
	MatNPC=matNPC; 
	MatNPC =  matBillboard*MatNPC  ;  
	Device->SetTransform(D3DTS_WORLD, &MatNPC ); 
	//开始绘制
	Device->SetTexture(0, Tex);  
	Device->SetStreamSource(0, Vb , 0, sizeof(vertex) );  
	Device->SetFVF(BillBoard::vertex::FVF);  
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2 );  
	Device->SetRenderState (D3DRS_ALPHABLENDENABLE,false);

}


BillBoard::~BillBoard()
{
	Vb->Release ();
	Tex->Release ();
}
