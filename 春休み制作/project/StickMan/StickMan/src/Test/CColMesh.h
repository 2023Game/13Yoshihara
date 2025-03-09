#pragma once
#include "CColBase.h"
#include "CColliderMesh.h"

class CColMesh : public CColBase
{
public:
	// コンストラクタ
	CColMesh(CModel* model, const CColor& defaultColor,
		bool isKinematic = false, float weight = 1.0f);
	// デストラクタ
	~CColMesh();

	/// <summary>
	/// 衝突判定
	/// </summary>
	/// <param name="self">衝突した自身のコライダ―</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 描画
	void Render() override;
private:
	// メッシュコライダ―
	CColliderMesh* mpCollider;
	// 三角コライダの配列
	std::list<STVertexData> mVertices;
};