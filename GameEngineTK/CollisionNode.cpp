//
// CollisionNode.cpp
// 衝突判定ノード
//

// ヘッダファイルのインクルード
#include "CollisionNode.h"

// 名前空間
using namespace DirectX::SimpleMath;

/// <summary>
/// CollisionNodeの親設定関数
/// </summary>
/// <param name="parent">親</param>
void CollisionNode::SetParent(Obj3d* parent)
{
	m_obj.SetParent(parent);
}

/// <summary>
/// SphereNodeのコンストラクタ
/// </summary>
SphereNode::SphereNode()
{
	m_radius = 1.0f;
}

/// <summary>
/// SphereNodeの初期化
/// </summary>
void SphereNode::Initialize()
{
	// デバッグ表示用モデルを読み込む
	m_obj.LoadModel(L"Resources/sphereNode.cmo");
}

/// <summary>
/// SphereNodeの更新
/// </summary>
void SphereNode::Update()
{
	m_obj.SetTranslate(m_trans);
	m_obj.SetScale(Vector3(m_radius));

	m_obj.Update();

	// ワールド座標での当たり判定球を計算
	{
		// 計算済みのワールド行列を計算
		const Matrix& worldM = m_obj.GetWorldMatrix();

		// モデル座標系での中心点
		Vector3 center(0, 0, 0);

		// モデル座標系での右端
		Vector3 right(1, 0, 0);

		// ワールド座標に変換
		center = Vector3::Transform(center, worldM);
		right = Vector3::Transform(right, worldM);

		// 当たり判定球に結果を代入
		Sphere::center = center;
		Sphere::radius = Vector3::Distance(center, right);
	}
}

/// <summary>
/// SphereNodeの描画
/// </summary>
void SphereNode::Draw()
{
	m_obj.Draw();
}