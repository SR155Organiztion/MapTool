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
	string Item;
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
};

/**
* @struct S_ENVOjbect
* @brief ȯ�� ������Ʈ�� ������Ʈ ����ü
*/

struct S_ENVOBJECT {
	string Env_Type;
	_vec3 vPos;
	float fAngle;
	_vec3 vScale;
};

/**
* @struct S_ENVIRONMENT
* @brief ȯ�� ������Ʈ ����ü
*/

struct S_ENVIRONMENT {
	std::vector<S_TILE> Tile;
	std::vector<S_ENVOBJECT> EnvObject;
};

/**
* @struct S_CAM
* @brief ī�޶� ������ ���� ����ü
*/
struct S_MAPSIZE {
	int iX;
	int iY;
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
* @struct S_EVENT
* @brief ī�޶� ������ ���� ����ü
*/
struct S_EVENT {
	bool bEvent;
	float fEventTime;
};

/**
* @struct S_STAGE
* @brief ���������� ��� ������ ���� ����ü
*/
struct S_STAGE {
	S_MAPSIZE MapSize;
	S_PLAYER Player;
	float Time;
	S_EVENT Event;
	std::vector<string> Recipe;
	S_GAMEOBJECT GameObject;
	S_ENVIRONMENT Environment;
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
	void    Plant_Block(string _sType, _vec3 _vPos, string _sDir, string _sItem = ""); /// �ҷ������ ����ġ
	void	Break_Block(_vec3 _vPos);
	
	void	Plant_Tile(_vec3 _vPos);		///��ġ�� Ÿ���� ������ ���� ����Ʈ�� ����
	void	Plant_HexTile(_vec3 _vPos);
	void    Plant_Tile(string _sType, _vec3 _vPos, string _sDir); /// �ҷ������ ����ġ
	void	Break_Tile(_vec3 _vPos);
	
	void	Plant_Environment(_vec3 _vPos);
	void	Plant_Environment(string _sType, _vec3 _vPos, float _fAngle, _vec3 _vScale);	///��ġ�� ȯ�� ������Ʈ�� ���� ����Ʈ�� ����
	void	Break_Environment(_vec3 _vPos);

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
	void    Set_Event(bool _bEvent, float _fTime) { m_tEvent.bEvent = _bEvent; m_tEvent.fEventTime = _fTime; }
	void	Add_Recipe(string _s) { m_sRecipeVec.push_back(_s); }
	vector<string>* Get_Recipe() { return &m_sRecipeVec; }
	const map<string, _bool>& Get_RecipeMap() { return m_mapRecipes; }
	vector<string>* Get_NameVec();
	void	Set_MapSize(int _iX, int _iY);
	vector<S_ENVOBJECT>* Get_EnvObjectVec() { return &m_tEnvObjVec; }

	void NextObject();
	_uint Get_NowObject();

	void NextRotate();
	void PrevRotate();
	_vec3 Get_DirLook();

	void TurnRight(_float _fTimeDelta);
	void TurnLeft(_float _fTimeDelta);
	_float Get_NowAngle();
	
	void NextStation();
	void PrevStation();
	_uint Get_NowStation();

	void NextRcTile();
	void PrevRcTile();
	_uint Get_NowRcTile();

	void NextEnvObject();
	void PrevEnvObject();
	_uint Get_NowEnvObject();

	void ChangePlayer();
	_uint Get_NowPlayer();

	string Get_Dir();

	//���� ��ȯ
	_vec3	String_To_Dir(string& _s);
	string	Dir_To_String();

	_uint	String_To_Block(string& _s);
	string	Block_To_String();

	_uint	String_To_Tile(string& _s);
	string	Tile_To_String();

	_uint   String_To_Food(string& _s);
	string  Food_To_String();

	_uint   String_To_Item(string& _s);
	string  Item_To_String();

	_uint	String_To_EnvObj(string& _s);
	string  EnvObj_To_String();

	string Stage_To_String();
	_uint  String_To_Stage();

	void Print_CurrentDataCounts(int& iBlockSize, int& iTileSize, int& iEnvSize, int& iRecipeSize)
	{
		iBlockSize	= (int)m_tBlockVec.size();
		iTileSize	= (int)m_tTileVec.size();
		iEnvSize	= (int)m_tEnvObjVec.size();
		iRecipeSize = (int)m_sRecipeVec.size();
	}

private:
	vector<S_BLOCK>			m_tBlockVec;		///���� ��ġ�Ǿ��ִ� ����� ������
	vector<S_TILE>			m_tTileVec;			///���� ��ġ�Ǿ��ִ� Ÿ���� ������
	vector<S_ENVOBJECT>		m_tEnvObjVec;		///���� ��ġ �Ǿ��ִ� ȯ�������Ʈ�� ������
	vector<string>			m_sRecipeVec;		///���� ���������� ���������� �������ִ� �����Ǹ� �����Ұ���
	S_MAPSIZE				m_tMapSize;			///���� �� ũ��
	S_PLAYER				m_tPlayer;			///���� �÷��̾���� ���� ��ġ
	S_EVENT					m_tEvent;			///���� ������������ �߻��� �̺�Ʈ ���οͽð�
	_float					m_fTimer;			///���� ���������� ���ӽð�

	map<string, S_STAGE> m_mapJson;				///��ü ���������� �����Ͱ�
	string				 m_sName;				///�����ϰų� �ҷ��� �̸�

	map<string, _bool>	 m_mapRecipes;			///���� ���������� �������ִ� ������ ����
	//��������-----------------------------------------------------------------------------------
	_uint			m_iSelectName;				///���� �������� ��������
	vector<string>	m_sNameVec;					///��ü �������� �̸�
	//�÷��̾�-----------------------------------------------------------------------------------
	_uint			m_iPlayer;					///���� �������� �÷��̾�	
	//������Ʈ-----------------------------------------------------------------------------------
	_int			m_iObject;					///���� �������� ������Ʈ
	//��---------------------------------------------------------------------------------------
	_int			m_iStation;					///���� �������� �����̼�
	_int			m_iIngredient;				///���� �������� ���
	//Ÿ��---------------------------------------------------------------------------------------
	_int			m_iRcTile;					///���� �������� Ÿ�� �̹��� ��ȣ
	//ȯ�������Ʈ-------------------------------------------------------------------------------
	_uint			m_iEnvObject;				///���� �������� ȯ�������Ʈ ��ȣ
	//����---------------------------------------------------------------------------------------
	_int			m_iDir;						///���� �������� ���� ��	(������ 6 ����)
	_vec3			m_vecDir;					///���� �������� ���� ���Ͱ�(�������� ���� ����) 
	_float			m_fAngle;					///���� ����
	_bool			m_bCreate;					///�����͸� �����Ұ��� ����

private:
	virtual void		Free();

};

END
