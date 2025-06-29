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

BEGIN(Engine)
/**
* @struct S_BLOCK
* @brief �� ����ü
*/
struct S_BLOCK {
	string Block_Name;
	_vec3 vPos;
	string Direction;
};
/**
* @struct S_Tile
* @brief Ÿ�� ����ü
*/
struct S_TILE {
	string Tile_Name;
	_vec3 vPos;
	string Direction;
};
/**
* @struct S_ENVIRONMENT
* @brief ȯ�� ������ ����ü
*/
struct S_ENVIRONMENT {
	string Tile_Name;
	_vec3 vPos;
	string Direction;
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
* @struct S_CAM
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

/// ///////////////////////////////////////////////////////////////////////////

class ENGINE_DLL CMapToolMgr : public CBase
{
	DECLARE_SINGLETON(CMapToolMgr)

private:
	explicit CMapToolMgr();
	virtual ~CMapToolMgr();

public:
	void	Plant_Block();				///��ġ�� ����� ������ ���� ����Ʈ�� ����
	void	Plant_Tile();				///��ġ�� Ÿ���� ������ ���� ����Ʈ�� ����
	void	Plant_Environment();		///��ġ�� ȯ�� ������Ʈ�� ���� ����Ʈ�� ����
	void	Plant_Camera();				///���� �ٶ󺸰� �ִ� ���� Eye�� at�� ����
	void	Plant_Player();				///�÷��̾��� ���� ��ġ ����
	HRESULT Save_Json();				///json�� ������ ����
	HRESULT Load_Json();				///json ������ �ҷ�����
	void	Reset();					///���� ���͸���Ʈ�� ��� �ʱ�ȭ �� ��ġ�� ��� ����

private:
	void	Key_Input();				///Ű�Է�

private:
	vector<S_BLOCK*>		m_pBlockData;		///���� ��ġ�Ǿ��ִ� ����� ������
	vector<S_TILE*>			m_pTileData;		///���� ��ġ�Ǿ��ִ� Ÿ���� ������
	vector<S_ENVIRONMENT*>	m_pEnvData;			///���� ��ġ �Ǿ��ִ� ȯ������ ������
	S_CAM					m_tCam;			///���� �÷��̾���� ���� ��ġ
	S_PLAYER				m_tPlayer;

	map<string, vector<S_STAGE>> m_mapJson;		///��ü ���������� �����Ͱ�

	_uint	m_iSet_Player;						///���� �������� �÷��̾�
	string	m_sStage;							///���� �������� �������� �̸�
	string	m_sDir;								///���� �������� ���� ��	(������ 4���� ����)
	_vec3	m_vecDir;							///���� �������� ���� ���Ͱ�(�������� ���� ����) 
	_float	m_fAngle;							///���� ����

private:
	virtual void		Free();

};

END
