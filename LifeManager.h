#pragma once

namespace LifeManager
{
	void Initialize();

	/// <summary>
	/// シーン遷移の時の初期化
	/// </summary>
	void SceneTransitionInitialize();

	/// <summary>
	/// ダメージ食らった時に呼ぶメソッド
	/// </summary>
	void Damage();

	/// <summary>
	/// HPUI描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ライフをリセット
	/// </summary>
	void ResetLife();

	/// <summary>
	/// ダメージ受けた時の演出を描画
	/// </summary>
	void DamageEffectDraw();

	/// <summary>
	/// 死んだどうか
	/// </summary>
	/// <returns>trueなら死んだ,falseならまだ生きてる</returns>
	bool IsDie();
}