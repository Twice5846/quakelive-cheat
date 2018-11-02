// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------

// ===== Project includes =====
#include "engine.h"

vec3_t	vec3_origin			= { 0, 0, 0 };
vec3_t	axisDefault[ 3 ]	= { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };

vec4_t	colorBlack			= { 0, 0, 0, 1 };
vec4_t	colorRed			= { 1, 0, 0, 1 };
vec4_t	colorGreen			= { 0, 1, 0, 1 };
vec4_t	colorBlue			= { 0, 0, 1, 1 };
vec4_t	colorYellow			= { 1, 1, 0, 1 };
vec4_t	colorMagenta		= { 1, 0, 1, 1 };
vec4_t	colorCyan			= { 0, 1, 1, 1 };
vec4_t	colorWhite			= { 1, 1, 1, 1 };
vec4_t	colorLtGrey			= { 0.75, 0.75, 0.75, 1 };
vec4_t	colorMdGrey			= { 0.5, 0.5, 0.5, 1 };
vec4_t	colorDkGrey			= { 0.25, 0.25, 0.25, 1 };

void vectoangles( const vec3_t value1, vec3_t angles )
{
	float	forward;
	float	yaw, pitch;
	
	if ( value1[1] == 0 && value1[0] == 0 ) {
		yaw = 0;
		if ( value1[2] > 0 ) {
			pitch = 90;
		}
		else {
			pitch = 270;
		}
	}
	else {
		if ( value1[0] ) {
			yaw = ( atan2 ( value1[1], value1[0] ) * 180 / M_PI );
		}
		else if ( value1[1] > 0 ) {
			yaw = 90;
		}
		else {
			yaw = 270;
		}
		if ( yaw < 0 ) {
			yaw += 360;
		}

		forward = sqrt ( value1[0]*value1[0] + value1[1]*value1[1] );
		pitch = ( atan2(value1[2], forward) * 180 / M_PI );
		if ( pitch < 0 ) {
			pitch += 360;
		}
	}

	angles[ 0 ] = -pitch;
	angles[ 1 ] = yaw;
	angles[ 2 ] = 0;
}



void AngleVectors( const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up )
{
	float		angle;
	static float		sr, sp, sy, cr, cp, cy;

	angle = angles[ 1 ] * (M_PI*2 / 360);
	sy = sin(angle);
	cy = cos(angle);
	angle = angles[ 0 ] * (M_PI*2 / 360);
	sp = sin(angle);
	cp = cos(angle);
	angle = angles[ 2 ] * (M_PI*2 / 360);
	sr = sin(angle);
	cr = cos(angle);

	if (forward)
	{
		forward[0] = cp*cy;
		forward[1] = cp*sy;
		forward[2] = -sp;
	}
	if (right)
	{
		right[0] = (-1*sr*sp*cy+-1*cr*-sy);
		right[1] = (-1*sr*sp*sy+-1*cr*cy);
		right[2] = -1*sr*cp;
	}
	if (up)
	{
		up[0] = (cr*sp*cy+-sr*-sy);
		up[1] = (cr*sp*sy+-sr*cy);
		up[2] = cr*cp;
	}
}