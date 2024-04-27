#include "Fbx.h"
#include "FbxParts.h"

Fbx::Fbx():_animSpeed(0)
{
}


Fbx::~Fbx()
{
	for (int i = 0; i < parts_.size(); i++)
	{
		delete parts_[i];
	}
	parts_.clear();

	pFbxScene_->Destroy();
	pFbxManager_->Destroy();
}

HRESULT Fbx::Load(std::string fileName)
{
	// FBXの読み込み
	pFbxManager_ = FbxManager::Create();
	pFbxScene_ = FbxScene::Create(pFbxManager_, "fbxscene");
	FbxString FileName(fileName.c_str());
	FbxImporter *fbxImporter = FbxImporter::Create(pFbxManager_, "imp");
	if (!fbxImporter->Initialize(FileName.Buffer(), -1, pFbxManager_->GetIOSettings()))
	{
		//失敗
		return E_FAIL;
	}
	fbxImporter->Import(pFbxScene_);
	fbxImporter->Destroy();

	// アニメーションのタイムモードの取得(フレームレートとかの（60fps)）
	_frameRate = pFbxScene_->GetGlobalSettings().GetTimeMode();


	//現在のカレントディレクトリを覚えておく
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	//カレントディレクトリをファイルがあった場所に変更
	char dir[MAX_PATH];
	_splitpath_s(fileName.c_str(), nullptr, 0, dir, MAX_PATH, nullptr, 0, nullptr, 0);
	SetCurrentDirectory(dir);

	//FbxSceneオブジェクト->このオブジェクトの中にFBXファイルのすべての情報が格納されています。
	//FBXは情報の巨大なツリー構造になっています。そのツリーは基本すべて「FbxNode」というノードが基本となっています
	//巨大なツリーのトップは「ルートノード」と呼ばれます。そこの下にあらゆる情報がぶら下がっています
	
	//ルートノードを取得して
	FbxNode* rootNode = pFbxScene_->GetRootNode();

	//そいつの子供の数を調べて
	int childCount = rootNode->GetChildCount();

	//1個ずつチェック
	for (int i = 0; childCount > i; i++)
	{
		CheckNode(rootNode->GetChild(i), &parts_);
	}

	//カレントディレクトリを元の位置に戻す
	SetCurrentDirectory(defaultCurrentDir);

	return S_OK;
}

void Fbx::CheckNode(FbxNode * pNode, std::vector<FbxParts*>* pPartsList)
{
	//そのノードにはメッシュ情報が入っているだろうか？
	//FbxNodeAttribute = これはそのノードが具体的にどういう情報を持っているかを表すオブジェクト（クラスの定義を見れば分かるよ
	FbxNodeAttribute* attr = pNode->GetNodeAttribute();
	if (attr != nullptr && attr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		//パーツを用意
		FbxParts* pParts = new FbxParts;
		pParts->Init(pNode);

		//パーツ情報を動的配列に追加
		pPartsList->push_back(pParts);
	}


	//子ノードにもデータがあるかも！！
	{
		//子供の数を調べて
		int childCount = pNode->GetChildCount();

		//一人ずつチェック
		for (int i = 0; i < childCount; i++)
		{
			CheckNode(pNode->GetChild(i), pPartsList);
		}
	}
}


void Fbx::Release()
{
}

bool Fbx::GetBoneIndex(std::string boneName, int* index, int* partIndex)
{
	*index = -1;
	*partIndex = -1;
	
	for (int i = 0; i < parts_.size(); i++)
	{
		if (parts_[i]->GetBoneIndex(boneName, index)) {
			*partIndex = i;
			return true;
		}
	}
	return false;
}

XMFLOAT3 Fbx::GetBonePosition(int index, int partIndex)
{
	return parts_[partIndex]->GetBonePosition(index);
}

XMFLOAT3 Fbx::GetBoneAnimBlendPosition(int index, int partIndex, int frame1, int frame2, float blendFactor)
{
	FbxTime time1, time2;
	time1.SetTime(0, 0, 0, frame1, 0, 0, _frameRate);
	time2.SetTime(0, 0, 0, frame2, 0, 0, _frameRate);

	return parts_[partIndex]->GetBonePosition(index, time1, time2, blendFactor);
}

XMFLOAT3 Fbx::GetBoneAnimPosition(int index, int partIndex, int frame)
{
	FbxTime time;
	time.SetTime(0, 0, 0, frame, 0, 0, _frameRate);
	return parts_[partIndex]->GetBonePosition(index, time);
}

XMFLOAT3 Fbx::GetBoneAnimRotate(int index, int partIndex, int frame)
{
	FbxTime time;
	time.SetTime(0, 0, 0, frame, 0, 0, _frameRate);
	return parts_[partIndex]->GetBoneRotate(index, time);
}

XMFLOAT3 Fbx::GetBoneAniBlendRotate(int index, int partIndex, int frame1, int frame2, float blendFactor)
{
	FbxTime time1, time2;
	time1.SetTime(0, 0, 0, frame1, 0, 0, _frameRate);
	time2.SetTime(0, 0, 0, frame2, 0, 0, _frameRate);
	return parts_[partIndex]->GetBoneRotate(index, time1, time2, blendFactor);
}

void Fbx::Draw(Transform& transform, Direct3D::SHADER_TYPE shader, int frame1, int frame2, float blendFactor)
{
	Direct3D::SetBlendMode(Direct3D::BLEND_DEFAULT);
	Direct3D::SetShader(shader);

	//パーツを1個ずつ描画
	for (int k = 0; k < parts_.size(); k++)
	{
		// その瞬間の自分の姿勢行列を得る

		FbxTime time1, time2;
		time1.SetTime(0, 0, 0, frame1, 0, 0, _frameRate);
		time2.SetTime(0, 0, 0, frame2, 0, 0, _frameRate);

		//スキンアニメーション（ボーン有り）の場合
		if (parts_[k]->GetSkinInfo() != nullptr)
		{
			parts_[k]->DrawBlendedSkinAnim(transform, time1, time2, blendFactor);
		}
	}
}

void Fbx::Draw(Transform& transform, Direct3D::SHADER_TYPE shader, int frame)
{
	Direct3D::SetBlendMode(Direct3D::BLEND_DEFAULT);
	Direct3D::SetShader(shader);

	//パーツを1個ずつ描画
	for (int k = 0; k < parts_.size(); k++)
	{
		// その瞬間の自分の姿勢行列を得る
		FbxTime     time;
		time.SetTime(0, 0, 0, frame, 0, 0, _frameRate);


		//スキンアニメーション（ボーン有り）の場合
		if (parts_[k]->GetSkinInfo() != nullptr)
		{
			parts_[k]->DrawSkinAnim(transform, time);
		}

		//メッシュアニメーションの場合
		else
		{
			parts_[k]->DrawMeshAnim(transform, time, pFbxScene_);
		}
	}
}


//レイキャスト（レイを飛ばして当たり判定）
void Fbx::RayCast(RayCastData * data)
{
	//すべてのパーツと判定
	for (int i = 0; i < parts_.size(); i++)
	{
		parts_[i]->RayCast(data);
	}
}
