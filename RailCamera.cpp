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
	// 移動ベクトルを加算
	worldTransform_.translation_ += Vector3{ 0,0,0.1f };
	// 行列再計算
	MatWorldCreate(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// ワールド行列の平行移動成分を取得（ワールド座標）
	viewProjection_.eye = worldTransform_.translation_;
	// ワールド前方ベクトル
	Vector3 forward(0, 0, 1);
	// レールカメラの回転を反映（レールカメラの前方ベクトル）
	forward = Vector3TransformNormal(forward, worldTransform_);
	// 視点から前方に適当な距離進んだ差が注意点
	forward += viewProjection_.eye;
	viewProjection_.target = forward;
	// ワールド上方ベクトル
	Vector3 up(0, 1, 0);
	// レールカメラの回転を反映（レールカメラの上方ベクトル）
	viewProjection_.up = Vector3TransformNormal(up, worldTransform_);

	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	worldTransform_.matWorld_ *= 	MatWorldCreate(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// ビュープロジェクションを更新
	viewProjection_.UpdateMatrix();
	// デバッグ用表示
	debugText_->SetPos(50, 150);
	debugText_->Printf("RailCameraPos:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
}