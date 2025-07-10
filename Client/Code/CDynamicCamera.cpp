#include "pch.h"
#include "CDynamicCamera.h"
#include "CDInputMgr.h"
#include "CTransform.h"
#include "CTerrainTex.h"
#include "CManagement.h"
#include "CTerrain.h"
#include "CCalculator.h"
#include "CProtoMgr.h"
#include "CMapToolMgr.h"
#include "CBlock.h"
#include "CRcTile.h"
#include "CCollisionMgr.h"
#include "CImguiMgr.h"
#include <tchar.h>
#include "CPlayerPoint.h"
#include "CHexTile.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCamera(pGraphicDev), m_bFix(false), m_bCheck(false),
	m_bClickedLB(false), m_bClickedRB(false), m_bPressedQ(false), m_bPressedE(false), m_bPressedR(false), m_bPressedL(false), m_bPressedP(false)
{
}

CDynamicCamera::~CDynamicCamera()
{
}

HRESULT CDynamicCamera::Ready_GameObject(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	m_pCalculatorCom = dynamic_cast<Engine::CCalculator*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Calculator"));
	if (nullptr == m_pCalculatorCom)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Calculator", m_pCalculatorCom });

	if (FAILED(Engine::CCamera::Ready_GameObject()))
		return E_FAIL;

	m_fSpeed = 5.f;

	LoadCallBackToImguiMgr();
	ClearCallBackToImguiMgr();
	TerrianEnableCallBackToImguiMgr();

	return S_OK;
}

_int CDynamicCamera::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = Engine::CCamera::Update_GameObject(fTimeDelta);

	return iExit;
}

void CDynamicCamera::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CCamera::LateUpdate_GameObject(fTimeDelta);

	Key_Input(fTimeDelta);

	if (false == m_bFix)
	{
		Mouse_Move();
		Mouse_Fix();
	}
}

void CDynamicCamera::Key_Input(const _float& fTimeDelta)
{
	_matrix	matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);
	_float fSpeed = m_fSpeed;

	//달리기
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_LSHIFT) & 0x80) {
		fSpeed *= 2.f;
	}
	else {
		fSpeed = m_fSpeed;
	}
	//앞
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_W) & 0x80)
	{
		_vec3	vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * fTimeDelta * fSpeed;

		m_vEye += vLength;
		m_vAt += vLength;
	}
	//뒤
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_S) & 0x80)
	{
		_vec3	vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * fTimeDelta * fSpeed;

		m_vEye -= vLength;
		m_vAt  -= vLength;
	}
	//오른쪽
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_D) & 0x80)
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta * fSpeed;

		m_vEye += vLength;
		m_vAt += vLength;
	}
	//왼쪽
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_A) & 0x80)
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta * fSpeed;

		m_vEye -= vLength;
		m_vAt  -= vLength;
	}  
	//올라가기
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_SPACE) & 0x80)
	{		
		m_vEye.y += fTimeDelta * m_fSpeed;;
		m_vAt.y += fTimeDelta * m_fSpeed;;
	}
	//앉기
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_LCONTROL) & 0x80)
	{
		m_vEye.y -= fTimeDelta * m_fSpeed;;
		m_vAt.y -= fTimeDelta * m_fSpeed;;
	}
	//마우스 고정
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_Z) & 0x80)
	{
		if (m_bCheck)
			return;

		m_bCheck = true;

		if (m_bFix)
			m_bFix = false;

		else
			m_bFix = true;
	}
	else
	{
		m_bCheck = false;
	}
	//설치
	if (CDInputMgr::GetInstance()->Get_DIMouseState(DIM_LB) & 0x80) {
		if (!m_bFix) {
			if (!m_bClickedLB) {
				Create_Objects();
				m_bClickedLB = true;
			}
		}
	}
	else {
		m_bClickedLB = false;
	}
	//우클릭
	if (CDInputMgr::GetInstance()->Get_DIMouseState(DIM_RB) & 0x80) {
		if (!m_bFix) {
			if (!m_bClickedRB) {
				Delete_Objects();
				m_bClickedRB = true;
			}
		}
	}
	else {
		m_bClickedRB = false;
	}

	//블럭 방향회전
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_Q) & 0x80)
	{
		if (!m_bPressedQ) {
			CMapToolMgr::GetInstance()->PrevRotate();
			m_bPressedQ = true;
		}
	}
	else {
		m_bPressedQ = false;
	}
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_E) & 0x80)
	{
		if (!m_bPressedE) {
			CMapToolMgr::GetInstance()->NextRotate();
			m_bPressedE = true;
		}
	}
	else {
		m_bPressedE = false;
	}
	///블럭 변경
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_1) & 0x80)
	{
		if (!m_bPressed1) {
			Prev_Type();
			m_bPressed1 = true;
		}
	}
	else {
		m_bPressed1 = false;
	}
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_3) & 0x80)
	{
		if (!m_bPressed3) {
			Next_Type();
			m_bPressed3 = true;
		}
	}
	else {
		m_bPressed3 = false;
	}
	///오브젝트 변경
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_R) & 0x80)
	{
		if (!m_bPressedR) {
			CMapToolMgr::GetInstance()->NextObject();
			m_bPressedR = true;
		}
	}
	else {
		m_bPressedR = false;
	}

	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_P) & 0x80)
	{
		if (!m_bPressedP) {
			Create_HexTile();
			m_bPressedP = true;
		}
	}
	else {
		m_bPressedP = false;
	}

	if (false == m_bFix)
		return;
}

void CDynamicCamera::Mouse_Move()
{
	_matrix	matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);


	_long	dwMouseMove(0);

	if (dwMouseMove = CDInputMgr::GetInstance()->Get_DIMouseMove(DIMS_Y))
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLook = m_vAt - m_vEye;
		_matrix matRot;

		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 10.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;
	}

	if (dwMouseMove = CDInputMgr::GetInstance()->Get_DIMouseMove(DIMS_X))
	{
		_vec3	vUp{ 0.f, 1.f, 0.f };		

		_vec3	vLook = m_vAt - m_vEye;
		_matrix matRot;

		D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove / 10.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;
	}

}

void CDynamicCamera::Mouse_Fix()
{
	POINT	ptMouse{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

void CDynamicCamera::ALL_RESET()
{
	CMapToolMgr::GetInstance()->Reset();
	CScene* pScene = CManagement::GetInstance()->Get_Scene();

	//환경 레이어
	CLayer* pLayer = pScene->Get_Layer(L"Environment_Layer");
	for (auto it : *(pLayer->Get_ObjectMap())) {
		Safe_Release(it.second);
	}
	(pLayer->Get_ObjectMap())->clear();

	//타일 레이어
	pLayer = pScene->Get_Layer(L"Tile_Layer");
	for (auto it : *(pLayer->Get_ObjectMap())) {
		Safe_Release(it.second);
	}
	(pLayer->Get_ObjectMap())->clear();

	//블럭 레이어
	pLayer = pScene->Get_Layer(L"Block_Layer");
	for (auto it : *(pLayer->Get_ObjectMap())) {
		Safe_Release(it.second);
	}
	(pLayer->Get_ObjectMap())->clear();

	//게임오브젝트 레이어
	pLayer = pScene->Get_Layer(L"GameObject_Layer");
	map<const _tchar*, CGameObject*>* pObjectMap = pLayer->Get_ObjectMap();
	map<const _tchar*, CGameObject*>::iterator iter;
	CGameObject* pObj;
	_vec3 vTmp = { 0.f, 0.f, 0.f };

	iter = std::find_if(pObjectMap->begin(), pObjectMap->end(), CTag_Finder(L"1Player"));
	pObj = iter->second;
	CMapToolMgr::GetInstance()->Plant_Player(CMapToolMgr::GetInstance()->Get_NowPlayer(), vTmp);
	dynamic_cast<CPlayerPoint*>(pObj)->Set_Plant(FALSE);
	dynamic_cast<CPlayerPoint*>(pObj)->Set_Pos(vTmp);
	
	iter = std::find_if(pObjectMap->begin(), pObjectMap->end(), CTag_Finder(L"2Player"));
	pObj = iter->second;
	CMapToolMgr::GetInstance()->Plant_Player(CMapToolMgr::GetInstance()->Get_NowPlayer(), vTmp);
	dynamic_cast<CPlayerPoint*>(pObj)->Set_Plant(FALSE);
	dynamic_cast<CPlayerPoint*>(pObj)->Set_Pos(vTmp);
	
	for (auto& a : Release_tchar) {
		Safe_Delete(a);
	}
}

void CDynamicCamera::Load_Objects()
{
	int s_Index = 0;
	//초기화
	ALL_RESET();

	//씬 불러오기
	CScene* pScene = CManagement::GetInstance()->Get_Scene();
	CLayer* pLayer;
	//블럭레이어
	pLayer = pScene->Get_Layer(L"Block_Layer");
	for (auto it : (CMapToolMgr::GetInstance()->Get_Data(CMapToolMgr::GetInstance()->Get_Name()).GameObject.Block)) {
		
		if (nullptr == pLayer)
			return;

		//블럭 생성
		Engine::CGameObject* pGameObject = CBlock::Create(m_pGraphicDev);
		if (nullptr == pGameObject)
			return;
		//위치 설정
		CTransform* pObjectTransformCom = dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
		pObjectTransformCom->Set_Pos(it.vPos.x, it.vPos.y, it.vPos.z);

		//타입 설정
		dynamic_cast<CBlock*>(pGameObject)->Set_TextureNum(CMapToolMgr::GetInstance()->String_To_Block(it.Block_Type));
		
		//만약에 음식 상자 타입이면 추가로 생성하라
		if (CMapToolMgr::GetInstance()->String_To_Block(it.Block_Type) == Engine::STATIONID::S_CREATE) {
			const string prefix = "Create_";
			if (it.Block_Type.find(prefix) == 0 && it.Block_Type.length() > prefix.length()) {
				string szFood = (it.Block_Type.substr(prefix.length())); // "Create_" 이후 문자열만 반환

				dynamic_cast<CBlock*>(pGameObject)->Set_Create(CMapToolMgr::GetInstance()->String_To_Food(szFood));
			}
			
		}

		//만약에 아이템이 비어있지 않다면 추가로 생성하라
		if (it.Item != "") {
			dynamic_cast<CBlock*>(pGameObject)->Set_Tools(CMapToolMgr::GetInstance()->String_To_Item(it.Item));
		}

		//방향 설정
		_vec3 vLook = CMapToolMgr::GetInstance()->String_To_Dir((it.Direction));
		pObjectTransformCom->Set_Look(vLook.x, vLook.y, vLook.z);

		//생성
		_tchar szTag[64] = {};

		while (true) {
			_stprintf_s(szTag, 64, L"Block_%d", s_Index);
			_tchar* pTag = new _tchar[lstrlen(szTag) + 1];
			lstrcpy(pTag, szTag);

			if (SUCCEEDED(pLayer->Add_GameObject(pTag, pGameObject))) {
				Release_tchar.push_back(pTag);
				break; // 성공 시 탈출
			}
			else {
				Safe_Delete(pTag); // 실패 시 메모리 해제 후 시도 계속
			}
		}
		CMapToolMgr::GetInstance()->Plant_Block(it.Block_Type, it.vPos, it.Direction);
		s_Index++;
	}

	//타일레이어
	pLayer = pScene->Get_Layer(L"Tile_Layer");
	for (auto it : (CMapToolMgr::GetInstance()->Get_Data(CMapToolMgr::GetInstance()->Get_Name()).GameObject.Tile)) {

		if (nullptr == pLayer)
			return;

		if (it.Tile_Type == "TileHex") {

			Engine::CGameObject* pGameObject = CHexTile::Create(m_pGraphicDev);
			if (nullptr == pGameObject)
				return;
			//위치 설정
			CTransform* pObjectTransformCom = dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
			pObjectTransformCom->Set_Pos(it.vPos.x, it.vPos.y, it.vPos.z);

			//생성
			_tchar szTag[64] = {};

			while (true) {
				_stprintf_s(szTag, 64, L"Tile_%d", s_Index);
				_tchar* pTag = new _tchar[lstrlen(szTag) + 1];
				lstrcpy(pTag, szTag);

				if (SUCCEEDED(pLayer->Add_GameObject(pTag, pGameObject))) {
					Release_tchar.push_back(pTag);
					break; // 성공 시 탈출
				}
				else {
					Safe_Delete(pTag); // 실패 시 메모리 해제 후 시도 계속
				}
			}

			CMapToolMgr::GetInstance()->Plant_HexTile(it.vPos);
			s_Index++;
			continue;
		}

		//블럭 생성
		Engine::CGameObject* pGameObject = CRcTile::Create(m_pGraphicDev);
		if (nullptr == pGameObject)
			return;
		//위치 설정
		CTransform* pObjectTransformCom = dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
		pObjectTransformCom->Set_Pos(it.vPos.x, it.vPos.y, it.vPos.z);

		//타입 설정
		dynamic_cast<CRcTile*>(pGameObject)->Set_TextureNum(CMapToolMgr::GetInstance()->String_To_Tile(it.Tile_Type));

		//방향 설정
		_vec3 vLook = CMapToolMgr::GetInstance()->String_To_Dir((it.Direction));
		pObjectTransformCom->Set_Look(vLook.x, vLook.y, vLook.z);

		//생성
		_tchar szTag[64] = {};

		while (true) {
			_stprintf_s(szTag, 64, L"Tile_%d", s_Index);
			_tchar* pTag = new _tchar[lstrlen(szTag) + 1];
			lstrcpy(pTag, szTag);

			if (SUCCEEDED(pLayer->Add_GameObject(pTag, pGameObject))) {
				Release_tchar.push_back(pTag);
				break; // 성공 시 탈출
			}
			else {
				Safe_Delete(pTag); // 실패 시 메모리 해제 후 시도 계속
			}
		}

		CMapToolMgr::GetInstance()->Plant_Tile(it.Tile_Type, it.vPos, it.Direction);
		s_Index++;
	}

	//게임 오브젝트 레이어
	pLayer = pScene->Get_Layer(L"GameObject_Layer");
	map<const _tchar*, CGameObject*>* pObjectMap = pLayer->Get_ObjectMap();
	map<const _tchar*, CGameObject*>::iterator iter;
	CGameObject* pObj;

	iter = std::find_if(pObjectMap->begin(), pObjectMap->end(), CTag_Finder(L"1Player"));
	pObj = iter->second;
	_vec3 pos1 = CMapToolMgr::GetInstance()->Get_Data(CMapToolMgr::GetInstance()->Get_Name()).Player.P1;
	if (pos1.x != 0.f && pos1.y != 0.f && pos1.z != 0.f) {
		dynamic_cast<CPlayerPoint*>(pObj)->Set_Plant(TRUE);
		dynamic_cast<CPlayerPoint*>(pObj)->Set_Pos(pos1);
	}
	else {
		dynamic_cast<CPlayerPoint*>(pObj)->Set_Plant(FALSE);
	}

	iter = std::find_if(pObjectMap->begin(), pObjectMap->end(), CTag_Finder(L"2Player"));
	pObj = iter->second;
	_vec3 pos2 = CMapToolMgr::GetInstance()->Get_Data(CMapToolMgr::GetInstance()->Get_Name()).Player.P2;
	if (pos2.x != 0.f && pos2.y != 0.f && pos2.z != 0.f) {
		dynamic_cast<CPlayerPoint*>(pObj)->Set_Plant(TRUE);
		dynamic_cast<CPlayerPoint*>(pObj)->Set_Pos(pos2);
	}
	else {
		dynamic_cast<CPlayerPoint*>(pObj)->Set_Plant(FALSE);
	}
}

void CDynamicCamera::LoadCallBackToImguiMgr()
{
	Engine::CImguiMgr::GetInstance()->SetLoadCallback([this]() {
		this->Load_Objects();  // Load 시 실행될 함수
		});
}

void CDynamicCamera::ClearCallBackToImguiMgr()
{
	Engine::CImguiMgr::GetInstance()->SetClearCallback([this]() {
		this->ALL_RESET();  // Clear 시 실행될 함수
		});
}

void CDynamicCamera::TerrianEnableCallBackToImguiMgr()
{
	Engine::CImguiMgr::GetInstance()->SetTerrianEnableCallback([this]() {
		this->TerrianEnable();  // Clear 시 실행될 함수
		});
}

void CDynamicCamera::TerrianEnable()
{
	CScene* pScene = CManagement::GetInstance()->Get_Scene();
	CLayer* pLayer = pScene->Get_Layer(L"GameObject_Layer");
	map<const _tchar*, CGameObject*>* pObjectMap = pLayer->Get_ObjectMap();
	map<const _tchar*, CGameObject*>::iterator iter;
	CGameObject* pObj;

	iter = std::find_if(pObjectMap->begin(), pObjectMap->end(), CTag_Finder(L"Terrain"));
	pObj = iter->second;
	dynamic_cast<CTerrain*>(pObj)->Set_Enable();
}

void CDynamicCamera::Prev_Type()
{
	switch (CMapToolMgr::GetInstance()->Get_NowObject())
	{
	case Engine::O_BLOCK:
		CMapToolMgr::GetInstance()->PrevStation();
		break;
	case Engine::O_RCTILE:
		CMapToolMgr::GetInstance()->PrevRcTile();
		break;
	case Engine::O_HEXTILE:
		break;
	case Engine::O_ENV:
		break;
	case Engine::O_SPAWN:
		CMapToolMgr::GetInstance()->ChangePlayer();
		break;
	case Engine::O_END:
		break;
	default:
		break;
	}
}

void CDynamicCamera::Next_Type()
{
	switch (CMapToolMgr::GetInstance()->Get_NowObject())
	{
	case Engine::O_BLOCK:
		CMapToolMgr::GetInstance()->NextStation();
		break;
	case Engine::O_RCTILE:
		CMapToolMgr::GetInstance()->NextRcTile();
		break;
	case Engine::O_HEXTILE:
		break;
	case Engine::O_ENV:
		break;
	case Engine::O_SPAWN:
		CMapToolMgr::GetInstance()->ChangePlayer();
		break;
	case Engine::O_END:
		break;
	default:
		break;
	}

}

void CDynamicCamera::Create_Objects()
{
	switch (CMapToolMgr::GetInstance()->Get_NowObject())
	{
	case O_BLOCK:
		Create_Block();
		break;
	case O_RCTILE:
		Create_RcTile();
		break;
	case O_HEXTILE:
		Create_HexTile();
		break;
	case O_ENV:
		break;
	case O_SPAWN:
		Create_Player();
	default:
		break;
	}
}

void CDynamicCamera::Delete_Objects()
{
	switch (CMapToolMgr::GetInstance()->Get_NowObject())
	{
	case O_BLOCK:
		Delete_Block();
		break;
	case O_RCTILE:
		Delete_RcTile();
		break;
	case O_HEXTILE:
		break;
	case O_ENV:
		break;
	case O_SPAWN:
		Delete_Player();
	default:
		break;
	}
}

HRESULT CDynamicCamera::Create_Block()
{
	static int s_BlockIndex = 0;

	CScene* pScene = CManagement::GetInstance()->Get_Scene();
	CLayer* pLayer = pScene->Get_Layer(L"Block_Layer");

	if (nullptr == pLayer)
		return E_FAIL;

	Engine::CGameObject* pGameObject = CBlock::Create(m_pGraphicDev);

	if (nullptr == pGameObject)
		return E_FAIL;

	CTransform* pObjectTransformCom = dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
	_vec3 vTmp;
	dynamic_cast<CTransform*>(CManagement::GetInstance()->Get_Component(ID_DYNAMIC, L"GameObject_Layer", L"ShowBox", L"Com_Transform"))->Get_Info(INFO_POS, &vTmp);
	pObjectTransformCom->Set_Pos(vTmp.x, vTmp.y, vTmp.z);

	_vec3 vLook = CMapToolMgr::GetInstance()->Get_DirLook();
	pObjectTransformCom->Set_Look(vLook.x, vLook.y, vLook.z);
	
	dynamic_cast<CBlock*>(pGameObject)->Set_TextureNum((CMapToolMgr::GetInstance()->Get_NowStation()));

	//만약에 음식 상자 타입이면 추가로 생성하라
	if ((CMapToolMgr::GetInstance()->Get_NowStation() == Engine::STATIONID::S_CREATE)) {
		dynamic_cast<CBlock*>(pGameObject)->Set_Create(CImguiMgr::GetInstance()->Get_CurFood());
	}

	//만약에 아이템이 비어있지 않다면 추가로 생성하라
	if (CImguiMgr::GetInstance()->Get_CurItem() != Engine::ITEMID::I_NONE) {
		dynamic_cast<CBlock*>(pGameObject)->Set_Tools(CImguiMgr::GetInstance()->Get_CurItem());
	}

	_tchar szTag[64] = {};

	while (true) {
		_stprintf_s(szTag, 64, L"Block_%d", s_BlockIndex);
		_tchar* pTag = new _tchar[lstrlen(szTag) + 1];
		lstrcpy(pTag, szTag);

		if (SUCCEEDED(pLayer->Add_GameObject(pTag, pGameObject))) {
			Release_tchar.push_back(pTag);
			break; // 성공 시 탈출
		}
		else {
			Safe_Delete(pTag); // 실패 시 메모리 해제 후 시도 계속
			++s_BlockIndex;
		}
	}

	CMapToolMgr::GetInstance()->Plant_Block(vTmp);
	s_BlockIndex++;
	return S_OK;
}

void CDynamicCamera::Delete_Block()
{
	CScene* pScene = CManagement::GetInstance()->Get_Scene();
	CLayer* pLayer = pScene->Get_Layer(L"Block_Layer");

	auto objectmap = pLayer->Get_ObjectMap();
	_vec3 vBlockPos, vColPos;
	vColPos = CCollisionMgr::GetInstance()->Get_ColPos();


	float ftmp = 0.f;
	if (CMapToolMgr::GetInstance()->Get_NowStation() == 0) {
		ftmp = 0.25f;
	}

	vColPos.x = (vColPos.x >= 0) ? floorf(vColPos.x) + 0.5f : ceil(vColPos.x) - 0.5f;
	vColPos.y = (vColPos.y >= 0) ? floorf(vColPos.y) + (0.25f + ftmp) : ceil(vColPos.y) - (0.25f + ftmp);
	vColPos.z = (vColPos.z >= 0) ? floorf(vColPos.z) + 0.5f : ceil(vColPos.z) - 0.5f;

	//모든 오브젝트를 순회
	for (auto it = objectmap->begin(); it != objectmap->end();) {
		dynamic_cast<CTransform*>(pLayer->Get_Component(ID_DYNAMIC, (it->first), L"Com_Transform"))->Get_Info(INFO_POS, &vBlockPos);
		
		if (vBlockPos == vColPos) {
			it->second->Release();
			it = objectmap->erase(it);
			CMapToolMgr::GetInstance()->Break_Block(vColPos);
		}
		else {
			it++;
		}
	}
}

HRESULT CDynamicCamera::Create_RcTile()
{
	static int s_RcTileIndex = 0;

	CScene* pScene = CManagement::GetInstance()->Get_Scene();
	CLayer* pLayer = pScene->Get_Layer(L"Tile_Layer");

	if (nullptr == pLayer)
		return E_FAIL;

	Engine::CGameObject* pGameObject = CRcTile::Create(m_pGraphicDev);

	if (nullptr == pGameObject)
		return E_FAIL;

	CTransform* pObjectTransformCom = dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
	_vec3 vTmp;
	dynamic_cast<CTransform*>(CManagement::GetInstance()->Get_Component(ID_DYNAMIC, L"GameObject_Layer", L"ShowRcTile", L"Com_Transform"))->Get_Info(INFO_POS, &vTmp);
	pObjectTransformCom->Set_Pos(vTmp.x, vTmp.y, vTmp.z);

	_vec3 vLook = CMapToolMgr::GetInstance()->Get_DirLook();
	pObjectTransformCom->Set_Look(vLook.x, vLook.y, vLook.z);

	dynamic_cast<CRcTile*>(pGameObject)->Set_TextureNum((CMapToolMgr::GetInstance()->Get_NowRcTile()));

	_tchar szTag[64] = {};

	while (true) {
		_stprintf_s(szTag, 64, L"RcTile_%d", s_RcTileIndex);
		_tchar* pTag = new _tchar[lstrlen(szTag) + 1];
		lstrcpy(pTag, szTag);

		if (SUCCEEDED(pLayer->Add_GameObject(pTag, pGameObject))) {
			Release_tchar.push_back(pTag);
			break; // 성공 시 탈출
		}
		else {
			Safe_Delete(pTag); // 실패 시 메모리 해제 후 시도 계속
			++s_RcTileIndex;
		}
	}

	
	CMapToolMgr::GetInstance()->Plant_Tile(vTmp);
	++s_RcTileIndex;
	return S_OK;
}

void CDynamicCamera::Delete_RcTile()
{
	CScene* pScene = CManagement::GetInstance()->Get_Scene();
	CLayer* pLayer = pScene->Get_Layer(L"Tile_Layer");

	auto objectmap = pLayer->Get_ObjectMap();
	_vec3 vBlockPos, vColPos;
	vColPos = CCollisionMgr::GetInstance()->Get_ColPos();

	vColPos.x = floorf(vColPos.x) + 0.5f;
	vColPos.y = floorf(vColPos.y);
	vColPos.z = floorf(vColPos.z) + 0.5f;

	//모든 오브젝트를 순회
	for (auto it = objectmap->begin(); it != objectmap->end();) {
		dynamic_cast<CTransform*>(pLayer->Get_Component(ID_DYNAMIC, (it->first), L"Com_Transform"))->Get_Info(INFO_POS, &vBlockPos);

		if (vBlockPos == vColPos) {
			it->second->Release();
			it = objectmap->erase(it);
			CMapToolMgr::GetInstance()->Break_Tile(vColPos);
		}
		else {
			it++;
		}
	}
}

HRESULT CDynamicCamera::Create_HexTile()
{
	static int s_HexTileIndex = 0;
	int iRow, iCol;
	iRow = iCol = 30;

	CScene* pScene = CManagement::GetInstance()->Get_Scene();
	CLayer* pLayer = pScene->Get_Layer(L"Tile_Layer");

	if (nullptr == pLayer)
		return E_FAIL;

	float fHexRadius = 0.5f;
	float fHexHeight = sqrtf(3.f) * fHexRadius;  // 높이 = √3 * r

	for (int i = 0; i < iCol; ++i) { //z
		for (int j = 0; j < iRow; ++j) { //x

			Engine::CGameObject* pGameObject = CHexTile::Create(m_pGraphicDev);

			if (nullptr == pGameObject)
				return E_FAIL;

			CTransform* pObjectTransformCom = dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
			
			_vec3 vTmp = { 0.f ,0.f, 0.f };
			
			vTmp.x = j * 1.5f * fHexRadius;
			vTmp.z = i * fHexHeight;
			if (j % 2 == 1)
				vTmp.z += fHexHeight * 0.5f;

			pObjectTransformCom->Set_Pos(vTmp.x, 0.f, vTmp.z);
			CMapToolMgr::GetInstance()->Plant_HexTile(vTmp);

			_vec3 vLook = CMapToolMgr::GetInstance()->Get_DirLook();
			//pObjectTransformCom->Set_Look(vLook.x, vLook.y, vLook.z);

			_tchar szTag[64] = {};
			while (true) {
				_stprintf_s(szTag, 64, L"HexTile_%d", s_HexTileIndex);
				_tchar* pTag = new _tchar[lstrlen(szTag) + 1];
				lstrcpy(pTag, szTag);

				if (SUCCEEDED(pLayer->Add_GameObject(pTag, pGameObject))) {
					Release_tchar.push_back(pTag);
					break; // 성공 시 탈출
				}
				else {
					Safe_Delete(pTag); // 실패 시 메모리 해제 후 시도 계속
					++s_HexTileIndex;
				}
			}
		}
	}
	return S_OK;
}

void CDynamicCamera::Delete_HexTile()
{
}

HRESULT CDynamicCamera::Create_Player()
{
	CScene* pScene = CManagement::GetInstance()->Get_Scene();
	CLayer* pLayer = pScene->Get_Layer(L"GameObject_Layer");

	if (nullptr == pLayer)
		return E_FAIL;

    map<const _tchar*, CGameObject*>* pObjectMap = pLayer->Get_ObjectMap();
	if (nullptr == pObjectMap)
		return E_FAIL;

	//showPoint의 위치
	_vec3 vTmp;
	dynamic_cast<CTransform*>(CManagement::GetInstance()->Get_Component(ID_DYNAMIC, L"GameObject_Layer", L"ShowPlayerPoint", L"Com_Transform"))->Get_Info(INFO_POS, &vTmp);

	map<const _tchar*, CGameObject*>::iterator iter;

	if (CMapToolMgr::GetInstance()->Get_NowPlayer() == 0) {
		iter = std::find_if(pObjectMap->begin(), pObjectMap->end(), CTag_Finder(L"1Player"));
		CMapToolMgr::GetInstance()->Plant_Player(CMapToolMgr::GetInstance()->Get_NowPlayer(), vTmp);
	}
	if (CMapToolMgr::GetInstance()->Get_NowPlayer() == 1) {
		iter = std::find_if(pObjectMap->begin(), pObjectMap->end(), CTag_Finder(L"2Player"));
		CMapToolMgr::GetInstance()->Plant_Player(CMapToolMgr::GetInstance()->Get_NowPlayer(), vTmp);
	}

	if (iter != pObjectMap->end())
	{
		CGameObject* pObj = iter->second;

		dynamic_cast<CPlayerPoint*>(pObj)->Set_Plant(TRUE);
		dynamic_cast<CPlayerPoint*>(pObj)->Set_Pos(vTmp);
		
	}

	return S_OK;
}

void CDynamicCamera::Delete_Player()
{
}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CDynamicCamera* pCamera = new CDynamicCamera(pGraphicDev);

	if (FAILED(pCamera->Ready_GameObject(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pCamera);
		MSG_BOX("DynamicCamera Create Failed");
		return nullptr;
	}

	return pCamera;
}

void CDynamicCamera::Free()
{
	for (auto& a : Release_tchar) {
		Safe_Delete(a);
	}

	Engine::CCamera::Free();
}
