/**
* @file		CMapToolMgr.h
* @date		2025-06-30
* @author	����ȣ
* @brief	���� �Ŵ���
* @details  �� ������ ������� �Ѱ����ϴ� �Ŵ���
*/

#pragma once
#include "CBase.h"
#include "Engine_Define.h"

/**
* @struct S_BLOCK
* @brief �� ����ü
*/
struct S_BLOCK {
	string Block_Type;
	_vec3 vPos;
	string Direction;
};


/**
* @struct S_Tile
* @brief Ÿ�� ����ü
*/
struct S_TILE {
	string Tile_Type;
	_vec3 vPos;
	string Direction;
};


/**
* @struct S_ENVIRONMENT
* @brief ȯ�� ������ ����ü
*/
struct S_ENVIRONMENT {
	string Env_Type;
	_vec3 vPos;
	_vec3 Direction;
};


/**
* @struct S_CAM
* @brief ī�޶� ������ ���� ����ü
*/
struct S_CAM {
	_vec3 vEye;
	_vec3 vAt;
};
/**
* @struct S_PLAYER
* @brief ī�޶� ������ ���� ����ü
*/
struct S_PLAYER {
	_vec3 P1;
	_vec3 P2;
};

/**
* @struct S_STAGE
* @brief ���������� ��� ������ ���� ����ü
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
	void	Plant_Block(string _sType, _vec3 _vPos, string _sDir);		///��ġ�� ����� ������ ���� ����Ʈ�� ����
	void	Plant_Tile(string _sType, _vec3 _vPos, string _sDir);		///��ġ�� Ÿ���� ������ ���� ����Ʈ�� ����
	void	Plant_Environment(string _sType, _vec3 _vPos, _vec3 _vDir);	///��ġ�� ȯ�� ������Ʈ�� ���� ����Ʈ�� ����
	void	Plant_Camera(_vec3 _vEye, _vec3 _vAt);						///���� �ٶ󺸰� �ִ� ���� Eye�� at�� ����
	void	Plant_Player(_int _iPlayer,_vec3 _vPos);					///�÷��̾��� ���� ��ġ ����

	HRESULT Save_Json();				///json�� ������ ����
	HRESULT Load_Json();				///json ������ �ҷ�����
	void	Select_Map();				///�� �ҷ�����
	void	Reset();					///���� ���͸���Ʈ�� ��� �ʱ�ȭ �� ��ġ�� ��� ����

public:
	S_STAGE Get_Data(string s);

private:
	void	Key_Input();				///Ű�Է�
	void	Dummy_Data();				///�׽�Ʈ��

private:
	vector<S_BLOCK>			m_tBlockVec;		///���� ��ġ�Ǿ��ִ� ����� ������
	vector<S_TILE>			m_tTileVec;			///���� ��ġ�Ǿ��ִ� Ÿ���� ������
	vector<S_ENVIRONMENT>	m_tEnvVec;			///���� ��ġ �Ǿ��ִ� ȯ������ ������
	vector<string>			m_sRecipeVec;		
	S_CAM					m_tCam;				///��ġ�� �� ī�޶��� 
	S_PLAYER				m_tPlayer;			///���� �÷��̾���� ���� ��ġ

	map<string, S_STAGE> m_mapJson;				///��ü ���������� �����Ͱ�

	_uint			m_iSet_Player;				///���� �������� �÷��̾�
	vector<string>	m_sNameVec;					///��ü �������� �̸�
	_uint			m_iSelectName;				///���� �������� ��������
	string			m_sName;					///�����ϰų� �ҷ��� �̸�
	DIRECTIONID		m_eDir;						///���� �������� ���� ��	(������ 6 ����)
	_vec3			m_vecDir;					///���� �������� ���� ���Ͱ�(�������� ���� ����) 
	_float			m_fAngle;					///���� ����

private:
	virtual void		Free();

};

////////////////////////////////////////////////////////////////////
//////// _vec3 JSON ��ȯ		
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
//////// S_BLOCK JSON ��ȯ		
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
//////// S_TILE JSON ��ȯ			
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
////////	S_ENVIRONMENT JSON ��ȯ
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
////////	S_CAM JSON ��ȯ
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
////////	S_Player Json ��ȯ
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
////////	S_RECIPE Json ��ȯ �Ұ� ����
////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
////////	S_STAGE Json ��ȯ
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
