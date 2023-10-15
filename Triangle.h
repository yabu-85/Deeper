#pragma once
#include <DirectXMath.h>
#include "Engine/Fbx.h"

using namespace DirectX;

class Triangle
{
	XMFLOAT3 position_[3];

public:
	void CreatTriangle(XMVECTOR& p1, XMVECTOR& p2, XMVECTOR& p3);

	void SetPosition(XMFLOAT3* pos);
	XMFLOAT3* GetPosition() { return position_; };

	void RayCast(RayCastData* data, Triangle& tri);

};

