#pragma once
#include "Engine/Camera.h"
#include "Engine/Direct3D.h"
#include "Engine/Transform.h"
#include <vector>
#include "Triangle.h"

class BoxCollider;
class SphereCollider;

class Cell
{
	//�������̍��W
	XMFLOAT3 position_;

	//cube�̈�ӂ̒���
	float length_ = 0;

	//���_�ʒu posision
	XMFLOAT3 verPos_[8];

	//����CELL�ɑ�����O�p�|���S��
	std::vector<Triangle*>Triangles;
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
	std::vector<Triangle*>& GetTriangles() { return Triangles; };

	//���������ꍇ�̍ŏ�������Ԃ�
	bool SegmentVsTriangle(RayCastData* _data, float& minDist);

	void MapDataVsBox(BoxCollider* collider);
	void MapDataVsSphere(SphereCollider* collider, XMFLOAT3 prePos);

};


class CellBox : public GameObject
{
	int hModel_ = -1;

public:
	CellBox(GameObject* parent) : GameObject(parent, "CollisionMap"), hModel_(-1) {};
	~CellBox() {};
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override {};
};