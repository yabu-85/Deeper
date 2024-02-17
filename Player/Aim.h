#pragma once
#include "../Engine/GameObject.h"
#include "../Enemy/EnemyBase.h"

class Player;
class CollisionMap;

class Aim : public GameObject
{
    bool isMove_;                   //�G�C���𓮂������ǂ���
    bool isCompulsion_;             //�����I�Ɉړ��������Ԃ�
    bool isTarget_;                 //�^�[�Q�b�g��Ԃ�
    bool isTargetChange_;           //Mouse�̈ړ��Ń^�[�Q�b�g��ς���̂Ɏg���t���O
    
    int hPict_;
    int targetChangeTime_;          //Mouse�̈ړ��Ń^�[�Q�b�g��ς���N�[���_�E��
    int compulsionTime_;            //��������߂�̂Ɋ|���鎞��

    int iterations_;            //������
    float sign_;                //�v�Z�pCameraShake
    float shakeSpeed_;          //Shake���鑬��
    float range_;               //Shake���鋭���i�����j
    float moveDistance_;        //�v�Z�prange�̒l������
    float distanceDecrease_;    //�������Ƃ�speed�����n
    float rangeDecrease_;       //�������Ƃ�range�����n
    XMVECTOR center_;
    XMVECTOR shakeDirection_;

    float perspectiveDistance_;     //�ǂ̂��炢��납��f����
    float defPerspectDistance_;     //�f�t�H���g�̎��_�̋���
    float mouseSensitivity;         //�}�E�X���x

    XMFLOAT3 cameraTarget_;         //�J�����̏œ_�ڕW
    XMFLOAT3 cameraPosition_;       //�J�����̏ꏊ�ڕW
    XMFLOAT3 compulsionTarget_;     //�������̃J�����̏œ_�ڕW
    XMFLOAT3 compulsionPosisiton_;  //�������̃J�����̏ꏊ�ڕW
    XMFLOAT3 aimDirection_;         //���݂̎��_�Ɋ�Â����i�s�����x�N�g��
    XMFLOAT3 cameraOffset_;         //�J�����̈ړ���

    Player* pPlayer_;
    EnemyBase* pEnemyBase_;
    CollisionMap* pCollisionMap_;

    void DefaultAim();      //�ʏ��Aim�̌v�Z
    void Compulsion();      //�����ړ����̌v�Z
    void BackCompulsion();  //�����ړ�����ʏ�Ɉڂ�Ƃ��̌v�Z

    void FacingTarget();
    void ChangeTarget(XMFLOAT3 mouse);
    void CalcCameraOffset(float _aimMove);
    void RayCastStage();
    void CalcMouseMove();
    XMVECTOR CalcDirection(float x, float y);

    void CameraShake();

public:
    Aim(GameObject* parent);
    ~Aim();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void SetAimMove(bool b) { isMove_ = b; };
    bool IsAimMove() { return isMove_; }
    float GetMouseSensitivity() { return mouseSensitivity; };
    void SetMouseSensitivity(float f) { mouseSensitivity = f; };

    //�i�s�����x�N�g����Aim�����擾
    XMFLOAT3 GetAimDirection() { return aimDirection_; }

    //���񂾓G�̃|�C���^��n����Target��������̊֐�
    void TargetIsDead(EnemyBase* target);       
    //Target���X�V����
    void SetTargetEnemy();

    EnemyBase* GetTargetEnemy() { return pEnemyBase_; }
    bool IsTarget() { return isTarget_; };
    XMFLOAT3 GetTargetPos() { return isTarget_ ? pEnemyBase_->GetPosition() : XMFLOAT3(); }

    //���W���w�肵�ă|�W�V�����Əœ_��ݒ肷��
    void SetCompulsion(XMFLOAT3 pos, XMFLOAT3 tar);
    bool IsCompulsion() { return isCompulsion_; }

    //�����񐔁A�������鋗���A�X�s�[�h�̌����n�A�X�s�[�h
    void SetCameraShake(int iterat, float range, float range_decrease, float speed, float speed_decrease);
    void SetCameraShakeDirection(XMVECTOR v) { shakeDirection_ = v; }

};