#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

class CFieldBase : public CObjectBase
{
public:
	// フィールドのインスタンスを返す
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

protected:
	// フィールドのオブジェクトを生成
	virtual void CreateFieldObjects() = 0;

	// フィールドのインスタンス
	static CFieldBase* spInstance;
	// フィールドのモデル
	CModel* mpModel;
	// フィールドの地面の衝突判定
	CColliderMesh* mpGroundColliderMesh;
	// フィールドの壁の衝突判定
	CColliderMesh* mpWallColliderMesh;
	// フィールドのオブジェクトの衝突判定
	CColliderMesh* mpObjectColliderMesh;
};