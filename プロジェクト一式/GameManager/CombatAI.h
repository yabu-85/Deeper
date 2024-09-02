#pragma once

class EnemyBase;

/// <summary>
/// メタAIの作成を目指したクラス
/// 改良予定
/// プレイヤーの情報・Inputの情報・敵の情報
/// 他もろもろ見て状況判断ができるAIを目指す
/// </summary>
namespace CombatAI
{
	void Initialize();
	void Update();

	bool IsEnemyAttackPermission(EnemyBase* enemy);
	bool IsEnemyMovePermission(EnemyBase* enemy);
};

