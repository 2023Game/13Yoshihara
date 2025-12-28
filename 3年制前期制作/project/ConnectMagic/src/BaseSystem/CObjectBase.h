#pragma once
#include "CTask.h"
#include "CTransform.h"
#include "ObjectTag.h"
#include "CColor.h"

class CNavNode;
class CCollider;
struct CollisionData;
class CHitInfo;
// Bullet関連
class btCollisionObject;
class btRigidBody;
class btCollisionShape;
class btDefaultMotionState;
class btTriangleIndexVertexArray;

/// <summary>
/// 3D空間に配置するオブジェクトのベースクラス
/// </summary>
class CObjectBase : public CTask, public CTransform
{
public:
	using CTransform::Position;
	using CTransform::Rotation;

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

	// 剛体を取得
	btRigidBody* GetRigidBody() const;
	// 剛体を設定
	void SetRigidBody(btRigidBody* body,
		btCollisionShape* shape,
		btDefaultMotionState* ms,
		float halfHeightY,
		btTriangleIndexVertexArray* indexVertexArray = nullptr);

	// センサーを取得
	btCollisionObject* GetSensor() const;
	// センサーを設定
	void SetSensor(btCollisionObject* sensor);

	// 力を加える（加速する動き）
	void AddForce(const CVector& force);
	// インパルスを加える（瞬発的な動き）
	void AddImpulse(const CVector& impulse);
	// 力をリセット
	void ResetForce();

	// 剛体の半分の高さを取得
	float GetHalfHeight() const;
	// 剛体の半分の高さを設定
	void SetHalfHeight(float halfHeight);

	// 座標を設定
	void Position(const CVector& pos) override;
	// 回転を設定
	void Rotation(const CQuaternion& rot) override;


private:
	ETag mTag;			// オブジェクト識別用のタグ
	bool mIsEnableCol;	// 衝突判定を行うかどうか

	// Bullet Physics関連
	btRigidBody* mpRigidBody;						// 剛体本体
	btCollisionShape* mpCollisionShape;				// 衝突形状
	btDefaultMotionState* mpMotionState;			// モーションステート
	float mHalfHeightY;								// 剛体の半分の高さ
	btTriangleIndexVertexArray* mpIndexVertexArray;	// メッシュデータ
	btCollisionObject* mpSensorCol;					// 探知用のコライダー

	// 剛体の座標を設定
	void SetBodyPos(const CVector& pos);
	// 剛体の回転を設定
	void SetBodyRot(const CQuaternion& rot);

protected:

	// 衝突イベントのチェックと配布
	void DispatchCollisionEvents();
	// 衝突処理
	virtual void OnCollision(const CollisionData& data);
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
