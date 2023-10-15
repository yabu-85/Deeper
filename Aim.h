#pragma once
#include "Engine/GameObject.h"

class Player;

class Aim :
    public GameObject
{
    int hPict_;                     //�摜�ԍ�
    float mouseSensitivity;         //�}�E�X���x
    float perspectiveDistance_;     //�ǂ̂��炢��납��f����
    float heightDistance_;          //�œ_�̍���
    bool aimMove_;                  //�G�C���𓮂������ǂ���
    bool isTarget_;                 //�^�[�Q�b�g��Ԃ�

    XMFLOAT3 cameraTarget_;         //�J�����̏œ_�ڕW
    XMFLOAT3 cameraPos_;            //�J�����̏ꏊ�ڕW
    XMFLOAT3 aimDirection_;         //���݂̎��_�Ɋ�Â����i�s�����x�N�g��
    XMFLOAT3 plaPos_;               //�v���C���[�ʒu
    XMFLOAT3 cameraOffset_;         //�J�����̈ړ���

    Player* pPlayer_;

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

    float GetMouseSensitivity() { return mouseSensitivity; };
    void SetMouseSensitivity(float f) { mouseSensitivity = f; };

};