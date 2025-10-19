//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef GRENADE_TRIPMINE_H
#define GRENADE_TRIPMINE_H
#ifdef _WIN32
#pragma once
#endif

#include "basegrenade_shared.h"

class CBeam;

#ifdef HL2SB
// for constraints
#include "vphysics/constraints.h"
#endif // HL2SB

class CTripmineGrenade : public CBaseGrenade
{
public:
	DECLARE_CLASS( CTripmineGrenade, CBaseGrenade );

	CTripmineGrenade();
#ifdef HL2SB
	~CTripmineGrenade();
#endif // HL2SB

	void Spawn( void );
	void Precache( void );

#if 0 // FIXME: OnTakeDamage_Alive() is no longer called now that base grenade derives from CBaseAnimating
	int OnTakeDamage_Alive( const CTakeDamageInfo &info );
#endif	
#ifdef HL2SB
	virtual int OnTakeDamage( const CTakeDamageInfo &info );
#endif // HL2SB
	void WarningThink( void );
	void PowerupThink( void );
	void BeamBreakThink( void );
	void DelayDeathThink( void );
	void Event_Killed( const CTakeDamageInfo &info );

	void MakeBeam( void );
	void KillBeam( void );

#ifdef HL2SB
	// Added to create a constraint
	void AttachToEntity( CBaseEntity *pOther );
	bool MakeConstraint( CBaseEntity *pOther );
#endif // HL2SB

public:
	EHANDLE		m_hOwner;
#ifdef HL2SB
	// Added for following
	EHANDLE		m_hAttachEntity;
#endif // HL2SB

private:
	float		m_flPowerUp;
	Vector		m_vecDir;
	Vector		m_vecEnd;
	float		m_flBeamLength;

	CBeam		*m_pBeam;
	Vector		m_posOwner;
	Vector		m_angleOwner;

#ifdef HL2SB
	// signifies if we're attached to something, and need to update slightly differently.
	bool		m_bAttached;
	IPhysicsConstraint	*m_pConstraint;
	Vector		m_vAttachedPosition;	// if the attached position changes, we need to detonate
#endif // HL2SB

	DECLARE_DATADESC();
};

#endif // GRENADE_TRIPMINE_H
