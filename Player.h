#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Input.h"
#include "ViewProjection.h"
#include "PlayerBullet.h"
#include <memory>
#include<list>

class Player
{
public:
	Player();
	~Player();
	void Initialize(Model* model, uint32_t textureHandle);//初期化
	void Update();// 更新
	void Draw(ViewProjection& viewProjection);// 描画

	void Attack();// 攻撃

private:
	WorldTransform worldTransform_;// ワールド変換データ
	Model* model_ = nullptr;// モデル
	uint32_t textureHandle_ = 0u;// テクスチャハンドル

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	std::list<std::unique_ptr<PlayerBullet>> bullets_;// 弾
};