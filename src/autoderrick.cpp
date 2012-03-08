// autoderrick.cpp

#define STRICT
#define D3D_OVERLOADS

#include <windows.h>
#include <stdio.h>
#include <d3d.h>

#include "struct.h"
#include "D3DEngine.h"
#include "D3DMath.h"
#include "event.h"
#include "misc.h"
#include "iman.h"
#include "math3d.h"
#include "particule.h"
#include "terrain.h"
#include "camera.h"
#include "object.h"
#include "interface.h"
#include "button.h"
#include "window.h"
#include "sound.h"
#include "displaytext.h"
#include "cmdtoken.h"
#include "auto.h"
#include "autoderrick.h"



#define DERRICK_DELAY		10.0f		// dur�e de l'extraction
#define DERRICK_DELAYu		30.0f		// idem, mais pour l'uranium




// Constructeur de l'objet.

CAutoDerrick::CAutoDerrick(CInstanceManager* iMan, CObject* object)
						   : CAuto(iMan, object)
{
	CAuto::CAuto(iMan, object);

	Init();
	m_phase = ADP_WAIT;  // en pause jusqu'au premier Init()
	m_soundChannel = -1;
}

// Destructeur de l'objet.

CAutoDerrick::~CAutoDerrick()
{
	CAuto::~CAuto();
}


// D�truit l'objet.

void CAutoDerrick::DeleteObject(BOOL bAll)
{
	CObject*	fret;

	if ( !bAll )
	{
		fret = SearchFret();
		if ( fret != 0 && fret->RetLock() )
		{
			fret->DeleteObject();
			delete fret;
		}
	}

	if ( m_soundChannel != -1 )
	{
		m_sound->FlushEnvelope(m_soundChannel);
		m_sound->AddEnvelope(m_soundChannel, 0.0f, 1.0f, 1.0f, SOPER_STOP);
		m_soundChannel = -1;
	}

	CAuto::DeleteObject(bAll);
}


// Initialise l'objet.

void CAutoDerrick::Init()
{
	D3DMATRIX*	mat;
	D3DVECTOR	pos;
	TerrainRes	res;

	pos = m_object->RetPosition(0);
	res = m_terrain->RetResource(pos);

	if ( res == TR_STONE   ||
		 res == TR_URANIUM ||
		 res == TR_KEYa    ||
		 res == TR_KEYb    ||
		 res == TR_KEYc    ||
		 res == TR_KEYd    )
	{
		m_type = OBJECT_FRET;
		if ( res == TR_STONE   )  m_type = OBJECT_STONE;
		if ( res == TR_URANIUM )  m_type = OBJECT_URANIUM;
		if ( res == TR_KEYa    )  m_type = OBJECT_KEYa;
		if ( res == TR_KEYb    )  m_type = OBJECT_KEYb;
		if ( res == TR_KEYc    )  m_type = OBJECT_KEYc;
		if ( res == TR_KEYd    )  m_type = OBJECT_KEYd;

		m_phase    = ADP_EXCAVATE;
		m_progress = 0.0f;
		m_speed    = 1.0f/(m_type==OBJECT_URANIUM?DERRICK_DELAYu:DERRICK_DELAY);
	}
	else
	{
		m_phase    = ADP_WAIT;
		m_progress = 0.0f;
		m_speed    = 1.0f;
	}

	m_time = 0.0f;
	m_timeVirus = 0.0f;
	m_lastParticule = 0.0f;
	m_lastTrack = 0.0f;

	pos = D3DVECTOR(7.0f, 0.0f, 0.0f);
	mat = m_object->RetWorldMatrix(0);
	pos = Transform(*mat, pos);
	m_terrain->MoveOnFloor(pos);
	m_fretPos = pos;
}


// Gestion d'un �v�nement.

BOOL CAutoDerrick::EventProcess(const Event &event)
{
	CObject*	fret;
	D3DVECTOR	pos, speed;
	FPOINT		dim;
	float		angle, duration, factor;

	CAuto::EventProcess(event);

	if ( m_engine->RetPause() )  return TRUE;
	if ( event.event != EVENT_FRAME )  return TRUE;
	if ( m_phase == ADP_WAIT )  return TRUE;

	m_progress += event.rTime*m_speed;
	m_timeVirus -= event.rTime;

	if ( m_object->RetVirusMode() )  // contamin� par un virus ?
	{
		if ( m_timeVirus <= 0.0f )
		{
			m_timeVirus = 0.1f+Rand()*0.3f;

			pos.x = 0.0f;
			pos.z = 0.0f;
			pos.y = -2.0f*Rand();
			m_object->SetPosition(1, pos);  // monte/descend le foret

			m_object->SetAngleY(1, Rand()*0.5f);  // tourne le foret
		}
		return TRUE;
	}

	if ( m_phase == ADP_EXCAVATE )
	{
		if ( m_soundChannel == -1 )
		{
			if ( m_type == OBJECT_URANIUM )
			{
				factor = DERRICK_DELAYu/DERRICK_DELAY;
			}
			else
			{
				factor = 1.0f;
			}
			m_soundChannel = m_sound->Play(SOUND_DERRICK, m_object->RetPosition(0), 1.0f, 0.5f, TRUE);
			m_sound->AddEnvelope(m_soundChannel, 1.0f, 0.5f, 4.0f*factor, SOPER_CONTINUE);
			m_sound->AddEnvelope(m_soundChannel, 1.0f, 0.3f, 6.0f*factor, SOPER_CONTINUE);
			m_sound->AddEnvelope(m_soundChannel, 1.0f, 0.5f, 1.0f, SOPER_CONTINUE);
			m_sound->AddEnvelope(m_soundChannel, 1.0f, 0.5f, 4.0f, SOPER_STOP);
		}

		if ( m_progress >= 6.0f/16.0f &&  // p�n�tre dans le sol ?
			 m_lastParticule+m_engine->ParticuleAdapt(0.05f) <= m_time )
		{
			m_lastParticule = m_time;

			pos = m_object->RetPosition(0);
			speed.x = (Rand()-0.5f)*10.0f;
			speed.z = (Rand()-0.5f)*10.0f;
			speed.y = Rand()*5.0f;
			dim.x = Rand()*3.0f+2.0f;
			dim.y = dim.x;
			m_particule->CreateParticule(pos, speed, dim, PARTICRASH, 2.0f);
		}

		if ( m_progress >= 6.0f/16.0f &&  // p�n�tre dans le sol ?
			 m_lastTrack+m_engine->ParticuleAdapt(0.5f) <= m_time )
		{
			m_lastTrack = m_time;

			pos = m_object->RetPosition(0);
			speed.x = (Rand()-0.5f)*12.0f;
			speed.z = (Rand()-0.5f)*12.0f;
			speed.y = Rand()*10.0f+10.0f;
			dim.x = 0.6f;
			dim.y = dim.x;
			pos.y += dim.y;
			duration = Rand()*2.0f+2.0f;
			m_particule->CreateTrack(pos, speed, dim, PARTITRACK5,
									 duration, Rand()*10.0f+15.0f,
									 duration*0.2f, 1.0f);
		}

		if ( m_progress < 1.0f )
		{
			pos.x = 0.0f;
			pos.z = 0.0f;
			pos.y = -m_progress*16.0f;
			m_object->SetPosition(1, pos);  // descend le foret

			angle = m_object->RetAngleY(1);
			angle += event.rTime*8.0f;
			m_object->SetAngleY(1, angle);  // tourne le foret
		}
		else
		{
			m_phase    = ADP_ASCEND;
			m_progress = 0.0f;
			m_speed    = 1.0f/5.0f;
		}
	}

	if ( m_phase == ADP_ASCEND )
	{
		if ( m_progress <= 7.0f/16.0f &&
			 m_lastParticule+m_engine->ParticuleAdapt(0.1f) <= m_time )
		{
			m_lastParticule = m_time;

			pos = m_object->RetPosition(0);
			speed.x = (Rand()-0.5f)*10.0f;
			speed.z = (Rand()-0.5f)*10.0f;
			speed.y = Rand()*5.0f;
			dim.x = Rand()*3.0f+2.0f;
			dim.y = dim.x;
			m_particule->CreateParticule(pos, speed, dim, PARTICRASH, 2.0f);
		}

		if ( m_progress <= 4.0f/16.0f &&
			 m_lastTrack+m_engine->ParticuleAdapt(1.0f) <= m_time )
		{
			m_lastTrack = m_time;

			pos = m_object->RetPosition(0);
			speed.x = (Rand()-0.5f)*12.0f;
			speed.z = (Rand()-0.5f)*12.0f;
			speed.y = Rand()*10.0f+10.0f;
			dim.x = 0.6f;
			dim.y = dim.x;
			pos.y += dim.y;
			duration = Rand()*2.0f+2.0f;
			m_particule->CreateTrack(pos, speed, dim, PARTITRACK5,
									 duration, Rand()*10.0f+15.0f,
									 duration*0.2f, 1.0f);
		}

		if ( m_progress < 1.0f )
		{
			pos.x = 0.0f;
			pos.z = 0.0f;
			pos.y = -(1.0f-m_progress)*16.0f;
			m_object->SetPosition(1, pos);  // remonte le foret

			angle = m_object->RetAngleY(1);
			angle -= event.rTime*2.0f;
			m_object->SetAngleY(1, angle);  // tourne le foret
		}
		else
		{
			m_soundChannel = -1;
			m_bSoundFall = FALSE;

			m_phase    = ADP_EXPORT;
			m_progress = 0.0f;
			m_speed    = 1.0f/5.0f;
		}
	}

	if ( m_phase == ADP_ISFREE )
	{
		if ( m_progress >= 1.0f )
		{
			m_bSoundFall = FALSE;

			m_phase    = ADP_EXPORT;
			m_progress = 0.0f;
			m_speed    = 1.0f/5.0f;
		}
	}

	if ( m_phase == ADP_EXPORT )
	{
		if ( m_progress == 0.0f )
		{
			if ( SearchFree(m_fretPos) )
			{
				angle = m_object->RetAngleY(0);
				CreateFret(m_fretPos, angle, m_type, 16.0f);
			}
			else
			{
				m_phase    = ADP_ISFREE;
				m_progress = 0.0f;
				m_speed    = 1.0f/2.0f;
				return TRUE;
			}
		}

		fret = SearchFret();

		if ( fret != 0 &&
			 m_progress <= 0.5f &&
			 m_lastParticule+m_engine->ParticuleAdapt(0.1f) <= m_time )
		{
			m_lastParticule = m_time;

			if ( m_progress < 0.3f )
			{
				pos = fret->RetPosition(0);
				pos.x += (Rand()-0.5f)*5.0f;
				pos.z += (Rand()-0.5f)*5.0f;
				pos.y += (Rand()-0.5f)*5.0f;
				speed = D3DVECTOR(0.0f, 0.0f, 0.0f);
				dim.x = 3.0f;
				dim.y = dim.x;
				m_particule->CreateParticule(pos, speed, dim, PARTIFIRE, 1.0f, 0.0f, 0.0f);
			}
			else
			{
				pos = fret->RetPosition(0);
				pos.x += (Rand()-0.5f)*5.0f;
				pos.z += (Rand()-0.5f)*5.0f;
				pos.y += Rand()*2.5f;
				speed = D3DVECTOR(0.0f, 0.0f, 0.0f);
				dim.x = 1.0f;
				dim.y = dim.x;
				m_particule->CreateParticule(pos, speed, dim, PARTIGLINT, 2.0f, 0.0f, 0.0f);
			}
		}

		if ( m_progress < 1.0f )
		{
			if ( fret != 0 )
			{
				pos = fret->RetPosition(0);
				pos.y -= event.rTime*20.0f;  // tombe
				if ( !m_bSoundFall && pos.y < m_fretPos.y )
				{
					m_sound->Play(SOUND_BOUM, m_fretPos);
					m_bSoundFall = TRUE;
				}
				if ( pos.y < m_fretPos.y )
				{
					pos.y = m_fretPos.y;
					fret->SetLock(FALSE);  // objet utilisable
				}
				fret->SetPosition(0, pos);
			}
		}
		else
		{
			if ( ExistKey() )  // cl� existe d�j� ?
			{
				m_phase    = ADP_WAIT;
				m_progress = 0.0f;
				m_speed    = 1.0f/10.0f;
			}
			else
			{
				m_phase    = ADP_EXCAVATE;
				m_progress = 0.0f;
				m_speed    = 1.0f/(m_type==OBJECT_URANIUM?DERRICK_DELAYu:DERRICK_DELAY);
			}
		}
	}

	return TRUE;
}


// Cr�e toute l'interface lorsque l'objet est s�lectionn�.

BOOL CAutoDerrick::CreateInterface(BOOL bSelect)
{
	CWindow*	pw;
	FPOINT		pos, ddim;
	float		ox, oy, sx, sy;

	CAuto::CreateInterface(bSelect);

	if ( !bSelect )  return TRUE;

	pw = (CWindow*)m_interface->SearchControl(EVENT_WINDOW0);
	if ( pw == 0 )  return FALSE;

	ox = 3.0f/640.0f;
	oy = 3.0f/480.0f;
	sx = 33.0f/640.0f;
	sy = 33.0f/480.0f;

	pos.x = ox+sx*0.0f;
	pos.y = oy+sy*0;
	ddim.x = 66.0f/640.0f;
	ddim.y = 66.0f/480.0f;
	pw->CreateGroup(pos, ddim, 109, EVENT_OBJECT_TYPE);

	return TRUE;
}


// Sauve tous les param�tres de l'automate.

BOOL CAutoDerrick::Write(char *line)
{
	char	name[100];

	if ( m_phase == ADP_WAIT )  return FALSE;

	sprintf(name, " aExist=%d", 1);
	strcat(line, name);

	CAuto::Write(line);

	sprintf(name, " aPhase=%d", m_phase);
	strcat(line, name);

	sprintf(name, " aProgress=%.2f", m_progress);
	strcat(line, name);

	sprintf(name, " aSpeed=%.2f", m_speed);
	strcat(line, name);

	return TRUE;
}

// Restitue tous les param�tres de l'automate.

BOOL CAutoDerrick::Read(char *line)
{
	if ( OpInt(line, "aExist", 0) == 0 )  return FALSE;

	CAuto::Read(line);

	m_phase = (AutoDerrickPhase)OpInt(line, "aPhase", ADP_WAIT);
	m_progress = OpFloat(line, "aProgress", 0.0f);
	m_speed = OpFloat(line, "aSpeed", 1.0f);

	m_lastParticule = 0.0f;

	return TRUE;
}


// Cherche l'objet fret.

CObject* CAutoDerrick::SearchFret()
{
	CObject*	pObj;
	D3DVECTOR	oPos;
	ObjectType	type;
	int			i;

	for ( i=0 ; i<1000000 ; i++ )
	{
		pObj = (CObject*)m_iMan->SearchInstance(CLASS_OBJECT, i);
		if ( pObj == 0 )  break;

		type = pObj->RetType();
		if ( type == OBJECT_DERRICK )  continue;

		oPos = pObj->RetPosition(0);

		if ( oPos.x == m_fretPos.x &&
			 oPos.z == m_fretPos.z )  return pObj;
	}

	return 0;
}

// Cherche si un emplacement est libre.

BOOL CAutoDerrick::SearchFree(D3DVECTOR pos)
{
	CObject*	pObj;
	D3DVECTOR	sPos;
	ObjectType	type;
	float		sRadius, distance;
	int			i, j;

	for ( i=0 ; i<1000000 ; i++ )
	{
		pObj = (CObject*)m_iMan->SearchInstance(CLASS_OBJECT, i);
		if ( pObj == 0 )  break;

		type = pObj->RetType();
		if ( type == OBJECT_DERRICK )  continue;

		j = 0;
		while ( pObj->GetCrashSphere(j++, sPos, sRadius) )
		{
			distance = Length(sPos, pos);
			distance -= sRadius;
			if ( distance < 2.0f )  return FALSE;  // emplacement occup�
		}
	}

	return TRUE;  // emplacement libre
}

// Cr�e un objet transportable.

void CAutoDerrick::CreateFret(D3DVECTOR pos, float angle, ObjectType type,
							  float height)
{
	CObject*		fret;

	fret = new CObject(m_iMan);
	if ( !fret->CreateResource(pos, angle, type) )
	{
		delete fret;
		m_displayText->DisplayError(ERR_TOOMANY, m_object);
		return;
	}
	fret->SetLock(TRUE);  // objet pas encore utilisable

	if ( m_object->RetResetCap() == RESET_MOVE )
	{
		fret->SetResetCap(RESET_DELETE);
	}

	pos = fret->RetPosition(0);
	pos.y += height;
	fret->SetPosition(0, pos);
}

// Cherche s'il existe d�j� une cl�.

BOOL CAutoDerrick::ExistKey()
{
	CObject*	pObj;
	ObjectType	type;
	int			i;

	if ( m_type != OBJECT_KEYa &&
		 m_type != OBJECT_KEYb &&
		 m_type != OBJECT_KEYc &&
		 m_type != OBJECT_KEYd )  return FALSE;

	for ( i=0 ; i<1000000 ; i++ )
	{
		pObj = (CObject*)m_iMan->SearchInstance(CLASS_OBJECT, i);
		if ( pObj == 0 )  break;

		type = pObj->RetType();
		if ( type == m_type )  return TRUE;
	}

	return FALSE;
}


// Retourne une erreur li�e � l'�tat de l'automate.

Error CAutoDerrick::RetError()
{
	if ( m_object->RetVirusMode() )
	{
		return ERR_BAT_VIRUS;
	}

	if ( m_phase == ADP_WAIT )  return ERR_DERRICK_NULL;
	return ERR_OK;
}


