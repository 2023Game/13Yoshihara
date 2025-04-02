#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

// 道を塞ぐ壁クラス
class CBlockWall : public CObjectBase
{
public:
	// コンストラクタ
	CBlockWall();
	// デストラクタ
	~CBlockWall();

	// 描画
	void Render() override;

private:
	// コライダ―を生成
	void CreateCol();

	// コライダ―
	CCollider* mpMeshCol;
	// モデル
	CModel* mpModel;
};