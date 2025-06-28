#include "CCalculator.h"
#include "CTerrainTex.h"
#include "CTransform.h"

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

_float CCalculator::Compute_HeightOnTerrain(const _vec3* pPos, const _vec3* pTerrainVtxPos,
	const _ulong& dwCntX,
	const _ulong& dwCntZ,
	const _ulong& dwVtxItv)
{
	_ulong	dwIndex = _ulong(pPos->z / dwVtxItv) * dwCntX + _ulong(pPos->x / dwVtxItv);

	_float	fWidth = (pPos->x - pTerrainVtxPos[dwIndex + dwCntX].x) / dwVtxItv;
	_float	fHeight = (pTerrainVtxPos[dwIndex + dwCntX].z - pPos->z) / dwVtxItv;

	D3DXPLANE	Plane;

	// 오른쪽 위
	if (fWidth > fHeight)
	{
		D3DXPlaneFromPoints(&Plane,
			&pTerrainVtxPos[dwIndex + dwCntX],
			&pTerrainVtxPos[dwIndex + dwCntX + 1],
			&pTerrainVtxPos[dwIndex + 1]);
	}

	// 왼쪽 아래
	else
	{
		D3DXPlaneFromPoints(&Plane,
			&pTerrainVtxPos[dwIndex + dwCntX],
			&pTerrainVtxPos[dwIndex + 1],
			&pTerrainVtxPos[dwIndex]);
	}

	// ax + by + cz + d
	// 
	// -by = ax + cz + d
	// 
	// y = (-ax - cz - d) / b;

	return (-Plane.a * pPos->x - Plane.c * pPos->z - Plane.d) / Plane.b;
}

_vec3 CCalculator::Picking_OnTerrain(HWND hWnd, const CTerrainTex* pTerrainBufferCom, const CTransform* pTerrainTransformCom)
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	// 뷰포트 -> 투영

	_vec3		vMousePos;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

	m_pGraphicDev->GetViewport(&ViewPort);

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	// 투영 -> 뷰 스페이스
	D3DXMATRIX	matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	// 뷰 스페이스 -> 월드

	D3DXMATRIX	matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);

	_vec3		vRayPos{ 0.f, 0.f, 0.f };
	_vec3		vRayDir = vMousePos - vRayPos;
	D3DXVec3Normalize(&vRayDir, &vRayDir);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// 월드 -> 로컬
	_matrix		matWorld;
	pTerrainTransformCom->Get_World(&matWorld);
	D3DXMatrixInverse(&matWorld, 0, &matWorld);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	_ulong	dwVtxIdx[3]{};
	_float	fU(0.f), fV(0.f), fDist(0.f);

	const VTXTEX* pTerrainVtx = pTerrainBufferCom->Get_VTXTEX();

	for (_ulong i = 0; i < VTXCNTZ - 1; ++i)
	{
		for (_ulong j = 0; j < VTXCNTX - 1; ++j)
		{
			_ulong	dwIndex = i * VTXCNTX + j;

			// 오른쪽 위
			dwVtxIdx[0] = dwIndex + VTXCNTX;
			dwVtxIdx[1] = dwIndex + VTXCNTX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			// V1 + U(V2 - V1) + V(V3 - V1)	// 함수의 매개 변수로 삽입한 순서에 근거한 결과

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[1]].vPosition,
				&pTerrainVtx[dwVtxIdx[0]].vPosition,
				&pTerrainVtx[dwVtxIdx[2]].vPosition,
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return _vec3(pTerrainVtx[dwVtxIdx[1]].vPosition.x + (fU * (pTerrainVtx[dwVtxIdx[0]].vPosition.x - pTerrainVtx[dwVtxIdx[1]].vPosition.x)),
					0.f,
					pTerrainVtx[dwVtxIdx[1]].vPosition.z + (fV * (pTerrainVtx[dwVtxIdx[2]].vPosition.z - pTerrainVtx[dwVtxIdx[1]].vPosition.z)));
			}


			// 왼쪽 아래

			dwVtxIdx[0] = dwIndex + VTXCNTX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			// V1 + U(V2 - V1) + V(V3 - V1)	

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[2]].vPosition,
				&pTerrainVtx[dwVtxIdx[1]].vPosition,
				&pTerrainVtx[dwVtxIdx[0]].vPosition,
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return _vec3(pTerrainVtx[dwVtxIdx[2]].vPosition.x + (fU * (pTerrainVtx[dwVtxIdx[1]].vPosition.x - pTerrainVtx[dwVtxIdx[2]].vPosition.x)),
					0.f,
					pTerrainVtx[dwVtxIdx[2]].vPosition.z + (fV * (pTerrainVtx[dwVtxIdx[0]].vPosition.z - pTerrainVtx[dwVtxIdx[2]].vPosition.z)));
			}
		}

	}

	return _vec3(0.f, 0.f, 0.f);
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
