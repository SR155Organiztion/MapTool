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
* @struct S_BLOCK
* @brief �� ����ü
*/
struct S_GAMEOBJECT {
	std::vector<S_BLOCK> Block;
	std::vector<S_TILE> Tile;
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
	float Time;
	std::vector<string> Recipe;
	S_GAMEOBJECT GameObject;
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
	void	Plant_Block(_vec3 _vPos);		///��ġ�� ����� ������ ���� ����Ʈ�� ����
	void    Plant_Block(string _sType, _vec3 _vPos, string _sDir); /// ����� ����ġ
	void	Break_Block(_vec3 _vPos);
	void	Plant_Tile(_vec3 _vPos);		///��ġ�� Ÿ���� ������ ���� ����Ʈ�� ����
	void    Plant_Tile(string _sType, _vec3 _vPos, string _sDir); /// ����� ����ġ
	void	Break_Tile(_vec3 _vPos);
	void	Plant_Environment(string _sType, _vec3 _vPos, _vec3 _vDir);	///��ġ�� ȯ�� ������Ʈ�� ���� ����Ʈ�� ����
	void	Plant_Camera(_vec3 _vEye, _vec3 _vAt);						///���� �ٶ󺸰� �ִ� ���� Eye�� at�� ����
	void	Plant_Player(_int _iPlayer,_vec3 _vPos);					///�÷��̾��� ���� ��ġ ����

	HRESULT Save_Json();				///json�� ������ ����
	HRESULT Load_Json();				///json ������ �ҷ�����
	void	Select_Map();				///�� �ҷ�����
	void	Delete_Map(string _s);
	void	Reset();					///���� ���͸���Ʈ�� ��� �ʱ�ȭ �� ��ġ�� ��� ����
	void	Set_NoCreate();
public:
	S_STAGE Get_Data(string s);
	string  Get_Name();
	void    Set_Name(string _s);
	void	Set_Timer(float _fTimer) { m_fTimer = _fTimer; }
	void	Add_Recipe(string _s) { m_sRecipeVec.push_back(_s); }
	vector<string>* Get_Recipe() { return &m_sRecipeVec; }
	vector<string>* Get_NameVec();
	

	void NextRotate();
	void PrevRotate();
	_vec3 Get_DirLook();
	
	void NextStation();
	void PrevStation();
	_uint Get_NowStation();

	void NextObject();
	_uint Get_NowObject();

	void NextRcTile();
	void PrevRcTile();
	_uint Get_NowRcTile();

	string Get_Dir();

	_vec3	String_To_Dir(string& _s);
	_uint	String_To_Block(string& _s);
	_uint	String_To_Tile(string& _s);

private:
	void	Dummy_Data();				///�׽�Ʈ��
	string	Dir_To_String();
	string	Block_To_String();
	string	Tile_To_String();



private:
	vector<S_BLOCK>			m_tBlockVec;		///���� ��ġ�Ǿ��ִ� ����� ������
	vector<S_TILE>			m_tTileVec;			///���� ��ġ�Ǿ��ִ� Ÿ���� ������
	vector<S_ENVIRONMENT>	m_tEnvVec;			///���� ��ġ �Ǿ��ִ� ȯ������ ������
	vector<string>			m_sRecipeVec;		///���� �������� 
	S_CAM					m_tCam;				///��ġ�� �� ī�޶��� 
	S_PLAYER				m_tPlayer;			///���� �÷��̾���� ���� ��ġ
	_float					m_fTimer;			///���� ���������� ���ӽð�

	map<string, S_STAGE> m_mapJson;				///��ü ���������� �����Ͱ�
	string				 m_sName;				///�����ϰų� �ҷ��� �̸�

	//��������-----------------------------------------------------------------------------------
	_uint				 m_iSelectName;			///���� �������� ��������
	//�÷��̾�-----------------------------------------------------------------------------------
	_uint			m_iSet_Player;				///���� �������� �÷��̾�
	vector<string>	m_sNameVec;					///��ü �������� �̸�
	//������Ʈ-----------------------------------------------------------------------------------
	_int			m_iObject;					///���� �������� ������Ʈ
	//��---------------------------------------------------------------------------------------
	_int			m_iStation;					///���� �������� �����̼�
	_int			m_iIngredient;				///���� �������� ���
	//Ÿ��---------------------------------------------------------------------------------------
	_int			m_iRcTile;					///���� �������� Ÿ�� �̹��� ��ȣ
	//����---------------------------------------------------------------------------------------
	_int			m_iDir;						///���� �������� ���� ��	(������ 6 ����)
	_vec3			m_vecDir;					///���� �������� ���� ���Ͱ�(�������� ���� ����) 
	_float			m_fAngle;					///���� ����
	_bool			m_bCreate;					///�����͸� �����Ұ��� ����

private:
	virtual void		Free();

};

END
