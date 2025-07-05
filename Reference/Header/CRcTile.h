#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcTile : public CVIBuffer
{
private:
	explicit CRcTile();
	explicit CRcTile(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcTile(const CRcTile& rhs);
	virtual ~CRcTile();

public:
	virtual HRESULT	Ready_Buffer();
	virtual void	Render_Buffer();

public:
	virtual CComponent* Clone() override;
	static CRcTile* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free();

};

END