#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"
#include"PrimitiveDrawer.h"

GameScene::GameScene() {}
GameScene::~GameScene()
{
	delete model_;
	delete player_;
	delete enemy_;
}

void GameScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("mario.jpg");// ファイルを指定してテクスチャを読み込む
	model_ = Model::Create();// 3Dモデルの生成

	viewProjection_.Initialize();// ビュープロジェクションの初期化

	//debugCamera_ = new DebugCamera(50, 50);// デバッグカメラの生成
	AxisIndicator::GetInstance()->SetVisible(true);// 軸方向表示を有効にする
	// 軸方向表示が参照するビュープロジェクションを指定（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	// ライン描画が参照するビュープロジェクションを指定（アドレス渡し）
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	player_ = new Player();
	player_->Initialize(model_, textureHandle_);
	//std::unique_ptr<Player> newPlayer = std::make_unique<Player>();
	//newPlayer->Initialize(model_, textureHandle_);

	Vector3 velocity(0, 0, 0.2f);
	enemy_ = new Enemy();
	enemy_->Initialize(model_, velocity);
	//std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	//newEnemy->Initialize(model_, velocity);

	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);
}

void GameScene::Update()
{
	player_->Update();// 自キャラの更新
	enemy_->Update();

	viewProjection_.UpdateMatrix();// 行列の再計算

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
		int pos;
		pos = ((posB.x - posA.x) * (posB.x - posA.x)) + ((posB.y + posA.y) * (posB.y + posA.y)) + ((posB.z - posA.z) * (posB.z - posA.z));
		int rA;
		int rB;
		rA = (posA.x + 64) / 2;
		rB = (posB.x + 64) / 2;
		// 球と球の交差判定
		if (pos <= (rA + rB) * (rA + rB)) {
			// 衝突時コールバックを呼び出す
			player_->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
#pragma endregion
}