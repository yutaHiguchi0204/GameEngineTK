//
// Collisiom.h
// �Փ˔���̃N���X
//
#pragma once

// �w�b�_�t�@�C���̃C���N���[�h
#include <d3d11_1.h>
#include <SimpleMath.h>
#include <Windows.h>

// �N���X�̒�`

// ����
class Segment
{
public:
	DirectX::SimpleMath::Vector3 start;				// �n�_
	DirectX::SimpleMath::Vector3 end;				// �I�_
};

// ��
class Sphere
{
public:
	DirectX::SimpleMath::Vector3 center;			// ���S���W
	float radius;									// ���a

public:
	Sphere()
	{
		radius = 1.0f;
	}
};

bool CheckSphere2Sphere(const Sphere & _sphereA, const Sphere & _sphereB);