//
// Collisiom.h
// 衝突判定のクラス
//
#pragma once

// ヘッダファイルのインクルード
#include <d3d11_1.h>
#include <SimpleMath.h>
#include <Windows.h>

// クラスの定義

// 線分
class Segment
{
public:
	DirectX::SimpleMath::Vector3 start;				// 始点
	DirectX::SimpleMath::Vector3 end;				// 終点
};

// 球
class Sphere
{
public:
	DirectX::SimpleMath::Vector3 center;			// 中心座標
	float radius;									// 半径

public:
	Sphere()
	{
		radius = 1.0f;
	}
};

bool CheckSphere2Sphere(const Sphere & _sphereA, const Sphere & _sphereB);