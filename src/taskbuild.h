// taskbuild.h

#ifndef _TASKBUILD_H_
#define	_TASKBUILD_H_


class CInstanceManager;
class CTerrain;
class CBrain;
class CPhysics;
class CObject;



#define BUILDMARGIN		16.0f
#define TBMAXLIGHT		4


enum TaskBuildPhase
{
	TBP_TURN	= 1,	// tourne
	TBP_MOVE	= 2,	// avance/recule
	TBP_TAKE	= 3,	// prend arme
	TBP_PREP	= 4,	// pr�pare
	TBP_BUILD	= 5,	// construit
	TBP_TERM	= 6,	// termine
	TBP_RECEDE	= 7,	// recule terminal
};



class CTaskBuild : public CTask
{
public:
	CTaskBuild(CInstanceManager* iMan, CObject* object);
	~CTaskBuild();

	BOOL		EventProcess(const Event &event);

	Error		Start(ObjectType type);
	Error		IsEnded();
	BOOL		Abort();

protected:
	Error		FlatFloor();
	BOOL		CreateBuilding(D3DVECTOR pos, float angle);
	void		CreateLight();
	void		BlackLight();
	CObject*	SearchMetalObject(float &angle, float dMin, float dMax, float aLimit, Error &err);
	void		DeleteMark(D3DVECTOR pos, float radius);

protected:
	ObjectType		m_type;			// type de construction
	CObject*		m_metal;		// objet metal transform�
	CObject*		m_power;		// pile du v�hicule
	CObject*		m_building;		// batiment construit
	TaskBuildPhase	m_phase;		// phase de l'op�ration
	BOOL			m_bError;		// TRUE -> op�ration impossible
	BOOL			m_bBuild;		// TRUE -> batiment construit
	BOOL			m_bBlack;		// TRUE -> lumi�res noir -> blanc
	float			m_time;			// temps absolu
	float			m_lastParticule;// temps g�n�ration derni�re particule
	float			m_progress;		// progression (0..1)
	float			m_speed;		// vitesse de la progression
	float			m_angleY;		// angle de rotation du v�hicule
	float			m_angleZ;		// angle de rotation du canon
	D3DVECTOR		m_buildingPos;	// position initiale du batiment
	float			m_buildingHeight;// hauteur du building
	int				m_lightRank[TBMAXLIGHT];// lumi�res pour les effets
	int				m_soundChannel;
};


#endif //_TASKBUILD_H_
