#include "CCalculator.h"
#include "CTerrainTex.h"
#include "CTransform.h"
#include "CCollisionMgr.h"

CCalculator::CCalculator(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{
}

CCalculator::CCalculator(const CCalculator& rhs)
	: CComponent(rhs)
{
}

CCalculator::~CCalculator()
{
}

HRESULT CCalculator::Ready_Calculator()
{
	return S_OK;
}

void CCalculator::Calculate_AABB(_vec3* _vMin, _vec3* _vMax)
{
    _vec3 vRayPos, vRayDir;
    CCollisionMgr::GetInstance()->Get_Ray(&vRayPos, &vRayDir);

    // Normalize direction (안정성)
    D3DXVec3Normalize(&vRayDir, &vRayDir);

    float tMin = (_vMin->x - vRayPos.x) / vRayDir.x;
    float tMax = (_vMax->x - vRayPos.x) / vRayDir.x;
    if (tMin > tMax) std::swap(tMin, tMax);

    float tyMin = (_vMin->y - vRayPos.y) / vRayDir.y;
    float tyMax = (_vMax->y - vRayPos.y) / vRayDir.y;
    if (tyMin > tyMax) std::swap(tyMin, tyMax);

    if ((tMin > tyMax) || (tyMin > tMax))
        return;

    if (tyMin > tMin) tMin = tyMin;
    if (tyMax < tMax) tMax = tyMax;

    float tzMin = (_vMin->z - vRayPos.z) / vRayDir.z;
    float tzMax = (_vMax->z - vRayPos.z) / vRayDir.z;
    if (tzMin > tzMax) std::swap(tzMin, tzMax);

    if ((tMin > tzMax) || (tzMin > tMax))
        return;

    if (tzMin > tMin) tMin = tzMin;
    if (tzMax < tMax) tMax = tzMax;

    if (tMin < 0.0f)
        return; // Ray가 박스 뒤에서 시작

    CCollisionMgr::GetInstance()->Set_ColPos((vRayPos + vRayDir * tMin));
}


CCalculator* CCalculator::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCalculator* pCalculator = new CCalculator(pGraphicDev);

	if (FAILED(pCalculator->Ready_Calculator()))
	{
		Safe_Release(pCalculator);
		MSG_BOX("CCalculator Create Failed");
		return nullptr;
	}

	return pCalculator;
}

CComponent* CCalculator::Clone()
{
	return new CCalculator(*this);
}

void CCalculator::Free()
{
	CComponent::Free();
}
