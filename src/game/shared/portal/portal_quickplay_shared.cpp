//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Quickplay related code shared between GC and client
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "portal_quickplay_shared.h"
#include "portal_shareddefs.h"

//-----------------------------------------------------------------------------

EGameCategory GetCategoryForMap( const char *pMapName )
{
	if ( V_stristr( pMapName, "p2coop_" ) )
	{
		return kGameCategory_2Player;
	}
	else if ( V_stristr( pMapName, "p3coop_" ) )
	{
		return kGameCategory_3Player;
	}
	else if ( V_stristr( pMapName, "rex2c_" ) )
	{
		return kGameCategory_2Player_Rexaura;
	}

	// Default search is 2 player
	return kGameCategory_2Player;
}