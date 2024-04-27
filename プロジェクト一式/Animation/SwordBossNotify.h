#pragma once
#include "AnimationNotify.h"

class SowrdTestNotify : public DoubleFrame {
public:
    SowrdTestNotify(int first, int end) : DoubleFrame(first, end) {};
    void OnFrame(GameObject* obj) override;
    void OnEnter(GameObject* obj) override {};
    void OnExit(GameObject* obj) override {};
};

class SowrdRotateNotify : public DoubleFrame {
public:
    SowrdRotateNotify(int first, int end) : DoubleFrame(first, end) {};
    void OnFrame(GameObject* obj) override;
};