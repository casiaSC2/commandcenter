#include "match.h"
#include <stdio.h>
#include <string>
#include "fight.h"
JNIEXPORT jint JNICALL Java_ladder_service_MatchService_match
(JNIEnv *env, jobject object, jstring botA_path, jstring botB_path, jstring map_path)
{
	char *sc2_path_str = "D:\\game\\StarCraft II\\Versions\\Base58400\\SC2_x64.exe";
	const char *botA_path_str = env->GetStringUTFChars(botA_path, 0);
	const char *botB_path_str = env->GetStringUTFChars(botB_path, 0);
	const char *map_path_str = env->GetStringUTFChars(map_path, 0);
	//need to release this string when done with it in order to

	int res = fight(sc2_path_str, botA_path_str, botB_path_str, map_path_str);

	//avoid memory leak
	env->ReleaseStringUTFChars(botA_path, botA_path_str);
	env->ReleaseStringUTFChars(botB_path, botB_path_str);
	env->ReleaseStringUTFChars(map_path, map_path_str);
	return res;
}