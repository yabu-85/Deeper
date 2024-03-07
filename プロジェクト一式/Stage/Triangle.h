#pragma once
#include <DirectXMath.h>
#include "../Engine/Fbx.h"

using namespace DirectX;
class BoxCollider;
class SphereCollider;

class Triangle
{
	//ÇRí∏ì_ÇÃç¿ïW
	XMVECTOR position_[3];
public:
	void CreatTriangle(XMVECTOR& p1, XMVECTOR& p2, XMVECTOR& p3);
	
	XMFLOAT3* GetPosition();

	void RayCast(RayCastData* data);
};

