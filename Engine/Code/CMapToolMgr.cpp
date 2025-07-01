#include "CMapToolMgr.h"
#include <fstream>

//#include "CJsonConverter.h"

IMPLEMENT_SINGLETON(CMapToolMgr)

CMapToolMgr::CMapToolMgr()
    : m_fAngle(0.f), m_iSet_Player(0), m_sName("Stage13123321"), m_iSelectName(0)
{
    m_tBlockVec.clear();
    m_tTileVec.clear();
    m_tEnvVec.clear();
    m_sRecipeVec.clear();
    m_mapJson.clear();
}

CMapToolMgr::~CMapToolMgr()
{
}

void CMapToolMgr::Plant_Block()
{
}

void CMapToolMgr::Plant_Tile()
{
}

void CMapToolMgr::Plant_Environment()
{
}

void CMapToolMgr::Plant_Camera()
{
}

void CMapToolMgr::Plant_Player()
{
}

HRESULT CMapToolMgr::Save_Json()
{
    Dummy_Data();

    //////////////////////////////////////
    //데이터 종합
    S_STAGE stage = { m_tCam, m_tPlayer, m_sRecipeVec, m_tBlockVec, m_tTileVec, m_tEnvVec };
    m_mapJson.insert(pair<string, S_STAGE>(m_sName, stage));
    json j = { m_mapJson };// 여기에 파라미터받기

    /////////////////////////////////////
    ///데이터 저장시점

    std::ofstream file("../Bin/Data/SaveData.json");

    if (!file.is_open()) {
        MSG_BOX("저장 오류");
        return E_FAIL;
    }

    file << j.dump(2);
    file.close();

    return S_OK;
}

HRESULT CMapToolMgr::Load_Json()
{
    Reset();
    m_mapJson.clear();

    std::ifstream file("../Bin/Data/SaveData.json");
    if (!file.is_open()) {
        MSG_BOX("파일 열기 실패");
        return E_FAIL;
    }

    json j;
    file >> j;

    bool firstKey = false;
    // 순회하며 각 요소를 map<string, S_STAGE>로 변환
    for (const auto& Stage : j)
    {
        if (!Stage.is_object()) continue;

        for (auto it = Stage.begin(); it != Stage.end(); ++it)
        {
            
            const std::string& stageName = it.key();      
            const json& stageJson = it.value();
            
            if (!firstKey)
            {
                m_sName = stageName;
                firstKey = true;
            }
            // 변환 시도
            m_sNameVec.push_back(stageName);
            S_STAGE stage = stageJson.get<S_STAGE>();
            m_mapJson[stageName] = stage;
        }
    }
    Select_Map();

    file.close();
    return S_OK;
}
        
void CMapToolMgr::Select_Map()
{
    m_sRecipeVec = m_mapJson[m_sName].Recipe;
    m_tPlayer = m_mapJson[m_sName].Player;
    m_tCam = m_mapJson[m_sName].Cam;
    m_tBlockVec = m_mapJson[m_sName].Block;
    m_tTileVec = m_mapJson[m_sName].Tiles;
    m_tEnvVec = m_mapJson[m_sName].Environment;
}

void CMapToolMgr::Reset()
{
    m_tBlockVec.clear();
    m_tTileVec.clear();
    m_tEnvVec.clear();
    m_sRecipeVec.clear();
}

S_STAGE CMapToolMgr::Get_Data(string s)
{
    auto it = m_mapJson.find(s);
    if (it != m_mapJson.end())
        return it->second;

    MSG_BOX("없는 스테이지");
    return S_STAGE{};
}

void CMapToolMgr::Key_Input()
{
}

void CMapToolMgr::Dummy_Data()
{
    m_sName = "Stage1";

    _vec3 v = { 1.f, 2.f, 3.f };
    
    S_BLOCK b = { "A", v, "L" };
    m_tBlockVec.push_back(b);
    m_tBlockVec.push_back(b);

    S_TILE t = { "T", v, "R"};
    m_tTileVec.push_back(t);

    S_ENVIRONMENT e = { "T", v, v };
    m_tEnvVec.push_back(e);

    m_sRecipeVec.push_back("apple");
    m_tCam = { v, v };
    m_tPlayer = { v , v };
}

void CMapToolMgr::Free()
{
    m_tBlockVec.clear();
    m_tTileVec.clear();
    m_tEnvVec.clear();
    m_sRecipeVec.clear();
    m_mapJson.clear();
}