#pragma once
#include "AnimationNotify.h"

class SowrdBossVfxNotify : public DoubleFrame {
public:
    SowrdBossVfxNotify(int first, int end) : DoubleFrame(first, end) {};
    void OnFrame(GameObject* obj) override;
};

class SowrdBossAttackNotify : public DoubleFrame {
public:
    SowrdBossAttackNotify(int first, int end) : DoubleFrame(first, end) {};
    void OnFrame(GameObject* obj) override;
    void OnEnter(GameObject* obj) override;
    void OnExit(GameObject* obj) override;
};

class SowrdBossRotateNotify : public DoubleFrame {
public:
    SowrdBossRotateNotify(int first, int end) : DoubleFrame(first, end) {};
    void OnFrame(GameObject* obj) override;
};