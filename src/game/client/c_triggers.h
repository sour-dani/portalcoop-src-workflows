//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#ifndef C_TRIGGERS_H
#define C_TRIGGERS_H
#ifdef _WIN32
#pragma once
#endif

#include "c_basetoggle.h"
#include "triggers_shared.h"

class C_BaseVPhysicsTrigger : public C_BaseTrigger
{
	DECLARE_CLASS( C_BaseVPhysicsTrigger , C_BaseTrigger );
	DECLARE_CLIENTCLASS();

public:
	
	//virtual bool PassesTriggerFilters(C_BaseEntity *pOther);

protected:
	string_t					m_iFilterName;
	//CHandle<class C_BaseFilter>	m_hFilter; //CBaseFilter is not networked yet. Only really care about m_bDisabled for this first pass.
};

#endif // C_TRIGGERS_H
