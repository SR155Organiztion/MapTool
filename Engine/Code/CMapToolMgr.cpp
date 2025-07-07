#include "../Header/json.hpp"
#include "CMapToolMgr.h"
#include "CManagement.h"
#include <fstream>
#include "CImguiMgr.h"

using json = nlohmann::json;
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(_vec3, x, y, z)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_BLOCK, Block_Type, vPos, Direction)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_TILE, Tile_Type, vPos, Direction)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_ENVIRONMENT, Env_Type, vPos, Direction)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_CAM, vEye, vAt)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_PLAYER, P1, P2)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_STAGE, Cam, Player, Time, Recipe, Block, Tiles, Environment);

IMPLEMENT_SINGLETON(CMapToolMgr)

CMapToolMgr::CMapToolMgr()
    : m_fAngle(0.f), m_iSet_Player(0), m_sName("None"), m_iSelectName(0), m_iDir(DIRECTIONID::NX), m_iObject(CREATEOBJECT_ID::O_BLOCK), m_iRcTile(0), m_fTimer(0.f), m_bCreate(true)
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

void CMapToolMgr::Plant_Block(_vec3 _vPos)
{
    S_BLOCK tBlock = { Block_To_String() , _vPos, Dir_To_String() };
    m_tBlockVec.push_back(tBlock);
}

void CMapToolMgr::Plant_Block(string _sType, _vec3 _vPos, string _sDir)
{
    S_BLOCK tBlock = { _sType , _vPos, _sDir };
    m_tBlockVec.push_back(tBlock);
}

void CMapToolMgr::Break_Block(_vec3 _vPos)
{
    for (vector<S_BLOCK>::iterator it = m_tBlockVec.begin(); it != m_tBlockVec.end(); ) {
        if ((*it).vPos == _vPos) {
            it = m_tBlockVec.erase(it);
        }
        else {
            it++;
        }
    }
}

void CMapToolMgr::Plant_Tile(_vec3 _vPos)
{
    S_TILE tTile = { Tile_To_String(), _vPos, Dir_To_String() };
    m_tTileVec.push_back(tTile);
}

void CMapToolMgr::Plant_Tile(string _sType, _vec3 _vPos, string _sDir)
{
    S_TILE tTile = { _sType, _vPos, _sDir };
    m_tTileVec.push_back(tTile);
}

void CMapToolMgr::Break_Tile(_vec3 _vPos)
{
    for (vector<S_TILE>::iterator it = m_tTileVec.begin(); it != m_tTileVec.end(); ) {
        if ((*it).vPos == _vPos) {
            it = m_tTileVec.erase(it);
        }
        else {
            it++;
        }
    }
}

void CMapToolMgr::Plant_Environment(string _sType, _vec3 _vPos, _vec3 _vDir)
{
}

void CMapToolMgr::Plant_Camera(_vec3 _vEye, _vec3 _vAt)
{
}

void CMapToolMgr::Plant_Player(_int _iPlayer, _vec3 _vPos)
{
}

HRESULT CMapToolMgr::Save_Json()
{
    m_sName = CImguiMgr::GetInstance()->Get_Name();
    //데이터 종합

    S_STAGE stage = { m_tCam, m_tPlayer ,m_fTimer, m_sRecipeVec, m_tBlockVec, m_tTileVec, m_tEnvVec };
    //저장하기 전 json에 있는 모든 맵의 키값을 스테이지 이름 벡터와 비교찾는다
    bool bFound = false;
    for (auto it = m_mapJson.begin(); it != m_mapJson.end(); ++it) {
        //만약 겹치는 키 값이 있다면 그 키의 second를 비우고 현재 스테이지 데이터를 넣음
        if ((*it).first == m_sName) {
            (*it).second = stage;
            bFound = true;
            break;
        }
    }
    //없다면 추가함
    if (!bFound && m_bCreate) {
        m_mapJson.insert(pair<string, S_STAGE>(m_sName, stage));
    }
    json j = m_mapJson;
    /////////////////////////////////////
    ///데이터 저장시점
    {
        std::ofstream file("../Bin/Data/SaveData.json");
        if (!file.is_open()) {
            MSG_BOX("저장 오류");
            return E_FAIL;
        }
        file << j.dump(2);
        file.close();
    }

    //한번 불러와서 확인
    Load_Json();
    m_bCreate = true;
    return S_OK;
}

HRESULT CMapToolMgr::Load_Json()
{
    Reset();
    m_mapJson.clear();
    m_sNameVec.clear();
    std::ifstream file("../Bin/Data/SaveData.json");

    if (!file.is_open()) {
        MSG_BOX("파일 열기 실패");
        return E_FAIL;
    }

    {
        json j;
        file >> j;

        bool firstKey = false;
        // 순회하며 각 요소를 map<string, S_STAGE>로 변환
        for (auto it = j.begin(); it != j.end(); ++it)
        {
            const std::string& stageName = it.key();
            const json& stageJson = it.value();

            if (m_sNameVec.empty())
                m_sName = stageName;

            m_sNameVec.push_back(stageName);
            S_STAGE stage = stageJson.get<S_STAGE>();
            m_mapJson[stageName] = stage;
        }

        Select_Map();

        file.close();
    }

    return S_OK;
}

void CMapToolMgr::Select_Map()
{
    m_tCam = m_mapJson[m_sName].Cam;
    m_tPlayer = m_mapJson[m_sName].Player;
    m_fTimer = m_mapJson[m_sName].Time;
    m_sRecipeVec = m_mapJson[m_sName].Recipe;
    m_tBlockVec = m_mapJson[m_sName].Block;
    m_tTileVec = m_mapJson[m_sName].Tiles;
    m_tEnvVec = m_mapJson[m_sName].Environment;
}

void CMapToolMgr::Delete_Map(string _s)
{
    auto it = m_mapJson.find(_s);

    if (it != m_mapJson.end()) {
        Reset();
        m_mapJson.erase(it);

        if (m_sName == _s && !m_mapJson.empty()) {
            m_sName = m_mapJson.begin()->first;
        }

        Save_Json();
    }
}

void CMapToolMgr::Reset()
{
    ZeroMemory(&m_tCam, sizeof(S_CAM));
    ZeroMemory(&m_tPlayer, sizeof(S_PLAYER));
    m_fTimer = 0.f;
    m_sRecipeVec.clear();
    m_tBlockVec.clear();
    m_tTileVec.clear();
    m_tEnvVec.clear();
}

void CMapToolMgr::Set_NoCreate()
{
    m_bCreate = false;
}

S_STAGE CMapToolMgr::Get_Data(string s)
{
    auto it = m_mapJson.find(s);
    if (it != m_mapJson.end())
        return it->second;

    MSG_BOX("없는 스테이지");
    return S_STAGE{};
}

string CMapToolMgr::Get_Name()
{
    return m_sName;
}

void CMapToolMgr::Set_Name(string _s)
{
    for (auto a : m_sNameVec) {
        if (a == _s) {
            m_sName = _s;
            return;
        }
    }

    MSG_BOX("SetName Error");
}

vector<string>* CMapToolMgr::Get_NameVec()
{
    return &m_sNameVec;
}

void CMapToolMgr::NextRotate()
{
    ++m_iDir;
    if (m_iDir == static_cast<_uint>(DIRECTIONID::DIR_END)) {
        m_iDir = 0;
    }
}

void CMapToolMgr::PrevRotate()
{
    --m_iDir;
    if (m_iDir < 0) {
        m_iDir = static_cast<_uint>(DIRECTIONID::DIR_END) - 1;
    }
}

_vec3 CMapToolMgr::Get_DirLook()
{
    switch (m_iDir)
    {
    case Engine::PX:
        return _vec3(0.f, 0.f, 0.f);
    case Engine::NX:
        return _vec3(0.f, D3DXToRadian(90.f), 0.f);
    case Engine::PZ:
        return _vec3(0.f, D3DXToRadian(180.f), 0.f);
    case Engine::NZ:
        return _vec3(0.f, D3DXToRadian(270.f), 0.f);
    default:
        return _vec3(0.f, 0.f, 0.f);
    }
}

void CMapToolMgr::NextStation()
{
    ++m_iStation;
    if (m_iStation >= static_cast<_uint>(STATIONID::S_END)) {
        m_iStation = 0;
    }
}

void CMapToolMgr::PrevStation()
{
    --m_iStation;
    if (m_iStation < 0) {
        m_iStation = static_cast<_uint>(STATIONID::S_END) - 1;
    }
}

_uint CMapToolMgr::Get_NowStation()
{
    return m_iStation;
}

void CMapToolMgr::NextObject()
{
    ++m_iObject;
    if (m_iObject >= static_cast<_uint>(CREATEOBJECT_ID::O_END)) {
        m_iObject = 0;
    }
}

_uint CMapToolMgr::Get_NowObject()
{
    return m_iObject;
}

void CMapToolMgr::NextRcTile()
{
    ++m_iRcTile;
    if (m_iRcTile >= static_cast<_uint>(STATIONID::S_END)) {
        m_iRcTile = 0;
    }
}

void CMapToolMgr::PrevRcTile()
{
    --m_iRcTile;
    if (m_iRcTile < 0) {
        m_iRcTile = static_cast<_uint>(STATIONID::S_END) - 1;
    }
}

_uint CMapToolMgr::Get_NowRcTile()
{
    return m_iRcTile;
}

string CMapToolMgr::Get_Dir()
{
    switch (m_iDir)
    {
    case Engine::DIRECTIONID::PX:
        return "PX";
    case Engine::DIRECTIONID::NX:
        return "NX";
    case Engine::DIRECTIONID::PZ:
        return "PZ";
    case Engine::DIRECTIONID::NZ:
        return "NZ";
    default:
        return "???";
    }
}

void CMapToolMgr::Dummy_Data()
{
    MSG_BOX("더미호출");
    m_sName = "Stage1";

    _vec3 v = { 1.f, 2.f, 3.f };

    S_BLOCK b = { "A", v, "L" };
    m_tBlockVec.push_back(b);
    m_tBlockVec.push_back(b);

    S_TILE t = { "T", v, "R" };
    m_tTileVec.push_back(t);

    S_ENVIRONMENT e = { "T", v, v };
    m_tEnvVec.push_back(e);

    m_sRecipeVec.push_back("apple");
    m_tCam = { v, v };
    m_tPlayer = { v , v };
}

string CMapToolMgr::Dir_To_String()
{
    switch (m_iDir)
    {
    case Engine::DIRECTIONID::PX:
        return "PX";
    case Engine::DIRECTIONID::NX:
        return "NX";
    case Engine::DIRECTIONID::PZ:
        return "PZ";
    case Engine::DIRECTIONID::NZ:
        return "NZ";
    default:
        return "???";
    }
}

string CMapToolMgr::Block_To_String()
{
    switch (m_iStation)
    {
    case Engine::STATIONID::S_INV:
        return "InvWall";
        break;
    case Engine::STATIONID::S_EMPTY:
        return "Empty";
        break;
    case Engine::STATIONID::S_CREATE:
        return "Create_";
        break;
    case Engine::STATIONID::S_CHOP:
        return "Chop";
        break;
    case Engine::STATIONID::S_GAS:
        return "Gas";
        break;
    case Engine::STATIONID::S_PLATE:
        return "Plate";
        break;
    case Engine::STATIONID::S_SINK_W:
        return "Sink_Wash";
        break;
    case Engine::STATIONID::S_SINK_P:
        return "Sink_Plate";
        break;
    case Engine::STATIONID::S_TRASH:
        return "Trash";
        break;
    case Engine::STATIONID::S_SERVING:
        return "Serving";
        break;
    case Engine::STATIONID::S_END:
        return "???";
        break;
    default:
        break;
    }

    return "???";
}

string CMapToolMgr::Tile_To_String()
{
    switch (m_iRcTile)
    {
    case Engine::RT_1:
        return "TILE_1";
    case Engine::RT_END:
        break;
    default:
        return "???";
        break;
    }
    return "???";
}

_vec3 CMapToolMgr::String_To_Dir(string& _s)
{
    if (_s == "PX") return _vec3(0.f, 0.f, 0.f);
    if (_s == "NX") return _vec3(0.f, D3DXToRadian(90.f), 0.f);
    if (_s == "PZ") return _vec3(0.f, D3DXToRadian(180.f), 0.f);
    if (_s == "NZ") return _vec3(0.f, D3DXToRadian(270.f), 0.f);

    // 잘못된 문자열일 경우
    return _vec3(0.f, 0.f, 0.f);
}

_uint CMapToolMgr::String_To_Block(string& _s)
{
    if (_s == "InvWall")
        return Engine::STATIONID::S_INV;
    else if (_s == "Empty")
        return Engine::STATIONID::S_EMPTY;
    else if (_s == "Create_")
        return Engine::STATIONID::S_CREATE;
    else if (_s == "Chop")
        return Engine::STATIONID::S_CHOP;
    else if (_s == "Gas")
        return Engine::STATIONID::S_GAS;
    else if (_s == "Plate")
        return Engine::STATIONID::S_PLATE;
    else if (_s == "Sink_Wash")
        return Engine::STATIONID::S_SINK_W;
    else if (_s == "Sink_Plate")
        return Engine::STATIONID::S_SINK_P;
    else if (_s == "Trash")
        return Engine::STATIONID::S_TRASH;
    else if (_s == "Serving")
        return Engine::STATIONID::S_SERVING;
    else if (_s == "???")
        return Engine::STATIONID::S_END;

    //에러
    return -1;
}

_uint CMapToolMgr::String_To_Tile(string& _s)
{
    if (_s == "TILE_1")
        return Engine::RCTILEID::RT_1;
    else if (_s == "???")
        return Engine::RCTILEID::RT_END;

    //에러처리
    return -1;
}

void CMapToolMgr::Free()
{
    m_tBlockVec.clear();
    m_tTileVec.clear();
    m_tEnvVec.clear();
    m_sRecipeVec.clear();
    m_mapJson.clear();
}