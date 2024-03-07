#include "Triangle.h"
#include "../Engine/Direct3D.h"
#include "../Engine/BoxCollider.h"
#include "../Engine/SphereCollider.h"
#include "../Engine/GameObject.h"

void Triangle::CreatTriangle(XMVECTOR& p1, XMVECTOR& p2, XMVECTOR& p3)
{
    position_[0] = p1;
    position_[1] = p2;
    position_[2] = p3;
}

XMFLOAT3* Triangle::GetPosition()
{
    XMFLOAT3 pos[3];
    for (int i = 0; i < 3; i++) XMStoreFloat3(&pos[i], position_[i]);

    return pos;
}

void Triangle::RayCast(RayCastData* data)
{
    XMFLOAT3 pos[3];
    for (int i = 0; i < 3; i++) XMStoreFloat3(&pos[i], position_[i]);

    BOOL  hit = FALSE;
    float dist = 0.0f;
	hit = Direct3D::Intersect(data->start, data->dir, pos[0], pos[1], pos[2], &dist);

	if (hit && dist < data->dist)
	{
		data->hit = TRUE;
		data->dist = dist;
	}
}