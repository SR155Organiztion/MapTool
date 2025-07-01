#include "Engine_Typedef.h"
#include "Engine_Struct.h"
#include "json.hpp"

namespace Engine
{
	using json = nlohmann::json;

	struct S_BLOCK {
		std::string Block_Name;
		_vec3 vPos;
		std::string Direction;
	};
	/**
	* @struct S_Tile
	* @brief 타일 구조체
	*/
	struct S_TILE {
		std::string Tile_Name;
		_vec3 vPos;
		std::string Direction;
	};
	/**
	* @struct S_ENVIRONMENT
	* @brief 환경 구조물 구조체
	*/
	struct S_ENVIRONMENT {
		std::string Tile_Name;
		_vec3 vPos;
		std::string Direction;
	};
	/**
	* @struct S_CAM
	* @brief 카메라를 저장할 벡터 구조체
	*/
	struct S_CAM {
		_vec3 vEye;
		_vec3 vAt;
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
	* @struct S_STAGE
	* @brief 스테이지의 모든 정보를 가진 구조체
	*/
	struct S_STAGE {
		S_CAM Cam;
		S_PLAYER Player;
		std::vector<std::string> Recipe;
		std::vector<S_BLOCK> Block;
		std::vector<S_TILE> Tiles;
		std::vector<S_ENVIRONMENT> Environment;
	};

	//////////////////////////////////////////////////////////////

	inline void to_json(json& j, const _vec3& v) {
		j = json{ {"x", v.x}, {"y", v.y}, {"z", v.z} };
	}
	inline void from_json(const json& j, _vec3& v) {
		j.at("x").get_to(v.x);
		j.at("y").get_to(v.y);
		j.at("z").get_to(v.z);
	}


	inline void to_json(json& j, const S_BLOCK& b) {
		j = json{
			{"Block_Name", b.Block_Name},
			{"vPos", b.vPos},
			{"Direction", b.Direction}
		};
	}

	inline void from_json(const json& j, S_TILE& t) {
		j.at("Tile_Name").get_to(t.Tile_Name);
		j.at("vPos").get_to(t.vPos);
		j.at("Direction").get_to(t.Direction);
	}

}

