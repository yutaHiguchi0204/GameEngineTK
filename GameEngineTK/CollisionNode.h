//
// CollisionNode.h
// 衝突判定ノード
//
#pragma once

// ヘッダファイルのインクルード
#include "Collision.h"
#include "Obj3d.h"

// クラスの定義
class CollisionNode
{
protected:
	Obj3d m_obj;
	DirectX::SimpleMath::Vector3 m_trans;

public:
	virtual void Initialize() = 0;				// 初期化処理
	virtual void Update() = 0;					// 更新処理
	virtual void Draw() = 0;					// 描画処理

	void SetParent(Obj3d* parent);

	void SetTrans(const DirectX::SimpleMath::Vector3& trans) { m_trans = trans; };
};

// 球の当たり判定ノード
class SphereNode : public CollisionNode, public Sphere
{
protected:
	float m_radius;					// ローカル半径

public:
	SphereNode();

	void Initialize();				// 初期化処理
	void Update();					// 更新処理
	void Draw();					// 描画処理

	void SetLocalRadius(float radius) { m_radius = radius; };
};