#include "CTerrainTex.h"

CTerrainTex::CTerrainTex()
{
}

CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev)
    : CVIBuffer(pGraphicDev)
{
}

CTerrainTex::CTerrainTex(const CTerrainTex& rhs)
    : CVIBuffer(rhs), m_fH(rhs.m_fH), m_iH(rhs.m_iH), m_hFile(rhs.m_hFile)
{
	m_dwCntX = rhs.m_dwCntX;
	m_dwCntZ = rhs.m_dwCntZ;
	m_dwVtxItv = rhs.m_dwVtxItv;
	m_pVertex = rhs.m_pVertex;
	m_pIndex = rhs.m_pIndex;
}

CTerrainTex::~CTerrainTex()
{
}

HRESULT CTerrainTex::Ready_Buffer(const _ulong& dwCntX, 
                                  const _ulong& dwCntZ, 
                                  const _ulong& dwVtxItv)
{
	//m_hFile = CreateFile(L"../Bin/Resource/Texture/Terrain/Height.bmp",
	//						GENERIC_READ, 
	//						0, 
	//						NULL, 
	//						OPEN_EXISTING, 
	//						FILE_ATTRIBUTE_NORMAL, 
	//						0);
	//
	//if (INVALID_HANDLE_VALUE == m_hFile)
	//	return E_FAIL;
	//
	//_ulong	dwByte(0);
	//
	//ReadFile(m_hFile, &m_fH, sizeof(BITMAPFILEHEADER), &dwByte, NULL);
	//ReadFile(m_hFile, &m_iH, sizeof(BITMAPINFOHEADER), &dwByte, NULL);
	//
	//_ulong* pPixel = new _ulong[m_iH.biWidth * m_iH.biHeight];
	//
	//ReadFile(m_hFile, pPixel, sizeof(_ulong) * m_iH.biWidth * m_iH.biHeight, &dwByte, NULL);
	//
	//CloseHandle(m_hFile);

	m_dwCntX = dwCntX;
	m_dwCntZ = dwCntZ;
	m_dwVtxItv = dwVtxItv;
	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;
	m_dwVtxCnt = dwCntX * dwCntZ;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwFVF = FVF_TEX;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	_ulong dwIndex(0);

	m_pVertex = NULL;

	// 버텍스 버퍼에 보관된 정점 중 첫 번째 정점의 주소를 얻어오는 매개 변수
	m_pVB->Lock(0, 0, (void**)&m_pVertex, 0);

	for (_ulong i = 0; i < dwCntZ; ++i)
	{
		for (_ulong j = 0; j < dwCntX; ++j)
		{
			dwIndex = i * dwCntX + j;
			
			m_pVertex[dwIndex].vPosition = {
				(_float)j * dwVtxItv, 
				0,//_float((pPixel[dwIndex] & 0x000000ff) / 20.f),
				(_float)i * dwVtxItv 
			};

			m_pVertex[dwIndex].vNormal = { 0.f, 0.f, 0.f };

			m_pVertex[dwIndex].vTexUV = { (_float(j) / (dwCntX - 1)) * (dwCntX - 1),
										(_float(i) / (dwCntZ - 1)) * (dwCntZ - 1)
			};
		}
	}

	//Safe_Delete_Array(pPixel);

	m_pIndex = NULL;
	_vec3		vDst, vSrc, vNormal;

	m_pIB->Lock(0, 0, (void**)&m_pIndex, 0);

	_ulong	dwTriCnt(0);

	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			dwIndex = i * dwCntX + j;

			// 오른쪽 위
			m_pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
			m_pIndex[dwTriCnt]._1 = dwIndex + dwCntX + 1;
			m_pIndex[dwTriCnt]._2 = dwIndex + 1;
			
			vDst = m_pVertex[m_pIndex[dwTriCnt]._1].vPosition - m_pVertex[m_pIndex[dwTriCnt]._0].vPosition;
			vSrc = m_pVertex[m_pIndex[dwTriCnt]._2].vPosition - m_pVertex[m_pIndex[dwTriCnt]._1].vPosition;
			
			D3DXVec3Cross(&vNormal, &vDst, &vSrc);
			m_pVertex[m_pIndex[dwTriCnt]._0].vNormal += vNormal;
			m_pVertex[m_pIndex[dwTriCnt]._1].vNormal += vNormal;
			m_pVertex[m_pIndex[dwTriCnt]._2].vNormal += vNormal;

			dwTriCnt++;
			
			// 왼쪽 아래
			m_pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
			m_pIndex[dwTriCnt]._1 = dwIndex + 1;
			m_pIndex[dwTriCnt]._2 = dwIndex;
			
			vDst = m_pVertex[m_pIndex[dwTriCnt]._1].vPosition - m_pVertex[m_pIndex[dwTriCnt]._0].vPosition;
			vSrc = m_pVertex[m_pIndex[dwTriCnt]._2].vPosition - m_pVertex[m_pIndex[dwTriCnt]._1].vPosition;

			D3DXVec3Cross(&vNormal, &vDst, &vSrc);

			m_pVertex[m_pIndex[dwTriCnt]._0].vNormal += vNormal;
			m_pVertex[m_pIndex[dwTriCnt]._1].vNormal += vNormal;
			m_pVertex[m_pIndex[dwTriCnt]._2].vNormal += vNormal;

			dwTriCnt++;
		}
	}

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
		D3DXVec3Normalize(&m_pVertex[i].vNormal, &m_pVertex[i].vNormal);
	
	m_pVB->Unlock();
	m_pIB->Unlock();

    return S_OK;
}

void CTerrainTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CComponent* CTerrainTex::Clone()
{
    return new CTerrainTex(*this);
}

CTerrainTex* CTerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
    CTerrainTex* pTerrainTex = new CTerrainTex(pGraphicDev);

    if (FAILED(pTerrainTex->Ready_Buffer(dwCntX, dwCntZ, dwVtxItv)))
    {
        Safe_Release(pTerrainTex);
        MSG_BOX("CTerrainTex Create Failed");
        return nullptr;
    }
    return pTerrainTex;
}

void CTerrainTex::Free()
{
	CVIBuffer::Free();
}
