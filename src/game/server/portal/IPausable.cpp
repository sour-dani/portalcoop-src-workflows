#include "cbase.h"
#include "IPausable.h"

CUtlVector<IPausable*> g_AllPausables;

IPausable::IPausable()
{
	g_AllPausables.AddToTail( this );
}

IPausable::~IPausable()
{
	g_AllPausables.FindAndRemove( this );
}