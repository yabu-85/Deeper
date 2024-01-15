#pragma once
#include "Engine/GameObject.h"
#include "EnemyBase.h"

class Player;
class CollisionMap;

class Aim : public GameObject
{
    float perspectiveDistance_;     //�ǂ̂��炢��납��f����
    float defPerspectDistance_;     //�f�t�H���g�̎��_�̋���
    float mouseSensitivity;         //�}�E�X���x

    bool isMove_;                   //�G�C���𓮂������ǂ���
    bool isCompulsion_;             //�����I�Ɉړ��������Ԃ�
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

    void SetCompulsion(bool b) { isCompulsion_ = b; }
    bool IsCompulsion() { return isCompulsion_; }
    void SetAimMove(bool b) { isMove_ = b; };
    bool IsAimMove() { return isMove_; }
    float GetMouseSensitivity() { return mouseSensitivity; };
    void SetMouseSensitivity(float f) { mouseSensitivity = f; };

    //�i�s�����x�N�g����Aim�����擾
    XMFLOAT3 GetAimDirection() { return aimDirection_; }
    XMFLOAT3 GetTargetPos() { return isTarget_ ? pEnemyBase_->GetPosition() : XMFLOAT3(); }

    //Target�̍X�V
    void SetTargetEnemy();
    bool IsTarget() { return isTarget_; };
    EnemyBase* GetTargetEnemy() { return pEnemyBase_; }

    //���W���w�肵�ă|�W�V�����Əœ_��ݒ肷��
    void SetCompulsionPosition(XMFLOAT3 pos);
    void SetCompulsionAimTarget(XMFLOAT3 pos);
    void SetCompulsionAimPositionAndTarget(XMFLOAT3 pos, XMFLOAT3 target);

};