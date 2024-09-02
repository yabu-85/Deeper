#pragma once
#include <string>
#include "../Engine/GameObject.h"

/// <summary>
/// �X�J�C�{�b�N�X��\�����邽�߂̃N���X
/// </summary>
class SkyBox : public GameObject
{
    int hModel_;

public:
    SkyBox(GameObject* parent);
    ~SkyBox();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    
    void LoadModel(std::string fileName);

};

