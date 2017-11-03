#include "match.h"
#include <stdio.h>
#include "fight.h"
JNIEXPORT jint JNICALL Java_ladder_service_MatchService_match
(JNIEnv *env, jobject object, jstring sc2_path, jstring botA_path, jstring botB_path, jstring map_path)
{
	const char *sc2_path_str = env->GetStringUTFChars(sc2_path, 0);
	const char *botA_path_str = env->GetStringUTFChars(botA_path, 0);
	const char *botB_path_str = env->GetStringUTFChars(botB_path, 0);
	const char *map_path_str = env->GetStringUTFChars(map_path, 0);
	//need to release this string when done with it in order to
	char *buf = (char*)malloc(strlen(sc2_path_str) + 1);
	strcpy(buf, sc2_path_str);
	int res = fight(buf, botA_path_str, botB_path_str, map_path_str);

	//avoid memory leak
	env->ReleaseStringUTFChars(sc2_path, sc2_path_str);
	env->ReleaseStringUTFChars(botA_path, botA_path_str);
	env->ReleaseStringUTFChars(botB_path, botB_path_str);
	env->ReleaseStringUTFChars(map_path, map_path_str);
	free(buf);
	return res;
}