//
// Collision.cpp
// �Փ˔���̃N���X
//

// �w�b�_�t�@�C���̃C���N���[�h
#include "Collision.h"

// ���O���
using namespace DirectX::SimpleMath;

/// <summary>
/// ���Ƌ��̓����蔻��
/// </summary>
/// <param name="_sphereA">���P</param>
/// <param name="_sphereB">���Q</param>
/// <returns>�������Ă�����true�A�����łȂ����false</returns>
bool CheckSphere2Sphere(const Sphere& _sphereA, const Sphere& _sphereB)
{
	// ���S���W�̍������v�Z
	Vector3 sub = _sphereB.center - _sphereA.center;

	// �O�����̒藝���g�p���ăx�N�g���̒������v�Z
	float distanceSquare = sub.x * sub.x + sub.y * sub.y + sub.z * sub.z;

	float radiusSquare = _sphereA.radius + _sphereB.radius;
	radiusSquare *= radiusSquare;

	// ���������a�̘a���傫����Γ������Ă��Ȃ�
	if (distanceSquare > radiusSquare)
	{
		return false;
	}

	return true;
}