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
	string Item;
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
* @struct S_BLOCK
* @brief 블럭 구조체
*/
struct S_GAMEOBJECT {
	std::vector<S_BLOCK> Block;
};

/**
* @struct S_ENVOjbect
* @brief 환경 오브젝트의 오브젝트 구조체
*/

struct S_ENVOBJECT {
	string Env_Type;
	_vec3 vPos;
	float fAngle;
	_vec3 vScale;
};

/**
* @struct S_ENVIRONMENT
* @brief 환경 오브젝트 구조체
*/

struct S_ENVIRONMENT {
	std::vector<S_TILE> Tile;
	std::vector<S_ENVOBJECT> EnvObject;
};

/**
* @struct S_CAM
* @brief 카메라를 저장할 벡터 구조체
*/
struct S_MAPSIZE {
	int iX;
	int iY;
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
* @struct S_EVENT
* @brief 카메라를 저장할 벡터 구조체
*/
struct S_EVENT {
	bool bEvent;
	float fEventTime;
};

/**
* @struct S_STAGE
* @brief 스테이지의 모든 정보를 가진 구조체
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
	void	Plant_Block(_vec3 _vPos);		///설치한 블록의 정보를 벡터 리스트에 넣음
	void    Plant_Block(string _sType, _vec3 _vPos, string _sDir, string _sItem = ""); /// 불러오기용 블럭설치
	void	Break_Block(_vec3 _vPos);
	
	void	Plant_Tile(_vec3 _vPos);		///설치한 타일의 정보를 벡터 리스트에 넣음
	void	Plant_HexTile(_vec3 _vPos);
	void    Plant_Tile(string _sType, _vec3 _vPos, string _sDir); /// 불러오기용 블럭설치
	void	Break_Tile(_vec3 _vPos);
	
	void	Plant_Environment(_vec3 _vPos);
	void	Plant_Environment(string _sType, _vec3 _vPos, float _fAngle, _vec3 _vScale);	///설치한 환경 오브젝트를 벡터 리스트에 넣음
	void	Break_Environment(_vec3 _vPos);

	void	Plant_Camera(_vec3 _vEye, _vec3 _vAt);						///현재 바라보고 있는 곳의 Eye와 at을 저장
	void	Plant_Player(_int _iPlayer,_vec3 _vPos);					///플레이어의 스폰 위치 설정

	HRESULT Save_Json();				///json에 데이터 저장
	HRESULT Load_Json();				///json 데이터 불러오기
	void	Select_Map();				///맵 불러오기
	void	Delete_Map(string _s);
	void	Reset();					///현재 벡터리스트를 모두 초기화 및 설치한 블록 제거
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

	//글자 변환
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
	vector<S_BLOCK>			m_tBlockVec;		///현재 설치되어있는 블록의 데이터
	vector<S_TILE>			m_tTileVec;			///현재 설치되어있는 타일의 데이터
	vector<S_ENVOBJECT>		m_tEnvObjVec;		///현재 설치 되어있는 환경오브젝트의 데이터
	vector<string>			m_sRecipeVec;		///현재 스테이지가 실질적으로 가지고있는 레시피를 저장할공간
	S_MAPSIZE				m_tMapSize;			///현재 맵 크기
	S_PLAYER				m_tPlayer;			///현재 플레이어들의 시작 위치
	S_EVENT					m_tEvent;			///현재 스테이지에서 발생할 이벤트 여부와시간
	_float					m_fTimer;			///현재 스테이지의 게임시간

	map<string, S_STAGE> m_mapJson;				///전체 스테이지의 데이터값
	string				 m_sName;				///저장하거나 불러올 이름

	map<string, _bool>	 m_mapRecipes;			///현재 스테이지가 가지고있는 레시피 저장
	//스테이지-----------------------------------------------------------------------------------
	_uint			m_iSelectName;				///내가 선택중인 스테이지
	vector<string>	m_sNameVec;					///전체 스테이지 이름
	//플레이어-----------------------------------------------------------------------------------
	_uint			m_iPlayer;					///현재 선택중인 플레이어	
	//오브젝트-----------------------------------------------------------------------------------
	_int			m_iObject;					///현재 선택중인 오브젝트
	//블럭---------------------------------------------------------------------------------------
	_int			m_iStation;					///현재 선택중인 스테이션
	_int			m_iIngredient;				///현재 선택중인 재료
	//타일---------------------------------------------------------------------------------------
	_int			m_iRcTile;					///현재 선택중인 타일 이미지 번호
	//환경오브젝트-------------------------------------------------------------------------------
	_uint			m_iEnvObject;				///현재 선택중인 환경오브젝트 번호
	//각도---------------------------------------------------------------------------------------
	_int			m_iDir;						///현재 선택중인 방향 값	(고정된 6 방향)
	_vec3			m_vecDir;					///현재 선택중인 방향 벡터값(고정되지 않은 각도) 
	_float			m_fAngle;					///현재 각도
	_bool			m_bCreate;					///데이터를 생성할건지 여부

private:
	virtual void		Free();

};

END
