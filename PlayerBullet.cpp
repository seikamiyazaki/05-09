#include "PlayerBullet.h"
#include <cassert>

PlayerBullet::PlayerBullet() {}
PlayerBullet::~PlayerBullet() {}

void PlayerBullet::Initialize(Model* model, const Vector3& position)
{
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("black.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
}

void PlayerBullet::Update()
{
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	worldTransform_.matWorld_ *= MatRotYCreate1(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= MatTransCreate(worldTransform_.translation_);

	worldTransform_.TransferMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);// Ž©ƒLƒƒƒ‰‚Ì•`‰æ
}
