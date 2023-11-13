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
        APPEAR = 0, //�o������
        IDLE,       //�����Ȃ�
        PATROL,     //�����
        COMBAT,     //�퓬���
        DEAD,       //�ł�
    };

protected:
    int hp_;
    int maxHp_;
    float moveSpeed_;   		//�ړ��X�s�[�h
    float moveRange_;   		//�ړ��I������͈̔�
    float aimTargetPos_;        //Aim��Target���鎞�̏ꏊ
    float rotateRatio_;         //��]��
    XMFLOAT3 moveTargetPos_;    //�ړ�
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

    virtual bool Move();    //�ړ�������True�iTarget�ʒu�ɂ��ĂȂ�
    virtual void Rotate();
    virtual void ApplyDamage(int da);
    void SetEnemyType(ENEMY_TYPE type) { type_ = type; }
    XMFLOAT3 GetTargetPos() { return moveTargetPos_; }
    float GetAimTargetPos() { return aimTargetPos_; }

    State IsState() { return state_; }
    void SetState(State state) { state_ = state; }
};

