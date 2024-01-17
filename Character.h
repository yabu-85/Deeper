#pragma once
#include "Engine/GameObject.h"

class Character : public GameObject
{
protected:
    float bodyRadius_;  //�߂荞�ݔ��˂̔��a
    float bodyWeight_;  //�߂荞�ݎ��Ɏg���d��

    XMVECTOR movement_; //���̈ړ���

public:
    Character(GameObject* parent);
    virtual ~Character() {};
    virtual void Initialize() override = 0 {};
    virtual void Update() override = 0 {};
    virtual void Draw() override = 0 {};
    virtual void Release() override = 0 {};

    XMVECTOR GetMovement() { return movement_; }

    //�L�����N�^�[���m�̏Փ˂̍ۂ߂荞�݂������
    void ReflectCharacter(Character* pCharacter);

};

