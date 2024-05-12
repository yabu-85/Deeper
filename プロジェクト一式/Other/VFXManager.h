#pragma once
#include <DirectXMath.h>
using namespace DirectX;

enum VFX_TYPE {
	Explode = 0,
	EnemySpawn,
	Smoke,
	SwordSlash,
	Recovery,
	BossSowrd,
};

namespace VFXManager
{
	//�K�����͌Ă�VFX�f�[�^�̏�����
	void Initialize();

	//Type�Ŏw�肵�Đ��� other��SwordSlahs�Ƃ��Ŏg��
	void CreateVfx(XMFLOAT3 pos, VFX_TYPE type, XMFLOAT3 other = XMFLOAT3());

	//���ڐ���
	void CreateVfxExplode1(XMFLOAT3 pos);
	void CreateVfxEnemySpawn(XMFLOAT3 pos);
	void CreateVfxSmoke(XMFLOAT3 pos);
	void CreateVfxSwordSlash(XMFLOAT3 pos, XMFLOAT3 dir);
	void CreateVfxRecovery(XMFLOAT3 pos);
	void CreateVfxBossSword(XMFLOAT3 pos, XMFLOAT3 dir);
};

