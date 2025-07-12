#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CHexPrismTex : public CVIBuffer
{
private:
	explicit CHexPrismTex();
	explicit CHexPrismTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CHexPrismTex(const CHexPrismTex& rhs);
	virtual ~CHexPrismTex();

public:
	virtual HRESULT	Ready_Buffer();
	virtual void	Render_Buffer();

public:
	virtual CComponent* Clone() override;
	static CHexPrismTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free();

};

END