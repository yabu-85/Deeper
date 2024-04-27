#pragma once
#include <vector>
class OneFrame;
class GameObject;

//Animation Notify 
//UE�̋@�\
//�A�j���[�V�����ɒʒm���o���t���[�������߂�
//�ʒm�͈͓��Ȃ�o�^����Ă���ʒm�̏���������

//Animation Notify = �P�t���[������
//Animation Notify State = x �` y�̊�

//Play Sound
//Effect
//�I���W�i��

//Notify���e�iGameObject���j���擾������@
//

struct AnimData {
	int startFrame;
	int endFrame;
	std::vector<OneFrame*> frameList;
};

class AnimationController
{
	int modelNum_;			//�S�����Ă郂�f���ԍ�
	int currentAnim_;		//���Đ����Ă���Animation��Id
	float currentBlend_;	//����Blend�̋���
	float blendDecrease_;	//currentBlend��1�t���[���ŉ�����l
	GameObject* pGameObject_;			//�e
	std::vector<AnimData> animDatas_;	//�S�Ẵf�[�^��ۑ�

public:
	AnimationController(int number, GameObject* obj);
	~AnimationController();
	void Update();

	//�A�j���[�V�����f�[�^��ǉ�
	void AddAnim(int s, int e);
	
	//�ʒm�ǉ�
	void AddAnimNotify(int animId, OneFrame* action);

	//�A�j���[�V�����f�[�^�̎擾
	AnimData GetAnim(int id);

	//�A�j���[�V�����̍��v���Ԃ��擾
	int GetAnimTime(int id);

	//���̃A�j���[�V������ݒ�iBlendFactor���蓮�Őݒ�
	void SetNextAnim(int id, float blendFactor, float decrease);

	//���̃A�j���[�V������ݒ�iBlendFactor�������Őݒ�
	void SetNextAnim(int id, float decrease);
};
