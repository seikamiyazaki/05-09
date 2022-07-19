#include "Enemy.h"
#include <cassert>
#include "Player.h"

Enemy::Enemy() {}
Enemy::~Enemy() {}

void Enemy::Initialize(Model* model, const Vector3& velocity)
{
	assert(model);

	debugText_ = DebugText::GetInstance();

	model_ = model;
	textureHandle_ = TextureManager::Load("kamata.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = { 10.0f,2.0f,20.0f };
	velocity_ = velocity;

	// 接近フェーズ初期化
	ApproachInitialize();
}

void Enemy::Update()
{
	switch (phase_)
	{
	case Phase::Approach:
		//// 移動ベクトル
		//worldTransform_.translation_ += Vector3(0.0f, 0.0f, -0.1f);
		//// 規定の位置に到達したら離脱
		//if (worldTransform_.translation_.z < 0.0f) {
		//	phase_ = Phase::Leave;
		//}
		ApproachInitialize();
		break;
	//case Phase::Leave:
	//	// 移動ベクトル
	//	worldTransform_.translation_ += Vector3(-0.1f, 0.1f, 0);
	//	break;
	}

	// デスフラグの立った弾を削除
	enemyBullet_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

	for (std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullet_) {
		enemyBullet->Update();
	}

	//worldTransform_.translation_ -= velocity_;

	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	worldTransform_.matWorld_ *= MatRotYCreate1(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= MatTransCreate(worldTransform_.translation_);

	worldTransform_.TransferMatrix();

	// デバッグ用表示
	debugText_->SetPos(50, 90);
	debugText_->Printf("enemy:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// 弾描画
	for (std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullet_) {
		enemyBullet->Draw(viewProjection);
	}
}

void Enemy::Fire()
{
	// 弾の速度
	const float kBulletSpeed = 1.0f;
	// 座標の取得
	Vector3 playerPos = player_->GetWorldPosition();
	Vector3 enemyPos = this->GetWorldPosition();
	// 差分ベクトル = ゴール地点 - スタート地点
	Vector3 velocity = playerPos;
	velocity -= enemyPos;
	// ベクトルの正規化
	velocity.normalize();
	// ベクトルの長さを、早さに合わせる
	velocity *= kBulletSpeed;
	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = VW(velocity, worldTransform_);
	// 弾を発生し、初期化
	std::unique_ptr<EnemyBullet> newEnemyBullet = std::make_unique<EnemyBullet>();
	newEnemyBullet->Initialize(model_, worldTransform_.translation_, velocity);
	// 弾を登録する
	enemyBullet_.push_back(std::move(newEnemyBullet));
}

void Enemy::ApproachInitialize()
{
	// 発射タイマーカウントダウン
	kFireTimer--;
	// 指定時間に達した
	if (kFireTimer < 0) {
		// 弾を発射
		Fire();
		// 発射タイマーを初期化
		kFireTimer = kFireInteral;
	}
}

Vector3 Enemy::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void Enemy::OnCollision()
{
}
