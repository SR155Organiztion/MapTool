#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CHexTileTex : public CVIBuffer
{
private:
	explicit CHexTileTex();
	explicit CHexTileTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CHexTileTex(const CHexTileTex& rhs);
	virtual ~CHexTileTex();

public:
	virtual HRESULT	Ready_Buffer();
	virtual void	Render_Buffer();

public:
	virtual CComponent* Clone() override;
	static CHexTileTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free();

};

END