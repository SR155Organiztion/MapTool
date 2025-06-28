#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainCol :  public CVIBuffer
{
private:
	explicit CTerrainCol();
	explicit CTerrainCol(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrainCol(const CTerrainCol& rhs);
	virtual ~CTerrainCol();

public:
	HRESULT	Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	virtual void	Render_Buffer();

public:
	const VTXCOL* Get_VtxCol() const { return m_pVertex; }


private:
	HANDLE				m_hFile;
	BITMAPFILEHEADER	m_fH;
	BITMAPINFOHEADER	m_iH;

	_ulong				m_dwCntX;
	_ulong				m_dwCntZ;
	_ulong				m_dwVtxItv;

	VTXCOL*		m_pVertex;
	INDEX32*	m_pIndex;
public:
	virtual CComponent* Clone() override;
	static CTerrainCol* Create(LPDIRECT3DDEVICE9 pGraphicDev,
								const _ulong& dwCntX, 
								const _ulong& dwCntZ, 
								const _ulong& dwVtxItv);

private:
	virtual void		Free();
};

END