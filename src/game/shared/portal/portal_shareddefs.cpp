//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//
#include "cbase.h"
#include "portal_shareddefs.h"
#include "filesystem.h"
#ifdef CLIENT_DLL
#include "replay/IEngineReplay.h"
#endif

ConVar pcoop_require_all_players( "pcoop_require_all_players", "1", FCVAR_REPLICATED | FCVAR_ARCHIVE | FCVAR_NOTIFY, "Effectively pauses the game when there are not enough players in the server" );
ConVar pcoop_require_all_players_force_amount( "pcoop_require_all_players_force_amount", "-2", FCVAR_NOTIFY | FCVAR_REPLICATED, "Force a certain amount of required players instead of using max players. -1 = use max players\n-2 = use the map's data file", true, -2, true, MAX_PLAYERS );

#ifdef GAME_DLL
ConVar sv_portal_game_update_on_map_load( "sv_portal_game_update_on_map_load", "1", FCVAR_REPLICATED, "Updates the server's portal game type\n" );
#endif
ConVar sv_portal_game( "sv_portal_game", "0", FCVAR_REPLICATED, "The server's portal game type, automatically changes on map load if sv_portal_game_update_on_map_load is enabled\n0 = Portal\n1 = Rexaura\n" );

char *g_ppszPortalPassThroughMaterials[] = 
{ 
	"lights/light_orange001", 
	NULL,
};

KeyValues *LoadRadioData()
{	
	KeyValues *radios = new KeyValues( "radios.txt" );
	if ( !radios->LoadFromFile( g_pFullFileSystem, RADIO_DATA_FILE, "MOD" ) )
	{
		AssertMsg( false, "Failed to load radio data" );
		return NULL;
	}

	return radios;
}

void CMapInfo::Reset( void )
{
	g_MapInfo.m_iRequiredPlayers = -1;
}

CMapInfo g_MapInfo;

int GetRequiredPlayers()
{
	int nRequiredPlayers = 0;
	if ( pcoop_require_all_players_force_amount.GetInt() == -1 )
	{
		nRequiredPlayers = gpGlobals->maxClients;
	}
	else if ( pcoop_require_all_players_force_amount.GetInt() == -2 )
	{
		if ( g_MapInfo.GetRequiredPlayers() != -1 )
		{
			nRequiredPlayers = g_MapInfo.GetRequiredPlayers();
		}
		else
		{
			//Warning( "Map didn't have required players set, using maxplayers\n" );
			// Just use maxclients if it fails to load
			nRequiredPlayers = gpGlobals->maxClients;
		}
	}
	else
	{
		nRequiredPlayers = pcoop_require_all_players_force_amount.GetInt();
	}

	//Msg("nRequiredPlayers: %i\n", nRequiredPlayers);
	return nRequiredPlayers;
}

bool PlayerShouldPlay( int index )
{
	return index <= GetRequiredPlayers();
}

KeyValues *LoadMapDataForMap( const char *pszMapName )
{
	char szMapFilePath[MAX_MAP_NAME];
	Q_snprintf( szMapFilePath, sizeof(szMapFilePath), "maps/mapdata/%s.txt", pszMapName );
	KeyValues *pMapData = new KeyValues("mapdata");
	if ( !pMapData->LoadFromFile( g_pFullFileSystem, szMapFilePath, "MOD" ) )
	{
		AssertMsg( false, "Map data not found" );
		pMapData->deleteThis();
		return NULL;
	}

	return pMapData;
}

class CMapDataLoader : public CAutoGameSystem
{
public:
	virtual void LevelInitPreEntity();
};

CMapDataLoader g_MapDataLoader;

void CMapDataLoader::LevelInitPreEntity()
{
#ifdef GAME_DLL
	const char *pszMapName = gpGlobals->mapname.ToCStr();
#else
	const char *pszMapName = g_pEngineClientReplay->GetLevelNameShort();
#endif

	KeyValues *pMapData = LoadMapDataForMap( pszMapName );
	if ( !pMapData )
	{
#ifdef DEBUG // Only assert on 2 player and 3 player maps
		if ( (V_stristr( pszMapName, "p2coop_" ) || V_stristr( pszMapName, "p3coop_" )) &&
			(V_stristr( pszMapName, "rex2c_" ) || V_stristr( pszMapName, "rex3c_" )) )
		{
			AssertMsg( false, "Failed to load map data" );
		}
#endif

		g_MapInfo.Reset();
		return;
	}

	Msg("Map loaded: %s\n", pszMapName);

	g_MapInfo.m_iRequiredPlayers = pMapData->GetInt( "required_players", -1 );

	pMapData->deleteThis();
}