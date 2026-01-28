//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Quickplay related code shared between GC and client
//
// $NoKeywords: $
//=============================================================================//

#ifndef _INCLUDED_PORTAL_QUICKPLAY_SHARED_H
#define _INCLUDED_PORTAL_QUICKPLAY_SHARED_H
#ifdef _WIN32
#pragma once
#endif

class CUtlStringList;

const int kTFMaxQuickPlayServersToScore = 25;
const int kTFQuickPlayIdealMaxNumberOfPlayers = 24;
const int kTFQuickPlayMinMaxNumberOfPlayers = 2; // don't auto match to servers with max players set too low
const int kTFQuickPlayMaxPlayers = 33;

extern float QuickplayCalculateServerScore( int numHumans, int numBots, int maxPlayers, int nNumInSearchParty );

extern const char k_szQuickplayFAQ_URL[];

enum EGameCategory
{
	kGameCategory_2Player	= 0,
	kGameCategory_2Player_Rexaura,
	kGameCategory_3Player,
	
	kGameCategory_Quickplay,

	// Note: Don't reorder this list.  Only add to the end

	eNumGameCategories,
};

EGameCategory GetCategoryForMap( const char *pMapName );

struct QuickplaySearchOptions
{
	EGameCategory m_eSelectedGameType;

	enum EMaps
	{
		EMapsOfficial,
		EMapsCustom,
		EMapsDontCare
	};
	EMaps m_eMaps;

	CUtlString m_strMapName;
};

#endif // #ifndef _INCLUDED_PORTAL_QUICKPLAY_SHARED_H
