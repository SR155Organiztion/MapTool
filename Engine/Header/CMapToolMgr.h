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

BEGIN(Engine)
/**
* @struct S_BLOCK
* @brief 블럭 구조체
*/
struct S_BLOCK {
	string Block_Name;
	_vec3 vPos;
	string Direction;
};
/**
* @struct S_Tile
* @brief 타일 구조체
*/
struct S_TILE {
	string Tile_Name;
	_vec3 vPos;
	string Direction;
};
/**
* @struct S_ENVIRONMENT
* @brief 환경 구조물 구조체
*/
struct S_ENVIRONMENT {
	string Tile_Name;
	_vec3 vPos;
	string Direction;
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
* @struct S_CAM
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

/// ///////////////////////////////////////////////////////////////////////////

class ENGINE_DLL CMapToolMgr : public CBase
{
	DECLARE_SINGLETON(CMapToolMgr)

private:
	explicit CMapToolMgr();
	virtual ~CMapToolMgr();

public:
	void	Plant_Block();				///설치한 블록의 정보를 벡터 리스트에 넣음
	void	Plant_Tile();				///설치한 타일의 정보를 벡터 리스트에 넣음
	void	Plant_Environment();		///설치한 환경 오브젝트를 벡터 리스트에 넣음
	void	Plant_Camera();				///현재 바라보고 있는 곳의 Eye와 at을 저장
	void	Plant_Player();				///플레이어의 스폰 위치 설정
	HRESULT Save_Json();				///json에 데이터 저장
	HRESULT Load_Json();				///json 데이터 불러오기
	void	Reset();					///현재 벡터리스트를 모두 초기화 및 설치한 블록 제거

private:
	void	Key_Input();				///키입력

private:
	vector<S_BLOCK*>		m_pBlockData;		///현재 설치되어있는 블록의 데이터
	vector<S_TILE*>			m_pTileData;		///현재 설치되어있는 타일의 데이터
	vector<S_ENVIRONMENT*>	m_pEnvData;			///현재 설치 되어있는 환경블록의 데이터
	S_CAM					m_tCam;			///현재 플레이어들의 시작 위치
	S_PLAYER				m_tPlayer;

	map<string, vector<S_STAGE>> m_mapJson;		///전체 스테이지의 데이터값

	_uint	m_iSet_Player;						///현재 선택중인 플레이어
	string	m_sStage;							///현재 선택중인 스테이지 이름
	string	m_sDir;								///현재 선택중인 방향 값	(고정된 4방향 각도)
	_vec3	m_vecDir;							///현재 선택중인 방향 벡터값(고정되지 않은 각도) 
	_float	m_fAngle;							///현재 각도

private:
	virtual void		Free();

};

END
