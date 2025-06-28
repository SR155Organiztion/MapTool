#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex :  public CVIBuffer
{
private:
	explicit CTerrainTex();
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrainTex(const CTerrainTex& rhs);
	virtual ~CTerrainTex();

public:
	HRESULT	Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	virtual void	Render_Buffer();
	const VTXTEX* Get_VTXTEX() const { return m_pVertex; }

private:
	HANDLE				m_hFile;
	BITMAPFILEHEADER	m_fH;
	BITMAPINFOHEADER	m_iH;

	_ulong				m_dwCntX;
	_ulong				m_dwCntZ;
	_ulong				m_dwVtxItv;

	VTXTEX*		m_pVertex;
	INDEX32*	m_pIndex;
public:
	virtual CComponent* Clone() override;
	static CTerrainTex* Create(LPDIRECT3DDEVICE9 pGraphicDev,
								const _ulong& dwCntX, 
								const _ulong& dwCntZ, 
								const _ulong& dwVtxItv);

private:
	virtual void		Free();
};

END