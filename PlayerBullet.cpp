#include "PlayerBullet.h"
#include <cassert>

PlayerBullet::PlayerBullet() {}
PlayerBullet::~PlayerBullet() {}

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("black.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
	velocity_ = velocity;
}

void PlayerBullet::Update()
{
	// 座標を移動させる
	worldTransform_.translation_ += velocity_;
	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	worldTransform_.matWorld_ = MatScaleCreate(worldTransform_.scale_);
	worldTransform_.matWorld_ *= MatRotYCreate1(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= MatTransCreate(worldTransform_.translation_);

	worldTransform_.TransferMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

Vector3 PlayerBullet::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}