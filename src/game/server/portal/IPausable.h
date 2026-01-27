#ifndef PAUSABLE_ENTITY_H
#define PAUSABLE_ENTITY_H
#ifdef _WIN32
#pragma once
#endif

abstract_class IPausable
{
	IPausable();
	~IPausable();
public:
	virtual void OnPause( void ) = 0;
	virtual void OnUnPause( float flAddedTime ) = 0;
};

extern CUtlVector<IPausable*> g_AllPausables;

#endif // PAUSABLE_ENTITY_H