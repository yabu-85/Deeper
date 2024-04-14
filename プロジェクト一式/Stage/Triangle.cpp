#include "Triangle.h"
#include "../Engine/Direct3D.h"
#include "../Engine/BoxCollider.h"
#include "../Engine/SphereCollider.h"
#include "../Engine/GameObject.h"

void Triangle::CreatTriangle(XMVECTOR& p1, XMVECTOR& p2, XMVECTOR& p3)
{
	XMStoreFloat3(&position_.pos1, p1);
	XMStoreFloat3(&position_.pos2, p2);
	XMStoreFloat3(&position_.pos3, p3);

}

Polygons Triangle::GetAllPosition()
{
    return position_;
}

void Triangle::RayCast(RayCastData* data)
{
    BOOL  hit = FALSE;
    float dist = 0.0f;
	hit = Direct3D::Intersect(data->start, data->dir, position_.pos1, position_.pos2, position_.pos3, &dist);

	if (hit && dist < data->dist)
	{
		data->hit = TRUE;
		data->dist = dist;
	}
}