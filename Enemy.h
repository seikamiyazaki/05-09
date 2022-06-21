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
	// �ڋ߃t�F�[�Y������
	void ApproachInitialize();

	// ���ˊԊu
	static const int kFireInteral = 60;

	void SetPlayer(Player* player) { player_ = player; }

	// ���[���h���W���擾
	Vector3 GetWorldPosition();

private:
	WorldTransform worldTransform_;
	DebugText* debugText_ = nullptr;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Vector3 velocity_;

	// �s���t�F�[�Y
	enum class Phase {
		Approach,// �ڋ߂���
		Leave,// ���E����
	};
	Phase phase_ = Phase::Approach;// �����t�F�[�Y

	std::list<std::unique_ptr<EnemyBullet>> enemyBullet_;// �e

	// ���˃^�C�}�[
	int32_t kFireTimer = 0;

	Player* player_ = nullptr;
};