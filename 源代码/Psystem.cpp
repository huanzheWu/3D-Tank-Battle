#include"d3dUtility.h"
#include"Psystem.h"

PSystem::PSystem()
{
	_device = 0;
	_vb     = 0;
	_tex    = 0;
}

PSystem::~PSystem()
{
	d3d::Release<IDirect3DVertexBuffer9*>(_vb);
	d3d::Release<IDirect3DTexture9*>(_tex);
}

//��ʼ������
bool PSystem:: init (IDirect3DDevice9 * device,wchar_t *texFileName)
{
	_device = device; 
	HRESULT hr=0;
	//�������㻺��
	hr=device->CreateVertexBuffer (
		_vbSize* sizeof(Particle),
		D3DUSAGE_DYNAMIC|D3DUSAGE_POINTS|D3DUSAGE_WRITEONLY,
		D3DFVF_XYZ|D3DFVF_DIFFUSE,
		D3DPOOL_DEFAULT, 
		&_vb,
		0
		);
	if(FAILED(hr))
	{
		::MessageBox(0, L"CreateVertexBuffer() - FAILED", L"PSystem", 0);
		return false;
	}
	//��������
	hr=D3DXCreateTextureFromFile(
		device,
		texFileName,
		&_tex);
	if(FAILED(hr))
	{
		::MessageBox(0, L"CreateTextureFromFile - FAILED", L"PSystem", 0);
		return false;
	}
}

//��Ⱦǰ�����ú���
void PSystem ::preRender()
{
	
	_device->SetRenderState(D3DRS_LIGHTING, false);
	_device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	_device->SetRenderState(D3DRS_POINTSCALEENABLE, true); 
	_device->SetRenderState(D3DRS_POINTSIZE, d3d::FtoDw(_size));
	_device->SetRenderState(D3DRS_POINTSIZE_MIN, d3d::FtoDw(0.0f));

	// control the size of the particle relative to distance
	_device->SetRenderState(D3DRS_POINTSCALE_A, d3d::FtoDw(0.0f));
	_device->SetRenderState(D3DRS_POINTSCALE_B, d3d::FtoDw(0.0f));
	_device->SetRenderState(D3DRS_POINTSCALE_C, d3d::FtoDw(1.0f));
		
	// use alpha from texture
	_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    _device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}


void PSystem ::render()
{
	//����任
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	_device->SetTransform(D3DTS_WORLD,&world);

	//�����������Ԫ�� �������
	if(!_particles.empty ())
	{	
		preRender ();
		_device->SetTexture (0,_tex); //���ò���
		_device->SetFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE); //���ö����ʽ
		_device->SetStreamSource (0,_vb,0,sizeof(Particle)); //������Դ��
	
		//���ƫ�����Ƿ񳬹��ڴ��С
		if(_vbOffset>=_vbSize)
			_vbOffset=0;
	
		DWORD numParticlesInBatch =0;

		Particle *v=0;
		_vb->Lock (
			_vbOffset *sizeof(Particle),
			_vbBatchSize * sizeof(Particle),
			(void **)&v,
			_vbOffset? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD );

		//�������� ��ʼ��
		std::list <Attribute>::iterator i;
		for(i=_particles.begin();i!=_particles.end ();i++)
		{
			if(i->_isAlive==true)
			{
				v->_color=i->_color;
				v->_position=i->_position;
				v++;
				numParticlesInBatch++;
				if(numParticlesInBatch==_vbBatchSize)
				{
					_vb->Unlock ();
					//���ԣ�����ʵ����ֻ�ǰ����ӻ���������λ���޹أ�
					_device->DrawPrimitive(D3DPT_POINTLIST,_vbOffset,_vbBatchSize);
					_vbOffset+=_vbBatchSize;
					if(_vbOffset>=_vbSize)
						_vbOffset=0;
					numParticlesInBatch=0;
					_vb->Lock (
								_vbOffset *sizeof(Particle),
								_vbBatchSize * sizeof(Particle),
								(void **)&v,
								_vbOffset? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD );
				}
			}
		}
		_vb->Unlock ();

	//��Ҫ����һ����������˴���Ⱦ�л��ŵ�����������_vbBatchSize�ı�����Ҳ���޷��������һ��Ƭ���У�numParticlesInBatch==_vbBatchSize��������
	//��ʱ���һ��Ƭ��û�б����Ƶ�������Ҫ���������һ��Ƭ��
		if(numParticlesInBatch)  //���һ��Ƭ����������
		{
			_device->DrawPrimitive(D3DPT_POINTLIST,_vbOffset,numParticlesInBatch);
		}
		_vbOffset+=_vbBatchSize;
		postRender();	
	}
}



//���ƺ���
void PSystem::postRender ()
{
	_device->SetRenderState (D3DRS_LIGHTING,true);
	_device->SetRenderState (D3DRS_ALPHABLENDENABLE,false);
	_device->SetRenderState (D3DRS_POINTSPRITEENABLE,false);
	_device->SetRenderState (D3DRS_POINTSCALEENABLE,false);
}

//�����趨ϵͳ��ÿ�����ӵ�����
void PSystem::reset ()
{
	std::list <Attribute>::iterator i;
	for(i=_particles.begin ();i!=_particles.end ();i++)
	{
		resetParticle(&(*i)); //��������ǳ�������ǿ��Ҫ����������ʵ��
	}
}

void PSystem::addParticle ()
{
	Attribute attribute;
	resetParticle(&attribute);//��ʼ��һ��
	_particles.push_back (attribute);//��β������һ��Ԫ�أ�
}

//�Ƴ�����������
void PSystem::removeDeadParticles ()
{
	std::list<Attribute>::iterator i;
	i=_particles.begin();
	while(i!=_particles.end ())
	{
		if(i->_isAlive==false)
			i=_particles.erase (i);
		else 
			i++;
	}
}


//�жϵ�ǰϵͳ�Ƿ�������

bool PSystem::isEmpty ()
{
	return _particles.empty();
}

//�ж������Ƿ�ȫ������
bool PSystem::isDead ()
{
	std::list <Attribute> ::iterator i;
	for(i=_particles.begin ();i!=_particles.end ();i++)
	{
		if(i->_isAlive ==true)
			return false;
	}
	return true ;
}
