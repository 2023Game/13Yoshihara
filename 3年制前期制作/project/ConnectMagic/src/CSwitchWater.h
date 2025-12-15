#pragma once
#include "CSwitchObject.h"

class CWater;

// スイッチで水位が下げられる水クラス
class CSwitchWater : public CSwitchObject
{
public:
	// CSavableの純粋仮想関数のオーバーライド
	std::vector<char> SaveState() const override;
	void LoadState(const std::vector<char>& data) override;
	size_t GetTypeID() const override;
	unsigned int GetUniqueInstanceID() const override;

	CSwitchWater(const CVector& pos, const CVector& downPos, const CVector& scale);
	~CSwitchWater();

private:
	// データ保存で使用
	void SetElapsedTime(float time);
	float GetElapsedTime() const;
	unsigned int mUniqueID;

	// 作用していないときの更新
	void UpdateOff() override;
	// 作用しているときの更新
	void UpdateOn() override;

	void SetEnable(bool enable) override;
	
	void ChangeState(EState state) override;

	// 初期座標
	CVector mDefaultPos;
	// 下がった後の座標
	CVector mDownPos;
	// 経過時間
	float mElapsedTime;

	CWater* mpWater;
};