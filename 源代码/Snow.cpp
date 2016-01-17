#include"Snow.h"


Snow::Snow(d3d::BoundingBox *boundingBox,int numParticles)
{
	_boundingBox=*boundingBox;
	_size=5.0f; //ϵͳ��ÿ�����ӵĴ�С
	_vbSize=2048;//���㻺��Ĵ�С
	_vbOffset=0;
	_vbBatchSize=512;//һ�λ���Ĵ�С
	for(int i=0;i<numParticles;i++)
		addParticle();
}

void Snow::resetParticle(Attribute *attribute)
{
	attribute->_isAlive =true;
	//����������������һ��λ��
	d3d::GetRandomVector(&attribute->_position ,&_boundingBox._min,&_boundingBox._max);
	//yȡ��ߵ�
	attribute->_position.y =_boundingBox._max.y ;
	//����������ٶ�
	attribute->_velocity.x=d3d::GetRandomFloat (0.0f,1.0f)*-30.0f;
	attribute->_velocity .y =d3d::GetRandomFloat (0.0f,1.0f)*-100.0f;
	attribute->_velocity.z =0.0f;
	//��ɫ
	attribute->_color =d3d::WHILT ;
}
void Snow::update (float timeDelta)
{
	std::list <Attribute>::iterator i;
	for(i=_particles.begin();i!=_particles.end ();i++)
	{
		i->_position +=i->_velocity *timeDelta;

		if (_boundingBox.isPointInside(i->_position )==false)
		{
			resetParticle(&(*i));
		}
	}
}