#include "cbase.h"

static const char *s_pszUnlockThinkContext = "UnlockThinkContext";
static const char *s_pszTimerEndThinkContext = "TimerEndThinkContext";

class CFuncSwitch : public CBaseEntity
{
public:
	DECLARE_CLASS( CFuncSwitch, CBaseEntity );
	DECLARE_DATADESC();
	
	CFuncSwitch();

	virtual void Spawn( void ) OVERRIDE;

	virtual void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) OVERRIDE;	
	virtual int	ObjectCaps( void ) OVERRIDE
	{
		return BaseClass::ObjectCaps() | FCAP_IMPULSE_USE | FCAP_USE_IN_RADIUS;
	};

	void UnlockThink( void );
	void TimerEndThink( void );

private:
	float m_flLockTime;
	float m_flTimeLength;

	bool m_bPressable;
	bool m_bSet1;

	COutputEvent m_OnPressed;
	COutputEvent m_OnSet1;
	COutputEvent m_OnSet2;
	COutputEvent m_OnTimerStarted;
	COutputEvent m_OnTimerEnded;
	COutputEvent m_OnBecomePressable;
};

BEGIN_DATADESC( CFuncSwitch )

	DEFINE_KEYFIELD( m_flLockTime, FIELD_FLOAT, "locktime" ),
	DEFINE_KEYFIELD( m_flTimeLength, FIELD_FLOAT, "timelength" ),

	DEFINE_OUTPUT( m_OnPressed, "OnPressed" ),
	DEFINE_OUTPUT( m_OnSet1, "OnSet1" ),
	DEFINE_OUTPUT( m_OnSet2, "OnSet2" ),
	DEFINE_OUTPUT( m_OnTimerStarted, "OnTimerStarted" ),
	DEFINE_OUTPUT( m_OnTimerEnded, "OnTimerEnded" ),
	DEFINE_OUTPUT( m_OnBecomePressable, "OnBecomePressable" ),

	DEFINE_THINKFUNC( UnlockThink ),
	DEFINE_THINKFUNC( TimerEndThink ),

END_DATADESC()

LINK_ENTITY_TO_CLASS( func_switch, CFuncSwitch )

CFuncSwitch::CFuncSwitch()
{
	m_flLockTime = 0.0f;
	m_flTimeLength = 0.0f;

	m_bPressable = true;
	m_flLockTime = 1.0f;

	m_bSet1 = true;
}

void CFuncSwitch::Spawn( void )
{
	BaseClass::Spawn();
	
	if ( GetMoveParent() && GetRootMoveParent()->GetSolid() == SOLID_BSP )
	{
		SetSolid( SOLID_BSP );
	}
	else
	{
		SetSolid( SOLID_VPHYSICS );
	}
	
	SetModel( STRING( GetModelName() ) );
}

void CFuncSwitch::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	if ( !m_bPressable )
		return;

	m_OnPressed.FireOutput( pActivator, pCaller );

	m_bPressable = false;
	SetContextThink( &CFuncSwitch::UnlockThink, gpGlobals->curtime + m_flLockTime, s_pszUnlockThinkContext );

	// For the special button
	if ( m_bSet1 )
	{
		m_OnSet1.FireOutput( pActivator, pCaller );
	}
	else
	{
		m_OnSet2.FireOutput( pActivator, pCaller );
	}
	
	m_bSet1 = !m_bSet1;

	if ( GetNextThinkTick( s_pszTimerEndThinkContext ) == -1 )
	{
		m_OnTimerStarted.FireOutput( pActivator, pCaller );
	}

	SetContextThink( &CFuncSwitch::TimerEndThink, gpGlobals->curtime + m_flTimeLength, s_pszTimerEndThinkContext );
}

void CFuncSwitch::UnlockThink( void )
{
	m_bPressable = true;
	m_OnBecomePressable.FireOutput( this, this ); // Technically, the activator should be the one who pressed the button.
}

void CFuncSwitch::TimerEndThink( void )
{
	m_bSet1 = true;
	m_OnTimerEnded.FireOutput( this, this ); // Technically, the activator should be the one who pressed the button.
}