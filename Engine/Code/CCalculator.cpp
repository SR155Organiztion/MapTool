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
    D3DXVec3Normalize(&vRayDir, &vRayDir);

    float tMin = -FLT_MAX, tMax = FLT_MAX;
    _vec3 vNormal = { 0.f, 0.f, 0.f };

    for (int i = 0; i < 3; ++i)
    {
        float rayDir = vRayDir[i];
        float rayOri = vRayPos[i];
        float boxMin = (*_vMin)[i];
        float boxMax = (*_vMax)[i];

        if (fabs(rayDir) < 1e-6f)
        {
            if (rayOri < boxMin || rayOri > boxMax)
                return;
            continue;
        }

        float invD = 1.f / rayDir;
        float t0 = (boxMin - rayOri) * invD;
        float t1 = (boxMax - rayOri) * invD;
        if (t0 > t1) std::swap(t0, t1);

        if (t0 > tMin)
        {
            tMin = t0;
            vNormal = { 0.f, 0.f, 0.f };
            vNormal[i] = (invD < 0.f) ? 1.f : -1.f; // 충돌면 법선
        }

        tMax = min(tMax, t1);
        if (tMax < tMin)
            return;
    }

    if (tMin < 0.f)
        return;

    _vec3 vColPos = vRayPos + vRayDir * tMin;

    CCollisionMgr::GetInstance()->Set_ColPos(vColPos);
    CCollisionMgr::GetInstance()->Set_ColNormal(vNormal); // <-- 법선 저장
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
