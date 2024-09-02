#pragma once
#include <DirectXMath.h>
#include "../Engine/Fbx.h"

using namespace DirectX;
class BoxCollider;
class SphereCollider;

struct Polygons {
	XMFLOAT3 pos1, pos2, pos3;
};

/// <summary>
/// éOäpå`ÇÃèÓïÒ
/// </summary>
class Triangle
{
	Polygons position_;
public:
	void CreatTriangle(XMVECTOR& p1, XMVECTOR& p2, XMVECTOR& p3);
	
	Polygons GetAllPosition();

	void RayCast(RayCastData* data);
};

