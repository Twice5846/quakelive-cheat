// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------

// ===== Platform includes =====
#include <algorithm>

// ===== Project includes =====
#include "feature_aimbot.h"


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
Feature_Aimbot::Feature_Aimbot( Game* game ) : 
	Feature( game )
{
	is_active_ = false;
	current_target_entity_index_ = -1;
	memset( &cvar_aim_enabled_, 0, sizeof( cvar_aim_enabled_ ) );
	memset( &cvar_aim_silent_, 0, sizeof( cvar_aim_silent_ ) );
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
Feature_Aimbot::~Feature_Aimbot( )
{

}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void Feature_Aimbot::RegisterCVars( )
{
	game_->GetCGImport( )->Cvar_Register( &cvar_aim_enabled_, "aim_enabled", "1", 1 );
	game_->GetCGImport( )->Cvar_Register( &cvar_aim_silent_, "aim_silent", "1", 1 );
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void Feature_Aimbot::UpdateCVars( )
{
	game_->GetCGImport( )->Cvar_Update( &cvar_aim_enabled_ );
	game_->GetCGImport( )->Cvar_Update( &cvar_aim_silent_ );
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void Feature_Aimbot::Render( )
{
	if ( cvar_aim_enabled_.integer == 0 )
		return;

	if ( GetAsyncKeyState( VK_MBUTTON ) & 1 )
		is_active_ = !is_active_;

	game_->DrawFilledRectangle( game_->GetCG( )->refdef.width / 2 - 75 + 1, game_->GetCG( )->refdef.height - 5 - 20 + 1, 150 - 2, 20 - 2, colorLtGrey );
	game_->DrawOutlinedRectangle( game_->GetCG( )->refdef.width / 2 - 75, game_->GetCG( )->refdef.height - 5 - 20, 150, 20, colorBlack );

	char text_string[ 256 ] = { 0 };
	vec4_t text_color = { 0, 0, 0, 0 };

	if ( is_active_ )
	{
		strcpy( text_string, "AIMBOT ACTIVE" );
		Vector4Copy( colorGreen, text_color );
	}
	else
	{
		strcpy( text_string, "AIMBOT INACTIVE" );
		Vector4Copy( colorRed, text_color );
	}

	game_->DrawString( game_->GetCG( )->refdef.width / 2, game_->GetCG( )->refdef.height - 5 - 20 + 2, text_string, text_color, 0, true, 16 );
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void Feature_Aimbot::ClientCommand( )
{
	if ( cvar_aim_enabled_.integer == 0 )
		return;

	if ( !is_active_ )
		return;

	Target current_target;
	if ( !CheckTarget( current_target_entity_index_, &current_target ) )
	{
		std::vector< Target > target_list;
		if ( FindTargets( target_list ) )
			current_target = target_list[ 0 ];
		else
			return;
	}

	usercmd_t* client_command = CreateClientCommand( );

	AimAtPosition( current_target.position, client_command, cvar_aim_silent_.integer != 0 );

	client_command->buttons |= 1;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
usercmd_t* Feature_Aimbot::CreateClientCommand( )
{
	( *game_->GetClientCommandNumber( ) )++;

	memcpy( game_->GetCurrentClientCommand( ), game_->GetPreviousClientCommand( ), sizeof( usercmd_t ) );

	game_->GetPreviousClientCommand( )->serverTime--;

	return game_->GetPreviousClientCommand( );
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void Feature_Aimbot::GetMuzzlePosition( vec3_t& muzzle_position ) const
{
	VectorClear( muzzle_position );

	vec3_t view_position = { 0, 0, 0 };
	VectorCopy( game_->GetCG( )->predictedPlayerState.origin, view_position );

	view_position[ 2 ] += game_->GetCG( )->predictedPlayerState.viewheight;

	VectorCopy( view_position, muzzle_position );

	/* The muzzle position is actually 14 units in the forward direction.
	vec3_t forward = { 0, 0, 0 };
	AngleVectors( game_->GetCG( )->predictedPlayerState.viewangles, forward, nullptr, nullptr );

	VectorMA( view_position, 14, forward, muzzle_position );

	SnapVector( muzzle_position );
	*/
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
int Feature_Aimbot::GetWeaponRange( int weapon_id ) const
{
	switch ( weapon_id )
	{
		case 3:		return 300;		// Shotgun
		case 4:		return 200;		// Grenade Launcher
		case 5:		return 400;		// Rocket Launcher
		case 8:		return 1000;	// Plasma Gun
		case 9:		return 1000;	// BFG
		default:	return game_->GetWeaponRange( weapon_id );
	}
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
bool Feature_Aimbot::CheckTarget( int entity_index, Target* out_target )
{
	centity_t* entity = game_->GetEntity( entity_index );
	if ( entity == nullptr )
		return false;

	if ( !entity->currentValid || entity->currentState.eType != ET_PLAYER || entity->currentState.eFlags & EF_DEAD )
		return false;

	clientInfo_t* client_info = game_->GetClientInfo( entity->currentState.clientNum );
	if ( client_info == nullptr )
		return false;

	if ( !client_info->infoValid )
		return false;

	if ( game_->Player_IsFriend( entity->currentState.clientNum ) )
		return false;

	vec3_t target_position = { 0, 0, 0 };
	if ( !game_->Player_GetPosition( entity->currentState.clientNum, target_position, Game::PPT_TORSO ) )
		return false;

	vec3_t view_position = { 0, 0, 0 };
	GetMuzzlePosition( view_position );

	vec3_t target_delta = { 0, 0, 0 };
	VectorSubtract( target_position, view_position, target_delta );

	int weapon_id = game_->GetCG( )->predictedPlayerState.weapon;
	int weapon_speed =  game_->GetWeaponSpeed( weapon_id );

	if ( weapon_speed > 0 && game_->GetCG( )->snap && game_->GetCG( )->nextSnap )
	{
		vec3_t target_velocity = { 0, 0, 0 };
		VectorSubtract( entity->nextState.pos.trBase, entity->currentState.pos.trBase, target_velocity );
		VectorScale( target_velocity, 1000.0 / ( game_->GetCG( )->nextSnap->serverTime - game_->GetCG( )->snap->serverTime ), target_velocity );
			
		float time = VectorLength( target_delta ) / ( weapon_speed  - VectorLength( target_velocity ) );

		vec3_t target_position_predicted = { 0, 0, 0 };
		VectorMA( target_position, time, target_velocity, target_position_predicted );

		if ( !game_->TraceLine( target_position, target_position_predicted ) )
			return false;

		VectorCopy( target_position_predicted, target_position );
		VectorSubtract( target_position, view_position, target_delta );
	}

	int weapon_range = GetWeaponRange( weapon_id );
	int target_distance = VectorLength( target_delta );

	if ( weapon_range > 0 && weapon_range < target_distance )
		return false;

	if ( !game_->TraceLine( view_position, target_position ) )
		return false;

	if ( out_target != nullptr )
	{
		out_target->entity_index = entity_index;
		out_target->distance = target_distance;

		VectorCopy( target_position, out_target->position );
	}

	return true;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
bool Feature_Aimbot::FindTargets( std::vector< Target >& target_list )
{
	for ( int i = 0; i < MAX_ENTITIES; i++ )
	{
		Target target;
		if ( !CheckTarget( i, &target ) )
			continue;

		target_list.push_back( target );
	}

	std::sort( target_list.begin( ), target_list.end( ) );

	return target_list.size( ) > 0;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void Feature_Aimbot::AimAtPosition( vec3_t position, usercmd_t* client_command, bool silent )
{
	vec3_t view_position = { 0, 0, 0 };
	GetMuzzlePosition( view_position );

	vec3_t delta = { 0, 0, 0 };
	VectorSubtract( position, view_position, delta );

	vec3_t angles = { 0, 0, 0 };
	vectoangles( delta, angles );

	angles[ 0 ] -= SHORT2ANGLE( game_->GetCG( )->predictedPlayerState.delta_angles[ 0 ] );
	angles[ 1 ] -= SHORT2ANGLE( game_->GetCG( )->predictedPlayerState.delta_angles[ 1 ] );
	angles[ 2 ] = 0;

	client_command->angles[ 0 ] = ANGLE2SHORT( angles[ 0 ] );
	client_command->angles[ 1 ] = ANGLE2SHORT( angles[ 1 ] );
	client_command->buttons |= 1;

	if ( silent )
	{
		// 
		int forwardmove = client_command->forwardmove;
		int rightmove = client_command->rightmove;
					
		if ( forwardmove != 0 || rightmove != 0 )
		{
			float move_angle = AngleNormalize360( RAD2DEG( atan2( -rightmove / 127.0, forwardmove / 127.0 ) ) );
			float delta_angle = AngleNormalize360( angles[ 1 ] - ( game_->GetCG( )->predictedPlayerState.viewangles[ 1 ] - SHORT2ANGLE( game_->GetCG( )->predictedPlayerState.delta_angles[ 1 ] ) ) );
			float destination_angle = AngleNormalize360( move_angle - delta_angle );
			float forwardmove_ratio = cos( DEG2RAD( destination_angle ) );
			float rightmove_ratio = -sin( DEG2RAD( destination_angle ) );

			if ( abs( forwardmove_ratio ) < abs( rightmove_ratio ) )
			{
				forwardmove_ratio *= 1.0 / abs( rightmove_ratio );
				rightmove_ratio = rightmove_ratio > 0.0 ? 1.0 : -1.0;
			}
			else if ( abs( forwardmove_ratio ) > abs( rightmove_ratio ) )
			{
				rightmove_ratio *= 1.0 / abs( forwardmove_ratio );
				forwardmove_ratio = forwardmove_ratio > 0.0 ? 1.0 : -1.0;
			}
			else
			{
				forwardmove_ratio = 1.0;
				rightmove_ratio = 1.0;
			}

			forwardmove = forwardmove_ratio * 127;
			rightmove = rightmove_ratio * 127;

			client_command->forwardmove = forwardmove;
			client_command->rightmove = rightmove;
		}
	}
	else
	{
		VectorCopy( angles, *game_->GetClientViewAngles( ) );
	}
}

