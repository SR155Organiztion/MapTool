#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

struct S_BLOCK {
	string Block_Name;
	_vec3 vPos;
	string Direction;
};

struct S_TILE {
	string Tile_Name;
	_vec3 vPos;
	string Direction;
};

struct S_ENVIRONMENT {
	string Tile_Name;
	_vec3 vPos;
	string Direction;
};

struct S_CAM {
	_vec3 vEye;
	_vec3 vAt;
};

struct S_PLAYER {
	_vec3 P1;
	_vec3 P2;
};

struct S_STAGE {
	S_CAM						Cam;
	S_PLAYER					Player;
	std::vector<string>			Recipe;
	std::vector<S_BLOCK>		Block;
	std::vector<S_TILE>			Tiles;
	std::vector<S_ENVIRONMENT>	Environment;
};

class ENGINE_DLL CMapToolMgr : public CBase
{
	DECLARE_SINGLETON(CMapToolMgr)

private:
	explicit CMapToolMgr();
	virtual ~CMapToolMgr();

public:
	void	Plant_Block();				
	void	Plant_Tile();				
	void	Plant_Environment();		
	void	Plant_Camera();				
	void	Plant_Player();				
	HRESULT Save_Json();				
	HRESULT Load_Json();				
	void	Reset();					

private:
	void	Key_Input();				

private:
	vector<S_BLOCK*>		m_pBlockData;		
	vector<S_TILE*>			m_pTileData;		
	vector<S_ENVIRONMENT*>	m_pEnvData;			
	S_CAM					m_tCam;				
	S_PLAYER				m_tPlayer;

	map<string, vector<S_STAGE>> m_mapJson;		

	_uint	m_iSet_Player;						
	string	m_sStage;							
	string	m_sDir;								
	_vec3	m_vecDir;							
	_float	m_fAngle;							

private:
	virtual void		Free();
};

END
