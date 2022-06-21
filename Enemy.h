#pragma once
#include "Function.h"
#include "EnemyBullet.h"

class Player;

class Enemy
{
public:
	Enemy();
	~Enemy();
	void Initialize(Model* mode, const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	void Fire();
	// 接近フェーズ初期化
	void ApproachInitialize();

	// 発射間隔
	static const int kFireInteral = 60;

	void SetPlayer(Player* player) { player_ = player; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

private:
	WorldTransform worldTransform_;
	DebugText* debugText_ = nullptr;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Vector3 velocity_;

	// 行動フェーズ
	enum class Phase {
		Approach,// 接近する
		Leave,// 離脱する
	};
	Phase phase_ = Phase::Approach;// 初期フェーズ

	std::list<std::unique_ptr<EnemyBullet>> enemyBullet_;// 弾

	// 発射タイマー
	int32_t kFireTimer = 0;

	Player* player_ = nullptr;
};