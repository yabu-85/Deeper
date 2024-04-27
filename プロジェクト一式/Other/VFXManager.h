#pragma once
#include <DirectXMath.h>
using namespace DirectX;

enum VFX_TYPE {
	Explode = 0,
	EnemySpawn,
	Smoke,
	SwordSlash,
	Recovery,

};

namespace VFXManager
{
	//必ず一回は呼ぶVFXデータの初期化
	void Initialize();

	//Typeで指定して生成 other＝SwordSlahsとかで使う
	void CreateVfx(XMFLOAT3 pos, VFX_TYPE type, XMFLOAT3 other = XMFLOAT3());

	//直接生成
	void CreateVfxExplode1(XMFLOAT3 pos);
	void CreateVfxEnemySpawn(XMFLOAT3 pos);
	void CreateVfxSmoke(XMFLOAT3 pos);
	void CreateVfxSwordSlash(XMFLOAT3 pos, XMFLOAT3 dir);
	void CreateVfxRecovery(XMFLOAT3 pos);
};

