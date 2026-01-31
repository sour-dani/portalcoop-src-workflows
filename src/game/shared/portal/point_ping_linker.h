#ifndef POINT_PING_LINKER_H
#define POINT_PING_LINKER_H
#ifdef WIN32
#pragma once
#endif

#include "cbase.h"

#ifdef CLIENT_DLL
#define CPointPingLinker C_PointPingLinker
#endif

#define PING_ENTITY_NAME_COUNT 10

#ifdef GAME_DLL
class CPointPingLinker : public CBaseEntity
#else
class CPointPingLinker : public C_BaseEntity
#endif
{
#ifdef GAME_DLL
	DECLARE_CLASS( CPointPingLinker, CBaseEntity );
#else
	DECLARE_CLASS( CPointPingLinker, C_BaseEntity );
#endif

public:
	DECLARE_NETWORKCLASS();

	CPointPingLinker();

#ifdef GAME_DLL
	DECLARE_DATADESC();
	

	void PingLinkedEntity( const char *pszName, Vector vColor );
	void PingLinkedEntities( Vector vColor, CBaseEntity *pOwner );

	bool HasThisEntity( CBaseEntity *pAnimating );

private:

	string_t m_iszEntityName[PING_ENTITY_NAME_COUNT];
#endif
};

#endif //POINT_PING_LINKER_H