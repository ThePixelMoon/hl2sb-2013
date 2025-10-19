//========== Copyleft � 2011, Team Sandbox, Some rights reserved. ===========//
//
// Purpose:
//
//===========================================================================//

#include "cbase.h"
#include "filesystem.h"
#include "KeyValues.h"
#include "mountsteamcontent.h"
#include "steam/steam_api.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#ifdef CLIENT_DLL
#define UTIL_VarArgs VarArgs // Andrew; yep.
#endif

typedef struct
{
	const char *m_pPathName;
	int m_nAppId;
} gamePaths_t;
gamePaths_t g_GamePaths[10] =
{
	{ "hl2",		220 },
	{ "cstrike",	240 },
	{ "hl1",		280 },
	{ "dod",		300 },
	{ "lostcoast",	340 },
	{ "hl1mp",		360 },
	{ "episodic",	380 },
	{ "portal",		400 },
	{ "ep2",		420 },
	{ "tf",			440 }
};
const int g_GamePathsSize = sizeof(g_GamePaths) / sizeof(g_GamePaths[0]);

bool AddSearchPathByAppId(int nExtraAppId)
{
    if (!steamapicontext || !steamapicontext->SteamApps())
        return false;

    char szInstallDir[MAX_FILEPATH];
    int len = steamapicontext->SteamApps()->GetAppInstallDir(nExtraAppId, szInstallDir, sizeof(szInstallDir));
    if (len <= 0)
        return false; // App not installed!!!

    for (int i = 0; i < g_GamePathsSize; i++)
    {
        if (g_GamePaths[i].m_nAppId != nExtraAppId)
            continue;

        char fullPath[MAX_FILEPATH];
        Q_snprintf(fullPath, sizeof(fullPath), "%s/%s", szInstallDir, g_GamePaths[i].m_pPathName);

        Msg("Mounting path: %s\n", fullPath);
        g_pFullFileSystem->AddSearchPath(fullPath, "GAME");

        // The special case: HL1MP
        if (nExtraAppId == 360)
        {
            char hl1Path[MAX_FILEPATH];
            Q_snprintf(hl1Path, sizeof(hl1Path), "%s/hl1", szInstallDir);
            g_pFullFileSystem->AddSearchPath(hl1Path, "GAME");
        }

        // The special case..again: HL2 Anniversary Update
        if (nExtraAppId == 220)
        {
            const char* episodes[] = {"ep2", "episodic", "lostcoast"};
            for (int j = 0; j < ARRAYSIZE(episodes); j++)
            {
                char epPath[MAX_FILEPATH];
                Q_snprintf(epPath, sizeof(epPath), "%s/%s", szInstallDir, episodes[j]);
                g_pFullFileSystem->AddSearchPath(epPath, "GAME");
            }
        }
    }

    return true;
}

//Andrew; this allows us to mount content the user wants on top of the existing
//game content which is automatically loaded by the engine, and then by the
//game code
void MountUserContent()
{
	KeyValues *pMainFile, *pFileSystemInfo;
#ifdef CLIENT_DLL
	const char *gamePath = engine->GetGameDirectory();
#else
	char gamePath[MAX_PATH];
	engine->GetGameDir( gamePath, MAX_PATH );
	Q_StripTrailingSlash( gamePath );
#endif

	pMainFile = new KeyValues("gamecontent.txt");
	if (pMainFile->LoadFromFile(filesystem, UTIL_VarArgs("%s/gamecontent.txt", gamePath), "MOD"))
	{
		pFileSystemInfo = pMainFile->FindKey("FileSystem");
		if (pFileSystemInfo)
		{
			for (KeyValues* pKey = pFileSystemInfo->GetFirstSubKey(); pKey; pKey = pKey->GetNextKey())
			{
				if (strcmp(pKey->GetName(), "appid") == 0)
				{
					int nExtraContentId = pKey->GetInt();
					if (nExtraContentId)
					{
						DevMsg("Mounting AppID %i\n", nExtraContentId);
						AddSearchPathByAppId(nExtraContentId);
					}
				}
			}
		}
	}
	pMainFile->deleteThis();
}
