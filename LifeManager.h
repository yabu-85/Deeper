#pragma once

namespace LifeManager
{
	void Initialize();

	/// <summary>
	/// �V�[���J�ڂ̎��̏�����
	/// </summary>
	void SceneTransitionInitialize();

	/// <summary>
	/// �_���[�W�H��������ɌĂԃ��\�b�h
	/// </summary>
	void Damage();

	/// <summary>
	/// HPUI�`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���C�t�����Z�b�g
	/// </summary>
	void ResetLife();

	/// <summary>
	/// �_���[�W�󂯂����̉��o��`��
	/// </summary>
	void DamageEffectDraw();

	/// <summary>
	/// ���񂾂ǂ���
	/// </summary>
	/// <returns>true�Ȃ玀��,false�Ȃ�܂������Ă�</returns>
	bool IsDie();
}