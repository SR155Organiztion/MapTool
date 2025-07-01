//#include "CJsonConverter.h"
//
//BEGIN(Engine)
//
//inline void CJsonConverter::to_json(json& j, const _vec3& vec3)
//{
//    j = json{ {"x", vec3.x}, {"y", vec3.y}, {"z", vec3.z} };
//}
//
//inline void CJsonConverter::from_json(const json& j, _vec3& vec3)
//{
//    j.at("x").get_to(vec3.x);
//    j.at("y").get_to(vec3.y);
//    j.at("z").get_to(vec3.z);
//}
//
//void CJsonConverter::to_json(json& j, const S_BLOCK& block) {
//    json posJson;
//    to_json(posJson, block.vPos); // 직접 호출
//
//    j = json{
//        {"BlockType", block.Block_Type},
//        {"Position", posJson},
//        {"Direction", block.Direction}
//    };
//}
//
//void CJsonConverter::from_json(const json& j, S_BLOCK& block) {
//    j.at("BlockType").get_to(block.Block_Type);
//    j.at("Position").get_to(block.vPos);     // _vec3용 from_json 자동 호출
//    j.at("Direction").get_to(block.Direction);
//}
//
//void CJsonConverter::to_json(json& j, const S_TILE& tile)
//{
//    j = json{
//        {"TileType", tile.Tile_Type},
//        {"Position", {tile.vPos.x, tile.vPos.y, tile.vPos.z}},
//        {"Direction", tile.Direction}
//    };
//}
//
//void CJsonConverter::from_json(const json& j, S_TILE& tile)
//{
//    j.at("TileType").get_to(tile.Tile_Type);
//    auto pos = j.at("Position");
//    tile.vPos.x = pos[0];
//    tile.vPos.y = pos[1];
//    tile.vPos.z = pos[2];
//    j.at("Direction").get_to(tile.Direction);
//}
//
//void CJsonConverter::to_json(json& j, const S_ENVIRONMENT& env)
//{
//    j = json{
//    {"EnvType", env.Env_Type},
//    {"Position", {env.vPos.x, env.vPos.y, env.vPos.z}},
//    {"Direction", env.Direction}
//    };
//}
//
//void CJsonConverter::from_json(const json& j, S_ENVIRONMENT& env)
//{
//    j.at("EnvType").get_to(env.Env_Type);
//    auto pos = j.at("Position");
//    env.vPos.x = pos[0];
//    env.vPos.y = pos[1];
//    env.vPos.z = pos[2];
//    j.at("Direction").get_to(env.Direction);
//}
//
//void CJsonConverter::to_json(json& j, const S_CAM& cam)
//{
//    j = json{
//        {"Eye", {cam.vEye.x, cam.vEye.y, cam.vEye.z}},
//        {"At", {cam.vAt.x, cam.vAt.y, cam.vAt.z}}
//    };
//}
//
//void CJsonConverter::from_json(const json& j, S_CAM& cam)
//{
//    auto eye = j.at("Eye");
//    cam.vEye.x = eye[0];
//    cam.vEye.y = eye[1];
//    cam.vEye.z = eye[2];
//
//    auto at = j.at("At");
//    cam.vAt.x = at[0];
//    cam.vAt.y = at[1];
//    cam.vAt.z = at[2];
//}
//
//void CJsonConverter::to_json(json& j, const S_PLAYER& player)
//{
//    j = json{
//    {"P1", {player.P1.x, player.P1.y, player.P1.z}},
//    {"P1", {player.P2.x, player.P2.y, player.P2.z}}
//    };
//}
//
//void CJsonConverter::from_json(const json& j, S_PLAYER& player)
//{
//    auto P1 = j.at("P1");
//    player.P1.x = P1[0];
//    player.P1.y = P1[1];
//    player.P1.z = P1[2];
//
//    auto P2 = j.at("P2");
//    player.P2.x = P2[0];
//    player.P2.y = P2[1];
//    player.P2.z = P2[2];
//}
//
//void CJsonConverter::to_json(json& j, const S_STAGE& stage)
//{
//
//}
//
//void CJsonConverter::from_json(const json& j, S_STAGE& stage)
//{
//}
//
//END
