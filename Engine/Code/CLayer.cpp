#include "CLayer.h"

CLayer::CLayer()
{
}

CLayer::~CLayer()
{
}

CComponent* CLayer::Get_Component(COMPONENTID eID, const _tchar* pObjTag, const _tchar* pComponentTag)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_mapObject.end())
		return nullptr;

	return iter->second->Get_Component(eID, pComponentTag);
}

HRESULT CLayer::Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	auto result = m_mapObject.insert({ pObjTag, pGameObject });


	if (!result.second) // 이미 존재함
	{
		Safe_Release(pGameObject); // 누수 방지
		//MSG_BOX("[CLayer] Add_GameObject 실패 - 태그 중복\n");
		return E_FAIL;
	}
	
	return S_OK;
}

map<const _tchar*, CGameObject*>* CLayer::Get_ObjectMap()
{
	return &m_mapObject;
}

HRESULT CLayer::Ready_Layer()
{
	return S_OK;
}

_int CLayer::Update_Layer(const _float& fTimeDelta)
{
	_int	iResult(0);

	for (auto& pObj : m_mapObject)
	{
		iResult = pObj.second->Update_GameObject(fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}

	return iResult;
}

void CLayer::LateUpdate_Layer(const _float& fTimeDelta)
{
	for (auto& pObj : m_mapObject)
		pObj.second->LateUpdate_GameObject(fTimeDelta);
}



CLayer* CLayer::Create()
{
	CLayer* pLayer = new CLayer;

	if (FAILED(pLayer->Ready_Layer()))
	{
		MSG_BOX("Layer Create Failed");
		Safe_Release(pLayer);
		return nullptr;
	}

	return pLayer;
}

void CLayer::Free()
{
	for_each(m_mapObject.begin(), m_mapObject.end(), CDeleteMap());
	m_mapObject.clear();
}
