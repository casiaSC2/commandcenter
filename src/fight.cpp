#include "Common.h"
#include "CCBot.h"
#include "rapidjson/document.h"
#include "JSONTools.h"
#include "Util.h"

#ifdef SC2API

#include "sc2utils/sc2_manage_process.h"
#include "sc2api/sc2_api.h"

std::string get_race(const std::string bot_json_path)
{
	rapidjson::Document doc;
	std::string config = JSONTools::ReadFile(bot_json_path);
	if (config.length() == 0)
	{
		std::cerr << "Config file could not be found, and is required for starting the bot\n";
		std::cerr << "Please read the instructions and try again\n";
		exit(-1);
	}

	bool parsingFailed = doc.Parse(config.c_str()).HasParseError();
	if (parsingFailed)
	{
		std::cerr << "Config file could not be parsed, and is required for starting the bot\n";
		std::cerr << "Please read the instructions and try again\n";
		exit(-1);
	}

	std::string bot_race;

	if (doc.HasMember("SC2API") && doc["SC2API"].IsObject())
	{
		const rapidjson::Value & info = doc["SC2API"];
		JSONTools::ReadString("BotRace", info, bot_race);
	}
	else
	{
		std::cerr << "Config file has no 'Game Info' object, required for starting the bot\n";
		std::cerr << "Please read the instructions and try again\n";
		exit(-1);
	}
	return bot_race;
}

int fight(char* sc2_path, std::string botA_path, std::string botB_path) 
{
	int argc = 3;
	char* argv[3];
	argv[0] = "CommandCenter";
	argv[1] = "-e";
	argv[2] = sc2_path;
    sc2::Coordinator coordinator;
    if (!coordinator.LoadSettings(argc, argv)) 
    {
        std::cout << "Unable to find or parse settings." << std::endl;
        return 1;
    }
    
    rapidjson::Document doc;
    std::string config = JSONTools::ReadFile("BotConfig.txt");
    if (config.length() == 0)
    {
        std::cerr << "Config file could not be found, and is required for starting the bot\n";
        std::cerr << "Please read the instructions and try again\n";
        exit(-1);
    }

    bool parsingFailed = doc.Parse(config.c_str()).HasParseError();
    if (parsingFailed)
    {
        std::cerr << "Config file could not be parsed, and is required for starting the bot\n";
        std::cerr << "Please read the instructions and try again\n";
        exit(-1);
    }

    
    std::string mapString;
    int stepSize = 1;

    if (doc.HasMember("SC2API") && doc["SC2API"].IsObject())
    {
        const rapidjson::Value & info = doc["SC2API"];
        JSONTools::ReadString("MapFile", info, mapString);
        JSONTools::ReadInt("StepSize", info, stepSize);
    }
    else
    {
        std::cerr << "Config file has no 'Game Info' object, required for starting the bot\n";
        std::cerr << "Please read the instructions and try again\n";
        exit(-1);
    }
	// read the bot race
	std::string botA_race = get_race(botA_path);
	std::string botB_race = get_race(botB_path);

    // Add the custom bot, it will control the players.
    CCBot botA(botA_path);
	CCBot botB(botB_path);

    
    // WARNING: Bot logic has not been thorougly tested on step sizes > 1
    //          Setting this = N means the bot's onFrame gets called once every N frames
    //          The bot may crash or do unexpected things if its logic is not called every frame
    coordinator.SetStepSize(stepSize);
    coordinator.SetRealtime(false);

    coordinator.SetParticipants({
        sc2::CreateParticipant(Util::GetRaceFromString(botA_race), &botA),
        sc2::CreateParticipant(Util::GetRaceFromString(botB_race), &botB)
    });

    // Start the game.
    coordinator.LaunchStarcraft();
    coordinator.StartGame(mapString);

    // Step forward the game simulation.
    while (true) 
    {
        coordinator.Update();
    }

    return 0;
}

#else

#endif