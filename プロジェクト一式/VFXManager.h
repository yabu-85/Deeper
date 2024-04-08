#pragma once
#include <DirectXMath.h>
using namespace DirectX;

namespace VFXManager
{
	//必ず一回は呼ぶVFXデータの初期化
	void Initialize();

	void CreatVfxExplode1(XMFLOAT3 pos);
	void CreatVfxEnemySpawn(XMFLOAT3 pos, int lifeTime);
	void CreatVfxSmoke(XMFLOAT3 pos);
	void CreatVfxSwordSlash(XMFLOAT3 pos, XMFLOAT3 dir);

};

