#include "Global.h"
#include "Model.h"

//3Dモデル（FBXファイル）を管理する
namespace Model
{
	//ロード済みのモデルデータ一覧
	std::vector<ModelData*>	_datas;

	//初期化
	void Initialize()
	{
		AllRelease();
	}

	//モデルをロード
	int Load(std::string fileName)
	{
		ModelData* pData = new ModelData;


		//開いたファイル一覧から同じファイル名のものが無いか探す
		bool isExist = false;
		for (int i = 0; i < _datas.size(); i++)
		{
			//すでに開いている場合
			if (_datas[i] != nullptr && _datas[i]->fileName == fileName)
			{
				pData->pFbx = _datas[i]->pFbx;
				isExist = true;
				break;
			}
		}

		//新たにファイルを開く
		if (isExist == false)
		{
			pData->pFbx = new Fbx;
			if (FAILED(pData->pFbx->Load(fileName)))
			{
				//開けなかった
				SAFE_DELETE(pData->pFbx);
				SAFE_DELETE(pData);
				return -1;
			}

			//無事開けた
			pData->fileName = fileName;
		}


		//使ってない番号が無いか探す
		for (int i = 0; i < _datas.size(); i++)
		{
			if (_datas[i] == nullptr)
			{
				_datas[i] = pData;
				return i;
			}
		}

		//新たに追加
		_datas.push_back(pData);
		return (int)_datas.size() - 1;
	}



	//描画
	void Draw(int handle, Direct3D::SHADER_TYPE type)
	{
		if (handle < 0 || handle >= _datas.size() || _datas[handle] == nullptr)
		{
			return;
		}

		//アニメーションを進める
		if (!_datas[handle]->isAimeStop) {
			_datas[handle]->nowFrame += _datas[handle]->animSpeed;
		
			//最後までアニメーションしたら戻す
			if (_datas[handle]->nowFrame > (float)_datas[handle]->endFrame) {
				_datas[handle]->nowFrame = (float)_datas[handle]->startFrame;
			}
		}
		
		if (_datas[handle]->pFbx)
		{
			if (_datas[handle]->isBlending) {
				_datas[handle]->pFbx->Draw(_datas[handle]->transform, type, (int)_datas[handle]->nowFrame, (int)_datas[handle]->blendFrame, _datas[handle]->blendWeight);
			}
			else {
				_datas[handle]->pFbx->Draw(_datas[handle]->transform, type, (int)_datas[handle]->nowFrame);
			}
		}
	}


	//任意のモデルを開放
	void Release(int handle)
	{
		if (handle < 0 || handle >= _datas.size() || _datas[handle] == nullptr)
		{
			return;
		}

		//同じモデルを他でも使っていないか
		bool isExist = false;
		for (int i = 0; i < _datas.size(); i++)
		{
			//すでに開いている場合
			if (_datas[i] != nullptr && i != handle && _datas[i]->pFbx == _datas[handle]->pFbx)
			{
				isExist = true;
				break;
			}
		}

		//使ってなければモデル解放
		if (isExist == false )
		{
			SAFE_DELETE(_datas[handle]->pFbx);
		}


		SAFE_DELETE(_datas[handle]);
	}


	//全てのモデルを解放
	void AllRelease()
	{
		for (int i = 0; i < _datas.size(); i++)
		{
			if (_datas[i] != nullptr)
			{
				Release(i);
			}
		}
		_datas.clear();
	}

	//アニメーションのフレーム数をセット
	void SetBlendingAnimFrame(int handle, int startFrame1, int endFrame1, int startFrame2, float animSpeed, float blendWeight)
	{
		_datas[handle]->isAimeStop = false;
		_datas[handle]->SetBlendingAnimFrame(startFrame1, endFrame1, startFrame2, animSpeed, blendWeight);
	}

	//アニメーションのフレーム数をセット
	void SetAnimFrame(int handle, int startFrame, int endFrame, float animSpeed)
	{
		_datas[handle]->isAimeStop = false;
		_datas[handle]->SetAnimFrame(startFrame, endFrame, animSpeed);
	}

	void SetBlend(int handle, bool b)
	{
		_datas[handle]->isBlending = b;
	}

	void SetBlendFactor(int handle, float weight)
	{
		_datas[handle]->blendWeight = weight;
	}

	float GetBlendFactor(int handle)
	{
		return _datas[handle]->blendWeight;
	}

	void SetAnimStop(int handle, bool b)
	{
		_datas[handle]->isAimeStop = b;
	}

	void SetAllAnimStop(bool b)
	{
		for (int i = 0; i < _datas.size(); i++)
		{
			if(_datas[i]) _datas[i]->isAimeStop = b;
		}
	}

	bool IsAnimStop(int handle)
	{
		return _datas[handle]->isAimeStop;
	}

	//現在のアニメーションのフレームを取得
	float GetAnimFrame(int handle)
	{
		return _datas[handle]->nowFrame;
	}

	bool GetBoneIndex(int handle, std::string boneName, int* index, int* partIndex)
	{
		return _datas[handle]->pFbx->GetBoneIndex(boneName, index, partIndex);
	}

	//任意のボーンの位置を取得
	XMFLOAT3 GetBonePosition(int handle, int boneIndex, int partIndex)
	{
		XMFLOAT3 pos = _datas[handle]->pFbx->GetBonePosition(boneIndex, partIndex);
		XMVECTOR vec = XMVector3TransformCoord(XMLoadFloat3(&pos), _datas[handle]->transform.GetWorldMatrix());
		XMStoreFloat3(&pos, vec);
		return pos;
	}

	XMFLOAT3 GetBoneAnimPosition(int handle, int boneIndex, int partIndex)
	{
		//相対座標（ボーンの中心からの位置）
		//ブレンディング中だったらそれ用の
		XMFLOAT3 pos = XMFLOAT3();
		if (_datas[handle]->isBlending) {
			pos = _datas[handle]->pFbx->GetBoneAnimBlendPosition(boneIndex, partIndex, (int)_datas[handle]->nowFrame, (int)_datas[handle]->blendFrame, _datas[handle]->blendWeight);
		}
		else {
			pos = _datas[handle]->pFbx->GetBoneAnimPosition(boneIndex, partIndex, (int)_datas[handle]->nowFrame);
		}

		XMVECTOR vec = XMVector3TransformCoord(XMLoadFloat3(&pos), _datas[handle]->transform.GetWorldMatrix()); //posをワールドマトリックスで計算する
		XMStoreFloat3(&pos, vec);
		return pos;
	}

	XMFLOAT3 GetBoneAnimRotate(int handle, int boneIndex, int partIndex)
	{
		//相対座標（ボーンの中心からの位置）
		//ブレンド中ならそれ用の
		XMFLOAT3 rot = XMFLOAT3();
		if(_datas[handle]->isBlending) rot = _datas[handle]->pFbx->GetBoneAniBlendRotate(boneIndex, partIndex, (int)_datas[handle]->nowFrame, (int)_datas[handle]->blendFrame, _datas[handle]->blendWeight);
		else rot = _datas[handle]->pFbx->GetBoneAnimRotate(boneIndex, partIndex, (int)_datas[handle]->nowFrame);
	
		if (rot.x >= 90.0f || rot.x <= -90.0f) rot.y *= -1.0f;
		return rot;
	}

	//ワールド行列を設定
	void SetTransform(int handle, Transform & transform)
	{
		if (handle < 0 || handle >= _datas.size())
		{
			return;
		}

		_datas[handle]->transform = transform;
	}


	//ワールド行列の取得
	XMMATRIX GetMatrix(int handle)
	{
		return _datas[handle]->transform.GetWorldMatrix();
	}

	Fbx* GetFbx(int handle)
	{
		return _datas[handle]->pFbx;
	}


	//レイキャスト（レイを飛ばして当たり判定）
	void RayCast(int handle, RayCastData *data)
	{
			XMFLOAT3 target = Transform::Float3Add(data->start, data->dir);
			XMMATRIX matInv = XMMatrixInverse(nullptr, _datas[handle]->transform.GetWorldMatrix());
			XMVECTOR vecStart = XMVector3TransformCoord(XMLoadFloat3(&data->start), matInv);
			XMVECTOR vecTarget = XMVector3TransformCoord(XMLoadFloat3(&target), matInv);
			XMVECTOR vecDir = vecTarget - vecStart;

			XMStoreFloat3(&data->start, vecStart);
			XMStoreFloat3(&data->dir, vecDir);

			_datas[handle]->pFbx->RayCast(data); 
	}
}