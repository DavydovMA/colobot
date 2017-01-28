// camera.h

#ifndef _CAMERA_H_
#define	_CAMERA_H_


class CInstanceManager;
class CD3DEngine;
class CTerrain;
class CWater;
class CObject;
class CRobotMain;
enum D3DMouse;


enum CameraType
{
	CAMERA_NULL		= 0,	// cam�ra ind�finie
	CAMERA_FREE		= 1,	// cam�ra libre (toujours en principe)
	CAMERA_ONBOARD	= 3,	// cam�ra � bord d'un robot
	CAMERA_BACK		= 4,	// cam�ra derri�re un robot
	CAMERA_FIX		= 5,	// cam�ra fixe apr�s robot
	CAMERA_SCRIPT	= 7,	// cam�ra pendant un film script�
	CAMERA_VISIT	= 9,	// visite du lieu d'une erreur
	CAMERA_DIALOG	= 10,	// cam�ra pendant dialogue
};

enum CameraSmooth
{
	CS_NONE			= 0,	// brusque
	CS_NORM			= 1,	// normal
	CS_HARD			= 2,	// dur
	CS_SOFT			= 3,	// dou
	CS_SPEC			= 4,	// sp�cial
};

enum CenteringPhase
{
	CP_NULL			= 0,
	CP_START		= 1,
	CP_WAIT			= 2,
	CP_STOP			= 3,
};

enum CameraEffect
{
	CE_NULL			= 0,	// pas d'effet
	CE_TERRAFORM	= 1,	// terrassement
	CE_CRASH		= 2,	// v�hicule volant pos� violemment
	CE_EXPLO		= 3,	// explosion
	CE_SHOT			= 4,	// coup non mortel
	CE_VIBRATION	= 5,	// vibration pendant construction
	CE_PET			= 6,	// rat� du r�acteur
};

enum OverEffect
{
	OE_NULL			= 0,	// pas d'effet
	OE_BLOOD		= 1,	// flash rouge
	OE_ORGA			= 2,	// flash vert
	OE_FADEINw		= 3,	// blanc -> rien
	OE_FADEOUTw		= 4,	// rien -> blanc
	OE_FADEOUTb		= 5,	// rien -> bleu
	OE_BLITZ		= 6,	// �clair
	OE_CRASH		= 7,	// collision
};



#define FREE_DIST	60.0f



class CCamera
{
public:
	CCamera(CInstanceManager* iMan);
	~CCamera();

	BOOL		EventProcess(const Event &event);

	void		Init(D3DVECTOR lookat, float dirH, float dirV, float dist);
	void		SetLookat(D3DVECTOR lookat);
	void		SetDirH(float dirH);
	void		SetDirV(float dirV);
	void		SetDistance(float dist);
	void		RetCamera(D3DVECTOR &eye, D3DVECTOR &lookat, float &dirH, float &dirV, float &dist);

	void		SetObject(CObject* object);
	CObject*	RetObject();

	void		SetType(CameraType type);
	CameraType	RetType();

	void		SetSmoothOneTime(CameraSmooth type);
	void		SetSmooth(CameraSmooth type);
	CameraSmooth RetSmoth();

	void		SetBackDist(float dist);
	float		RetBackDist();

	void		SetBackVerti(float angle);
	float		RetBackVerti();

	void		SetBackHoriz(float angle);
	float		RetBackHoriz();

	void		ResetLockRotate();
	void		SetLockRotate(BOOL bLock);
	BOOL		RetLockRotate();

	void		SetDist(float dist);
	float		RetDist();

	void		SetFixDirection(float angle);
	float		RetFixDirection();

	void		SetScrollMouse(FPOINT mouse, BOOL bFinal);

	void		StartVisit(D3DVECTOR goal, float dist);
	void		StopVisit();

	BOOL		StartCentering(CObject *object, float angleH, float angleV, float dist, float time);
	BOOL		StopCentering(CObject *object, float time);
	void		AbortCentering();

	void		FlushEffect();
	void		StartEffect(CameraEffect effect, D3DVECTOR pos, float force);

	void		FlushOver();
	void		SetOverBaseColor(D3DCOLORVALUE color);
	void		StartOver(OverEffect effect, D3DVECTOR pos, float force);

	void		FixCamera();
	void		SetScriptEye(D3DVECTOR eye);
	void		SetScriptLookat(D3DVECTOR lookat);

	D3DMouse	RetMouseDef(FPOINT pos, D3DMouse def);
	void		SetMotorSpeed(float speed);

protected:
	BOOL		EventMouseMove(const Event &event);
	void		EventMouseWheel(int dir, long param, BOOL bDown);
	BOOL		EventFrame(const Event &event);
	BOOL		EventFrameFree(const Event &event);
	BOOL		EventFrameDialog(const Event &event);
	BOOL		EventFrameBack(const Event &event);
	BOOL		EventFrameFix(const Event &event);
	BOOL		EventFrameOnBoard(const Event &event);
	BOOL		EventFrameVisit(const Event &event);
	BOOL		EventFrameScript(const Event &event);

	void		SetViewTime(const D3DVECTOR &vEyePt, const D3DVECTOR &vLookatPt, float rTime);
	BOOL		IsCollision(D3DVECTOR &eye, D3DVECTOR lookat);
	BOOL		IsCollisionBack(D3DVECTOR &eye, D3DVECTOR lookat);
	BOOL		IsCollisionFix(D3DVECTOR &eye, D3DVECTOR lookat);

	D3DVECTOR	ExcludeTerrain(D3DVECTOR eye, D3DVECTOR lookat, float &angleH, float &angleV);
	D3DVECTOR	ExcludeObject(D3DVECTOR eye, D3DVECTOR lookat, float &angleH, float &angleV);

	void		SetViewParams(const D3DVECTOR &eye, const D3DVECTOR &lookat, const D3DVECTOR &up);
	void		EffectFrame(const Event &event);
	void		OverFrame(const Event &event);

protected:
	CInstanceManager* m_iMan;
	CD3DEngine*	m_engine;
	CTerrain*	m_terrain;
	CWater*		m_water;
	CRobotMain*	m_main;

	CameraType	m_type;				// type de la cam�ra (CAMERA_*)
	CameraSmooth m_smooth;			// type de lissage
	CameraSmooth m_smoothNext;		// type de lissage
	CObject*	m_cameraObj;		// objet li� � la cam�ra

	float		m_eyeDistance;		// distance entre les yeux

	D3DVECTOR	m_actualEye;		// oeil actuel
	D3DVECTOR	m_actualLookat;		// vis�e actuelle
	D3DVECTOR	m_finalEye;			// oeil final
	D3DVECTOR	m_finalLookat;		// vis�e finale
	D3DVECTOR	m_normEye;			// oeil normal
	D3DVECTOR	m_normLookat;		// vis�e normale
	float		m_focus;

	BOOL		m_bRightDown;
	FPOINT		m_rightPosInit;
	FPOINT		m_rightPosCenter;
	FPOINT		m_rightPosMove;

	D3DVECTOR	m_eyePt;			// CAMERA_FREE: oeil
	D3DVECTOR	m_lookatPt;			// CAMERA_FREE: point vis�
	float		m_directionH;		// CAMERA_FREE: direction horizontale
	float		m_directionV;		// CAMERA_FREE: direction verticale
	float		m_rotH;				// CAMERA_FREE: rotation horizontale
	float		m_distance;			// CAMERA_FREE: �loignement de la cam�ra
	float		m_zoom;				// CAMERA_FREE: distance
	float		m_heightEye;		// CAMERA_FREE: hauteur au-dessus du sol
	float		m_heightShift;		// CAMERA_FREE: shift vertical
	float		m_heightLookat;		// CAMERA_FREE: hauteur au-dessus du sol
	float		m_speed;			// CAMERA_FREE: vitesse de d�placement
	FPOINT		m_scrollMouse;		// CAMERA_FREE: scroll bouton droite souris
	BOOL		m_bFinalScroll;		// CAMERA_FREE: scroll bouton rel�ch�

	float		m_backDist;			// CAMERA_BACK: �loignement
	float		m_backMin;			// CAMERA_BACK: �loignement minimal
	BOOL		m_bBackLockRotate;	// CAMERA_BACK: rotation autoris�e ?
	float		m_backMotorSpeed;	// CAMERA_BACK: vitesse moteur
	float		m_backSleepTime;	// CAMERA_BACK: dur�e depuis arr�t
	float		m_backRotSpeed;		// CAMERA_BACK: vitesse angulaire
	float		m_rotDirectionH;	// CAMERA_BACK: direction suppl�mentaire
	float		m_addDirectionH;	// CAMERA_BACK: direction suppl�mentaire
	float		m_addDirectionV;	// CAMERA_BACK: direction suppl�mentaire
	BOOL		m_bTransparency;

	float		m_fixDist;			// CAMERA_FIX: �loignement
	float		m_fixDirectionH;	// CAMERA_FIX: direction
	float		m_fixDirectionV;	// CAMERA_FIX: direction

	D3DVECTOR	m_visitGoal;		// CAMERA_VISIT: position vis�e
	float		m_visitDist;		// CAMERA_VISIT: �loignement
	float		m_visitTime;		// CAMERA_VISIT: temps relatif
	CameraType	m_visitType;		// CAMERA_VISIT: type initial
	float		m_visitDirectionH;	// CAMERA_VISIT: direction
	float		m_visitDirectionV;	// CAMERA_VISIT: direction

	FPOINT		m_mousePos;
	float		m_mouseDirH;
	float		m_mouseDirV;
	float		m_mouseMarging;

	CenteringPhase m_centeringPhase;
	float		m_centeringAngleH;
	float		m_centeringAngleV;
	float		m_centeringDist;
	float		m_centeringCurrentH;
	float		m_centeringCurrentV;
	float		m_centeringTime;
	float		m_centeringProgress;

	CameraEffect m_effectType;
	D3DVECTOR	m_effectPos;
	float		m_effectForce;
	float		m_effectProgress;
	D3DVECTOR	m_effectOffset;

	OverEffect	m_overType;
	float		m_overForce;
	float		m_overTime;
	D3DCOLORVALUE m_overColorBase;
	D3DCOLORVALUE m_overColor;
	int			m_overMode;
	float		m_overFadeIn;
	float		m_overFadeOut;

	D3DVECTOR	m_scriptEye;
	D3DVECTOR	m_scriptLookat;
};


#endif //_CAMERA_H_