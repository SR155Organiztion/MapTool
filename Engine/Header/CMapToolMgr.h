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
#include "../Header/json.hpp"

using json = nlohmann::json;


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



NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(_vec3, x, y, z)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_BLOCK, Block_Type, vPos, Direction)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_TILE, Tile_Type, vPos, Direction)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_ENVIRONMENT, Env_Type, vPos, Direction)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_CAM, vEye, vAt)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_PLAYER, P1, P2)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_STAGE, Cam, Player, Recipe, Block, Tiles, Environment);
///////////////////////////////////////////////////////////////////////////////
BEGIN(Engine)
class ENGINE_DLL CMapToolMgr : public CBase
{
	DECLARE_SINGLETON(CMapToolMgr)

private:
	explicit CMapToolMgr();
	virtual ~CMapToolMgr();

public:
	void	Ready_Manager();			

public:
	void	Plant_Block();				///설치한 블록의 정보를 벡터 리스트에 넣음
	void	Plant_Tile();				///설치한 타일의 정보를 벡터 리스트에 넣음
	void	Plant_Environment();		///설치한 환경 오브젝트를 벡터 리스트에 넣음
	void	Plant_Camera();				///현재 바라보고 있는 곳의 Eye와 at을 저장
	void	Plant_Player();				///플레이어의 스폰 위치 설정

	HRESULT Save_Json();				///json에 데이터 저장
	HRESULT Load_Json();				///json 데이터 불러오기
	void	Select_Map();				///맵 불러오기
	void	Reset();					///현재 벡터리스트를 모두 초기화 및 설치한 블록 제거

public:
	S_STAGE Get_Data(string s);

private:
	void	Key_Input();				///키입력

	void	Dummy_Data();				///테스트용

private:
	vector<S_BLOCK>			m_tBlockVec;		///현재 설치되어있는 블록의 데이터
	vector<S_TILE>			m_tTileVec;			///현재 설치되어있는 타일의 데이터
	vector<S_ENVIRONMENT>	m_tEnvVec;			///현재 설치 되어있는 환경블록의 데이터
	vector<string>			m_sRecipeVec;		
	S_CAM					m_tCam;				///설치를 한 카메라의 
	S_PLAYER				m_tPlayer;			///현재 플레이어들의 시작 위치

	map<string, S_STAGE> m_mapJson;		///전체 스테이지의 데이터값

	_uint	m_iSet_Player;						///현재 선택중인 플레이어
	vector<string>	m_sNameVec;					///전체 스테이지 이름
	_uint	m_iSelectName;						///내가 선택중인 스테이지
	string	m_sName;							///저장하거나 불러올 이름
	string	m_sDir;								///현재 선택중인 방향 값	(고정된 4방향 각도)
	_vec3	m_vecDir;							///현재 선택중인 방향 벡터값(고정되지 않은 각도) 
	_float	m_fAngle;							///현재 각도

private:
	virtual void		Free();

};

////////////////////////////////////////////////////////////////////
//////// _vec3 JSON 변환		
////////////////////////////////////////////////////////////////////
//inline void to_json(json& j, const _vec3& v) {
//	j = json{ {"x", v.x}, {"y", v.y}, {"z", v.z} };
//}
//
//inline void from_json(const json& j, _vec3& v) {
//	j.at("x").get_to(v.x);
//	j.at("y").get_to(v.y);
//	j.at("z").get_to(v.z);
//}
//
//
////////////////////////////////////////////////////////////////////
//////// S_BLOCK JSON 변환		
////////////////////////////////////////////////////////////////////
//inline void to_json(json& j, const Engine::S_BLOCK& b) {
//	j = json{
//		{"Block_Type", b.Block_Type},
//		{"vPos", b.vPos },
//		{"Direction", b.Direction}
//	};
//}
//
//inline void from_json(const json& j, Engine::S_BLOCK& b) {
//	j.at("Block_Type").get_to(b.Block_Type);
//	j.at("vPos").get_to(to_json());
//	j.at("Direction").get_to(b.Direction);
//}
//
//
////////////////////////////////////////////////////////////////////
//////// S_TILE JSON 변환			
////////////////////////////////////////////////////////////////////
//inline void to_json(json& j, const Engine::S_TILE& t) {
//	j = json{
//		{"Tile_Type", t.Tile_Type},
//		{"vPos", t.vPos},
//		{"Direction", t.Direction}
//	};
//}
//
//inline void from_json(const json& j, Engine::S_TILE& t) {
//	j.at("Tile_Type").get_to(t.Tile_Type);
//	j.at("vPos").get_to(t.vPos);
//	j.at("Direction").get_to(t.Direction);
//}
//
//
////////////////////////////////////////////////////////////////////
////////	S_ENVIRONMENT JSON 변환
////////////////////////////////////////////////////////////////////
//inline void to_json(json& j, const Engine::S_ENVIRONMENT& e) {
//	j = json{
//		{"Env_Type", e.Env_Type},
//		{"vPos", e.vPos},
//		{"Direction", e.Direction}
//	};
//}
//
//inline void from_json(const json& j, Engine::S_ENVIRONMENT& e) {
//	j.at("Env_Type").get_to(e.Env_Type);
//	j.at("vPos").get_to(e.vPos);
//	j.at("Direction").get_to(e.Direction);
//}
//
//
////////////////////////////////////////////////////////////////////
////////	S_CAM JSON 변환
////////////////////////////////////////////////////////////////////
//inline void to_json(json& j, const Engine::S_CAM& c) {
//	j = json{
//		{"Eye", c.vEye},
//		{"At", c.vAt}		
//	};
//}
//
//inline void from_json(const json& j, Engine::S_CAM& c) {
//	j.at("Eye").get_to(c.vEye);
//	j.at("At").get_to(c.vAt);
//}
//
//
////////////////////////////////////////////////////////////////////
////////	S_Player Json 변환
////////////////////////////////////////////////////////////////////
//inline void to_json(json& j, const Engine::S_PLAYER& p) {
//	j = json{
//		{"P1", p.P1},
//		{"P2", p.P2}
//	};
//}
//
//inline void from_json(const json& j, Engine::S_PLAYER& p) {
//	j.at("P1").get_to(p.P1);
//	j.at("P2").get_to(p.P2);
//}
//
////////////////////////////////////////////////////////////////////
////////	S_RECIPE Json 변환 할게 없음
////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
////////	S_STAGE Json 변환
////////////////////////////////////////////////////////////////////
//inline void to_json(json& j, const Engine::S_STAGE& s) {
//	j = json{
//		{"Cam", s.Cam},
//		{"Player", s.Player},
//		{"Recipe", s.Recipe},
//		{"Block", s.Block},
//		{"Tiles", s.Tiles},
//		{"Environment", s.Environment}
//	};
//}
//
//inline void from_json(const json& j, Engine::S_STAGE& s) {
//	j.at("Cam").get_to(s.Cam);
//	j.at("Player").get_to(s.Player);
//	j.at("Recipe").get_to(s.Recipe);
//	j.at("Block").get_to(s.Block);
//	j.at("Tiles").get_to(s.Tiles);
//	j.at("Environment").get_to(s.Environment);
//}

END
