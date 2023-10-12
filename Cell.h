#pragma once
#include <DirectXMath.h>
#include <vector>
using namespace DirectX;

class Triangle;

class Cell
{
	//左奥下の座標
	XMFLOAT3 position_;

	//オブジェクトより下にある最も近い床の座標を得る
	int GetNearestFloorY(
		const XMFLOAT3* pos,
		float offsetY, //落下速度が速すぎて、床を突き抜ける場合の調整値 
		float ny,
		float* tanTheta,
		float* floorY
	)const { return 0; };

	//Axis Aligned Cube Box----------------------------------------------------
	//cubeの一辺の長さ
	float Len = 0;
	//左奥下の座標
	float X = 0, Z = 0, Y = 0;
	//頂点位置 posision
	XMFLOAT3 P[8];
	//４角ポリゴンの法線
	XMFLOAT3 N[6] = {
		XMFLOAT3(0, 0, 1),
		XMFLOAT3(0, 0,-1),
		XMFLOAT3(1, 0, 0),
		XMFLOAT3(-1, 0, 0),
		XMFLOAT3(0, 1, 0),
		XMFLOAT3(0,-1, 0),
	};
	//４角ポリゴンのインデックス
	int Idx[6][4] = {
		0,1,2,3,
		4,5,6,7,
		1,5,3,7,
		0,4,2,6,
		4,5,0,1,
		6,7,2,3
	};
	//このCELLに属する三角ポリゴン---------------------------------------------------------
	std::vector<Triangle*>Triangles;
	int NumTriangles = 0;

public:
	//Axis Aligned Cube Boxの一辺の長さと頂点座標P[8]設定
	void setLen_Pos(float len, float x, float y, float z) {};
	
	//三角ポリゴンがAABに触れていたらこのCELLに登録
	void regist(Triangle& t) {};
	
	//カプセルと壁、天井との当たり判定処理
	int capsule_triangles(XMFLOAT3* pos, float* dirY, int* jumpFlag, float radius, float height, float degree, int* step) const { return 0; };
	
	//カプセルと床ポリゴンとの当たり判定処理
	int capsule_floor_triangles(XMFLOAT3* pos, float* dirY, int* jumpFlag, float speed, float radius, float height, float degree, int step) const 
	{ return 0; };
	
	//
	int segment_triangles(XMFLOAT3& sp, XMFLOAT3* ep, float correct) { return 0; };

	int select(const XMFLOAT3& sp, const XMFLOAT3& ep) { return 0; };
	
	//Getter
	XMFLOAT3 GetPosision() { return position_; }
	std::vector<Triangle*>& GetTriangles() { return Triangles; }

};

class Quad
{
	//コンスタントバッファー
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;
		XMMATRIX	matNormal; //matWからNormal用に改名
	};

	//頂点情報
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR normal;
	};

	Transform transform_;

protected:
	std::vector<int> index_; //インデックス情報
	int indexNum_;
	int vertexNum_;
	std::vector<VERTEX> vertices_;

	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ

public:
	Quad();
	virtual ~Quad();

	void Release();
	HRESULT Initialize();
	void Draw();

	void SetTransform(Transform& _t) { transform_ = _t; };
	void SetPosition(XMFLOAT3& _t) { transform_.position_ = _t; };

private:
	//  Initializeから呼ばれる関数-----------------
	virtual void InitVertexData(); //頂点情報の準備
	HRESULT CreateVertexBuffer();  //頂点バッファを作成

	virtual void InitIndexData();  //インデックス情報を準備
	HRESULT CreateIndexBuffer();  //インデックスバッファを作成

	HRESULT CreateConstantBuffer(); //コンスタントバッファを作成

	// Draw関数から呼ばれる関数--------------------
	void PassDataToCB(Transform transform);	//コンスタントバッファに各種情報を渡す
	void SetBufferToPipeline();                         //各バッファ
};