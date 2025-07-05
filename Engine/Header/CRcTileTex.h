#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcTileTex : public CVIBuffer
{
private:
	explicit CRcTileTex();
	explicit CRcTileTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcTileTex(const CRcTileTex& rhs);
	virtual ~CRcTileTex();

public:
	virtual HRESULT	Ready_Buffer();
	virtual void	Render_Buffer();

public:
	virtual CComponent* Clone() override;
	static CRcTileTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free();

};

END