#pragma once
#include "../Engine/Camera.h"
#include "../Engine/Direct3D.h"
#include "../Engine/Transform.h"
#include <vector>
#include "Triangle.h"

class BoxCollider;
class SphereCollider;

/// <summary>
/// �R���W�����}�b�v�ɓo�^����郏�[���h���}�X�ŋ�؂��ēo�^
/// Cell�̒���Triangle���o�^����A�o�^���ꂽ�̂œ����蔻��Ƃ����
/// </summary>
class Cell
{
	//�������̍��W
	XMFLOAT3 position_;

	//cube�̈�ӂ̒���
	float length_ = 0;

	//���_�ʒu posision
	XMFLOAT3 verPos_[8];

	//����CELL�ɑ�����O�p�|���S��
	std::vector<Triangle*> triangles_;
	int NumTriangles = 0;

public:
	Cell();

	void SetPosLeng(XMFLOAT3 pos, float leng);

	//�O�p�|���S����AAB�ɐG��Ă����炱��CELL�ɓo�^
	bool SetTriangle(Triangle& t);

	//Setter
	void ResetTriangles();

	//Getter
	XMFLOAT3 GetPosision() { return position_; };
	std::vector<Triangle*>& GetTriangles() { return triangles_; };

	//���������ꍇ�̍ŏ�������Ԃ�
	bool SegmentVsTriangle(RayCastData* _data, float& minDist);

};