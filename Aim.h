#pragma once
#include "Engine/GameObject.h"

class Player;
class EnemyBase;
class CollisionMap;

class Aim : public GameObject
{
    float mouseSensitivity;         //�}�E�X���x
    float perspectiveDistance_;     //�ǂ̂��炢��납��f����
    float heightDistance_;          //�œ_�̍���
    float upMouselimit_;            //�}�E�X�ړ��̐����l�i�㉺
    float donwMouselimit_;          //�}�E�X�ړ��̐����l�i���E
    float mouseSpeed_;              //�}�E�X�ړ��X�s�[�h
    float defPerspectDistance_;     //�f�t�H���g�̎��_�̋���
    float heightRay_;               //Ray�̒l�Ƀv���X�����
    float numSupress_;              //�}�E�X�ړ���Offset�̒l��߂���
    float maxCameraOffset_;         //cameraOffset�̍ő勗��
    float moveAimTime_;             //�������̗}���̒l
    float stopAimTime_;             //�~�܂鎞�̗}���̒l
    float targetRange_;             //�^�[�Q�b�g�̗L���͈�
    float fovRadian_;               //Target�͈̔�
    float rotateRatio_;             //Target���̉�]��
    bool aimMove_;                  //�G�C���𓮂������ǂ���
    bool isTarget_;                 //�^�[�Q�b�g��Ԃ�

    XMFLOAT3 cameraTarget_;         //�J�����̏œ_�ڕW
    XMFLOAT3 cameraPos_;            //�J�����̏ꏊ�ڕW
    XMFLOAT3 aimDirection_;         //���݂̎��_�Ɋ�Â����i�s�����x�N�g��
    XMFLOAT3 plaPos_;               //�v���C���[�ʒu
    XMFLOAT3 cameraOffset_;         //�J�����̈ړ���

    Player* pPlayer_;
    EnemyBase* pEnemyBase_;
    CollisionMap* pCollisionMap_;

    void FacingTarget();
    void CalcCameraOffset(float _aimMove);
    void RayCastStage(XMFLOAT3 _start);

public:
    Aim(GameObject* parent);
    ~Aim();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //�i�s�����x�N�g����Aim�����擾
    XMFLOAT3 GetAimDirection() { return aimDirection_; }
    void SetAimMove(bool b) { aimMove_ = b; };
    XMFLOAT3 GetTargetPos();

    float GetMouseSensitivity() { return mouseSensitivity; };
    void SetMouseSensitivity(float f) { mouseSensitivity = f; };

    //Target�̍X�V
    void SetTargetEnemy();
    bool IsTarget() { return isTarget_; };

    EnemyBase* GetTargetEnemy() { return pEnemyBase_; }

};