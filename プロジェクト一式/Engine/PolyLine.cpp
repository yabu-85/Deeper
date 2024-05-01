#include "Global.h"
#include "Camera.h"
#include "PolyLine.h"
#include "Direct3D.h"
#include <vector>

PolyLine::PolyLine() : width_(0.3f), length_(50), alpha_(1.0f), moveAlpha_(false), smooth_(0), first_(true), clear_(false), allClearReset_(false),
	pVertexBuffer_(nullptr), pConstantBuffer_(nullptr), pTexture_(nullptr)
{
	ResetPosition();
}

void PolyLine::Update()
{
	if (clear_) {
		ClearLastPositions();
		if (first_ && allClearReset_) {
			clear_ = false;
			allClearReset_ = false;
		}
	}

}

void PolyLine::ResetPosition()
{
	//リストの先頭に現在位置を追加
	positions_.clear();
	positionsSub_.clear();
	positions_.push_front(XMFLOAT3(0,0,0));
	positionsSub_.push_front(XMFLOAT3(0, 0, 0));
	
	alpha_ = 1.0f;
	first_ = true;

	//頂点バッファをクリア（今から作るから）
	SAFE_RELEASE(pVertexBuffer_);

	//頂点データを作るための配列を準備
	VERTEX* vertices = new VERTEX;

	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX);
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices;
	Direct3D::pDevice_->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);

	delete vertices;
}

void PolyLine::ClearLastPositions()
{
	//firstならListに値内から終わる
	if (first_) return;

	//データの後ろから消してく
	positions_.pop_back();
	positionsSub_.pop_back();

	//データなくなったらfirstをtureに
	if (positions_.size() <= 0)
	{
		positions_.push_front(XMFLOAT3(0, 0, 0));
		positionsSub_.push_front(XMFLOAT3(0, 0, 0));
		first_ = true;
	}

	//頂点バッファをクリア（今から作るから）
	SAFE_RELEASE(pVertexBuffer_);

	//頂点データを作るための配列を準備
	VERTEX* vertices = new VERTEX[length_ * 2];

	//頂点データを作る
	int index = 0;
	auto itr = positions_.begin();
	auto itrSub = positionsSub_.begin();
	for (int i = 0; i < length_; i++)
	{
		//記憶してた位置取得
		XMVECTOR vPos1 = XMLoadFloat3(&(*itr));
		XMVECTOR vPos2 = XMLoadFloat3(&(*itrSub));

		itr++;
		itrSub++;
		if (itr == positions_.end()) break;

		//頂点情報を入れていく
		XMFLOAT3 pos;
		XMStoreFloat3(&pos, vPos1);
		VERTEX vertex1 = { pos, XMFLOAT3((float)i / (float)length_, 1, 0) };

		XMStoreFloat3(&pos, vPos2);
		VERTEX vertex2 = { pos, XMFLOAT3((float)i / (float)length_, 0, 0) };

		vertices[index] = vertex1;
		index++;
		vertices[index] = vertex2;
		index++;
	}

	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * length_ * 2;
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices;
	Direct3D::pDevice_->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);

	delete[] vertices;
}

void PolyLine::SetClear(bool allClear)
{
	clear_ = true;
	allClearReset_ = allClear;
}

void PolyLine::ClearCancel()
{
	clear_ = false;
}

void PolyLine::AddPosition(XMFLOAT3 pos1, XMFLOAT3 pos2)
{
	if (first_) {
		positions_.clear();
		positionsSub_.clear();
		first_ = false;
	}

	positions_.push_front(pos1);
	positionsSub_.push_front(pos2);

	//指定の長さを超えてたら終端のデータを削除
	if (positions_.size() > length_)
	{
		positions_.pop_back();
		positionsSub_.pop_back();
	}

	//頂点バッファをクリア（今から作るから）
	SAFE_RELEASE(pVertexBuffer_);

	//頂点データを作るための配列を準備
	VERTEX* vertices = new VERTEX[length_ * 2];

	//頂点データを作る
	int index = 0;
	auto itr = positions_.begin();
	auto itrSub = positionsSub_.begin();
	for (int i = 0; i < length_; i++)
	{
		//記憶してた位置取得
		XMVECTOR vPos1 = XMLoadFloat3(&(*itr));
		XMVECTOR vPos2 = XMLoadFloat3(&(*itrSub));

		if (smooth_ >= 1) {
			int firstI = i - smooth_;
			int endI = i + smooth_;
			int max = (int)positions_.size();
			if (firstI >= 0 && endI < max) {
				//smoothの数でイテレータを計算
				auto it = itr;
				auto itSub = itrSub;
				for (int i = 0; i < smooth_; i++) {
					it--;
					itSub--;
				}

				//smooth＊２＋１の平均を求める
				XMVECTOR addVec1 = XMVectorZero();
				XMVECTOR addVec2 = XMVectorZero();
				for (; firstI <= endI; firstI++) {
					addVec1 += XMLoadFloat3(&(*it));
					addVec2 += XMLoadFloat3(&(*itSub));
					it++;
					itSub++;
				}
				float s1 = 1.0f / ((float)smooth_ * 2.0f + 1.0f);
				float s2 = 1.0f / ((float)smooth_ * 2.0f + 1.0f);
				vPos1 = addVec1 * s1;
				vPos2 = addVec2 * s2;
				XMFLOAT3 fPos1, fPos2;
				XMStoreFloat3(&fPos1, vPos1);
				XMStoreFloat3(&fPos2, vPos2);
				(*itr) = fPos1;
				(*itrSub) = fPos2;
			}
		}
		
		//頂点情報を入れていく
		XMFLOAT3 pos;
		XMStoreFloat3(&pos, vPos1);
		VERTEX vertex1 = { pos, XMFLOAT3((float)i / (float)length_, 1, 0) };

		XMStoreFloat3(&pos, vPos2);
		VERTEX vertex2 = { pos, XMFLOAT3((float)i / (float)length_, 0, 0) };

		vertices[index] = vertex1;
		index++;
		vertices[index] = vertex2;
		index++;

		itr++;
		itrSub++;
		if (itr == positions_.end()) break;

	}

	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * length_ * 2;
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices;
	Direct3D::pDevice_->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);

	delete[] vertices;
}

void PolyLine::AddPosition(XMFLOAT3 pos)
{
	positions_.push_front(pos);

	//指定の長さを超えてたら終端のデータを削除
	if (positions_.size() > length_)
	{
		positions_.pop_back();
	}

	//頂点バッファをクリア（今から作るから）
	SAFE_RELEASE(pVertexBuffer_);

	//ベクトルを求める
	XMVECTOR vPVec = XMLoadFloat3(&positions_.back());
	XMVECTOR vPre = XMLoadFloat3(&pos);
	vPVec = XMVector3NormalizeEst(vPVec - vPre);
	XMFLOAT3 camPos = XMFLOAT3(0, 1, 0);
	vPVec = XMLoadFloat3(&camPos);

	//頂点データを作るための配列を準備
	VERTEX* vertices = new VERTEX[length_ * 2];

	//頂点データを作る
	int index = 0;
	auto itr = positions_.begin();
	for (int i = 0; i < length_; i++)
	{
		//記憶してた位置
		XMVECTOR vPos = XMLoadFloat3(&(*itr));

		itr++;
		if (itr == positions_.end()) break;

		//記憶してた位置から、その次に記憶してた位置に向かうベクトル
		XMVECTOR vLine = XMLoadFloat3(&(*itr)) - vPos;

		//視線とラインに垂直なベクトル
		XMVECTOR vArm = XMVector3Cross(vLine, vPVec);
		vArm = XMVector3Normalize(vArm) * width_;

		//頂点情報を入れていく
		XMFLOAT3 pos;
		XMStoreFloat3(&pos, vPos + vArm);
		VERTEX vertex1 = { pos, XMFLOAT3((float)i / length_, 0, 0) };

		XMStoreFloat3(&pos, vPos - vArm);
		VERTEX vertex2 = { pos, XMFLOAT3((float)i / length_, 1, 0) };

		vertices[index] = vertex1;
		index++;
		vertices[index] = vertex2;
		index++;

	}

	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * length_ * 2;
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices;
	Direct3D::pDevice_->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);

	delete[] vertices;
}

HRESULT PolyLine::Load(std::string fileName)
{
	//コンスタントバッファ作成
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// コンスタントバッファの作成
	Direct3D::pDevice_->CreateBuffer(&cb, nullptr, &pConstantBuffer_);


	//テクスチャ
	pTexture_ = new Texture;
	pTexture_->Load(fileName.c_str());

	return S_OK;
}

void PolyLine::Draw()
{
	Direct3D::SetShader(Direct3D::SHADER_BILLBOARD);
	Direct3D::SetBlendMode(Direct3D::BLEND_ADD);

	if (moveAlpha_)
		alpha_ -= 0.005f;

	//コンスタントバッファに渡す情報
	CONSTANT_BUFFER cb;
	cb.matWVP = XMMatrixTranspose(Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	cb.color = XMFLOAT4(1, 1, 1, alpha_);

	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext_->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// データを値を送る

	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext_->PSSetSamplers(0, 1, &pSampler);

	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext_->PSSetShaderResources(0, 1, &pSRV);

	Direct3D::pContext_->Unmap(pConstantBuffer_, 0);	//再開

	//頂点バッファ
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext_->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	//コンスタントバッファ
	Direct3D::pContext_->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
	Direct3D::pContext_->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用

	//頂点データの並び方を指定
	Direct3D::pContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//描画（インデックスバッファいらないタイプ）
	Direct3D::pContext_->Draw(((int)positions_.size() - 1) * 2, 0);

	//頂点データの並び方を指定を戻す
	Direct3D::pContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Direct3D::SetBlendMode(Direct3D::BLEND_DEFAULT);
}

void PolyLine::Release()
{
	SAFE_DELETE(pTexture_);
	SAFE_RELEASE(pConstantBuffer_);
	SAFE_RELEASE(pVertexBuffer_);
	positions_.clear();
	positionsSub_.clear();
}
