#include "LifeManager.h"
#include "GameManager.h"
#include "Engine/Transform.h"
#include "Engine/Global.h"
#include "Engine/Sprite.h"
#include "Engine/Text.h"

//定数
namespace
{
	static const int NORMAL_PLAYER_LIFE = 3; //普通のPlayerのライフの数

}

namespace LifeManager
{
	//Playerの現在のライフの総数
	int playerLife_;

	//画像
	Sprite* lifeImage[1]; //ライフごとの画像番号
	Sprite* damageImage_;              //ダメージを受けた時の画像番号
	Transform imageTransform_;		   //画像の位置・拡大率

	//テキスト
	Text* pLifeText_;                  //ライフの数表示する用
	XMFLOAT2 textPositiom_;            //テキストのポジション

	//拡大縮小
	XMVECTOR  beforeScale_;		       //補間する前の拡大率保存
	XMVECTOR  targetScale_;            //次の目標とする拡大率

	//ダメージ画像の透明度
	float damageImageAlpha_;

	//ダメージ画像を表示するかどうか
	bool  isDrawDamageImage_;

	//初期化
	void LifeManager::Initialize()
	{
		//各ライフの画像ロード
		for (int i = 0; i < 1; i++) { lifeImage[i] = new Sprite; }

		//ダメージの画像ロード
		damageImage_ = new Sprite;
		damageImage_->Load("Image/damage.png");

		//テキストの初期化
		pLifeText_ = new Text;
		pLifeText_->Initialize("Text/NumberFont.png", 128, 256, 10);

		//画像のポジション設定
		imageTransform_;

		//テキストのポジション設定
		textPositiom_ = XMFLOAT2(500.0f, 200.0f);

		playerLife_ = NORMAL_PLAYER_LIFE;
		damageImageAlpha_ = 1;
		isDrawDamageImage_ = false;
	}

	//シーン遷移の時の初期化
	void LifeManager::SceneTransitionInitialize()
	{
		//テキストの初期化
		(pLifeText_, new Text);
		pLifeText_->Initialize("Text/NumberFont.png", 128, 256, 10);
	}

	//ダメージ食らった時に呼ぶメソッド
	void LifeManager::Damage()
	{
		//ライフを削る
		playerLife_ -= 1;

		//もし死んでいないのならダメージ関連初期化
		if (!IsDie())
		{
			(damageImageAlpha_, 1);
			(isDrawDamageImage_, true);
		}
	}

	//HPUI描画
	void LifeManager::Draw()
	{
		//シーンがエンドロールなら描画しない
		//if (GameManager::GetpSceneManager()->GetSceneId() == SCENE_ID_ENDROLES) return;

		//画像
		//テクスチャのサイズ取得
		XMFLOAT3 size = damageImage_->GetTextureSize();

		//切り抜き範囲をリセット（画像全体を表示する）
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = (long)size.x;
		rect.bottom = (long)size.y;

		//表示位置用
		Transform t;

		//描画
		//damageImage_->Draw(t, rect, 150, 1);

		//テキスト
		//pLifeText_->Draw((int)textPositiom_.x, (int)textPositiom_.y, "asdfghj");

		//もしダメージ画像を描画するのなら
		if (true || isDrawDamageImage_)
			DamageEffectDraw();

	}

	//ダメージ受けた時の演出を描画
	void DamageEffectDraw()
	{
		//透明度徐々に下げる
		damageImageAlpha_ -= 0.05f;

		//画像
		//テクスチャのサイズ取得
		XMFLOAT3 size = damageImage_->GetTextureSize();

		//切り抜き範囲をリセット（画像全体を表示する）
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = (long)size.x;
		rect.bottom = (long)size.y;

		//表示位置用
		Transform t;

		//描画
		damageImage_->Draw(t, rect, damageImageAlpha_, 1);

		//もし完全に透明になっているのなら描画しなくする
		if (damageImageAlpha_ < 0) (isDrawDamageImage_, false);
	}

	//ライフをリセット
	void LifeManager::ResetLife() { playerLife_ = NORMAL_PLAYER_LIFE; }

	//死んだどうか
	bool LifeManager::IsDie() { return (playerLife_ <= 0); }
}