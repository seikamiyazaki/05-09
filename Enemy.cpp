#include "Enemy.h"
#include <cassert>

Enemy::Enemy(){}
Enemy::~Enemy(){}

void Enemy::Initialize(Model* model, const Vector3& velocity)
{
	assert(model);

	debugText_ = DebugText::GetInstance();

	model_ = model;
	textureHandle_ = TextureManager::Load("kamata.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = { 0,2.0f,0 };
	velocity_ = velocity;
}

void Enemy::Update()
{
	worldTransform_.translation_ -= velocity_;

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
}