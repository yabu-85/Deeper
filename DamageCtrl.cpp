#include "DamageCtrl.h"
#include "EnemySpawnCtrl.h"
#include "EnemyBase.h"
#include <vector>
#include <list>

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

    //BoxCollider* bCol = new BoxCollider(XMFLOAT3(0, 1, 0), XMFLOAT3(1.0f, 1.0f, 1.0f));

    //線分（レイ）と円の衝突判定パクった：おまけに衝突開始地点と終了地点とれる
    for (int i = 0; i < enemyList.size(); i++) {
        
        std::list<Collider*> col = enemyList.at(i)->GetColliderList();
    //    bool hit = col.front()->IsHit(bCol);  bColのGameObjectのポインタを設定していないからエラーが出るだけで、引数に入れてしまえば行ける

        XMFLOAT3 p = enemyList.at(i)->GetPosition(); //中心点の座標
        p.y += 1.0f;
        float r = 1.9f;

        //これStartがPの半径内に入っていたら無条件で当たってるとみなす
        double distSquared = pow(p.x - start.x, 2) + pow(p.y - start.y, 2) + pow(p.z - start.z, 2);
        double radSquared = pow(r, 2);

        if (distSquared > radSquared) {
            p.x = p.x - start.x;
            p.y = p.y - start.y;
            p.z = p.z - start.z;

            float A = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
            float B = vec.x * p.x + vec.y * p.y + vec.z * p.z;
            float C = p.x * p.x + p.y * p.y + p.z * p.z - r * r;

            if (A == 0.0f)
                continue;  // false レイの長さが0

            float s = B * B - A * C;
            if (s < 0.0f) continue; // false 衝突していない

            s = sqrtf(s);
            float a1 = (B - s) / A;
            float a2 = (B + s) / A;

            if (a1 < 0.0f || a2 < 0.0f) continue; // false レイの反対で衝突

            XMFLOAT3 q1; // , q2; //衝突開始地点、終了地点(戻り値)
            q1.x = start.x + a1 * vec.x;
            q1.y = start.y + a1 * vec.y;
            q1.z = start.z + a1 * vec.z;
            //q2.x = start.x + a2 * vec.x;
            //q2.y = start.y + a2 * vec.y;
            //q2.z = start.z + a2 * vec.z;

            // ベクトルの差分を計算
            XMVECTOR difference = XMVectorSubtract(XMLoadFloat3(&start), XMLoadFloat3(&q1));
            // ベクトルの長さ（距離）を計算
            XMVECTOR distance = XMVector3Length(difference);
            // 結果を float に変換して返す
            float distanceFloat;
            XMStoreFloat(&distanceFloat, distance);

            //rangeの範囲外だった場合次
            if (range < distanceFloat) continue;
        }

        enemyList.at(i)->ApplyDamage(1);
      //  delete bCol;
        return i; //true

    }
    
 //   delete bCol;
    return -1;
}
