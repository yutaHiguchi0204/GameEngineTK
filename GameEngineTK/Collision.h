//
// Collision.h
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

// 三角形
class Triangle
{
public:
	DirectX::SimpleMath::Vector3	P0;
	DirectX::SimpleMath::Vector3	P1;
	DirectX::SimpleMath::Vector3	P2;
	DirectX::SimpleMath::Vector3	Normal;			// 法線ベクトル
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

bool CheckSphere2Sphere(const Sphere& _sphereA, const Sphere & _sphereB);

// ３点から三角形を計算（反時計回りに指定）
void ComputeTriangle(const DirectX::SimpleMath::Vector3& _p0, const DirectX::SimpleMath::Vector3& _p1, const DirectX::SimpleMath::Vector3& _p2, Triangle* _triangle);

// 球と三角形の当たり判定
bool CheckSphere2Triangle(const Sphere& _sphere, const Triangle& _tri, DirectX::SimpleMath::Vector3* _inter);

// 線分と三角形の当たり判定
bool CheckSegment2Triangle(const Segment& _segment, const Triangle& _triangle, DirectX::SimpleMath::Vector3 *_inter)