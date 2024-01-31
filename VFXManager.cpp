#include "VFXManager.h"
#include "Engine/VFX.h"

namespace VFXManager
{
	EmitterData sparks;
	EmitterData explode;
	EmitterData enemySpawn;
}

void VFXManager::Initialize()
{
	//âŒÇÃï≤
	sparks.textureFileName = "Particle/cloudA.png";
	sparks.delay = 0;
	sparks.number = 30;
	sparks.lifeTime = 50;
	sparks.position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	sparks.positionRnd = XMFLOAT3(1.0f, 1.0f, 1.0f);
	sparks.direction = XMFLOAT3(1.0f, 1.0f, 1.0f);
	sparks.directionRnd = XMFLOAT3(90.0f, 90.0f, 90.0f);
	sparks.speed = 0.2f;
	sparks.speedRnd = 1.0f;
	sparks.accel = 0.9f;
	sparks.size = XMFLOAT2(0.3f, 0.3f);
	sparks.sizeRnd = XMFLOAT2(0.1f, 0.1f);
	sparks.scale = XMFLOAT2(0.97f, 0.97f);
	sparks.color = XMFLOAT4(1.0f, 1.0f, 0.1f, 1.0f);
	sparks.deltaColor = XMFLOAT4(0.0f, 0.0f, 0.0f, -1.0f / sparks.lifeTime);
	sparks.gravity = 0.001f;

	//îöî≠
	explode.textureFileName = "Particle/cloudA.png";
	explode.delay = 0;
	explode.number = 6;
	explode.lifeTime = 20;
	explode.position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	explode.positionRnd = XMFLOAT3(0.5f, 0.0f, 0.5f);
	explode.direction = XMFLOAT3(1.0f, 1.0f, 1.0f);
	explode.directionRnd = XMFLOAT3(90.0f, 90.0f, 90.0f);
	explode.speed = 0.1f;
	explode.speedRnd = 0.8f;
	explode.size = XMFLOAT2(1.2f, 1.2f);
	explode.sizeRnd = XMFLOAT2(0.4f, 0.4f);
	explode.scale = XMFLOAT2(1.05f, 1.05f);
	explode.color = XMFLOAT4(1.0f, 1.0f, 0.1f, 1.0f);
	explode.deltaColor = XMFLOAT4(0.0f, -1.0f / 20.0f, 0.0f, -1.0f / 20.0f);
	explode.gravity = 0.001f;

	//ìGÇÃÉXÉ|Å[ÉìVFX
	enemySpawn.textureFileName = "Particle/Red.png";
	enemySpawn.delay = 0;
	enemySpawn.number = 1;
	enemySpawn.position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	enemySpawn.positionRnd = XMFLOAT3(0.0f, 0.0f, 0.0f);
	enemySpawn.direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
	enemySpawn.directionRnd = XMFLOAT3(0.0f, 0.0f, 0.0f);
	enemySpawn.speed = 0.0f;
	enemySpawn.speedRnd = 0.0f;
	enemySpawn.accel = 0.0f;
	enemySpawn.size = XMFLOAT2(2.0f, 2.0f);
	enemySpawn.sizeRnd = XMFLOAT2(0.0f, 0.0f);
	enemySpawn.scale = XMFLOAT2(1.0f, 1.0f);
	enemySpawn.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	enemySpawn.rotate = XMFLOAT3(90.0f, 0.0f, 0.0f);
	enemySpawn.rotateRnd = XMFLOAT3(0.0f, 0.0f, 0.0f);
	enemySpawn.spin = XMFLOAT3(0.0f, 10.0f, 0.0f);
	enemySpawn.gravity = 0.0f;
	enemySpawn.isBillBoard = false;

}

void VFXManager::CreatVfxExplode1(XMFLOAT3 &pos)
{
	//âŒÇÃï≤
	sparks.position = pos;
	VFX::Start(sparks);

	//îöî≠
	explode.position = pos;
	VFX::Start(explode);

}

void VFXManager::CreatVfxEnemySpawn(XMFLOAT3& pos, int lifeTime)
{
	enemySpawn.position = pos;
	enemySpawn.position.y += 0.01f;
	enemySpawn.lifeTime = lifeTime;
	enemySpawn.deltaColor = XMFLOAT4(0.0f, 0.0f, 0.0f, -1.0f / enemySpawn.lifeTime);
	VFX::Start(enemySpawn);
	
}
