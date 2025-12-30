#pragma once
#include "CObjectBase.h"

class CModel;

class CFieldBase : public CObjectBase
{
public:
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
	bool CollisionRay(const CVector& start, const CVector& end) override;

	// 更新
	virtual void Update();
	// 描画
	virtual void Render();

protected:
	// フィールドのオブジェクトを生成
	virtual void CreateFieldObjects() = 0;
	// 経路探索用のノードを作成
	virtual void CreateNavNodes();
	// コライダ―を生成する
	void CreateCol(std::string fieldCol);

	// フィールドのモデル
	CModel* mpModel;
	// 空のモデル
	CModel* mpSkyModel;
	// メッシュのデータ
	std::vector<float> mMeshVertices;
	std::vector<int> mMeshIndices;
};