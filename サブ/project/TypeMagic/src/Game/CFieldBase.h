#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

class CFieldBase : public CObjectBase
{
public:
	// インスタンスのポインタの取得
	static CFieldBase* Instance();

	// コンストラクタ
	CFieldBase();
	// デストラクタ
	~CFieldBase();

	/// <summary>
	/// レイとフィールドオブジェクトとの衝突判定
	/// </summary>
	/// <param name="start">レイの開始位置</param>
	/// <param name="end">レイの終了位置</param>
	/// <param name="hit">衝突情報返却用</param>
	/// <returns>衝突していたら、trueを返す</returns>
	bool CollisionRay(const CVector& start, const CVector& end,
		CHitInfo* hit) override;

	// 更新
	virtual void Update();
	// 描画
	virtual void Render();

	// 地面のコライダ―取得
	CCollider* GetGroundCol();
	// 壁のコライダ―取得
	CCollider* GetWallCol();
	// オブジェクトのコライダ―取得
	CCollider* GetObjCol();

protected:
	// フィールドのオブジェクトを生成
	virtual void CreateFieldObjects() = 0;
	// 経路探索用のノードを作成
	virtual void CreateNavNodes();
	// コライダ―を生成する
	void CreateCol(
		std::string groundCol,
		std::string wallCol,
		std::string objCol);

	// フィールドのモデル
	CModel* mpModel;
	// 空のモデル
	CModel* mpSkyModel;
	// フィールドの地面の衝突判定
	CColliderMesh* mpGroundColliderMesh;
	// フィールドの壁の衝突判定
	CColliderMesh* mpWallColliderMesh;
	// フィールドのオブジェクトの衝突判定
	CColliderMesh* mpObjectColliderMesh;

	// フィールドのインスタンス
	static CFieldBase* spInstance;
};