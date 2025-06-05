#pragma once
#include "CCollider.h"

/// <summary>
/// 四角形コライダ―
/// </summary>
class CColliderRectangle : public CCollider
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">コライダ―の持ち主</param>
	/// <param name="layer">衝突判定用レイヤー</param>
	/// <param name="v0">四角形の頂点 0</param>
	/// <param name="v1">四角形の頂点 1</param>
	/// <param name="v2">四角形の頂点 2</param>
	/// <param name="v3">四角形の頂点 3</param>
	/// <param name="isKinematic">trueならば、衝突時に押し戻しの影響を受けない</param>
	/// <param name="weight">コライダ―の重量</param>
	CColliderRectangle(CObjectBase* owner, ELayer layer,
		const CVector& v0, const CVector& v1, 
		const CVector& v2, const CVector& v3,
		bool isKinematic = false, float weight = 1.0f);

	/// <summary>
	/// 四角形コライダ―の設定
	/// </summary>
	/// <param name="owner">コライダ―の持ち主</param>
	/// <param name="layer">衝突判定用レイヤー</param>
	/// <param name="v0">四角形の頂点 0</param>
	/// <param name="v1">四角形の頂点 1</param>
	/// <param name="v2">四角形の頂点 2</param>
	/// <param name="v3">四角形の頂点 3</param>
	void Set(CObjectBase* owner, ELayer layer,
		const CVector& v0, const CVector& v1, 
		const CVector& v2, const CVector& v3);

	/// <summary>
	/// 四角形の頂点を取得
	/// </summary>
	/// <param name="v0">頂点1格納用</param>
	/// <param name="v1">頂点2格納用</param>
	/// <param name="v2">頂点3格納用</param>
	/// <param name="v3">頂点4格納用</param>
	void Get(CVector* v0, CVector* v1,
		CVector* v2, CVector* v3) const;

	// コライダ―描画
	void Render() override;
protected:
	// コライダ―の情報を更新
	void UpdateCol() override;
private:
	CVector mV[4];	// 四角形の頂点の配列
	CVector mWV[4];	// 四角形の頂点のワールド座標
};