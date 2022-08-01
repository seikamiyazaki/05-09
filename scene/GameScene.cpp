#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"
#include"PrimitiveDrawer.h"

GameScene::GameScene() {}
GameScene::~GameScene()
{
	delete model_;
	delete skydome_;
	delete modelSkydome_;
}

void GameScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	// ファイルを指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	// 3Dモデルの生成
	model_ = Model::Create();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	// 軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	// ライン描画が参照するビュープロジェクションを指定（アドレス渡し）
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	// レールカメラの生成
	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Initialize(Vector3(0, 0, -50), Vector3(0, 0, 0));

	// プレイヤー
	player_ = std::make_unique<Player>();
	player_->Initialize(model_, textureHandle_);
	player_->SetParent(railCamera_->GetWorldTransform());
	
	// エネミー
	Vector3 velocity(0, 0, 0.2f);
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(model_, velocity);
	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_.get());

	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Intialize(modelSkydome_);
}

void GameScene::Update()
{
	railCamera_->Update();
	// railCmara_をゲームシーンのカメラに適応する
	viewProjection_.matView = railCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
	// 転送
	viewProjection_.TransferMatrix();

	player_->Update();
	enemy_->Update();
	skydome_->Update();

	CheckAllCollision();

	// デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf("eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	// ここに背景スプライトの描画処理を追加できる

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	// ここに3Dオブジェクトの描画処理を追加できる
	// 3Dモデル描画
	skydome_->Draw(viewProjection_);
	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

	// ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	// ここに前景スプライトの描画処理を追加できる

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollision()
{
	// 判定対象AとBの座標
	Vector3 posA, posB;
	// 自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullet = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullet = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	// 自キャラのワールド座標
	posA = player_->GetWorldPosition();
	// 自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullet) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();
		// 座標AとBの距離を求める
		float dx = ((posB.x - posA.x) * (posB.x - posA.x));
		float dy = ((posB.y - posA.y) * (posB.y - posA.y));
		float dz = ((posB.z - posA.z) * (posB.z - posA.z));
		float dist = dx + dy + dz;
		float rA = 2.0f;
		float rB = 2.0f;
		float L = (rA + rB) * (rA + rB);

		// 球と球の交差判定
		if (dist <= L) {
			// 衝突時コールバックを呼び出す
			player_->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	// 自キャラのワールド座標
	posA = enemy_->GetWorldPosition();
	// 自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullet) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();
		// 座標AとBの距離を求める
		float dx = ((posB.x - posA.x) * (posB.x - posA.x));
		float dy = ((posB.y - posA.y) * (posB.y - posA.y));
		float dz = ((posB.z - posA.z) * (posB.z - posA.z));
		float dist = dx + dy + dz;
		float rA = 2.0f;
		float rB = 2.0f;
		float L = (rA + rB) * (rA + rB);

		// 球と球の交差判定
		if (dist <= L) {
			// 衝突時コールバックを呼び出す
			enemy_->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	// 自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullets : playerBullet) {
		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullet) {
			// 敵弾の座標
			posA = bullets->GetWorldPosition();
			posB = bullet->GetWorldPosition();
			// 座標AとBの距離を求める
			float dx = ((posB.x - posA.x) * (posB.x - posA.x));
			float dy = ((posB.y - posA.y) * (posB.y - posA.y));
			float dz = ((posB.z - posA.z) * (posB.z - posA.z));
			float dist = dx + dy + dz;
			float rA = 2.0f;
			float rB = 2.0f;
			float L = (rA + rB) * (rA + rB);

			// 球と球の交差判定
			if (dist <= L) {
				// 衝突時コールバックを呼び出す
				bullets->OnCollision();
				bullet->OnCollision();
			}
		}
	}
#pragma endregion
}