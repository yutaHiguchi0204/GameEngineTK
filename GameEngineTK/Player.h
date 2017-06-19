/* =====================================================================
//! @param		「Player」クラスのヘッダファイル
//! @create		樋口　裕太
//! @date		17/06/01
===================================================================== */
#pragma once

// ヘッダファイルのインクルード
#include <vector>
#include "CollisionNode.h"
#include "Obj3d.h"

// クラスの定義
class Player
{
public:

	// プレイヤーのパーツ
	enum PLAYER_PARTS
	{
		PARTS_BODY,
		PARTS_LEFTFOOT,
		PARTS_RIGHTFOOT,
		PARTS_LEFTARM,
		PARTS_RIGHTARM,
		PARTS_HEAD,
		PARTS_FLOWER,

		PARTS_NUM
	};

	// プレイヤーステート
	enum PLAYER_STATE
	{
		STATE_MOVE_FORWARD,
		STATE_MOVE_BACK,
		STATE_MOVE_LEFT,
		STATE_MOVE_RIGHT,
		STATE_TURN,
		STATE_FLOAT,
		STATE_SPLITS,

		STATE_NUM
	};

	static bool m_isFire;

private:

	std::vector<Obj3d> m_parts;							// プレイヤーのパーツ

	bool m_isState[STATE_NUM];							// ステート

	DirectX::SimpleMath::Vector3 m_bulletVel;			// 弾丸の速度ベクトル

	SphereNode m_collisionNodeBullet;					// 弾丸用の当たり判定

public:

	Player() {};										// コンストラクタ
	~Player() {};										// デストラクタ

	void Initialize();									// 初期化
	void LoadPartsModel();								// パーツのモデル読み込み
	void SetPartsParent();								// パーツの親子関係を設定
	void SetOffSet();									// 親からの座標差分を設定

	void SetState(PLAYER_STATE state, bool flg);		// ステート設定

	void RotateFlower();								// 花を回転させる

	void Move();										// 移動
	void ForwardSomersault();							// 前方宙返り
	void FloatFlowers();								// 花を浮かせる
	void Splits();										// 股割り（開脚）

	void FireBurret();									// パーツを発射する
	void ResetBurret();									// 発射させたパーツをリセットする

	Obj3d& GetParts(PLAYER_PARTS parts);				// パーツを取得

	const SphereNode& GetCollisionNodeBullet() { return m_collisionNodeBullet; };

	void Update();										// 更新処理
	void Draw();										// 描画処理
};