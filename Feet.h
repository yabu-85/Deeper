#pragma once
#include "EnemyBase.h"

class NavigationAI;

//あし：NavigationとかAI開発用の敵もぶ
class Feet : public EnemyBase
{
    int hModel_;
    NavigationAI* pNavigationAI_;

    XMFLOAT3 targetPos_;
    int stopTime_;
    int currtime_;
    bool flag_;

public:
    Feet(GameObject* parent);
    ~Feet() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void ApplyDamage(int da) override;

};

