#pragma once
#include <DirectXMath.h>
#include "Engine/Fbx.h"

using namespace DirectX;
class BoxCollider;
class SphereCollider;

class Triangle
{
	//�R���_�̍��W
	XMVECTOR position_[3];
	
	//�@���x�N�g��
	XMVECTOR normal_;

public:
	void CreatTriangle(XMVECTOR& p1, XMVECTOR& p2, XMVECTOR& p3);
	XMFLOAT3* GetPosition();

	void RayCast(RayCastData* data);
	bool TestSphereTriangle(SphereCollider* collider);

};

