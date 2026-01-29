#include "cbase.h"
#include "point_ping_linker.h"
#ifdef GAME_DLL
#include "portal_player.h"
#endif
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

IMPLEMENT_NETWORKCLASS_ALIASED( PointPingLinker, DT_PortalGameRulesProxy )

BEGIN_NETWORK_TABLE( CPointPingLinker, DT_PointPingLinker )
END_NETWORK_TABLE()

LINK_ENTITY_TO_CLASS_ALIASED( point_ping_linker, PointPingLinker )

#ifdef GAME_DLL

BEGIN_DATADESC(CPointPingLinker)

	DEFINE_KEYFIELD( m_iszEntityName[0], FIELD_STRING, "LinkedEntity1"),
	DEFINE_KEYFIELD( m_iszEntityName[1], FIELD_STRING, "LinkedEntity2" ),
	DEFINE_KEYFIELD( m_iszEntityName[2], FIELD_STRING, "LinkedEntity3" ),
	DEFINE_KEYFIELD( m_iszEntityName[3], FIELD_STRING, "LinkedEntity4" ),
	DEFINE_KEYFIELD( m_iszEntityName[4], FIELD_STRING, "LinkedEntity5" ),
	DEFINE_KEYFIELD( m_iszEntityName[5], FIELD_STRING, "LinkedEntity6" ),
	DEFINE_KEYFIELD( m_iszEntityName[6], FIELD_STRING, "LinkedEntity7" ),
	DEFINE_KEYFIELD( m_iszEntityName[7], FIELD_STRING, "LinkedEntity8" ),
	DEFINE_KEYFIELD( m_iszEntityName[8], FIELD_STRING, "LinkedEntity9" ),
	DEFINE_KEYFIELD( m_iszEntityName[9], FIELD_STRING, "LinkedEntity10" ),

END_DATADESC()
#endif

CPointPingLinker::CPointPingLinker()
{

}

#ifdef GAME_DLL
void CPointPingLinker::PingLinkedEntity( const char *pszName, Vector vColor )
{
	CBaseEntity *pEntity = NULL;
	while ( ( pEntity = gEntList.FindEntityByName( pEntity, pszName ) ) != NULL )
	{
		CBaseAnimating *pAnimating = pEntity ? pEntity->GetBaseAnimating() : NULL;
		if ( !pAnimating )
			continue;
		
		if (pAnimating->m_bGlowEnabled)
		{
			pAnimating->RemoveGlowEffect();
		}

		pAnimating->SetGlowEffectColor(vColor.x, vColor.y, vColor.z);
		pAnimating->AddGlowTime(gpGlobals->curtime);
		pAnimating->RemoveGlowTime(PINGTIME);
	}
}


void CPointPingLinker::PingLinkedEntities( Vector vColor, CBaseEntity *pOwner )
{
	for ( int i = 0; i < PING_ENTITY_NAME_COUNT; ++i )
	{
		PingLinkedEntity( m_iszEntityName[i].ToCStr(), vColor);
	}
	
	extern void ShowAnnotation( Vector location, int follow_entindex, int entindex, int forcedpingicon = -1 );
	ShowAnnotation( GetAbsOrigin(), entindex(), pOwner->entindex(), m_iPingIcon );
}

bool CPointPingLinker::HasThisEntity( CBaseEntity *pAnimating )
{
	if (!pAnimating)
		return false;	

	for ( int i = 0; i < PING_ENTITY_NAME_COUNT; ++i )
	{
		if ( gEntList.FindEntityByName( NULL, m_iszEntityName[i].ToCStr() ) == pAnimating)
			return true;
	}
	
	return false;
}
#endif