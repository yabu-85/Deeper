#pragma once
#include "Engine/GameObject.h"
#include "EnemySpawnCtrl.h"

class EnemyUi;
class DropTable;
class StateManager;

class EnemyBase : public GameObject
{
public:
    enum class State {
        APPEAR = 0, //出現する
        IDLE,       //動かない
        PATROL,     //見回る
        COMBAT,     //戦闘状態
        DEAD,       //です
    };

protected:
    int hp_;
    int maxHp_;
    float moveSpeed_;   		//移動スピード
    float moveRange_;   		//移動終了判定の範囲
    float aimTargetPos_;        //AimのTargetする時の場所
    float rotateRatio_;         //回転率
    XMFLOAT3 moveTargetPos_;    //移動
    XMFLOAT3 prePos_;
    ENEMY_TYPE type_;
    EnemyUi* pEnemyUi_;
    State state_;

public:
    EnemyBase(GameObject* parent);
    virtual ~EnemyBase() = 0 {};
    virtual void Initialize() override = 0 {};
    virtual void Update() override = 0 {};
    virtual void Draw() override = 0 {};
    virtual void Release() override = 0 {};

    virtual bool Move();    //移動したらTrue（Target位置についてない
    virtual void Rotate();
    virtual void ApplyDamage(int da);
    void SetEnemyType(ENEMY_TYPE type) { type_ = type; }
    XMFLOAT3 GetTargetPos() { return moveTargetPos_; }
    float GetAimTargetPos() { return aimTargetPos_; }

    State IsState() { return state_; }
    void SetState(State state) { state_ = state; }
};

