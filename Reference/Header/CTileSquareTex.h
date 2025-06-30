#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTileSquareTex : public CVIBuffer
{
private:
	explicit CTileSquareTex();
	explicit CTileSquareTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTileSquareTex(const CTileSquareTex& rhs);
	virtual ~CTileSquareTex();

public:
	virtual HRESULT	Ready_Buffer();
	virtual void	Render_Buffer();

public:
	virtual CComponent* Clone() override;
	static CTileSquareTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free();

};

END