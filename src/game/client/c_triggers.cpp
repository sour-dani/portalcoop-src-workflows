//========= Copyright © 1996-2008, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "c_triggers.h"
#include "in_buttons.h"
#include "c_prop_vehicle.h"
#include "c_ai_basenpc.h"
//#include "c_func_brush.h"
#include "collisionutils.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

IMPLEMENT_CLIENTCLASS_DT( C_BaseVPhysicsTrigger, DT_BaseVPhysicsTrigger, CBaseVPhysicsTrigger )
	//RecvPropBool	( RECVINFO( m_bDisabled ) )
END_RECV_TABLE()


//-----------------------------------------------------------------------------
// Purpose: Make this trigger touchable for the client
//-----------------------------------------------------------------------------
void C_BaseTrigger::UpdatePartitionListEntry()
{
#ifdef CLIENT_DLL
	AssertMsg( IsPredicted(), "We shouldn't be networked, how is this being called?" );
#endif

	::partition->RemoveAndInsert(
		PARTITION_CLIENT_STATIC_PROPS,  // remove
		PARTITION_CLIENT_TRIGGER_ENTITIES | PARTITION_CLIENT_SOLID_EDICTS | PARTITION_CLIENT_RESPONSIVE_EDICTS | PARTITION_CLIENT_NON_STATIC_EDICTS,  // add
		CollisionProp()->GetPartitionHandle() );

	BaseClass::UpdatePartitionListEntry();

}