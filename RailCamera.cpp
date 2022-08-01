#include "RailCamera.h"
#include "Function.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& rot)
{
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rot;

	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();

	debugText_ = DebugText::GetInstance();
}

void RailCamera::Update()
{
	// �ړ��x�N�g�������Z
	worldTransform_.translation_ += Vector3{ 0,0,0.1f };
	// �s��Čv�Z
	MatWorldCreate(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// ���[���h�s��̕��s�ړ��������擾�i���[���h���W�j
	viewProjection_.eye = worldTransform_.translation_;
	// ���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	// ���[���J�����̉�]�𔽉f�i���[���J�����̑O���x�N�g���j
	forward = Vector3TransformNormal(forward, worldTransform_);
	// ���_����O���ɓK���ȋ����i�񂾍������ӓ_
	forward += viewProjection_.eye;
	viewProjection_.target = forward;
	// ���[���h����x�N�g��
	Vector3 up(0, 1, 0);
	// ���[���J�����̉�]�𔽉f�i���[���J�����̏���x�N�g���j
	viewProjection_.up = Vector3TransformNormal(up, worldTransform_);

	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	worldTransform_.matWorld_ *= 	MatWorldCreate(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// �r���[�v���W�F�N�V�������X�V
	viewProjection_.UpdateMatrix();
	// �f�o�b�O�p�\��
	debugText_->SetPos(50, 150);
	debugText_->Printf("RailCameraPos:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
}