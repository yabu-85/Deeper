#include "Triangle.h"

void Triangle::SetPosition(XMFLOAT3* pos)
{
	position_[0] = pos[0];
	position_[1] = pos[1];
	position_[2] = pos[2];

}

void Triangle::CreatTriangle(XMVECTOR& p1, XMVECTOR& p2, XMVECTOR& p3)
{
	XMStoreFloat3(&position_[0], p1);
	XMStoreFloat3(&position_[1], p2);
	XMStoreFloat3(&position_[2], p3);

}