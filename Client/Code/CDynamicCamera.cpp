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

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCamera(pGraphicDev), m_bFix(false), m_bCheck(false),
	m_bClickedLB(false), m_bClickedRB(false), m_bPressedQ(false), m_bPressedE(false), m_bPressedR(false), m_bPressedL(false)
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
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_LALT) & 0x80)
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
			CMapToolMgr::GetInstance()->PrevStation();
			m_bPressed1 = true;
		}
	}
	else {
		m_bPressed1 = false;
	}
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_3) & 0x80)
	{
		if (!m_bPressed3) {
			CMapToolMgr::GetInstance()->NextStation();
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
	CLayer* pLayer = pScene->Get_Layer(L"Environment_Layer");
	for (auto it : *(pLayer->Get_ObjectMap())) {//; it != pLayer->Get_ObjectMap()->end(); ) 
		Safe_Release(it.second);
	}
	(pLayer->Get_ObjectMap())->clear();

	pLayer = pScene->Get_Layer(L"Tile_Layer");
	for (auto it : *(pLayer->Get_ObjectMap())) {//; it != pLayer->Get_ObjectMap()->end(); ) 
		Safe_Release(it.second);
	}
	(pLayer->Get_ObjectMap())->clear();

	pLayer = pScene->Get_Layer(L"Block_Layer");
	for (auto it : *(pLayer->Get_ObjectMap())) {//; it != pLayer->Get_ObjectMap()->end(); ) 
		Safe_Release(it.second);
	}
	(pLayer->Get_ObjectMap())->clear();

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
	for (auto it : (CMapToolMgr::GetInstance()->Get_Data(CMapToolMgr::GetInstance()->Get_Name()).Block)) {
		
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

		CMapToolMgr::GetInstance()->Plant_Block(it.vPos);
		s_Index++;
	}

	//타일레이어
	pLayer = pScene->Get_Layer(L"Tile_Layer");
	for (auto it : (CMapToolMgr::GetInstance()->Get_Data(CMapToolMgr::GetInstance()->Get_Name()).Tiles)) {

		if (nullptr == pLayer)
			return;

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

		CMapToolMgr::GetInstance()->Plant_Block(it.vPos);
		s_Index++;
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
		break;
	case O_ENV:
		break;
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

	vColPos.x = floorf(vColPos.x) + 0.5f;
	vColPos.y = floorf(vColPos.y) + 0.5f;
	vColPos.z = floorf(vColPos.z) + 0.5f;

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
	return E_NOTIMPL;
}

void CDynamicCamera::Delete_HexTile()
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
