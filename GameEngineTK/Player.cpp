/* =====================================================================
//! @param		「Player」クラスのソースファイル
//! @create		樋口　裕太
//! @date		17/06/01
===================================================================== */

// ヘッダファイルのインクルード
#include "Player.h"
#include <SimpleMath.h>

// 名前空間
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

bool Player::m_isFire;

/* =====================================================================
//! 内　容		初期化
//! 引　数		なし
//! 戻り値		なし
===================================================================== */
void Player::Initialize()
{
	// パーツのモデル読み込み
	LoadPartsModel();

	// パーツの親子関係を設定
	SetPartsParent();

	// 親からの座標差分を設定
	SetOffSet();

	// 根の部分（BODY）の座標を調整（少し上にあげる）
	m_parts[PARTS_BODY].SetTranslate(Vector3(0.0f, 0.6f, 0.0f));

	// ステートの初期化（何もやっていない状態）
	for (int i = 0; i < STATE_NUM; i++)
	{
		m_isState[i] = false;
	}

	m_isFire = false;
}

/* =====================================================================
//! 内　容		パーツのモデル読み込み
//! 引　数		なし
//! 戻り値		なし
===================================================================== */
void Player::LoadPartsModel()
{
	// パーツ数分の領域を確保
	m_parts.resize(PARTS_NUM);

	// モデル読み込み
	m_parts[PARTS_BODY		].LoadModel(L"Resources/bodyPik.cmo");
	m_parts[PARTS_LEFTFOOT	].LoadModel(L"Resources/leftFootPik.cmo");
	m_parts[PARTS_RIGHTFOOT	].LoadModel(L"Resources/rightFootPik.cmo");
	m_parts[PARTS_LEFTARM	].LoadModel(L"Resources/leftArmPik.cmo");
	m_parts[PARTS_RIGHTARM	].LoadModel(L"Resources/rightArmPik.cmo");
	m_parts[PARTS_HEAD		].LoadModel(L"Resources/pikmin.cmo");
	m_parts[PARTS_FLOWER	].LoadModel(L"Resources/flowerPik.cmo");
}

/* =====================================================================
//! 内　容		パーツの親子関係を設定
//! 引　数		なし
//! 戻り値		なし
===================================================================== */
void Player::SetPartsParent()
{
	// 親をセット
	m_parts[PARTS_LEFTFOOT	].SetParent(&m_parts[PARTS_BODY]);
	m_parts[PARTS_RIGHTFOOT	].SetParent(&m_parts[PARTS_BODY]);
	m_parts[PARTS_LEFTARM	].SetParent(&m_parts[PARTS_BODY]);
	m_parts[PARTS_RIGHTARM	].SetParent(&m_parts[PARTS_BODY]);
	m_parts[PARTS_HEAD		].SetParent(&m_parts[PARTS_BODY]);
	m_parts[PARTS_FLOWER	].SetParent(&m_parts[PARTS_HEAD]);
}

/* =====================================================================
//! 内　容		親からの座標差分を設定
//! 引　数		なし
//! 戻り値		なし
===================================================================== */
void Player::SetOffSet()
{
	m_parts[PARTS_LEFTFOOT	].SetTranslate(Vector3(-0.2f, 0.0f, 0.0f));
	m_parts[PARTS_RIGHTFOOT	].SetTranslate(Vector3(0.2f, 0.0f, 0.0f));
	m_parts[PARTS_LEFTARM	].SetTranslate(Vector3(-0.25f, 0.6f, 0.2f));
	m_parts[PARTS_RIGHTARM	].SetTranslate(Vector3(0.25f, 0.6f, 0.2f));
	m_parts[PARTS_HEAD		].SetTranslate(Vector3(0.0f, 0.7f, 0.0f));
	m_parts[PARTS_FLOWER	].SetTranslate(Vector3(0.0f, 1.2f, 0.1f));
}

/* =====================================================================
//! 内　容		ステートを設定
//! 引　数		ステート（PLAYER_STATE）、ステート状態（bool）
//! 戻り値		なし
===================================================================== */
void Player::SetState(PLAYER_STATE state, bool flg)
{
	m_isState[state] = flg;
}

/* =====================================================================
//! 内　容		花を回転させる
//! 引　数		なし
//! 戻り値		なし
===================================================================== */
void Player::RotateFlower()
{
	// 現在の角度を保存
	float flowerAngle = m_parts[PARTS_FLOWER].GetRotate().z;

	// 回転
	m_parts[PARTS_FLOWER].SetRotate(Vector3(0.0f, 0.0f, flowerAngle + 0.1f));
}

/* =====================================================================
//! 内　容		移動
//! 引　数		なし
//! 戻り値		なし
===================================================================== */
void Player::Move()
{
	// 前後移動
	if (m_isState[STATE_MOVE_FORWARD] || m_isState[STATE_MOVE_BACK])
	{
		// Ｚ軸の動く距離を設定
		Vector3 moveV = Vector3(0.0f, 0.0f, -0.1f);

		// 後退の場合は進む方向（Ｚ軸）を反転
		if (m_isState[STATE_MOVE_BACK])
		{
			moveV.z *= -1.0f;
		}

		// 移動量ベクトルを自機の角度分回転させる
		float angle = m_parts[PARTS_BODY].GetRotate().y;
		Matrix rotMat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotMat);

		// 平行移動行列の計算
		Vector3 pos = m_parts[PARTS_BODY].GetTranslate();
		m_parts[PARTS_BODY].SetTranslate(pos + moveV);

		// 前後移動のステートをリセット
		m_isState[STATE_MOVE_FORWARD] = false;
		m_isState[STATE_MOVE_BACK] = false;
	}

	// 旋回
	if (m_isState[STATE_MOVE_LEFT] || m_isState[STATE_MOVE_RIGHT])
	{
		// Ｙ軸の回転させる角度を設定
		float turningAngle = 0.03f;

		// 右旋回の場合は回転させる角度（Ｙ軸）を反転
		if (m_isState[STATE_MOVE_RIGHT])
		{
			turningAngle *= -1.0f;
		}

		// 自機の回転角度を保存
		float angleX = m_parts[PARTS_BODY].GetRotate().x;
		float angleY = m_parts[PARTS_BODY].GetRotate().y;
		m_parts[PARTS_BODY].SetRotate(Vector3(angleX, angleY + turningAngle, 0.0f));
	}
}

/* =====================================================================
//! 内　容		前方宙返り
//! 引　数		なし
//! 戻り値		なし
===================================================================== */
void Player::ForwardSomersault()
{
	// 自機の回転角度と座標を保存
	float angleX = m_parts[PARTS_BODY].GetRotate().x;
	float angleY = m_parts[PARTS_BODY].GetRotate().y;
	Vector3 transMat = m_parts[PARTS_BODY].GetTranslate();

	// 前宙
	if (angleX > -XM_2PI)
	{
		m_parts[PARTS_BODY].SetRotate(Vector3(angleX - 0.1f, angleY, 0.0f));
		m_parts[PARTS_BODY].SetTranslate(Vector3(transMat.x, transMat.y - sinf(angleX) * 0.2f, transMat.z));
	}
	else
	{
		// 角度を初期（直立）に戻す
		m_parts[PARTS_BODY].SetRotate(Vector3(0.0f, angleY, 0.0f));

		// ステートの前宙状態を終える
		m_isState[STATE_TURN] = false;
	}
}

/* =====================================================================
//! 内　容		花を浮かせる
//! 引　数		なし
//! 戻り値		なし
===================================================================== */
void Player::FloatFlowers()
{
	// 現在の花の位置を保存
	Vector3 pos = m_parts[PARTS_FLOWER].GetTranslate();

	if (m_isState[STATE_FLOAT])
	{
		// 花を浮かす
		if (pos.y < 3.0f)
		{
			m_parts[PARTS_FLOWER].SetTranslate(pos + Vector3(0.0f, 0.05f, 0.0f));
		}
	}
	else
	{
		// 花をもとの位置に戻す
		if (pos.y > 1.2f)
		{
			m_parts[PARTS_FLOWER].SetTranslate(pos + Vector3(0.0f, -0.05f, 0.0f));
		}
	}
}

/* =====================================================================
//! 内　容		股割り
//! 引　数		なし
//! 戻り値		なし
===================================================================== */
void Player::Splits()
{
	// 現在の位置と足の角度を保存
	Vector3 pos = m_parts[PARTS_BODY].GetTranslate();
	Vector3 angleLeft = m_parts[PARTS_LEFTFOOT].GetRotate();
	Vector3 angleRight = m_parts[PARTS_RIGHTFOOT].GetRotate();

	if (m_isState[STATE_SPLITS])
	{
		// 股を開く
		if (pos.y > 0.15f)
		{
			m_parts[PARTS_BODY].SetTranslate(pos + Vector3(0.0f, -0.02f, 0.0f));
			m_parts[PARTS_LEFTFOOT].SetRotate(angleLeft + Vector3(0.0f, 0.0f, -0.03f));
			m_parts[PARTS_RIGHTFOOT].SetRotate(angleRight + Vector3(0.0f, 0.0f, 0.03f));
		}
	}
	else
	{
		// 股を戻す
		if (pos.y < 0.6f)
		{
			m_parts[PARTS_BODY].SetTranslate(pos + Vector3(0.0f, 0.02f, 0.0f));
			m_parts[PARTS_LEFTFOOT].SetRotate(angleLeft + Vector3(0.0f, 0.0f, 0.03f));
			m_parts[PARTS_RIGHTFOOT].SetRotate(angleRight + Vector3(0.0f, 0.0f, -0.03f));
		}
	}
}

/* =====================================================================
//! 内　容		パーツを発射させる
//! 引　数		なし
//! 戻り値		なし
===================================================================== */
void Player::FireBurret()
{
	m_isFire = false;

	// 発射するパーツのワールド行列を取得
	Matrix worldM = m_parts[PARTS_FLOWER].GetWorldMatrix();

	// ワールド行列から各要素を抽出する
	Vector3 scale;
	Quaternion rotate;
	Vector3 translate;

	worldM.Decompose(scale, rotate, translate);

	// 発射パーツを親から分離させて独立
	m_parts[PARTS_FLOWER].SetParent(nullptr);
	m_parts[PARTS_FLOWER].SetScale(scale);
	m_parts[PARTS_FLOWER].SetRotateQ(rotate);
	m_parts[PARTS_FLOWER].SetTranslate(translate);

	// 弾丸の速度を設定
	m_bulletVel = Vector3(0.0f, 0.0f, -0.5f);
	m_bulletVel = Vector3::Transform(m_bulletVel, rotate);

	m_isFire = true;
}

/* =====================================================================
//! 内　容		発射したパーツをリセットする
//! 引　数		なし
//! 戻り値		なし
===================================================================== */
void Player::ResetBurret()
{
	if (!m_isFire) return;

	// 親子関係を戻す
	m_parts[PARTS_FLOWER].SetParent(&m_parts[PARTS_HEAD]);

	// それぞれの要素を設定
	m_parts[PARTS_FLOWER].SetScale(Vector3(1.0f, 1.0f, 1.0f));
	m_parts[PARTS_FLOWER].SetRotate(Vector3(0.0f, 0.0f, 0.0f));
	m_parts[PARTS_FLOWER].SetTranslate(Vector3(0.0f, 1.2f, 0.1f));
}

/* =====================================================================
//! 内　容		パーツを取得する
//! 引　数		パーツ（PLAYER_PARTS）
//! 戻り値		パーツオブジェクト（Obj3d*）
===================================================================== */
Obj3d& Player::GetParts(PLAYER_PARTS parts)
{
	return m_parts[parts];
}

/* =====================================================================
//! 内　容		更新処理
//! 引　数		なし
//! 戻り値		なし
===================================================================== */
void Player::Update()
{
	// 各パーツの更新
	for (vector<Obj3d>::iterator itr = m_parts.begin(); itr != m_parts.end(); itr++)
	{
		(*itr).Update();
	}

	// ステートによって処理を行う
	{
		// 移動
		Move();

		// 前方宙返り
		if (m_isState[STATE_TURN])
		{
			ForwardSomersault();
		}

		// 花を浮かせる
		FloatFlowers();

		// 股割りする
		Splits();
	}

	// 発射されたパーツの処理
	if (m_isFire)
	{
		Vector3 pos = m_parts[PARTS_FLOWER].GetTranslate();
		m_parts[PARTS_FLOWER].SetTranslate(pos + m_bulletVel);
	}

	// 花を回転させる
	RotateFlower();
}

/* =====================================================================
//! 内　容		描画処理
//! 引　数		なし
//! 戻り値		なし
===================================================================== */
void Player::Draw()
{
	// 各パーツの更新
	for (vector<Obj3d>::iterator itr = m_parts.begin(); itr != m_parts.end(); itr++)
	{
		(*itr).Draw();
	}
}