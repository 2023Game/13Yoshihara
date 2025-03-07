#pragma once
#include "CCharaBase.h"
#include "RoadType.h"

class CModel;
class CNavNode;
class CTrashVehicleManager;
class CFlamethrower;
class CSound;

// 前方コライダ―の座標
#define FRONT_COL_OFFSET_POS	CVector(0.0f,0.0f,60.0f)
// 横コライダーの座標
#define LEFT_COL_OFFSET_POS		CVector(-40.0f,0.0f,0.0f)	// 左
#define RIGHT_COL_OFFSET_POS	CVector( 40.0f,0.0f,0.0f)	// 右

// 車両の基底クラス
class CTrashVehicleBase : public CCharaBase
{
	friend CTrashVehicleManager;
public:
	// コンストラクタ
	CTrashVehicleBase(CModel* model, const CVector& pos, const CVector& rotation,
		ERoadType road, std::vector<CNavNode*> patrolPoints);
	// デストラクタ
	~CTrashVehicleBase();
	/// <summary>
	/// ダメージを受ける
	/// </summary>
	/// <param name="damage">受けるダメージ</param>
	/// <param name="causer">攻撃してきた相手</param>
	virtual void TakeDamage(int damage, CObjectBase* causer) = 0;
	/// <summary>
	/// クリティカルダメージを受ける
	/// </summary>
	/// <param name="damage">受けるダメージ</param>
	/// <param name="causer">攻撃してきた相手</param>
	virtual void TakeCritical(int damage, CObjectBase* causer) = 0;

	// 更新
	void Update();
	// 衝突処理
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);
	// 描画
	void Render();

	/// <summary>
	/// 移動中かどうか
	/// </summary>
	/// <returns>trueならば、移動中</returns>
	bool IsMove() const;
	// 最後の巡回ポイントまでの移動が終了したかどうか
	bool GetMoveEnd() const;

	// 壊れているか
	bool IsBroken() const;

	// 前方に停止している車両がいるかどうか
	bool GetIsFrontVehicle() const;

	/// <summary>
	/// 車線を変更する
	/// </summary>
	/// <param name="isEnd">trueならば、終了した</param>
	void ChangeRoad(bool& isEnd);

	// どの道にいる状態かを変更する
	void ChangeRoadType(ERoadType roadType);
	// 今どの道にいるか取得する
	ERoadType GetRoadType() const;

	// 本体コライダ―を取得する
	CCollider* GetBodyCol() const;

	// 巡回ポイントのリストを設定する
	void SetPatrolPoints(std::vector<CNavNode*> patrolPoints);

	// 車両の有効無効を切り替える
	virtual void SetOnOff(bool setOnOff);
	// 変数をリセット
	virtual void Reset();
protected:
	// どの道にいる状態か
	ERoadType mRoadType;
	// 今の道の進む方向
	CVector mCurrentRoadRotation;
	// 移動しているか
	bool mIsMove;
	// 壊れているか
	bool mIsBroken;

	// 移動速度
	CVector mMoveSpeed;

	CModel* mpModel;

	// 車両の周りのノード
	CNavNode* mpNode0;
	CNavNode* mpNode1;
	CNavNode* mpNode2;
	CNavNode* mpNode3;

	// 本体のコライダ―
	CCollider* mpBodyCol;
	// 前方に車がいるかの確認用コライダ―
	CCollider* mpFrontCol;
	// 横に車がいるかの確認用コライダ―
	CCollider* mpSideCol;
	// 炎のエフェクト
	CFlamethrower* mpFlamethrower;

	// 指定した位置まで移動する
	bool MoveTo(const CVector& targetPos, float speed, float rotateSpeed);
	// 次に巡回するポイントを変更
	void ChangePatrolPoint();
	/// <summary>
	/// 車線変更で移動するノードの座標を設定する
	/// </summary>
	/// <param name="frontVehicle">前にいる車のポインタ</param>
	void SetChangeRoadPoint(CTrashVehicleBase* frontVehicle);

	// 車線変更時の移動先のノード
	CNavNode* mpChangeRoadPoint;

	// 巡回ポイントのリスト
	std::vector<CNavNode*> mpPatrolPoints;
	int mNextPatrolIndex;	// 次に巡回するポイントの番号

	std::vector<CNavNode*> mpMoveRoute;	// 求めた最短経路記憶用
	int mNextMoveIndex;					// 次に移動するノードのインデックス値

	bool mIsMoveEnd;	// 最後まで移動したかどうか
	bool mIsMovePause;	// 移動の中断中かどうか

	bool mIsFrontVehicle;	// 前方に停止している車両がいるかどうか
	bool mIsSideVehicle;	// 横に車両がいるかどうか

	// 効果音
	CSound* mpCriticalSE;
};