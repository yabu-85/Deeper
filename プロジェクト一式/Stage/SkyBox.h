#pragma once
#include <string>
#include "../Engine/GameObject.h"

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

