#include "Triangle.h"
#include "Engine/Direct3D.h"
#include "Engine/BoxCollider.h"
#include "Engine/SphereCollider.h"
#include "Engine/GameObject.h"

void Triangle::CreatTriangle(XMVECTOR& p1, XMVECTOR& p2, XMVECTOR& p3)
{
    position_[0] = p1;
    position_[1] = p2;
    position_[2] = p3;

	//�@���x�N�g��
	normal_ = XMVector3Cross(p2 - p1, p3 - p2);
    normal_ = XMVector3Normalize(normal_);
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

// ���ƎO�p�`�̓����蔻��
bool Triangle::TestSphereTriangle(SphereCollider* collid)
{
    XMFLOAT3 fObjCenter = collid->GetGameObject()->GetPosition();
    XMFLOAT3 fCollidCenter = collid->GetCenter();

    XMVECTOR sphereCenter = XMLoadFloat3(&fObjCenter) + XMLoadFloat3(&fCollidCenter);
    float sphereRadius = collid->GetSize().x;

    //�O�p�`���܂ޕ��ʂƋ��̒��S�Ƃ̋������v�Z
    float distanceToPlane = XMVectorGetX(XMVector3Dot(normal_,  - position_[0]));

    //���������a���傫���ꍇ�A������Ɣ���
    if (fabs(distanceToPlane) > sphereRadius)
    {
        return false;
    }

    //���̒��S���O�p�`�̕��ʂɉ��낵�������v�Z
    XMVECTOR footPoint = sphereCenter - distanceToPlane * normal_;

    //�����O�p�`�̓����ɂ��邩�ǂ����𔻒�
    float u, v, w;
    XMVECTOR edge0 = position_[1] - position_[0];
    XMVECTOR edge1 = position_[2] - position_[0];
    XMVECTOR edge2 = footPoint - position_[0];

    float dot00 = XMVectorGetX(XMVector3Dot(edge0, edge0));
    float dot01 = XMVectorGetX(XMVector3Dot(edge0, edge1));
    float dot02 = XMVectorGetX(XMVector3Dot(edge0, edge2));
    float dot11 = XMVectorGetX(XMVector3Dot(edge1, edge1));
    float dot12 = XMVectorGetX(XMVector3Dot(edge1, edge2));

    float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
    v = (dot11 * dot02 - dot01 * dot12) * invDenom;
    w = (dot00 * dot12 - dot01 * dot02) * invDenom;
    u = 1.0f - v - w;

    //0.0��u, 0.0��v, u+v��1.0 �̏ꍇ�A�����Ɣ���
    if (u >= 0.0f && v >= 0.0f && (u + v) <= 1.0f)
    {
        //penetration �����O�p�`�ɂ߂荞��ł��鋗�������
        XMVECTOR penetration = distanceToPlane * normal_;

        //�߂荞�݃x�N�g����p���ċ���߂�
        XMVECTOR direction = XMVector3Normalize(penetration);
        XMVECTOR correction = direction * (sphereRadius - XMVectorGetX(XMVector3Length(penetration)));
        sphereCenter += correction;

        XMFLOAT3 newPos;
        XMStoreFloat3(&newPos, sphereCenter - XMLoadFloat3(&fCollidCenter));
        collid->GetGameObject()->SetPosition(newPos);

        return true;
    }

    return false;
}