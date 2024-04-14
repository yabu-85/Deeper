#include "Cell.h"
#include "../Engine/Model.h"
#include "../Engine/Input.h"

namespace {
	bool boxDraw = false;
}

Cell::Cell()
	:position_(0,-99999,0), length_(0)
{
	for (int i = 0; i < 8; i++) verPos_[i] = XMFLOAT3(0, 0, 0);
}

void Cell::SetPosLeng(XMFLOAT3 pos, float leng)
{
	position_ = pos;
	length_ = leng;
	
	//�����v��荶��X�^�[�g�i�ォ�王�_�j
	verPos_[0] = XMFLOAT3(pos.x, pos.y, pos.z + leng);
	verPos_[1] = XMFLOAT3(pos.x + leng, pos.y, pos.z + leng);
	verPos_[2] = XMFLOAT3(pos.x + leng, pos.y, pos.z);
	verPos_[3] = XMFLOAT3(pos.x, pos.y, pos.z);

	//�㎞�v���
	verPos_[4] = XMFLOAT3(pos.x, pos.y + leng, pos.z + leng);
	verPos_[5] = XMFLOAT3(pos.x + leng, pos.y + leng, pos.z + leng);
	verPos_[6] = XMFLOAT3(pos.x + leng, pos.y + leng, pos.z);
	verPos_[7] = XMFLOAT3(pos.x, pos.y + leng, pos.z);

}

bool Cell::SetTriangle(Triangle& t)
{
	Polygons tp = t.GetAllPosition();

	//�S���_�� Cell�̒��_�ʒu���O���ɂ���ꍇ�́A�֐����I���
	if (tp.pos1.x < verPos_[7].x && tp.pos2.x < verPos_[7].x && tp.pos3.x < verPos_[7].x || //�S���_���E
		tp.pos1.x > verPos_[6].x && tp.pos2.x > verPos_[6].x && tp.pos3.x > verPos_[6].x || //�S���_����
		
		tp.pos1.y < verPos_[3].y && tp.pos2.y < verPos_[3].y && tp.pos3.y < verPos_[3].y || //�S���_����
		tp.pos1.y > verPos_[7].y && tp.pos2.y > verPos_[7].y && tp.pos3.y > verPos_[7].y || //�S���_����

		tp.pos1.z < verPos_[7].z && tp.pos2.z < verPos_[7].z && tp.pos3.z < verPos_[7].z || //�S���_����
		tp.pos1.z > verPos_[4].z && tp.pos2.z > verPos_[4].z && tp.pos3.z > verPos_[4].z )  //�S���_���O
	{
		return false;
	}

	triangles_.push_back(&t);
	return true;
}

void Cell::ResetTriangles()
{
	triangles_.clear();
}

bool Cell::SegmentVsTriangle(RayCastData* _data, float& minDist)
{
	bool hit = false;
	minDist = FBXSDK_FLOAT_MAX;

	for (int i = 0; i < (int)triangles_.size(); i++) {
		triangles_.at(i)->RayCast(_data);

		//���C���������E�ŏ�������������㏑��
		if (_data->hit && minDist > _data->dist) {
			minDist = _data->dist;
			hit = true;
		}
	}

	return hit;
}