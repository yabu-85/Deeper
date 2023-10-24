#pragma once
#include "Engine/GameObject.h"

class Player;
class EnemyBase;
class CollisionMap;

class Aim :
    public GameObject
{
    int hPict_;                     //画像番号
    float mouseSensitivity;         //マウス感度
    float perspectiveDistance_;     //どのくらい後ろから映すか
    float heightDistance_;          //焦点の高さ
    bool aimMove_;                  //エイムを動かすかどうか
    bool isTarget_;                 //ターゲット状態か

    XMFLOAT3 cameraTarget_;         //カメラの焦点目標
    XMFLOAT3 cameraPos_;            //カメラの場所目標
    XMFLOAT3 aimDirection_;         //現在の視点に基づいた進行方向ベクトル
    XMFLOAT3 plaPos_;               //プレイヤー位置
    XMFLOAT3 cameraOffset_;         //カメラの移動量

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

    //進行方向ベクトルのAim情報を取得
    XMFLOAT3 GetAimDirection() { return aimDirection_; }
    void SetAimMove(bool b) { aimMove_ = b; };

    float GetMouseSensitivity() { return mouseSensitivity; };
    void SetMouseSensitivity(float f) { mouseSensitivity = f; };
    void SetTargetEnemy();

};