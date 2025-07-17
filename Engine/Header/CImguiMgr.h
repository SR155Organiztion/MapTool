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
	void SetTerrianEnableCallback(std::function<void()> callback) { m_TerrianEnableCallback = std::move(callback); }

	char* Get_Name() { return szName; }
	_int Get_CurItem() { return m_iCurrent_Item; }
	_int Get_CurFood() { return m_iCurrent_Food; }
	_vec3 Get_NowScale() { return m_vScale; }
	_int Get_Stage() { return m_iStage; }
	float Get_OffsetY() { return m_fOffestY; }
protected:
	LPDIRECT3DDEVICE9						m_pGraphicDev;
	std::function<void()>					m_LoadCallback;
	std::function<void()>					m_ClearCallback;
	std::function<void()>					m_TerrianEnableCallback;

	map<string, _bool>						m_mapRecipes;
	_int m_iCurrent_Item;
	_int m_iCurrent_Food;
	_bool m_bTerrainEnable;
	char szName[64];
	float fTimer;
	int iX, iY;
	_vec3 m_vScale;
	_int m_iStage;
	float m_fOffestY;
	bool m_bEvent;
	float m_fEventTime;

private:
	virtual void		Free();


};

END
