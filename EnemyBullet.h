#pragma once
#include "Function.h"

class EnemyBullet
{
public:
	EnemyBullet();
	~EnemyBullet();
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	bool IsDead() const { return isDead_; }

	// ���[���h���W���擾
	Vector3 GetWorldPosition();
	// �Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Vector3 velocity_;

	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;
};