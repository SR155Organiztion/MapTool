#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CImguiMgr : public CBase
{
	DECLARE_SINGLETON(CImguiMgr)

private:
	explicit CImguiMgr();
	virtual ~CImguiMgr();

public:
	void		Set_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HRESULT		Ready_Imgui(LPDIRECT3DDEVICE9 pGraphicDev, HWND hWnd);
	void		Update_Imgui();
	void		LateUpdate_Imgui(const _float& fTimeDelta);
	void		Render_Imgui();

	void SetLoadCallback(std::function<void()> callback) { m_LoadCallback = std::move(callback); }
	void SetClearCallback(std::function<void()> callback) { m_ClearCallback = std::move(callback); }
protected:
	LPDIRECT3DDEVICE9						m_pGraphicDev;
	std::function<void()>					m_LoadCallback;
	std::function<void()>					m_ClearCallback;
	char szName[64];

private:
	virtual void		Free();


};

END
