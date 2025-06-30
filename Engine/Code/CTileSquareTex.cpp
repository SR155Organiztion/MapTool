#include "CTileSquareTex.h"

CTileSquareTex::CTileSquareTex()
{
}

CTileSquareTex::CTileSquareTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CTileSquareTex::CTileSquareTex(const CTileSquareTex& rhs)
	: CVIBuffer(rhs)
{
}

CTileSquareTex::~CTileSquareTex()
{
}

HRESULT CTileSquareTex::Ready_Buffer()
{
	m_dwTriCnt = 2;
	m_dwVtxCnt = 4;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwFVF = FVF_TEX;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;


	VTXTEX* pVertex = NULL;

	// ���ؽ� ���ۿ� ������ ���� �� ù ��° ������ �ּҸ� ������ �Ű� ����
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = { -0.5f, 0.5f, 0.f };
	pVertex[0].vTexUV = { 0.f, 0.f };

	pVertex[1].vPosition = { 0.5f, 0.5f, 0.f };
	pVertex[1].vTexUV = { 1.f, 0.f };

	pVertex[2].vPosition = { 0.5f, -0.5f, 0.f };
	pVertex[2].vTexUV = { 1.f, 1.f };

	pVertex[3].vPosition = { -0.5f, -0.5f, 0.f };
	pVertex[3].vTexUV = { 0.f, 1.f };

	m_pVB->Unlock();

	INDEX32* pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// ������ ��
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	// ���� �Ʒ�
	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();


	return S_OK;
}

void CTileSquareTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CComponent* CTileSquareTex::Clone()
{
	return new CTileSquareTex(*this);
}

CTileSquareTex* CTileSquareTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTileSquareTex* pRcTex = new CTileSquareTex(pGraphicDev);

	if (FAILED(pRcTex->Ready_Buffer()))
	{
		Safe_Release(pRcTex);
		MSG_BOX("pRcTex Create Failed");
		return nullptr;
	}

	return pRcTex;
}

void CTileSquareTex::Free()
{
	CVIBuffer::Free();
}
