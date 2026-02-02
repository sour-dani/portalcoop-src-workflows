//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Filters are outboard entities that hold a set of rules that other
//			entities can use to determine behaviors.
//			
//			For example, triggers can use an activator filter to determine who
//			activates them. NPCs and breakables can use a damage filter to
//			determine what can damage them.
//
//			Current filter criteria are:
//
//				Activator name
//				Activator class
//				Activator team
//				Damage type (for damage filters only)
//
//			More than one filter can be combined to create a more complex boolean
//			expression by using filter_multi.
//
//=============================================================================//

#ifndef FILTERS_SHARED_H
#define FILTERS_SHARED_H
#ifdef _WIN32
#pragma once
#endif

#ifdef CLIENT_DLL
#include "c_baseentity.h"
#define CLogicalEntity C_BaseEntity
#define CBaseFilter C_BaseFilter
#define CFilterMultiple C_FilterMultiple
#define CFilterName C_FilterName
#define CFilterClass C_FilterClass
#define CFilterPlayerIndex C_FilterPlayerIndex
#define CFilterTeam C_FilterTeam
#define CFilterMassGreater C_FilterMassGreater
#else
#include "baseentity.h"
#include "entityoutput.h"
#endif


// ###################################################################
//	> BaseFilter
// ###################################################################
class CBaseFilter : public CLogicalEntity
{
	DECLARE_CLASS( CBaseFilter, CLogicalEntity );

public:
	DECLARE_NETWORKCLASS();
#ifndef CLIENT_DLL
	DECLARE_DATADESC();
#endif
	bool PassesFilter( CBaseEntity *pCaller, CBaseEntity *pEntity );
#ifndef CLIENT_DLL
	bool PassesDamageFilter( const CTakeDamageInfo &info );
#endif
	CNetworkVar( bool, m_bNegated );
#ifndef CLIENT_DLL
	virtual int UpdateTransmitState( void ) OVERRIDE
	{
		return SetTransmitState( FL_EDICT_ALWAYS );
	}

	// Inputs
	void InputTestActivator( inputdata_t &inputdata );
	// Outputs
	COutputEvent	m_OnPass;		// Fired when filter is passed
	COutputEvent	m_OnFail;		// Fired when filter is failed
#endif
protected:

	virtual bool PassesFilterImpl( CBaseEntity *pCaller, CBaseEntity *pEntity );
#ifndef CLIENT_DLL
	virtual bool PassesDamageFilterImpl(const CTakeDamageInfo &info);
#endif
};


// ###################################################################
//	> FilterMultiple
//
//   Allows one to filter through mutiple filters
// ###################################################################
#define MAX_FILTERS 5
enum filter_t
{
	FILTER_AND,
	FILTER_OR,
};

class CFilterMultiple : public CBaseFilter
{
	DECLARE_CLASS( CFilterMultiple, CBaseFilter );
	DECLARE_NETWORKCLASS();
#ifndef CLIENT_DLL
	DECLARE_DATADESC();
#endif

	CNetworkVar( filter_t, m_nFilterType );
	CNetworkArray( EHANDLE, m_hFilter, MAX_FILTERS );

	bool PassesFilterImpl( CBaseEntity *pCaller, CBaseEntity *pEntity );
#ifndef CLIENT_DLL
	bool PassesDamageFilterImpl(const CTakeDamageInfo &info);
	void Activate(void);
	
	string_t	m_iFilterName[MAX_FILTERS];
#endif
};


// ###################################################################
//	> FilterName
// ###################################################################
class CFilterName : public CBaseFilter
{
	DECLARE_CLASS( CFilterName, CBaseFilter );
#ifdef GAME_DLL
	DECLARE_DATADESC();
#endif
	DECLARE_NETWORKCLASS();

public:
#ifdef CLIENT_DLL
	char m_iFilterName[255];
#else
	CNetworkVar( string_t, m_iFilterName );
#endif
	bool PassesFilterImpl( CBaseEntity *pCaller, CBaseEntity *pEntity )
	{
		// special check for !player as GetEntityName for player won't return "!player" as a name
#ifdef CLIENT_DLL // Entity names don't get networked
		if (FStrEq( m_iFilterName, "!player"))
#else
		if (FStrEq(STRING(m_iFilterName.Get()), "!player"))
#endif
		{
			return pEntity->IsPlayer();
		}
		else
		{
#ifdef CLIENT_DLL // Entity names don't get networked
			return pEntity->NameMatches( m_iFilterName );
#else
			return pEntity->NameMatches( STRING(m_iFilterName.Get()) );
#endif
		}
	}
};

// ###################################################################
//	> FilterClass
// ###################################################################
class CFilterClass : public CBaseFilter
{
	DECLARE_CLASS( CFilterClass, CBaseFilter );
#ifdef GAME_DLL
	DECLARE_DATADESC();
#endif
	DECLARE_NETWORKCLASS();

public:
#ifdef CLIENT_DLL
	char m_iFilterClass[255];
#else
	CNetworkVar( string_t, m_iFilterClass );
#endif

	bool PassesFilterImpl( CBaseEntity *pCaller, CBaseEntity *pEntity )
	{
#ifdef CLIENT_DLL
		return pEntity->ClassMatches( m_iFilterClass );
#else
		return pEntity->ClassMatches( STRING(m_iFilterClass.Get()) );
#endif
	}
};

// ###################################################################
//	> FilterClass
// ###################################################################
class CFilterPlayerIndex: public CBaseFilter
{
	DECLARE_CLASS( CFilterPlayerIndex, CBaseFilter );
#ifdef GAME_DLL
	DECLARE_DATADESC();
#endif
	DECLARE_NETWORKCLASS();

public:
	CNetworkVar( int, m_iPlayerIndex );

	bool PassesFilterImpl( CBaseEntity *pCaller, CBaseEntity *pEntity )
	{
		if ( !pEntity->IsPlayer() )
			return false;

		if ( pEntity->entindex() == m_iPlayerIndex )
			return true;

		return false;
	}
};

// ###################################################################
//	> FilterTeam
// ###################################################################
class CFilterTeam : public CBaseFilter
{
	DECLARE_CLASS( CFilterTeam, CBaseFilter );
#ifdef GAME_DLL
	DECLARE_DATADESC();
#endif
	DECLARE_NETWORKCLASS();

public:
	CNetworkVar( int, m_iFilterTeam );

	bool PassesFilterImpl( CBaseEntity *pCaller, CBaseEntity *pEntity )
	{
	 	return ( pEntity->GetTeamNumber() == m_iFilterTeam );
	}
};


// ###################################################################
//	> FilterMassGreater
// ###################################################################
class CFilterMassGreater : public CBaseFilter
{
	DECLARE_CLASS( CFilterMassGreater, CBaseFilter );
#ifdef GAME_DLL
	DECLARE_DATADESC();
#endif
	DECLARE_NETWORKCLASS();

public:
	CNetworkVar( float, m_fFilterMass );

	bool PassesFilterImpl( CBaseEntity *pCaller, CBaseEntity *pEntity )
	{
		if ( pEntity->VPhysicsGetObject() == NULL )
			return false;

		return ( pEntity->VPhysicsGetObject()->GetMass() > m_fFilterMass );
	}
};

#endif // FILTERS_SHARED_H
