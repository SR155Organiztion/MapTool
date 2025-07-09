#pragma once

#include "CCamera.h"

namespace Engine
{
	class CCalculator;
}

class CDynamicCamera :  public CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDynamicCamera();

public:
	HRESULT	Ready_GameObject(const _vec3* pEye, 
		const _vec3* pAt, 
		const _vec3* pUp, 
		const _float& fFov, 
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);

	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject() {}

private:
	void		Key_Input(const _float& fTimeDelta);
	void		Mouse_Move();
	void		Mouse_Fix();

	void		ALL_RESET();		//맵매니저가 가진 현재맵의 모든데이터 증발 동시에 현재 씬의 모든 오브젝트 제거
	void		Load_Objects();		//불러오기
	void		Get_CamInfo(_vec3* vEye, _vec3*& vAt) {}

	void		LoadCallBackToImguiMgr();//Imgui에게 보내기
	void		ClearCallBackToImguiMgr();//Imgui에게 보내기
	
	void		Prev_Type();
	void		Next_Type();

	//블럭생성
	void		Create_Objects();
	void		Delete_Objects();
	
	HRESULT		Create_Block();
	void		Delete_Block();

	HRESULT		Create_RcTile();
	void		Delete_RcTile();

	HRESULT		Create_HexTile();
	void		Delete_HexTile();

	HRESULT		Create_Player();
	void		Delete_Player();

private:
	_float		m_fSpeed;
	_bool		m_bFix;
	_bool		m_bCheck;
	Engine::CCalculator* m_pCalculatorCom;
	_bool		m_bClickedLB;
	_bool		m_bClickedRB;
	_bool		m_bPressedQ;
	_bool		m_bPressedE;
	_bool		m_bPressed1;
	_bool		m_bPressed3;
	_bool		m_bPressedR;
	_bool		m_bPressedL;
	_bool		m_bShowBox;

	
	vector<_tchar*> Release_tchar;
public:
	static CDynamicCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev,
									const _vec3* pEye,
									const _vec3* pAt,
									const _vec3* pUp,
									const _float& fFov = D3DXToRadian(60.f),
									const _float& fAspect = (_float)WINCX / WINCY,
									const _float& fNear = 0.1f,
									const _float& fFar = 1000.f);

private:
	virtual void		Free();
};

