//
// Collision.h
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

// �O�p�`
class Triangle
{
public:
	DirectX::SimpleMath::Vector3	P0;
	DirectX::SimpleMath::Vector3	P1;
	DirectX::SimpleMath::Vector3	P2;
	DirectX::SimpleMath::Vector3	Normal;			// �@���x�N�g��
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

bool CheckSphere2Sphere(const Sphere& _sphereA, const Sphere & _sphereB);

// �R�_����O�p�`���v�Z�i�����v���Ɏw��j
void ComputeTriangle(const DirectX::SimpleMath::Vector3& _p0, const DirectX::SimpleMath::Vector3& _p1, const DirectX::SimpleMath::Vector3& _p2, Triangle* _triangle);

// ���ƎO�p�`�̓����蔻��
bool CheckSphere2Triangle(const Sphere& _sphere, const Triangle& _tri, DirectX::SimpleMath::Vector3* _inter);

// �����ƎO�p�`�̓����蔻��
bool CheckSegment2Triangle(const Segment& _segment, const Triangle& _triangle, DirectX::SimpleMath::Vector3 *_inter)