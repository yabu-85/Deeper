#include "Cell.h"
#include "Engine/Camera.h"
#include "Engine/Direct3D.h"
#include "Engine/Transform.h"













//---------------------------------------------------------------------------------------------

#define SAFE_RELEASE(p) {if ((p)!=nullptr) { p->Release(); (p)=nullptr;}}

Quad::Quad() :pVertexBuffer_(nullptr), pIndexBuffer_(nullptr), pConstantBuffer_(nullptr), vertexNum_(0), indexNum_(0)
{
}

Quad::~Quad()
{
	Quad::Release();
}

void Quad::Release()
{
	SAFE_RELEASE(pVertexBuffer_);
	SAFE_RELEASE(pIndexBuffer_);
	SAFE_RELEASE(pConstantBuffer_);
}

HRESULT Quad::Initialize()
{
	HRESULT hr;
	hr = CreateVertexBuffer(); //頂点情報の準備
	if (FAILED(hr)) {
		MessageBox(nullptr, "頂点バッファの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}

	hr = CreateIndexBuffer();  //インデックスバッファを作成
	if (FAILED(hr)) {
		MessageBox(nullptr, "インデックスバッファの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}

	hr = CreateConstantBuffer(); //コンスタントバッファを作成
	if (FAILED(hr)) {
		MessageBox(nullptr, "コンスタントバッファの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}

	return S_OK;
}

void Quad::Draw()
{
	Direct3D::SetShader(Direct3D::SHADER_UNLIT);

	transform_.Calclation();//トランスフォームを計算
	PassDataToCB(transform_);
	SetBufferToPipeline();
	Direct3D::pContext_->DrawIndexed(index_.size(), 0, 0);

	Direct3D::SetShader(Direct3D::SHADER_3D);
}

void Quad::InitVertexData()
{
	// 頂点情報
	vertices_ =
	{
		{ XMVectorSet(0.0f, 0.0f, 10.0f, 0.0f), XMVectorSet(-1.0f, 1.0f, -1.0f, 0.0f)},    // 四角形の頂点（左上）0
		{ XMVectorSet(10.0f, 0.0f, 10.0f, 0.0f), XMVectorSet(1.0f, 1.0f, -1.0f, 0.0f) },   // 四角形の頂点（右上）1
		{ XMVectorSet(10.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(1.0f, -1.0f, -1.0f, 0.0f) },   // 四角形の頂点（右下）2
		{ XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(-1.0f, -1.0f, -1.0f, 0.0f) },   // 四角形の頂点（左下）3

		{ XMVectorSet(0.0f, 10.0f, 10.0f, 0.0f), XMVectorSet(-1.0f, 1.0f, -1.0f, 0.0f)},   // 四角形の頂点 0の上
		{ XMVectorSet(10.0f, 10.0f, 10.0f, 0.0f), XMVectorSet(1.0f, 1.0f, -1.0f, 0.0f) },  // 四角形の頂点 1の上
		{ XMVectorSet(10.0f, 10.0f, 0.0f, 0.0f), XMVectorSet(1.0f, -1.0f, -1.0f, 0.0f) },  // 四角形の頂点 2の上
		{ XMVectorSet(0.0f, 10.0f, 0.0f, 0.0f), XMVectorSet(-1.0f, -1.0f, -1.0f, 0.0f) },  // 四角形の頂点 3の上

	};

	vertexNum_ = vertices_.size();
}

HRESULT Quad::CreateVertexBuffer()
{
	InitVertexData();

	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * vertexNum_;
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices_.data();

	HRESULT hr;
	hr = Direct3D::pDevice_->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FAILED(hr)) {

		//エラー処理
		MessageBox(nullptr, "頂点バッファの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}
	return S_OK;
}

void Quad::InitIndexData()
{
	//インデックス情報
	index_ = { 0,2,3, 0,1,2, 4,1,0, 4,5,1, 5,2,1, 5,6,2, 6,3,2, 6,7,3, 7,0,3, 7,4,0, 4,6,7, 4,5,6 };

	indexNum_ = sizeof(index_);
}

HRESULT Quad::CreateIndexBuffer()
{
	InitIndexData();

	// インデックスバッファを生成する
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * indexNum_;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = index_.data();
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	HRESULT hr;
	hr = Direct3D::pDevice_->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
	if (FAILED(hr)) {
		//エラー処理
		MessageBox(nullptr, "インデックスバッファの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}

	return S_OK;
}

void Quad::PassDataToCB(Transform transform)
{
	//コンスタントバッファに渡す情報
	CONSTANT_BUFFER cb;
	cb.matWVP = XMMatrixTranspose(transform.GetWorldMatrix() * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	cb.matNormal = XMMatrixTranspose(transform.matRotate_ * XMMatrixInverse(nullptr, transform.matScale_));

	//コンスタントバッファの更新
	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext_->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// データを値を送る
	Direct3D::pContext_->Unmap(pConstantBuffer_, 0);	//再開
}

void Quad::SetBufferToPipeline()
{
	//頂点バッファ
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext_->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	// インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext_->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//コンスタントバッファ
	Direct3D::pContext_->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
	Direct3D::pContext_->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用
}

HRESULT Quad::CreateConstantBuffer()
{
	//コンスタントバッファ作成
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	HRESULT hr;
	// コンスタントバッファの作成
	hr = Direct3D::pDevice_->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr)) {
		//エラー処理
		MessageBox(nullptr, "コンスタントバッファの作成に失敗しました", "エラー", MB_OK); //
		return hr;
	}
	return S_OK;
}