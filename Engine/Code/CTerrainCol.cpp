#include "CTerrainCol.h"

CTerrainCol::CTerrainCol()
{
}

CTerrainCol::CTerrainCol(LPDIRECT3DDEVICE9 pGraphicDev)
    : CVIBuffer(pGraphicDev)
{
}

CTerrainCol::CTerrainCol(const CTerrainCol& rhs)
    : CVIBuffer(rhs), m_fH(rhs.m_fH), m_iH(rhs.m_iH), m_hFile(rhs.m_hFile)
{
	m_dwCntX = rhs.m_dwCntX;
	m_dwCntZ = rhs.m_dwCntZ;
	m_dwVtxItv = rhs.m_dwVtxItv;
	m_pVertex = rhs.m_pVertex;
	m_pIndex = rhs.m_pIndex;
}

CTerrainCol::~CTerrainCol()
{
}

HRESULT CTerrainCol::Ready_Buffer(const _ulong& dwCntX, 
                                  const _ulong& dwCntZ, 
                                  const _ulong& dwVtxItv)
{
	m_hFile = CreateFile(L"../Bin/Resource/Texture/Terrain/Height.bmp",
							GENERIC_READ, 
							0, 
							NULL, 
							OPEN_EXISTING, 
							FILE_ATTRIBUTE_NORMAL, 
							0);

	if (INVALID_HANDLE_VALUE == m_hFile)
		return E_FAIL;

	_ulong	dwByte(0);

	ReadFile(m_hFile, &m_fH, sizeof(BITMAPFILEHEADER), &dwByte, NULL);
	ReadFile(m_hFile, &m_iH, sizeof(BITMAPINFOHEADER), &dwByte, NULL);

	_ulong* pPixel = new _ulong[m_iH.biWidth * m_iH.biHeight];

	ReadFile(m_hFile, pPixel, sizeof(_ulong) * m_iH.biWidth * m_iH.biHeight, &dwByte, NULL);

	CloseHandle(m_hFile);

	m_dwCntX = dwCntX;
	m_dwCntZ = dwCntZ;
	m_dwVtxItv = dwVtxItv;
	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;
	m_dwVtxCnt = dwCntX * dwCntZ;
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwFVF = FVF_COL;

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
				_float((pPixel[dwIndex] & 0x000000ff) / 20.f) + 0.1f,
				(_float)i * dwVtxItv 
			};

			m_pVertex[dwIndex].dwColor = D3DCOLOR_ARGB(255, 0, 0, 0);
		}
	}

	Safe_Delete_Array(pPixel);
	m_pVB->Unlock();

	m_pIndex = NULL;

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
			
			dwTriCnt++;
			
			// 왼쪽 아래
			m_pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
			m_pIndex[dwTriCnt]._1 = dwIndex + 1;
			m_pIndex[dwTriCnt]._2 = dwIndex;


			dwTriCnt++;
		}
	}
	
	m_pIB->Unlock();

    return S_OK;
}

void CTerrainCol::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CComponent* CTerrainCol::Clone()
{
    return new CTerrainCol(*this);
}

CTerrainCol* CTerrainCol::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
    CTerrainCol* pTerrainCol = new CTerrainCol(pGraphicDev);

    if (FAILED(pTerrainCol->Ready_Buffer(dwCntX, dwCntZ, dwVtxItv)))
    {
        Safe_Release(pTerrainCol);
        MSG_BOX("CTerrainCol Create Failed");
        return nullptr;
    }
    return pTerrainCol;
}

void CTerrainCol::Free()
{
	CVIBuffer::Free();
}
