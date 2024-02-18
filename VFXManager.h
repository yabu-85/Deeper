#pragma once
#include <DirectXMath.h>
using namespace DirectX;

namespace VFXManager
{
	void Initialize();

	void CreatVfxExplode1(XMFLOAT3 &pos);
	void CreatVfxEnemySpawn(XMFLOAT3& pos, int lifeTime);
	void CreatVfxSmoke(XMFLOAT3& pos);

};

