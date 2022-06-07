#pragma once
#include "Function.h"
#include "Model.h"
#include "WorldTransform.h"

class PlayerBullet
{
public:
	PlayerBullet();
	~PlayerBullet();
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	bool IsDead() const { return isDead_; }

private:
	WorldTransform worldTransform_;// ���[���h�ϊ��f�[�^
	Model* model_ = nullptr;// ���f��
	uint32_t textureHandle_ = 0u;// �e�N�X�`���n���h��
	// ���x
	Vector3 velocity_;
	// ����
	static const int32_t kLifeTime = 60 * 5;
	// �f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;
	// �f�X�t���O
	bool isDead_ = false;
};