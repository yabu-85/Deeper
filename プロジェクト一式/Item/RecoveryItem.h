#pragma once
#include "../Engine/GameObject.h"

/// <summary>
/// �񕜂���A�C�e��
/// �G��|���ƃh���b�v���āA�E���Ɖ񕜂���
/// </summary>
class RecoveryItem : public GameObject
{
    int hModel_;            //���f��
    int recoveryAmount_;    //�񕜗�
    float gravity_;         //�d��
public:
    RecoveryItem(GameObject* parent);
    ~RecoveryItem();
    void Initialize();
    void Update();
    void Draw();
    void Release();
    void OnCollision(GameObject* pTarget) override;
};

