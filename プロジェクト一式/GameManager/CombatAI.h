#pragma once

class EnemyBase;

/// <summary>
/// ���^AI�̍쐬��ڎw�����N���X
/// ���Ǘ\��
/// �v���C���[�̏��EInput�̏��E�G�̏��
/// ��������댩�ď󋵔��f���ł���AI��ڎw��
/// </summary>
namespace CombatAI
{
	void Initialize();
	void Update();

	bool IsEnemyAttackPermission(EnemyBase* enemy);
	bool IsEnemyMovePermission(EnemyBase* enemy);
};

