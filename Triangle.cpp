#include "Triangle.h"
#include "Engine/Direct3D.h"

void Triangle::SetPosition(XMFLOAT3* pos)
{
	position_[0] = pos[0];
	position_[1] = pos[1];
	position_[2] = pos[2];

}

void Triangle::RayCast(RayCastData* data, Triangle& tri)
{
	//3’¸“_
	XMFLOAT3 ver[3];
	ver[0] = tri.position_[0];
	ver[1] = tri.position_[1];
	ver[2] = tri.position_[2];

	BOOL  hit = FALSE;
	float dist = 0.0f;

	hit = Direct3D::Intersect(data->start, data->dir, ver[0], ver[1], ver[2], &dist);


	if (hit && dist < data->dist)
	{
		data->hit = TRUE;
		data->dist = dist;
	}
}

void Triangle::CreatTriangle(XMVECTOR& p1, XMVECTOR& p2, XMVECTOR& p3)
{
	XMStoreFloat3(&position_[0], p1);
	XMStoreFloat3(&position_[1], p2);
	XMStoreFloat3(&position_[2], p3);

}