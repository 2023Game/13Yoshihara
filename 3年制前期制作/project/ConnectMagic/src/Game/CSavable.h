#pragma once
#include "CUIDManager.h"

// 保存・ロード可能なすべてのゲームオブジェクトが継承する
class CSavable
{
public:
	virtual ~CSavable() = default;

	// 状態をバイト列としてシリアライズし、そのオブジェクト固有のセーブデータの中身を定義する
	virtual std::vector<char> SaveState() const = 0;

	// バイト列から状態をデシリアライズし、オブジェクトに復元
	virtual void LoadState(const std::vector<char>& data) = 0;

	// オブジェクトのタイプを一意に識別するためのID
	virtual size_t GetTypeID() const = 0;

	// オブジェクトを一意に識別するための永続的なID
	virtual unsigned int GetUniqueInstanceID() const = 0;
};