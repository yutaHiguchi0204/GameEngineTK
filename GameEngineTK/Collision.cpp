//
// Collision.cpp
// 衝突判定のクラス
//

// ヘッダファイルのインクルード
#include "Collision.h"

// 名前空間
using namespace DirectX::SimpleMath;

/// <summary>
/// 球と球の当たり判定
/// </summary>
/// <param name="_sphereA">球１</param>
/// <param name="_sphereB">球２</param>
/// <returns>当たっていたらtrue、そうでなければfalse</returns>
bool CheckSphere2Sphere(const Sphere& _sphereA, const Sphere& _sphereB)
{
	// 中心座標の差分を計算
	Vector3 sub = _sphereB.center - _sphereA.center;

	// 三平方の定理を使用してベクトルの長さを計算
	float distanceSquare = sub.x * sub.x + sub.y * sub.y + sub.z * sub.z;

	float radiusSquare = _sphereA.radius + _sphereB.radius;
	radiusSquare *= radiusSquare;

	// 距離が半径の和より大きければ当たっていない
	if (distanceSquare > radiusSquare)
	{
		return false;
	}

	return true;
}