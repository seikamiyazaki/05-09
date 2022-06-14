#pragma once
#include "Function.h"

class Enemy
{
public:
	Enemy();
	~Enemy();
	void Initialize(Model* mode, const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	DebugText* debugText_ = nullptr;

	Vector3 velocity_;
};

