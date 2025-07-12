#include "CHexTileTex.h"

CHexTileTex::CHexTileTex()
{
}

CHexTileTex::CHexTileTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CHexTileTex::CHexTileTex(const CHexTileTex& rhs)
	: CVIBuffer(rhs)
{
}

CHexTileTex::~CHexTileTex()
{
}

HRESULT CHexTileTex::Ready_Buffer()
{
	m_dwTriCnt = 6;                           // 6개의 삼각형
	m_dwVtxCnt = 7;                           // 중심점 + 둘레 6개
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwFVF = FVF_TEX;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	// 2. 버텍스 설정
	VTXTEX* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	// 중심점
	float y = -0.251f;

	pVertex[0].vPosition = { 0.f, y, 0.f };
	pVertex[0].vTexUV = { 0.5f, 0.5f };

	// 시계방향으로 둘레 6개
	for (int i = 0; i < 6; ++i)
	{
		float angle = D3DXToRadian(60.f * i);
		float x = 0.5f * cosf(angle);
		float z = 0.5f * sinf(angle);

		pVertex[i + 1].vPosition = { x, y, z };
		pVertex[i + 1].vTexUV = { 0.5f + x, 0.5f - z }; // UV는 중앙기준
	}

	m_pVB->Unlock();

	// 3. 인덱스 설정
	INDEX32* pIndex = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (int i = 0; i < 6; ++i)
	{
		pIndex[i]._0 = 0;
		pIndex[i]._1 = i + 1;
		pIndex[i]._2 = (i + 1) % 6 + 1;  // 반시계방향으로 순서 변경
	}

	m_pIB->Unlock();

	return S_OK;
}

void CHexTileTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CComponent* CHexTileTex::Clone()
{
	return new CHexTileTex(*this);
}

CHexTileTex* CHexTileTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHexTileTex* pHexTileTex = new CHexTileTex(pGraphicDev);

	if (FAILED(pHexTileTex->Ready_Buffer()))
	{
		Safe_Release(pHexTileTex);
		MSG_BOX("pHexTileTex Create Failed");
		return nullptr;
	}

	return pHexTileTex;
}

void CHexTileTex::Free()
{
	CVIBuffer::Free();
}
