#pragma once
#include <vector>
#include <string>

class GameObject;
enum AUDIO_TYPE;
enum VFX_TYPE;

/// <summary>
/// �A�j���[�V�������w�肵�����ԂɂȂ�����ʒm�����N���X
/// �P�t���[���̂݁i���Ԃ��P�O�ɂȂ�����Ă΂��
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
/// �A�j���[�V�������w�肵���Ԃ̎��ԂɂȂ�����ʒm�����N���X
/// �w�肵�����Ԃ̊ԁi�O�`�P�O�j
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
/// �A�j���[�V�������w�肵�����ԂɂȂ�����VFX�����������
/// </summary>
class CreatFrame : public OneFrame {
protected:
    VFX_TYPE type;
public:
    CreatFrame(int t, VFX_TYPE _type) : OneFrame(t), type(_type) {};
    void CalcFrame(int t, GameObject* obj = nullptr) override;
};

/// <summary>
/// �A�j���[�V�������w�肵�����ԂɂȂ�����w�肵���{�[���̈ʒu����VFX�����������
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
/// �A�j���[�V�������w�肵�����ԂɂȂ�����Sound���Đ������
/// </summary>
class PlaySoundFrame : public OneFrame{
    AUDIO_TYPE type;
public:
    PlaySoundFrame(int t, AUDIO_TYPE _type) : OneFrame(t), type(_type) {};
    void CalcFrame(int t, GameObject* obj = nullptr) override;
};
