#pragma once
#include "CCollider.h"

/// <summary>
/// ボックスコライダ
/// </summary>
class CColliderBox : public CCollider
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">コライダ―の持ち主</param>
	/// <param name="layer">衝突判定用レイヤー</param>
	/// <param name="v0">前面左上　ボックスの頂点 0</param>
	/// <param name="v1">前面左下　ボックスの頂点 1</param>
	/// <param name="v2">前面右下　ボックスの頂点 2</param>
	/// <param name="v3">前面右上　ボックスの頂点 3</param>
	/// <param name="v4">背面左上　ボックスの頂点 4</param>
	/// <param name="v5">背面左下　ボックスの頂点 5</param>
	/// <param name="v6">背面右下　ボックスの頂点 6</param>
	/// <param name="v7">背面右上　ボックスの頂点 7</param>
	/// <param name="isKinematic">trueならば、衝突時に押し戻しの影響を受けない</param>
	/// <param name="weight">コライダ―の重量</param>
	CColliderBox(CObjectBase* owner, ELayer layer,
		const CVector& v0, const CVector& v1, const CVector& v2, const CVector& v3,
		const CVector& v4, const CVector& v5, const CVector& v6, const CVector& v7,
		bool isKinematic = false, float weight = 1.0f);

	/// <summary>
	/// ボックスコライダを構成する四角形コライダの設定
	/// </summary>
	/// <param name="owner">コライダ―の持ち主</param>
	/// <param name="layer">衝突判定用レイヤー</param>
	/// <param name="v0">前面左上　ボックスの頂点 0</param>
	/// <param name="v1">前面左下　ボックスの頂点 1</param>
	/// <param name="v2">前面右下　ボックスの頂点 2</param>
	/// <param name="v3">前面右上　ボックスの頂点 3</param>
	/// <param name="v4">背面左上　ボックスの頂点 4</param>
	/// <param name="v5">背面左下　ボックスの頂点 5</param>
	/// <param name="v6">背面右下　ボックスの頂点 6</param>
	/// <param name="v7">背面右上　ボックスの頂点 7</param>
	void Set(CObjectBase* owner, ELayer layer,
		const CVector& v0, const CVector& v1, const CVector& v2, const CVector& v3,
		const CVector& v4, const CVector& v5, const CVector& v6, const CVector& v7,
		bool isKinematic, float weight);
	// 四角形コライダーの配列を返す
	const std::vector<CColliderRectangle>& Get() const;

	//コライダ―描画
	void Render() override;

protected:
	// コライダ―の情報を更新
	void UpdateCol() override;

private:
	// 四角形コライダ―の配列作成
	std::vector<CColliderRectangle> mRectangles;
};