//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#ifndef TRIGGERS_SHARED_H
#define TRIGGERS_SHARED_H
#ifdef _WIN32
#pragma once
#endif

#include "filters_shared.h"

#ifdef GAME_DLL
#include "basetoggle.h"
#else
#include "c_basetoggle.h"
#define CBaseTrigger C_BaseTrigger
#endif

//
// Spawnflags
//
enum
{
	SF_TRIGGER_ALLOW_CLIENTS				= 0x01,		// Players can fire this trigger
	SF_TRIGGER_ALLOW_NPCS					= 0x02,		// NPCS can fire this trigger
	SF_TRIGGER_ALLOW_PUSHABLES				= 0x04,		// Pushables can fire this trigger
	SF_TRIGGER_ALLOW_PHYSICS				= 0x08,		// Physics objects can fire this trigger
	SF_TRIGGER_ONLY_PLAYER_ALLY_NPCS		= 0x10,		// *if* NPCs can fire this trigger, this flag means only player allies do so
	SF_TRIGGER_ONLY_CLIENTS_IN_VEHICLES		= 0x20,		// *if* Players can fire this trigger, this flag means only players inside vehicles can 
	SF_TRIGGER_ALLOW_ALL					= 0x40,		// Everything can fire this trigger EXCEPT DEBRIS!
	SF_TRIGGER_ONLY_CLIENTS_OUT_OF_VEHICLES	= 0x200,	// *if* Players can fire this trigger, this flag means only players outside vehicles can 
	SF_TRIG_PUSH_ONCE						= 0x80,		// trigger_push removes itself after firing once
	SF_TRIG_PUSH_AFFECT_PLAYER_ON_LADDER	= 0x100,	// if pushed object is player on a ladder, then this disengages them from the ladder (HL2only)
	SF_TRIG_TOUCH_DEBRIS 					= 0x400,	// Will touch physics debris objects
	SF_TRIGGER_ONLY_NPCS_IN_VEHICLES		= 0X800,	// *if* NPCs can fire this trigger, only NPCs in vehicles do so (respects player ally flag too)
	SF_TRIGGER_DISALLOW_BOTS                = 0x1000,   // Bots are not allowed to fire this trigger
};

// Spawnflags for CTriggerPlayerMovement
const int SF_TRIGGER_MOVE_AUTODISABLE				= 0x0080;	// Disable auto movement
const int SF_TRIGGER_AUTO_DUCK						= 0x0800;	// Duck automatically
const int SF_TRIGGER_AUTO_WALK						= 0x1000;	// Walk automatically


// DVS TODO: get rid of CBaseToggle
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CBaseTrigger : public CBaseToggle
{
	DECLARE_CLASS( CBaseTrigger, CBaseToggle );
	DECLARE_NETWORKCLASS();
public:
	
	// By default, triggers aren't predicted, not really an accurate name
	virtual bool IsPredicted( void ) { return false; }
#ifdef GAME_DLL
	CBaseTrigger();

	virtual int UpdateTransmitState( void ) OVERRIDE
	{
		if ( IsPredicted() )
		{
			return SetTransmitState( FL_EDICT_PVSCHECK );
		}
		
		return SetTransmitState( FL_EDICT_DONTSEND );
	}

	void Activate( void );
	virtual void PostClientActive( void );
	void InitTrigger( void );

	void Enable( void );
	void Disable( void );
	void Spawn( void );
	void UpdateOnRemove( void );
	void TouchTest(  void );

	void SetDisabled( bool bDisabled );

	// Input handlers
	virtual void InputEnable( inputdata_t &inputdata );
	virtual void InputDisable( inputdata_t &inputdata );
	virtual void InputDisableAndEndTouch( inputdata_t &inputdata );
	virtual void InputToggle( inputdata_t &inputdata );
	virtual void InputTouchTest ( inputdata_t &inputdata );

	virtual void InputStartTouch( inputdata_t &inputdata );
	virtual void InputEndTouch( inputdata_t &inputdata );

	virtual bool UsesFilter( void ){ return ( m_hFilter.Get() != NULL ); }
	virtual void StartTouch(CBaseEntity *pOther);
	virtual void EndTouch(CBaseEntity *pOther);
	virtual void StartTouchAll() {}
	virtual void EndTouchAll() {}
	virtual bool IsTouching( const CBaseEntity *pOther ) const;
	bool AllPlayersAreTouching( void );
	bool MaxPlayersAreTouching( void );

	CBaseEntity *GetTouchedEntityOfType( const char *sClassName );

	int	 DrawDebugTextOverlays(void);

	// by default, triggers don't deal with TraceAttack
	void TraceAttack(CBaseEntity *pAttacker, float flDamage, const Vector &vecDir, trace_t *ptr, int bitsDamageType) {}

	bool PointIsWithin( const Vector &vecPoint );
	
	string_t	m_iFilterName;
#else
	CBaseTrigger();
	void UpdatePartitionListEntry( void );
	virtual void PreDataUpdate( DataUpdateType_t updatetype ) OVERRIDE;
	virtual void OnDataChanged( DataUpdateType_t updatetype ) OVERRIDE;
	bool m_bOldDisabled;
#endif
	CNetworkVar( bool, m_bDisabled );
	CNetworkHandle( CBaseFilter, m_hFilter );
	
	// Functions that are shared between the client & server
	virtual bool PassesTriggerFilters(CBaseEntity *pOther);

protected:
#ifdef GAME_DLL
	// Outputs
	COutputEvent m_OnStartTouch;
	COutputEvent m_OnStartTouchAll;
	COutputEvent m_OnEndTouch;
	COutputEvent m_OnEndTouchAll;
	COutputEvent m_OnTouching;
	COutputEvent m_OnNotTouching;
	COutputEvent m_OnAllPlayersTouching;
	COutputEvent m_OnMaxPlayersTouching;

	// Entities currently being touched by this trigger
	CUtlVector< EHANDLE >	m_hTouchingEntities;
	CUtlVector< CHandle<CBasePlayer> >	m_hTouchingPlayers;

	DECLARE_DATADESC();
#endif
};

#endif // TRIGGERS_SHARED_H
