#pragma once
#include "../Other/VFXManager.h"
#include "../Other/AudioManager.h"
#include <vector>
class GameObject;

class OneFrame {
protected:
    int time;
public:
    OneFrame(int t) : time(t) {};
    virtual void CalcFrame(int t, GameObject* obj = nullptr);
    virtual void OnFrame(GameObject* obj) {};
};

class DoubleFrame : public OneFrame {
    int endTime;
public:
    DoubleFrame(int first, int end) : OneFrame(first), endTime(end) {};
    void CalcFrame(int t, GameObject* obj = nullptr) override;
    virtual void OnEnter(GameObject* obj) {};
    virtual void OnExit(GameObject* obj) {};
};

class CreatFrame : public OneFrame {
    VFX_TYPE type;
public:
    CreatFrame(int t, VFX_TYPE _type) : OneFrame(t), type(_type) {};
    void CalcFrame(int t, GameObject* obj = nullptr) override;
};

class PlaySoundFrame : public OneFrame{
    VFX_TYPE type;
public:
    PlaySoundFrame(int t, VFX_TYPE _type) : OneFrame(t), type(_type) {};
    void CalcFrame(int t, GameObject* obj = nullptr) override;
};
