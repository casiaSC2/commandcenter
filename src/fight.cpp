#include "Common.h"
#include "CCBot.h"
#include "rapidjson/document.h"
#include "JSONTools.h"
#include "Util.h"
#include "fight.h"
#ifdef SC2API

#include "sc2utils/sc2_manage_process.h"
#include "sc2api/sc2_api.h"


int fight(char* sc2_path, std::string botA_path, std::string botA_race, std::string botB_path, std::string botB_race,
	std::string map_string, std::string replay_path) 
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
		exit(-1);
    }
    

    // Add the custom bot, it will control the players.
    CCBot botA(botA_path);
	CCBot botB(botB_path);


    // WARNING: Bot logic has not been thorougly tested on step sizes > 1
    //          Setting this = N means the bot's onFrame gets called once every N frames
    //          The bot may crash or do unexpected things if its logic is not called every frame
    coordinator.SetStepSize(1);
    coordinator.SetRealtime(false);
	coordinator.SetReplayPath(replay_path);

    coordinator.SetParticipants({
        sc2::CreateParticipant(Util::GetRaceFromString(botA_race), &botA),
        sc2::CreateParticipant(Util::GetRaceFromString(botB_race), &botB)
    });

    // Start the game.
    coordinator.LaunchStarcraft();
    coordinator.StartGame(map_string);
	// 1: botA win, 0: draw, -1: botB win
	int match_result = 0;
    // Step forward the game simulation.
    while (true) 
    {
        coordinator.Update();
		if(coordinator.AllGamesEnded())
		{
			uint32_t botA_id = botA.Observation()->GetPlayerID();
			auto result = botA.Observation()->GetResults();
			for(sc2::PlayerResult player_result : result)
			{
				if(player_result.player_id == botA_id)
				{
					if(player_result.result == sc2::GameResult::Win)
					{
						match_result = 1;
					}else if(player_result.result == sc2::GameResult::Loss)
					{
						match_result = -1;
					}else
					{
						match_result = 0;
					}
				}
			}
			break;
		}
		
    }

    return match_result;
}

#else

#endif