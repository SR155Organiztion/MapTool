#pragma once

#include "../../Base/CBase.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
#include "CManagement.h"

// namespace Engine
// {
// 	class CGraphicDev;
// 	class CManagement;
// }


class CMainApp : public CBase
{
private:// 생성자, 소멸자
	explicit CMainApp();
	virtual ~CMainApp();

public:
	HRESULT		Ready_MainApp();
	int			Update_MainApp(const float& fTimeDelta);
	void		LateUpdate_MainApp(const float& fTimeDelta);
	void		Render_MainApp();

private:
	HRESULT		Ready_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT		Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev);


private:
	Engine::CGraphicDev*	m_pDeviceClass;
	Engine::CManagement*	m_pManagementClass;
	LPDIRECT3DDEVICE9		m_pGraphicDev;


public: //외부 생성 함수
	static CMainApp* Create();

private: // 외부 소멸 함수
	virtual void	Free();

};

// 1. 평면의 방정식을 이용하여 지형을 타는 플레이어 상태를 구현하라.(키보드 입력)

// ax + by + cz + d = 0;
// D3DXPLANE Plane;

// 2. 마우스 충돌을 이용하여 지형을 타는 플레이어 상태를 구현하라.
// D3DXIntersectTri();	//ray-picking