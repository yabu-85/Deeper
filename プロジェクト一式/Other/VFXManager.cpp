#include "VFXManager.h"
#include "../Engine/VFX.h"
#include "../Engine/Easing.h"
#include "../Engine/Global.h"

namespace VFXManager
{
	EmitterData sparks;
	EmitterData explode;
	EmitterData smoke;
	EmitterData enemySpawn;
	EmitterData swordSlash;
	EmitterData recovery;

	EmitterData bossSwordSparks;
	EmitterData bossSowrdSmoke;
}

void VFXManager::Initialize()
{
	//ÎÌ²
	sparks.textureFileName = "Particle/cloudA.png";
	sparks.delay = 0;
	sparks.number = 15;
	sparks.lifeTime = 50;
	sparks.directionRnd = XMFLOAT3(90.0f, 90.0f, 90.0f);
	sparks.speed = 0.2f;
	sparks.speedRnd = 1.0f;
	sparks.accel = 0.9f;
	sparks.size = XMFLOAT2(0.25f, 0.25f);
	sparks.sizeRnd = XMFLOAT2(0.1f, 0.1f);
	sparks.scale = XMFLOAT2(0.97f, 0.97f);
	sparks.color = XMFLOAT4(1.0f, 1.0f, 0.1f, 1.0f);
	sparks.deltaColor = XMFLOAT4(0.0f, 0.0f, 0.0f, -1.0f / sparks.lifeTime);
	sparks.gravity = 0.001f;

	//­
	explode.textureFileName = "Particle/cloudA.png";
	explode.delay = 0;
	explode.number = 8;
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

	//ðo·
	smoke.textureFileName = "Particle/cloudA.png";
	smoke.delay = 0;
	smoke.number = 6;
	smoke.lifeTime = 30;
	smoke.position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	smoke.positionRnd = XMFLOAT3(0.2f, 0.1f, 0.2f);
	smoke.direction = XMFLOAT3(2.0f, 0.5f, 0.0f);
	smoke.directionRnd = XMFLOAT3(20.0f, 180.0f, 0.0f);
	smoke.speed = 0.05f;
	smoke.speedRnd = 0.02f;
	smoke.size = XMFLOAT2(1.3f, 1.3f);
	smoke.sizeRnd = XMFLOAT2(0.3f, 0.3f);
	smoke.scale = XMFLOAT2(1.02f, 1.02f);
	smoke.color = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	smoke.deltaColor = XMFLOAT4(0.0f, 0.0f, 0.0f, -1.0f / smoke.lifeTime);
	smoke.gravity = 0.0f;

	//GÌX|[VFX
	static const int APPER_TIME = 180;
	enemySpawn.textureFileName = "Particle/Red.png";
	enemySpawn.delay = 0;
	enemySpawn.lifeTime = APPER_TIME;
	enemySpawn.number = 1;
	enemySpawn.speed = 0.0f;
	enemySpawn.speedRnd = 0.0f;
	enemySpawn.size = XMFLOAT2(2.0f, 2.0f);
	enemySpawn.rotate = XMFLOAT3(90.0f, 0.0f, 0.0f);
	enemySpawn.spin = XMFLOAT3(0.0f, 10.0f, 0.0f);
	enemySpawn.gravity = 0.0f;
	enemySpawn.deltaColor = XMFLOAT4(0.0f, 0.0f, 0.0f, -1.0f / APPER_TIME);
	enemySpawn.isBillBoard = false;

	//ÌXbV
	swordSlash.textureFileName = "Particle/Burger.png";
	swordSlash.delay = 0;
	swordSlash.number = 1;
	swordSlash.lifeTime = 10;
	swordSlash.speed = 0.03f;
	swordSlash.accel = 0.9f;
	swordSlash.size = XMFLOAT2(3.0f, 1.0f);
	swordSlash.deltaColor = XMFLOAT4(1.0f, 1.0f, 1.0f, -0.1f);

	//ñ
	recovery.textureFileName = "Particle/cloudA.png";
	recovery.delay = 0;
	recovery.number = 3;
	recovery.lifeTime = 10;
	recovery.position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	recovery.positionRnd = XMFLOAT3(0.3f, 0.3f, 0.3f);
	recovery.direction = XMFLOAT3(0.0f, 0.5f, 0.0f);
	recovery.speed = 0.1f;
	recovery.size = XMFLOAT2(2.0f, 2.0f);
	recovery.scale = XMFLOAT2(0.9f, 0.9f);
	recovery.color = XMFLOAT4(0.0f, 1.0f, 0.1f, 1.0f);
	recovery.deltaColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	bossSwordSparks.textureFileName = "Particle/cloudA.png";
	bossSwordSparks.delay = 0;
	bossSwordSparks.number = 1;
	bossSwordSparks.lifeTime = 40;
	bossSwordSparks.positionRnd = XMFLOAT3(0.15f, 0.15f, 0.15f);
	bossSwordSparks.directionRnd = XMFLOAT3(20.0f, 20.0f, 20.0f);
	bossSwordSparks.speed = 0.03f;
	bossSwordSparks.speedRnd = 0.02f;
	bossSwordSparks.accel = 1.0f;
	bossSwordSparks.size = XMFLOAT2(0.15f, 0.15f);
	bossSwordSparks.sizeRnd = XMFLOAT2(0.15f, 0.15f);
	bossSwordSparks.scale = XMFLOAT2(0.97f, 0.97f);
	bossSwordSparks.color = XMFLOAT4(1.0f, 0.2f, 0.2f, 1.0f);
	bossSwordSparks.deltaColor = XMFLOAT4(0.0f, 0.0f, 0.0f, -1.0f / bossSwordSparks.lifeTime);
	bossSwordSparks.gravity = 0.0f;

	const float BOSS_SMOKE_ALPHA = 0.5f;
	bossSowrdSmoke.textureFileName = "Particle/cloudA.png";
	bossSowrdSmoke.delay = 0;
	bossSowrdSmoke.number = 1;
	bossSowrdSmoke.lifeTime = 40;
	bossSowrdSmoke.positionRnd = XMFLOAT3(0.2f, 0.2f, 0.2f);
	bossSowrdSmoke.directionRnd = XMFLOAT3(20.0f, 20.0f, 20.0f);
	bossSowrdSmoke.speed = 0.03f;
	bossSowrdSmoke.speedRnd = 0.02f;
	bossSowrdSmoke.accel = 1.0f;
	bossSowrdSmoke.size = XMFLOAT2(0.25f, 0.25f);
	bossSowrdSmoke.sizeRnd = XMFLOAT2(0.1f, 0.1f);
	bossSowrdSmoke.scale = XMFLOAT2(1.05f, 1.05f);
	bossSowrdSmoke.color = XMFLOAT4(1.0f, 0.2f, 0.2f, BOSS_SMOKE_ALPHA);
	bossSowrdSmoke.deltaColor = XMFLOAT4(0.0f, 0.0f, 0.0f, -1.0f / bossSowrdSmoke.lifeTime);
	bossSowrdSmoke.gravity = 0.0f;
}

void VFXManager::CreateVfx(XMFLOAT3 pos, VFX_TYPE type, XMFLOAT3 other)
{
	switch (type)
	{
	case Explode:		CreateVfxExplode1(pos);			break;
	case EnemySpawn:	CreateVfxEnemySpawn(pos);		break;
	case Smoke:			CreateVfxSmoke(pos);			break;
	case SwordSlash:	CreateVfxSwordSlash(pos, other); break;
	case Recovery:		CreateVfxRecovery(pos);			break;
	case BossSowrd:		CreateVfxBossSword(pos, other); break;
	default: break;
	}
}

void VFXManager::CreateVfxSwordSlash(XMFLOAT3 pos, XMFLOAT3 dir)
{
	swordSlash.position = pos;
	swordSlash.direction = dir;

	float rotation = XMConvertToDegrees(atan2f(dir.y, (dir.x + dir.z)));
	swordSlash.rotate.z = rotation;
	
	VFX::Start(swordSlash);
}

void VFXManager::CreateVfxRecovery(XMFLOAT3 pos)
{
	recovery.position = pos;
	VFX::Start(recovery);
}

void VFXManager::CreateVfxBossSword(XMFLOAT3 pos, XMFLOAT3 dir)
{
	bossSwordSparks.position = pos;
	bossSwordSparks.direction = dir;
	VFX::Start(bossSwordSparks);

	bossSowrdSmoke.position = pos;
	bossSowrdSmoke.direction = dir;
	VFX::Start(bossSowrdSmoke);
}

void VFXManager::CreateVfxExplode1(XMFLOAT3 pos)
{
	//ÎÌ²
	sparks.position = pos;
	VFX::Start(sparks);

	//­
	explode.position = pos;
	VFX::Start(explode);
}

void VFXManager::CreateVfxEnemySpawn(XMFLOAT3 pos)
{
	enemySpawn.position = pos;
	enemySpawn.position.y += 0.01f;
	VFX::Start(enemySpawn);
}

void VFXManager::CreateVfxSmoke(XMFLOAT3 pos)
{
	smoke.position = pos;
	smoke.position.y -= 0.25f;
	VFX::Start(smoke);
}
