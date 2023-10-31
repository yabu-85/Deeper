#include "DamageCtrl.h"
#include <vector>
#include "EnemySpawnCtrl.h"
#include "EnemyBase.h"

DamageCtrl::DamageCtrl(EnemySpawnCtrl* p)
	: pEnemySpawnCtrl_(p)
{
}

DamageCtrl::~DamageCtrl()
{
}

void DamageCtrl::ApplyDamage(DAMAGE_TYPE type, int d)
{
	std::vector<EnemyBase*> enemyList = pEnemySpawnCtrl_->GetAllEnemy();
	if (enemyList.empty()) return;

	if (type == DAMAGE_TYPE::ALL) {
		for (EnemyBase* e : enemyList) {
			e->ApplyDamage(d);
		}
	}
	else if (type == DAMAGE_TYPE::RAND) {
		int randomIndex = rand() % (int)enemyList.size();
		enemyList.at(randomIndex)->ApplyDamage(d);
	}

}

int DamageCtrl::CalcSword(XMFLOAT3 start, XMFLOAT3 vec, float range)
{
    std::vector<EnemyBase*> enemyList = pEnemySpawnCtrl_->GetAllEnemy();

    //�����i���C�j�Ɖ~�̏Փ˔���p�N�����F���܂��ɏՓˊJ�n�n�_�ƏI���n�_�Ƃ��
    for (int i = 0; i < enemyList.size(); i++) {
        XMFLOAT3 p = enemyList.at(i)->GetPosition(); //���S�_�̍��W
        XMFLOAT3 q1, q2; //�ՓˊJ�n�n�_�A�I���n�_(�߂�l)
        float r = 3.0f;

        p.x = p.x - start.x;
        p.y = p.y - start.y;
        p.z = p.z - start.z;

        float A = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
        float B = vec.x * p.x + vec.y * p.y + vec.z * p.z;
        float C = p.x * p.x + p.y * p.y + p.z * p.z - r * r;

        if (A == 0.0f) continue;  // false ���C�̒�����0

        float s = B * B - A * C;
        if (s < 0.0f) continue; // false �Փ˂��Ă��Ȃ�

        s = sqrtf(s);
        float a1 = (B - s) / A;
        float a2 = (B + s) / A;

        if (a1 < 0.0f || a2 < 0.0f) continue; // false ���C�̔��΂ŏՓ�

        //�������Ȃ��Ă�����
        q1.x = start.x + a1 * vec.x;
        q1.y = start.y + a1 * vec.y;
        q1.z = start.z + a1 * vec.z;
        q2.x = start.x + a2 * vec.x;
        q2.y = start.y + a2 * vec.y;
        q2.z = start.z + a2 * vec.z;

        // �x�N�g���̍������v�Z
        XMVECTOR difference = XMVectorSubtract(XMLoadFloat3(&start), XMLoadFloat3(&q1));
        // �x�N�g���̒����i�����j���v�Z
        XMVECTOR distance = XMVector3Length(difference);
        // ���ʂ� float �ɕϊ����ĕԂ�
        float distanceFloat;
        XMStoreFloat(&distanceFloat, distance);
 
        //range�͈̔͊O�������ꍇ��
        if (range < distanceFloat) continue;

        enemyList.at(i)->ApplyDamage(1);
        return i; //true

    }
    
    return -1;
}
