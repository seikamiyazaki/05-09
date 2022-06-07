#pragma once
#include "Function.h"
#include "Model.h"
#include "WorldTransform.h"

class PlayerBullet
{
public:
	PlayerBullet();
	~PlayerBullet();
	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;// ワールド変換データ
	Model* model_ = nullptr;// モデル
	uint32_t textureHandle_ = 0u;// テクスチャハンドル
};