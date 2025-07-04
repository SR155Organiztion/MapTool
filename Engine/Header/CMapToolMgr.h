/**
* @file		CMapToolMgr.h
* @date		2025-06-30
* @author	서원호
* @brief	맵툴 매니저
* @details  맵 데이터 입출력을 총관리하는 매니저
*/

#pragma once
#include "CBase.h"
#include "Engine_Define.h"

/**
* @struct S_BLOCK
* @brief 블럭 구조체
*/
struct S_BLOCK {
	string Block_Type;
	_vec3 vPos;
	string Direction;
};


/**
* @struct S_Tile
* @brief 타일 구조체
*/
struct S_TILE {
	string Tile_Type;
	_vec3 vPos;
	string Direction;
};


/**
* @struct S_ENVIRONMENT
* @brief 환경 구조물 구조체
*/
struct S_ENVIRONMENT {
	string Env_Type;
	_vec3 vPos;
	_vec3 Direction;
};


/**
* @struct S_CAM
* @brief 카메라를 저장할 벡터 구조체
*/
struct S_CAM {
	_vec3 vEye;
	_vec3 vAt;
};
/**
* @struct S_PLAYER
* @brief 카메라를 저장할 벡터 구조체
*/
struct S_PLAYER {
	_vec3 P1;
	_vec3 P2;
};

/**
* @struct S_STAGE
* @brief 스테이지의 모든 정보를 가진 구조체
*/
struct S_STAGE {
	S_CAM Cam;
	S_PLAYER Player;
	std::vector<string> Recipe;
	std::vector<S_BLOCK> Block;
	std::vector<S_TILE> Tiles;
	std::vector<S_ENVIRONMENT> Environment;
};

///////////////////////////////////////////////////////////////////////////////
BEGIN(Engine)
class ENGINE_DLL CMapToolMgr : public CBase
{
	DECLARE_SINGLETON(CMapToolMgr)

private:
	explicit CMapToolMgr();
	virtual ~CMapToolMgr();

public:
	void	Plant_Block(_vec3 _vPos);		///설치한 블록의 정보를 벡터 리스트에 넣음
	void	Plant_Tile(string _sType, _vec3 _vPos, string _sDir);		///설치한 타일의 정보를 벡터 리스트에 넣음
	void	Plant_Environment(string _sType, _vec3 _vPos, _vec3 _vDir);	///설치한 환경 오브젝트를 벡터 리스트에 넣음
	void	Plant_Camera(_vec3 _vEye, _vec3 _vAt);						///현재 바라보고 있는 곳의 Eye와 at을 저장
	void	Plant_Player(_int _iPlayer,_vec3 _vPos);					///플레이어의 스폰 위치 설정

	HRESULT Save_Json();				///json에 데이터 저장
	HRESULT Load_Json();				///json 데이터 불러오기
	void	Select_Map();				///맵 불러오기
	void	Reset();					///현재 벡터리스트를 모두 초기화 및 설치한 블록 제거

public:
	S_STAGE Get_Data(string s);

	void NextRotate();
	void PrevRotate();
	_vec3 Get_DirLook();
	
	void NextStation();
	void PrevStation();
	_uint Get_NowStation();

	//임시출력용
	const _tchar* Imsi_Get_Dir();

private:
	void	Dummy_Data();				///테스트용
	string	Dir_To_String();
	string	Block_To_String();

private:
	vector<S_BLOCK>			m_tBlockVec;		///현재 설치되어있는 블록의 데이터
	vector<S_TILE>			m_tTileVec;			///현재 설치되어있는 타일의 데이터
	vector<S_ENVIRONMENT>	m_tEnvVec;			///현재 설치 되어있는 환경블록의 데이터
	vector<string>			m_sRecipeVec;		
	S_CAM					m_tCam;				///설치를 한 카메라의 
	S_PLAYER				m_tPlayer;			///현재 플레이어들의 시작 위치

	map<string, S_STAGE> m_mapJson;				///전체 스테이지의 데이터값
	string			m_sName;					///저장하거나 불러올 이름

	_uint			m_iSelectName;				///내가 선택중인 스테이지

	_uint			m_iSet_Player;				///현재 선택중인 플레이어
	vector<string>	m_sNameVec;					///전체 스테이지 이름
	
	_int			m_iStation;					///현재 선택중인 스테이션
	_int			m_iIngredient;				///현재 선택중인 재료
	
	_int			m_iDir;						///현재 선택중인 방향 값	(고정된 6 방향)
	_vec3			m_vecDir;					///현재 선택중인 방향 벡터값(고정되지 않은 각도) 
	_float			m_fAngle;					///현재 각도

private:
	virtual void		Free();

};

END
