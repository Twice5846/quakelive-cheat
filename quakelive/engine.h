// ----------------------------------------------------------------------------
// Declares internal Quake Live structures and constants.
// ----------------------------------------------------------------------------

#pragma once

// ===== Platform Includes

#include <Windows.h>
#include <math.h>

// ===== Defines =====

#define MAX_ENTITIES					1024
#define MAX_CLIENTS						64
#define MAX_WEAPONS						16
#define	ANGLE2SHORT( x )				( ( int )( ( x ) * 65536 / 360 ) & 65535 )
#define	SHORT2ANGLE( x )				( ( x ) * ( 360.0 / 65536 ) )
#define	MAKERGB( v, r, g, b )			( v[ 0 ] = r; v[ 1 ] = g; v[ 2 ] = b )
#define	MAKERGBA( v, r, g, b, a )		( v[ 0 ] = r; v[ 1 ] = g; v[ 2 ] = b; v[ 3 ] = a )
#define DEG2RAD( a )					( ( ( a ) * M_PI ) / 180.0F )
#define RAD2DEG( a )					( ( ( a ) * 180.0f ) / M_PI )
#define	nanmask							( 255 << 23 )
#define	IS_NAN( x )						( ( ( *( int* ) & x ) & nanmask ) == nanmask )
#define SQRTFAST( x )					( (x) * sqrt( x ) )
#define DotProduct( x, y )				( ( x )[ 0 ] * ( y )[ 0 ] + ( x )[ 1 ] * ( y )[ 1 ] + ( x )[ 2 ] * ( y )[ 2 ] )
#define VectorSubtract( a, b, c )		( ( c )[ 0 ] = ( a )[ 0 ] - ( b )[ 0 ], ( c )[ 1 ] = ( a )[ 1 ] - ( b )[ 1 ], ( c )[ 2 ] = ( a )[ 2 ] - ( b )[ 2 ] )
#define VectorAdd( a, b, c )			( (c  )[ 0 ] = ( a )[ 0 ] + ( b )[ 0 ], ( c )[ 1 ] = ( a )[ 1 ] + ( b )[ 1 ], ( c )[ 2 ] = ( a )[ 2 ] + ( b )[ 2 ] )
#define VectorCopy( a, b )				( ( b )[ 0 ] = ( a )[ 0 ], ( b )[ 1 ] = ( a )[ 1 ], ( b )[ 2 ] = ( a )[ 2 ] )
#define	VectorScale( v, s, o )			( ( o )[ 0 ] = ( v )[ 0 ] * ( s ), ( o )[ 1 ] = ( v )[ 1 ] * ( s ), ( o )[ 2 ] = ( v )[ 2 ] * ( s ) )
#define	VectorMA( v, s, b, o )			( ( o )[ 0 ] = ( v )[ 0 ] + ( b )[ 0 ] * ( s ), ( o )[ 1 ] = ( v )[ 1 ] + ( b )[ 1 ] * ( s ), ( o )[ 2 ] = ( v )[ 2 ] + ( b )[ 2 ] * ( s ) )
#define VectorClear( a )				( ( a )[ 0 ] = ( a )[ 1 ] = ( a )[ 2 ]=0)
#define VectorNegate( a, b )			( ( b )[ 0 ] = -( a )[ 0 ], ( b )[ 1 ] = -( a )[ 1 ], ( b )[ 2 ] = -( a )[ 2 ] )
#define VectorSet( v, x, y, z )			( ( v )[ 0 ] = ( x ), ( v )[ 1 ] = ( y ), ( v )[ 2 ] = ( z ) )
#define Vector4Copy( a, b )				( ( b )[ 0 ] = ( a )[ 0 ],( b )[ 1 ] = ( a )[ 1 ], ( b )[ 2 ] =( a )[ 2 ], ( b )[ 3 ] = ( a )[ 3 ] )
#define Byte4Copy( a, b )				( ( b )[ 0 ] = ( a )[ 0 ],( b )[ 1 ] = ( a )[ 1 ], ( b )[ 2 ] =( a )[ 2 ], ( b )[ 3 ] = ( a )[ 3 ] )
#define	SnapVector( v )					{ v[ 0 ] = ( ( int )( v[ 0 ] ) ); v[ 1 ] = ( ( int )( v[ 1 ] ) ); v[ 2 ] = ( ( int )( v[ 2 ] ) ); }
#define random( )						( ( rand ( ) & 0x7fff) / ( ( float )0x7fff ) )
#define crandom( )						( 2.0 * ( random( ) - 0.5 ) )
#define MAX( x, y )						( ( x ) > ( y ) ? ( x ) : ( y ) )
#define MIN( x, y )						( ( x ) < ( y ) ? ( x ) : ( y ) )
#define	MASK_ALL						( -1 )
#define	CONTENTS_SOLID					1
#define	CONTENTS_BODY					0x2000000
#define	CONTENTS_CORPSE					0x4000000
#define	MASK_SHOT						( CONTENTS_SOLID | CONTENTS_BODY | CONTENTS_CORPSE )
#define M_PI							3.14159265358979323846f
#define	CMD_BACKUP						64	
#define	CMD_MASK						( CMD_BACKUP - 1 )

// ===== ENUMS =====

enum qboolean
{
	qfalse = 0,
	qtrue = 1
};

enum entityType_t
{
	ET_GENERAL,
	ET_PLAYER,
	ET_ITEM,
	ET_MISSILE,
	ET_MOVER,
	ET_BEAM,
	ET_PORTAL,
	ET_SPEAKER,
	ET_PUSH_TRIGGER,
	ET_TELEPORT_TRIGGER,
	ET_INVISIBLE,
	ET_GRAPPLE,
	ET_TEAM,
	ET_EVENTS
};

enum entityFlag_t
{
	EF_DEAD = 0x00000001,
	EF_TICKING = 0x00000002,
	EF_TELEPORT_BIT = 0x00000004,
	EF_AWARD_EXCELLENT = 0x00000008,
	EF_PLAYER_EVENT = 0x00000010,
	EF_BOUNCE = 0x00000010,
	EF_BOUNCE_HALF = 0x00000020,
	EF_AWARD_GAUNTLET = 0x00000040,
	EF_NODRAW = 0x00000080,
	EF_FIRING = 0x00000100,
	EF_KAMIKAZE = 0x00000200,
	EF_MOVER_STOP = 0x00000400,
	EF_AWARD_CAP = 0x00000800,
	EF_TALK = 0x00001000,
	EF_CONNECTION = 0x00002000,
	EF_VOTED = 0x00004000,
	EF_AWARD_IMPRESSIVE = 0x00008000,
	EF_AWARD_DEFEND = 0x00010000,
	EF_AWARD_ASSIST = 0x00020000,
	EF_AWARD_DENIED = 0x00040000,
	EF_TEAMVOTED = 0x00080000
};

enum trType_t
{
	TR_STATIONARY,
	TR_INTERPOLATE,
	TR_LINEAR,
	TR_LINEAR_STOP,
	TR_SINE,
	TR_GRAVITY
};

// ===== TYPEDEFS =====

typedef int				qhandle_t;
typedef int				cvarHandle_t;
typedef float			vec_t;
typedef vec_t			vec2_t[ 2 ];
typedef vec_t			vec3_t[ 3 ];
typedef vec_t			vec4_t[ 4 ];
typedef vec_t			vec5_t[ 5 ];

struct cplane_t
{
	vec3_t	normal;
	float	dist;
	BYTE	type;
	BYTE	signbits;
	BYTE	pad[ 2 ];
};

struct trace_t
{
	qboolean	allsolid;
	qboolean	startsolid;
	float		fraction;
	vec3_t		endpos;
	cplane_t	plane;
	int			surfaceFlags;
	int			contents;
	int			entityNum;
};

struct orientation_t
{
	vec3_t		origin;
	vec3_t		axis[ 3 ];
};

// ===== ENUMS =====

enum team_t
{
	TEAM_FREE,
	TEAM_RED,
	TEAM_BLUE,
	TEAM_SPECTATOR,

	TEAM_NUM_TEAMS
};

enum gametype_t
{
	GT_FFA,
	GT_TOURNAMENT,
	GT_SINGLE_PLAYER,
	GT_TEAM,
	GT_CTF,
	GT_1FCTF,
	GT_OBELISK,
	GT_HARVESTER,

	GT_MAX_GAME_TYPE
};

// ===== STRUCTS =====

struct cgameImport_t
{
	BYTE			unknown_001[ 4 * 4 ];
	void			( __cdecl * Cvar_Register )( struct vmCvar_t* vmCvar, const char* varName, const char* defaultValue, int flags );
	BYTE			unknown_002[ 1 * 4 ];
	void			( __cdecl * Cvar_Update )( struct vmCvar_t* vmCvar );
	BYTE			unknown_003[ 3 * 4 ];
	struct cvar_t*	( __cdecl * Cvar_FindVar )( const char* var_name );
	char			unknown_004[ 9 * 4 ];
	void			( __cdecl * SendConsoleCommand )( const char* text );
	BYTE			unknown_005[ 2 * 4 ];
	void			( __cdecl * SendClientCommand )( const char* s );
	BYTE			unknown_006[ 8 * 4 ];
	void			( __cdecl * CM_BoxTrace )( trace_t* results, const vec3_t start, const vec3_t end, vec3_t mins, vec3_t maxs, int model, int brushmask, int capsule );
	BYTE			unknown_007[ 19 * 4 ];
	int				( __cdecl * R_RegisterShader )( const char* name );
	int				( __cdecl * R_RegisterShaderNoMip )( const char* name );
	BYTE			unknown_008[ 24 * 4 ];
	void			( __cdecl * R_SetColor )( const float* rgba );
	void			( __cdecl * R_DrawStretchPic )( float x, float y, float w, float h, float s1, float t1, float s2, float t2, qhandle_t hShader );
	BYTE			unknown_009[ 2 * 4 ];
	int				( __cdecl* R_LerpTag )( orientation_t* tag, qhandle_t handle, int startFrame, int endFrame, float frac, const char* tagName );
	BYTE			unknown_010[ 37 * 4 ];
	qboolean		( __cdecl * R_TransformModelToClip )( vec3_t src, vec4_t eye, vec4_t clip );
	void			( __cdecl * R_TransformClipToWindow )( const vec4_t clip, vec4_t normalized, vec4_t window );
	BYTE			unknown_011[ 1 * 4 ];
	void			( __cdecl * CG_Text_Paint )( int x, int y, const char* text, qhandle_t font, float scale, int maxChars, int unknown_001, int forceColor );
	void			( __cdecl * CG_Text_Width_Height )( const char* text, int unknown_001, qhandle_t font, float scale, int limit, vec5_t out_rectangle );
	BYTE			unknown_012[ 31 * 4 ];
};

struct trajectory_t
{
	trType_t	trType;
	int			trTime;
	int			trDuration;
	vec3_t		trBase;
	vec3_t		trDelta;
};

struct entityState_t
{
	int				number;
	int				eType;
	int				eFlags;
	trajectory_t	pos;
	trajectory_t	apos;
	BYTE			unknown_001[ 84 ]; // 92
	int				modelindex;
	BYTE			unknown_002[ 4 ];
	int				clientNum;
	BYTE			unknown_003[ 28 ];
	int				weapon;
	BYTE			unknown_004[ 24 ];
};	// size = 236 bytes

struct centity_t
{
	entityState_t	currentState;
	entityState_t	nextState;
	qboolean		interpolate;
	qboolean		currentValid;
	BYTE			unknown_001[ 216 ];
	vec3_t			lerpOrigin;
	vec3_t			lerpAngles;
};

struct clientInfo_t
{
	qboolean		infoValid;
	BYTE			unknown_001[ 4 ];
	char			name[ 64 ];
	BYTE			unknown_002[ 192 ];
	team_t			team;
	BYTE			unknown_003[ 496 ];
	qhandle_t		legsModel;
	qhandle_t		legsSkin;
	qhandle_t		torsoModel;
	qhandle_t		torsoSkin;
	qhandle_t		headModel;
	qhandle_t		headSkin;
	BYTE			unknown_004[ 1060 ];
};	// size = 462 bytes

struct cgMedia_t
{
	qhandle_t		charsetShader;
	qhandle_t		charsetProp;
	qhandle_t		charsetPropGlow;
	qhandle_t		charsetPropB;
	qhandle_t		whiteShader;
	BYTE			unknown_001[ 5028 ];
};	// size =  bytes

struct cgs_t
{
	int				gameState;
	BYTE			unknown_001[ 31448 ];
	float			screenXScale;
	float			screenYScale;
	float			screenXBias;
	int				serverCommandSequence;
	int				processedSnapshotNum;
	qboolean		localServer;
	gametype_t		gametype;
	BYTE			unknown_003[ 76 ];
	char			mapname[ 64 ];
	BYTE			unknown_004[ 7500 ];
	clientInfo_t	clientinfo[ MAX_CLIENTS ];
	BYTE			unknown_005[ 2064 ];
	cgMedia_t		media;
};	// size = 164504 bytes

struct refdef_t
{
	int				x;
	int				y;
	int				width;
	int				height;
	float			fov_x;
	float			fov_y;
	vec3_t			vieworg;
	vec3_t			viewaxis[ 3 ];
	int				time;
	int				rdflags;
	BYTE			areamask[ 32 ];
	char			text[ 8 ][ 32];
};	// 

struct playerState_t
{
	int				commandTime;
	int				pm_type;
	int				bobCycle;
	int				pm_flags;
	int				pm_time;
	vec3_t			origin;
	vec3_t			velocity;
	int				weaponTime;
	int				gravity;
	int				speed;
	int				delta_angles[ 3 ];
	int				groundEntityNum;
	BYTE			unknown_001[ 68 ];
	int				clientNum;
	int				weapon;
	BYTE			unknown_002[ 4 ];
	int				weaponstate;
	BYTE			unknown_003[ 4 ];
	vec3_t			viewangles;
	int				viewheight;
	int				damageEvent;
	int				damageYaw;
	int				damagePitch;
	int				damageCount;
	// Stats start here. First field is current health.
	BYTE			unknown_004[ 400 ];
};	// size = 592 bytes

struct snapshot_t
{
	int				snapFlags;
	int				ping;
	int				serverTime;
	byte			areamask[ 32 ];
	playerState_t	ps;
	int				numEntities;
	entityState_t	entities[ 256 ];
	int				numServerCommands;
	int				serverCommandSequence;
};

struct cg_t
{
	int				clientFrame;
	int				clientNum;
	qboolean		demoPlayback;
	qboolean		levelShot;
	int				deferredPlayerLoading;
	qboolean		loading;
	qboolean		intermissionStarted;
	int				latestSnapshotNum;
	int				latestSnapshotTime;
	snapshot_t*		snap;
	snapshot_t*		nextSnap;
	BYTE			unknown_001[ 182544 ];
	float			frameInterpolation;
	qboolean		thisFrameTeleport;
	qboolean		nextFrameTeleport;
	int				frametime;
	int				time;
	int				oldTime;
	int				physicsTime;
	BYTE			unknown_002[ 24 ];
	playerState_t	predictedPlayerState;
	centity_t		predictedPlayerEntity;
	BYTE			unknown_003[ 240 ];
	refdef_t		refdef;
	vec3_t			refdefViewAngles;
	BYTE			unknown_004[ 123348 ];
};	// size = 307780 bytes

struct usercmd_t
{
	int				serverTime;
	int				angles[ 3 ];
	int 			buttons;
	BYTE			weapon;
	BYTE			unknown_001[ 2 ];
	char			forwardmove;
	char			rightmove;
	char			upmove;
	BYTE			unknown_002[ 2 ];
};

struct cvar_t
{
	char*		name;
	char*		string;
	BYTE		unknown_001[ 24 ];
	qboolean	modified;
	BYTE		unknown_002[ 4 ];
	int			modificationCount;
	BYTE		unknown_003[ 12 ];
	float		value;
	cvar_t*		prev;
	cvar_t*		next;
};	// size = 68 bytes

struct vmCvar_t
{
	cvarHandle_t	handle;
	int				modificationCount;
	float			value;
	int				integer;
	char			string[ 256 ];
};

struct weaponInfo_t
{
	qboolean		registered;
	BYTE			unknown_001[ 64 ];
	qhandle_t		weaponIcon;
	qhandle_t		ammoIcon;
	BYTE			unknown_002[ 60 ];
};	// size = 136 bytes

struct gitem_t
{
	char*	classname;
	char*	pickup_sound;
	char*	world_model[ 8 ];
	char*	icon;
	char*	pickup_name;
	int		quantity;
	int		giType;
	int		giTag;
	char*	precaches;
	char*	sounds;
	BYTE	unknown_001[ 4 ];
};	// size = 72 bytes

struct orientationr_t
{
	vec3_t		origin;
	vec3_t		axis[ 3 ];
	vec3_t		viewOrigin;
	float		modelMatrix[ 16 ];
};

struct trGlobals_t
{
	BYTE			unknown_001[ 1836 ];
	float			or_modelMatrix[ 16 ];
	BYTE			unknown_002[ 169728 ];
};	// size = 171628 bytes

struct unknown_001_t
{
	BYTE	unknown_001[ 722728 ];
	float	modelview_matrix[ 16 ];
	BYTE	unknown_002[ 72 ];
	float	projection_matrix[ 16 ];
};

// ===== EXTERNAL VARIABLES =====

extern vec4_t				colorBlack;
extern vec4_t				colorRed;
extern vec4_t				colorGreen;
extern vec4_t				colorBlue;
extern vec4_t				colorYellow;
extern vec4_t				colorMagenta;
extern vec4_t				colorCyan;
extern vec4_t				colorWhite;
extern vec4_t				colorLtGrey;
extern vec4_t				colorMdGrey;
extern vec4_t				colorDkGrey;
extern vec3_t				vec3_origin;
extern vec3_t				axisDefault[ 3 ];

void	vectoangles( const vec3_t value1, vec3_t angles);
void	AngleVectors( const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up );

// ===== INLINE FUNCTIONS =====

static inline int VectorCompare( const vec3_t v1, const vec3_t v2 )
{
	if ( v1[ 0 ] != v2[ 0 ] || v1[ 1 ] != v2[1] || v1[ 2 ] != v2[ 2 ] )
	{
		return 0;
	}

	return 1;
}

static inline vec_t VectorLength( const vec3_t v )
{
	return ( vec_t )sqrt( v[ 0 ] * v[ 0 ] + v[ 1 ] * v[ 1 ] + v[ 2 ] * v[ 2 ] );
}

static inline vec_t VectorLengthSquared( const vec3_t v )
{
	return ( v[ 0 ] * v[ 0 ] + v[ 1 ] * v[ 1 ] + v[ 2 ] * v[ 2 ] );
}

static inline vec_t Distance( const vec3_t p1, const vec3_t p2 )
{
	vec3_t	v;

	VectorSubtract( p2, p1, v );

	return VectorLength( v );
}

static inline vec_t DistanceSquared( const vec3_t p1, const vec3_t p2 )
{
	vec3_t	v;

	VectorSubtract( p2, p1, v );

	return v[ 0 ] * v[0] + v[ 1 ] * v[ 1 ] + v[ 2 ] * v[ 2 ];
}

static inline void VectorNormalizeFast( vec3_t v )
{
	float ilength;

	ilength = sqrt( DotProduct( v, v ) );

	v[ 0 ] *= ilength;
	v[ 1 ] *= ilength;
	v[ 2 ] *= ilength;
}

static inline void VectorInverse( vec3_t v )
{
	v[ 0 ] = -v[ 0 ];
	v[ 1 ] = -v[ 1 ];
	v[ 2 ] = -v[ 2 ];
}

static inline void CrossProduct( const vec3_t v1, const vec3_t v2, vec3_t cross )
{
	cross[ 0 ] = v1[ 1 ] * v2[ 2 ] - v1[ 2 ] * v2[ 1 ];
	cross[ 1 ] = v1[ 2 ] * v2[ 0 ] - v1[ 0 ] * v2[ 2 ];
	cross[ 2 ] = v1[ 0 ] * v2[ 1 ] - v1[ 1 ] * v2[ 0 ];
}

static void inline VectorRotate( vec3_t in, vec3_t matrix[3], vec3_t out )
{
	out[ 0 ] = DotProduct( in, matrix[ 0 ] );
	out[ 1 ] = DotProduct( in, matrix[ 1 ] );
	out[ 2 ] = DotProduct( in, matrix[ 2 ] );
}

static inline float AngleNormalize360( float angle )
{
	return ( 360.0 / 65536 ) * ( ( int )( angle * ( 65536 / 360.0 ) ) & 65535 );
}

static inline float AngleNormalize180( float angle )
{
	angle = AngleNormalize360( angle );
	if ( angle > 180.0 )
	{
		angle -= 360.0;
	}
	return angle;
}

static inline float AngleDelta( float angle1, float angle2 )
{
	return AngleNormalize180( angle1 - angle2 );
}