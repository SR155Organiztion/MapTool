#pragma once

#include "CScene.h"

class CStage : public Engine::CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage();

public:
	virtual			HRESULT		Ready_Scene();
	virtual			_int		Update_Scene(const _float& fTimeDelta);
	virtual			void		LateUpdate_Scene(const _float& fTimeDelta);
	virtual			void		Render_Scene();

private:
	HRESULT		Ready_Basic_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_Environment_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_Tile_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_Block_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_GameObject_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_UI_Layer(const _tchar* pLayerTag);

private:
	HRESULT		Ready_Prototype();
	HRESULT		Ready_Light();

public:
	static CStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free();

};