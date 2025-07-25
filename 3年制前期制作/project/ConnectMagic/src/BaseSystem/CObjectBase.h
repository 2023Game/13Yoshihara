#pragma once
#include "CTask.h"
#include "CTransform.h"
#include "ObjectTag.h"
#include "CCollider.h"
#include "CColor.h"

class CNavNode;

/// <summary>
/// 3D空間に配置するオブジェクトのベースクラス
/// </summary>
class CObjectBase : public CTask, public CTransform
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="tag">オブジェクトのタグ</param>
	/// <param name="prio">タスクの優先度</param>
	CObjectBase(ETag tag = ETag::eNone,
		ETaskPriority prio = ETaskPriority::eDefault,
		int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eDefault,
		bool dontDelete=false,
		bool addTaskList = true);

	// デストラクタ
	virtual ~CObjectBase();

	// オブジェクト削除を伝える関数
	virtual void DeleteObject(CObjectBase* obj);

	// オブジェクトタグを取得
	ETag Tag() const;
	// オブジェクトタグを設定
	void SetTag(ETag tag);

	// 衝突判定を行うか設定
	void SetEnableCol(bool isEnable);
	// 衝突判定を行うかどうか
	bool IsEnableCol() const;

	// カラーを設定
	virtual void SetColor(const CColor& color);
	// カラーを取得
	const CColor& GetColor() const;
	// アルファ値設定
	void SetAlpha(float alpha);
	// アルファ値取得
	float GetAlpha() const;

	// カメラまでの距離を計算
	virtual void CalcDepth();
	// カメラからの距離を取得
	float GetDepth() const;

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	virtual void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);
	/// <summary>
	/// レイとオブジェクトの衝突判定
	/// </summary>
	/// <param name="start">レイの開始位置</param>
	/// <param name="end">レイの終了位置</param>
	/// <param name="hit">衝突位置返却用</param>
	/// <returns></returns>
	virtual bool CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit);

	// 攻撃中か
	virtual bool IsAttacking() const;
	/// <summary>
	/// 攻撃開始
	/// </summary>
	virtual void AttackStart();
	/// <summary>
	/// 攻撃終了
	/// </summary>
	virtual void AttackEnd();

	// 経路探索用のノードを取得
	CNavNode* GetNavNode() const;

	// 攻撃を受けているかどうか
	bool IsDamaging() const;

private:
	ETag mTag;			// オブジェクト識別用のタグ
	bool mIsEnableCol;	// 衝突判定を行うかどうか

protected:
	// すべて削除
	virtual void Delete();
	// コライダーを作成
	virtual void CreateCol();
	/// <summary>
	/// 攻撃がヒットしたオブジェクトを追加
	/// </summary>
	/// <param name="obj"></param>
	void AddAttackHitObj(CObjectBase* obj);
	/// <summary>
	/// 既に攻撃がヒットしているオブジェクトかどうか
	/// </summary>
	/// <param name="obj"></param>
	/// <returns></returns>
	bool IsAttackHitObj(CObjectBase* obj) const;

	float mDepth;		// カメラからの距離
	CColor mColor;		// オブジェクトの色

	// 攻撃がヒットしたオブジェクトのリスト
	std::list<CObjectBase*> mAttackHitObjects;

	CNavNode* mpNavNode;	// 経路探索用のノードのポインタ

	// 攻撃を受けているか
	bool mIsDamage;
};
