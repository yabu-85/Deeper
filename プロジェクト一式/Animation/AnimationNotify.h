#pragma once
#include <vector>
#include <string>

class GameObject;
enum AUDIO_TYPE;
enum VFX_TYPE;

/// <summary>
/// アニメーションが指定した時間になったら通知されるクラス
/// １フレームのみ（時間が１０になったら呼ばれる
/// </summary>
class OneFrame {
protected:
    int time;
public:
    OneFrame(int t) : time(t) {};
    virtual void CalcFrame(int t, GameObject* obj = nullptr);
    virtual void OnFrame(GameObject* obj) {};
};

/// <summary>
/// アニメーションが指定した間の時間になったら通知されるクラス
/// 指定した時間の間（０～１０）
/// </summary>
class DoubleFrame : public OneFrame {
    int endTime;
public:
    DoubleFrame(int first, int end) : OneFrame(first), endTime(end) {};
    void CalcFrame(int t, GameObject* obj = nullptr) override;
    virtual void OnEnter(GameObject* obj) {};
    virtual void OnExit(GameObject* obj) {};
};

/// <summary>
/// アニメーションが指定した時間になったらVFXが生成される
/// </summary>
class CreatFrame : public OneFrame {
protected:
    VFX_TYPE type;
public:
    CreatFrame(int t, VFX_TYPE _type) : OneFrame(t), type(_type) {};
    void CalcFrame(int t, GameObject* obj = nullptr) override;
};

/// <summary>
/// アニメーションが指定した時間になったら指定したボーンの位置からVFXが生成される
/// </summary>
class CreatFrameBone : public CreatFrame {
    int hModel;
    int boneIndex;
    int partIndex;
public:
    CreatFrameBone(int t, VFX_TYPE _type, int model, std::string name);
    void CalcFrame(int t, GameObject* obj = nullptr) override;
};

/// <summary>
/// アニメーションが指定した時間になったらSoundが再生される
/// </summary>
class PlaySoundFrame : public OneFrame{
    AUDIO_TYPE type;
public:
    PlaySoundFrame(int t, AUDIO_TYPE _type) : OneFrame(t), type(_type) {};
    void CalcFrame(int t, GameObject* obj = nullptr) override;
};
