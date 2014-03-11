// auto generator by trans tools don't alter
#ifndef PlayerTudiPosConfig_h__
#define PlayerTudiPosConfig_h__
#include "configdef.hpp"
typedef struct PlayerTudiPosConfigConfig
{
	 uint8 TudiPos; 	//徒弟位置  
	 uint16 OpenLevel; 	//开放等级  
}PlayerTudiPosConfigConfig;

class PlayerTudiPosConfigConfigMgr
{
	typedef map<int, PlayerTudiPosConfigConfig> MAP_PlayerTudiPosConfig;
public:
	int LoadData(const char* filename)
	{
		ifstream newfile ( filename, ios::binary);
		while (!newfile.eof())
		{
			PlayerTudiPosConfigConfig config; 
			memset(&config, 0, sizeof(config)); 
			newfile.read((char*)&config.TudiPos, sizeof(config.TudiPos));
			newfile.read((char*)&config.OpenLevel, sizeof(config.OpenLevel));
			map_PlayerTudiPosConfig.insert(MAP_PlayerTudiPosConfig::value_type(config.TudiPos, config));
		}
		newfile.close();
		map_PlayerTudiPosConfig.erase(0);
		return 0;
	}
	PlayerTudiPosConfigConfig* GetConfig(uint32 key)
	{
		MAP_PlayerTudiPosConfig::iterator it= map_PlayerTudiPosConfig.find(key);
		if (it != map_PlayerTudiPosConfig.end())
		{
			return &it->second;
		}
		return NULL;
	}

	PlayerTudiPosConfigConfig* GetFirst()
	{
		it = map_PlayerTudiPosConfig.begin();
		if (it != map_PlayerTudiPosConfig.end())
		{
			return &it->second;
		}
		return NULL;
	}

	PlayerTudiPosConfigConfig* GetNext()
	{
		it++;
		if (it != map_PlayerTudiPosConfig.end())
		{
			return &it->second;
		}
		return NULL;
	}
private:
	MAP_PlayerTudiPosConfig::iterator it;
	MAP_PlayerTudiPosConfig map_PlayerTudiPosConfig;
};
#endif // PlayerTudiPosConfig_h__
