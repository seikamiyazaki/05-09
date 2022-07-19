#include "Skydome.h"
#include <cassert>

void Skydome::Intialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 200.0f,200.0f,200.0f };
	worldTransform_.translation_ = { 0,0,0 };
}

void Skydome::Update()
{
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	worldTransform_.matWorld_ *= MatScaleCreate(worldTransform_.scale_);
	worldTransform_.matWorld_ *= MatTransCreate(worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Skydome::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(worldTransform_, viewProjection_);
}