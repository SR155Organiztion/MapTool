#pragma once

#include "CBase.h"
#include "Engine_Define.h"

class CLoading : public CBase
{
public:
	enum LOADINGID { LOADING_STAGE, LOADING_BOSS, LOADING_END };

private:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading(void);

public:
	LOADINGID		Get_LoadingID() const { return m_LoadingID; }
	_bool			Get_Finish() const { return m_bFinish; }
	CRITICAL_SECTION* Get_Crt()  { return &m_Crt; }

	const _tchar* Get_String() { return m_szLoading; }
	
public:
	static unsigned int __stdcall Thread_Main(void* pArg);

public:
	HRESULT Ready_Loading(LOADINGID eID);
	_uint  Loaing_ForStage();


private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;

	_bool						m_bFinish;
	LOADINGID					m_LoadingID;

	HANDLE						m_hThread;

	CRITICAL_SECTION			m_Crt;

	_tchar						m_szLoading[128];

public:
	static CLoading* Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID);

private:
	virtual void	Free();


};

