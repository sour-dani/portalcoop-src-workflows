#include "cbase.h"
#include "filters_shared.h"

IMPLEMENT_NETWORKCLASS_ALIASED( BaseFilter, DT_BaseFilter );
BEGIN_NETWORK_TABLE( CBaseFilter, DT_BaseFilter )
#ifdef CLIENT_DLL
RecvPropBool( RECVINFO( m_bNegated ) ),
#else
SendPropBool( SENDINFO( m_bNegated ) ),
#endif
END_NETWORK_TABLE()

//-----------------------------------------------------------------------------

bool CBaseFilter::PassesFilterImpl( CBaseEntity *pCaller, CBaseEntity *pEntity )
{
	return true;
}


bool CBaseFilter::PassesFilter( CBaseEntity *pCaller, CBaseEntity *pEntity )
{
	bool baseResult = PassesFilterImpl( pCaller, pEntity );
	return (m_bNegated) ? !baseResult : baseResult;
}

IMPLEMENT_NETWORKCLASS_ALIASED( FilterMultiple, DT_FilterMultiple );
BEGIN_NETWORK_TABLE( CFilterMultiple, DT_FilterMultiple )
#ifdef CLIENT_DLL
RecvPropInt( RECVINFO( m_nFilterType ) ),
RecvPropArray3( RECVINFO_ARRAY(m_hFilter), RecvPropInt( RECVINFO( m_hFilter[ 0 ] ))),
#else
SendPropInt( SENDINFO( m_nFilterType ) ),
SendPropArray3(SENDINFO_ARRAY3(m_hFilter), SendPropInt( SENDINFO_ARRAY( m_hFilter ), 1, SPROP_UNSIGNED ) ),
#endif
END_NETWORK_TABLE()

//-----------------------------------------------------------------------------
// Purpose: Returns true if the entity passes our filter, false if not.
// Input  : pEntity - Entity to test.
//-----------------------------------------------------------------------------
bool CFilterMultiple::PassesFilterImpl( CBaseEntity *pCaller, CBaseEntity *pEntity )
{
	// Test against each filter
	if (m_nFilterType == FILTER_AND)
	{
		for (int i=0;i<MAX_FILTERS;i++)
		{
			if (m_hFilter[i] != NULL)
			{
				CBaseFilter* pFilter = (CBaseFilter *)(m_hFilter[i].Get());
				if (!pFilter->PassesFilter( pCaller, pEntity ) )
				{
					return false;
				}
			}
		}
		return true;
	}
	else  // m_nFilterType == FILTER_OR
	{
		for (int i=0;i<MAX_FILTERS;i++)
		{
			if (m_hFilter[i] != NULL)
			{
				CBaseFilter* pFilter = (CBaseFilter *)(m_hFilter[i].Get());
				if (pFilter->PassesFilter( pCaller, pEntity ) )
				{
					return true;
				}
			}
		}
		return false;
	}
}

IMPLEMENT_NETWORKCLASS_ALIASED( FilterName, DT_FilterName );
BEGIN_NETWORK_TABLE( CFilterName, DT_FilterName )
#ifdef CLIENT_DLL
RecvPropString( RECVINFO( m_iFilterName ) ),
#else
SendPropStringT( SENDINFO( m_iFilterName ) ),
#endif
END_NETWORK_TABLE()

IMPLEMENT_NETWORKCLASS_ALIASED( FilterClass, DT_FilterClass );
BEGIN_NETWORK_TABLE( CFilterClass, DT_FilterClass )
#ifdef CLIENT_DLL
RecvPropString( RECVINFO( m_iFilterClass ) ),
#else
SendPropStringT( SENDINFO( m_iFilterClass ) ),
#endif
END_NETWORK_TABLE()

IMPLEMENT_NETWORKCLASS_ALIASED( FilterPlayerIndex, DT_FilterPlayerIndex );
BEGIN_NETWORK_TABLE( CFilterPlayerIndex, DT_FilterPlayerIndex )
#ifdef CLIENT_DLL
RecvPropInt( RECVINFO( m_iPlayerIndex ) ),
#else
SendPropInt( SENDINFO( m_iPlayerIndex ) ),
#endif
END_NETWORK_TABLE()

IMPLEMENT_NETWORKCLASS_ALIASED( FilterTeam, DT_FilterTeam );
BEGIN_NETWORK_TABLE( CFilterTeam, DT_FilterTeam )
#ifdef CLIENT_DLL
RecvPropInt( RECVINFO( m_iFilterTeam ) ),
#else
SendPropInt( SENDINFO( m_iFilterTeam ) ),
#endif
END_NETWORK_TABLE()

IMPLEMENT_NETWORKCLASS_ALIASED( FilterMassGreater, DT_FilterMassGreater );
BEGIN_NETWORK_TABLE( CFilterMassGreater, DT_FilterMassGreater )
#ifdef CLIENT_DLL
RecvPropFloat( RECVINFO( m_fFilterMass ) ),
#else
SendPropFloat( SENDINFO( m_fFilterMass ) ),
#endif
END_NETWORK_TABLE()