#include "VFXManager.h"
#include "Engine/VFX.h"

void VFXManager::CreatVfxExplode1(XMFLOAT3 &pos)
{
	//‰Î‚Ì•²
	EmitterData data;
	data.textureFileName = "Particle/cloudA.png";
	data.delay = 0;
	data.number = 30;
	data.lifeTime = 50;
	data.position = pos;
	data.positionRnd = XMFLOAT3(1.0f, 1.0f, 1.0f);
	data.direction = XMFLOAT3(1.0f, 1.0f, 1.0f);
	data.directionRnd = XMFLOAT3(90.0f, 90.0f, 90.0f);
	data.speed = 0.2f;
	data.speedRnd = 1.0f;
	data.accel = 0.9f;
	data.size = XMFLOAT2(0.3f, 0.3f);
	data.sizeRnd = XMFLOAT2(0.1f, 0.1f);
	data.scale = XMFLOAT2(0.97f, 0.97f);
	data.color = XMFLOAT4(1.0f, 1.0f, 0.1f, 1.0f);
	data.deltaColor = XMFLOAT4(0.0f, 0.0f, 0.0f, -1.0f / data.lifeTime);
	data.gravity = 0.001f;
	VFX::Start(data);

	//”š”­
	data.textureFileName = "Particle/cloudA.png";
	data.delay = 0;
	data.number = 6;
	data.lifeTime = 20;
	data.position = pos;
	data.positionRnd = XMFLOAT3(0.5f, 0.0f, 0.5f);
	data.direction = XMFLOAT3(1.0f, 1.0f, 1.0f);
	data.directionRnd = XMFLOAT3(90.0f, 90.0f, 90.0f);
	data.speed = 0.1f;
	data.speedRnd = 0.8f;
	data.size = XMFLOAT2(1.2f, 1.2f);
	data.sizeRnd = XMFLOAT2(0.4f, 0.4f);
	data.scale = XMFLOAT2(1.05f, 1.05f);
	data.color = XMFLOAT4(1.0f, 1.0f, 0.1f, 1.0f);
	data.deltaColor = XMFLOAT4(0.0f, -1.0f / 20.0f, 0.0f, -1.0f / 20.0f);
	VFX::Start(data);

}

void VFXManager::CreatVfxEnemySpawn(XMFLOAT3& pos)
{
	EmitterData data;
	data.textureFileName = "Particle/Red.png";
	data.delay = 0;
	data.number = 1;
	data.lifeTime = 180;
	data.position = pos;
	data.position.y += 0.01f;
	data.positionRnd = XMFLOAT3(0.0f, 0.0f, 0.0f);
	data.direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
	data.directionRnd = XMFLOAT3(0.0f, 0.0f, 0.0f);
	data.speed = 0.0f;
	data.speedRnd = 0.0f;
	data.accel = 0.0f;
	data.size = XMFLOAT2(10.0f, 10.0f);
	data.sizeRnd = XMFLOAT2(0.0f, 0.0f);
	data.scale = XMFLOAT2(1.0f, 1.0f);
	data.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	data.deltaColor = XMFLOAT4(0.0f, 0.0f, 0.0f, -1.0f / data.lifeTime);
	data.rotate = XMFLOAT3(90.0f, 0.0f, 0.0f);
	data.rotateRnd = XMFLOAT3(0.0f, 0.0f, 0.0f);
	data.spin = XMFLOAT3(0.0f, 10.0f, 0.0f);
	data.gravity = 0.0f;
	data.isBillBoard = false;
	VFX::Start(data);
	
}
