#pragma once
#include"Psystem.h"
#include"d3dUtility.h"
//�ӻ���PSystem�м̳ж���
class Snow :public PSystem
{
	public:
		Snow(d3d::BoundingBox *boundingBox,int numParticles);//���캯��
		void update (float timeDelta);
		void resetParticle(Attribute * attribute);
};