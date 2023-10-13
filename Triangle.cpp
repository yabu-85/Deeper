#include "Triangle.h"

void Triangle::CreatTriangle(XMVECTOR& p1, XMVECTOR& p2, XMVECTOR& p3)
{
	XMStoreFloat3(&position_[0], p1);
	XMStoreFloat3(&position_[1], p2);
	XMStoreFloat3(&position_[2], p3);

}

XMFLOAT3* Triangle::GetPosition()
{
	return position_;
}