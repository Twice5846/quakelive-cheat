// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------

// ===== Platform includes =====
#include <Windows.h>
#include <stdio.h>
#include <Psapi.h>

// ===== Project includes =====
#include "game.h"


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
Game::Game( DWORD main_thread_id, HMODULE module_game_exe, HMODULE module_cgame )
{
	main_thread_id_ = main_thread_id;
	module_game_exe_ = module_game_exe;
	module_cgame_ = module_cgame;

	address_cg_init_ = FindPattern( module_cgame_, "xxxxxxxxxxxxxxxx", ( const BYTE* )"\x8B\x54\x24\x08\x53\x33\xC0\x55\x56\x57\xB9\xA6\xA0\x00\x00\xBF" );
	address_cg_draw_active_frame_ = FindPattern( module_cgame_, "xxxxxxxxxxxxxxxxxx", ( const BYTE* )"\x55\x8B\xEC\x83\xE4\xC0\x8B\x45\x08\x8B\x4D\x10\x83\xEC\x74\x53\x56\x57" );
	address_re_render_scene_ = FindPattern( module_game_exe_, "xxxxxxxxxx????xxxxxx", ( const BYTE* )"\x55\x8B\xEC\x81\xEC\x48\x02\x00\x00\xA1\x00\x00\x00\x00\x33\xC5\x89\x45\xFC\x83" );
	address_cl_create_cmd_ = FindPattern( module_game_exe_, "xxxxxxxx????xxxxxx????xxxx", ( const BYTE* )"\x55\x8B\xEC\x83\xEC\x0C\xD9\x05\x00\x00\x00\x00\x53\xD9\x5D\xF4\xD9\x05\x00\x00\x00\x00\xD9\x5D\xF8\xE8" );
	address_cl_create_new_commands_ = FindPattern( module_game_exe_, "xxxxxxxx????xxxxx????xx", ( const BYTE* )"\x55\x8B\xEC\x83\xEC\x1C\x83\x3D\x00\x00\x00\x00\x07\x7C\x63\x8B\x0D\x00\x00\x00\x00\x8B\xC1" );
	DWORD address_unknown_001 = FindPattern( module_cgame_, "xxxxxx????x", ( const BYTE* )"\x8D\x04\xC9\x8D\x04\xC5\x00\x00\x00\x00\xC3" );
		
	ReadProcessMemory( GetCurrentProcess( ), ( void* )( address_cg_init_ + 28 ), &address_cg_, sizeof( address_cg_ ), nullptr );
	ReadProcessMemory( GetCurrentProcess( ), ( void* )( address_cg_init_ + 16 ), &address_cgs_, sizeof( address_cgs_ ), nullptr );
	ReadProcessMemory( GetCurrentProcess( ), ( void* )( address_cg_init_ + 40 ), &address_cg_entities_, sizeof( address_cg_entities_ ), nullptr );
	ReadProcessMemory( GetCurrentProcess( ), ( void* )( address_cg_init_ + 52 ), &address_cg_weapons_, sizeof( address_cg_weapons_ ), nullptr );
	ReadProcessMemory( GetCurrentProcess( ), ( void* )( address_cg_init_ + 99 ), &address_cg_import_pointer_, sizeof( address_cg_import_pointer_ ), nullptr );
	ReadProcessMemory( GetCurrentProcess( ), ( void* )( address_cl_create_cmd_ + 8 ), &address_cl_viewangles_, sizeof( address_cl_viewangles_ ), nullptr );
	ReadProcessMemory( GetCurrentProcess( ), ( void* )( address_cl_create_new_commands_ + 52 ), &address_cl_command_number_, sizeof( address_cl_command_number_ ), nullptr );
	ReadProcessMemory( GetCurrentProcess( ), ( void* )( address_cl_create_new_commands_ + 99 ), &address_cl_commands_, sizeof( address_cl_commands_ ), nullptr );
	ReadProcessMemory( GetCurrentProcess( ), ( void* )( address_re_render_scene_ + 595 ), &address_unknown_001_, sizeof( address_unknown_001_ ), nullptr );
	ReadProcessMemory( GetCurrentProcess( ), ( void* )( address_unknown_001 + 6 ), &address_bg_itemlist_, sizeof( address_bg_itemlist_ ), nullptr );

	FILE* debug_file = fopen( "quakelive_offsets.txt", "w+" );
	if ( debug_file )
	{
		fprintf( debug_file, "===== FUNCTIONS =====\n" );
		fprintf( debug_file, "CG_Init.............: 0x%08X (cgamex86.dll)\n", address_cg_init_ - ( DWORD )module_cgame_ );
		fprintf( debug_file, "CG_DrawActiveFrame..: 0x%08X (cgamex86.dll)\n", address_cg_draw_active_frame_ - ( DWORD )module_cgame_ );
		fprintf( debug_file, "RE_RenderScene......: 0x%08X (quakelive_steam.exe)\n", address_re_render_scene_ - ( DWORD )module_game_exe_ );
		fprintf( debug_file, "CL_CreateCmd........: 0x%08X (quakelive_steam.exe)\n", address_cl_create_cmd_ - ( DWORD )module_game_exe_ );
		fprintf( debug_file, "CL_CreateNewCommands: 0x%08X (quakelive_steam.exe)\n", address_cl_create_new_commands_ - ( DWORD )module_game_exe_ );

		fprintf( debug_file, "\n===== STRUCTURES =====\n" );
		fprintf( debug_file, "cg..................: 0x%08X (cgamex86.dll)\n", address_cg_ - ( DWORD )module_cgame_ );
		fprintf( debug_file, "cgs.................: 0x%08X (cgamex86.dll)\n", address_cgs_ - ( DWORD )module_cgame_ );
		fprintf( debug_file, "cg_entities.........: 0x%08X (cgamex86.dll)\n", address_cg_entities_ - ( DWORD )module_cgame_ );
		fprintf( debug_file, "bg_itemlist.........: 0x%08X (cgamex86.dll)\n", address_bg_itemlist_ - (DWORD)module_cgame_);
		fprintf( debug_file, "cg_import_pointer...: 0x%08X (cgamex86.dll)\n", address_cg_import_pointer_ - ( DWORD )module_cgame_ );
		fprintf( debug_file, "cl_viewangles.......: 0x%08X (quakelive_steam.exe)\n", address_cl_viewangles_ - ( DWORD )module_game_exe_ );
		fprintf( debug_file, "unknown_001.........: 0x%08X (quakelive_steam.exe)\n", address_unknown_001_ - ( DWORD )module_game_exe_ );

		fclose( debug_file );
	}
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
Game::~Game( )
{

}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
centity_t* Game::GetEntity( int entity_index ) const
{
	return &( ( GetCGEntities( ) )[ entity_index ] );
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
centity_t* Game::GetEntityByClientNumber( int client_number ) const
{
	for ( int i = 0; i < MAX_ENTITIES; i++ )
	{
		centity_t* entity = GetEntity( i );
		if ( entity->currentState.eType == ET_PLAYER && entity->currentState.clientNum == client_number )
			return entity;
	}

	return nullptr;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
usercmd_t* Game::GetClientCommand( int command_index ) const
{
	return &( ( GetClientCommands( ) )[ command_index & CMD_MASK ] );
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
usercmd_t* Game::GetCurrentClientCommand( ) const
{
	return GetClientCommand( *GetClientCommandNumber( ) );
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
usercmd_t* Game::GetPreviousClientCommand( ) const
{
	return GetClientCommand( *GetClientCommandNumber( ) - 1 );
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
clientInfo_t* Game::GetClientInfo( int client_number ) const
{
	if ( client_number < 0 || client_number >= 64 )
		client_number = 0;

	return &( GetCGS( )->clientinfo[ client_number ] );
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
clientInfo_t* Game::GetLocalPlayerClientInfo( ) const
{
	return &( GetCGS( )->clientinfo[ GetCG( )->predictedPlayerEntity.currentState.clientNum ] );
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
gametype_t Game::GetGameType( ) const
{
	return GetCGS( )->gametype;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
bool Game::Player_IsFriend( int client_number ) const
{
	return ( GetLocalPlayerClientInfo( )->team == GetClientInfo( client_number )->team && GetGameType( ) != GT_FFA );
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
bool Game::Player_GetPosition( int client_number, vec3_t& position,  PlayerPositionType player_position_type ) const
{
	VectorClear( position );

	centity_t* entity = GetEntityByClientNumber( client_number );
	if ( entity == nullptr )
		return false;

	clientInfo_t* client_info = GetClientInfo( client_number );
	if ( client_info == nullptr )
		return false;

	if ( player_position_type == PPT_BASE )
	{
		VectorCopy( entity->lerpOrigin, position );

		return true;
	}

	vec3_t position_offset_torso = { 0, 0, 0 };
	if ( !GetTagPositionOffset( client_info->legsModel, "tag_torso", position_offset_torso ) )
		return false;

	vec3_t position_offset_head = { 0, 0, 0 };
	if ( !GetTagPositionOffset( client_info->torsoModel, "tag_head", position_offset_head ) )
		return false;

	switch ( player_position_type )
	{
		case PPT_LEGS:
		{
			VectorMA( entity->lerpOrigin, 0.5f, position_offset_torso, position );

			return true;
		}
		case PPT_TORSO:
		{
			VectorAdd( entity->lerpOrigin, position_offset_torso, position );
			VectorMA( position, 0.5f, position_offset_head, position );

			return true;
		}
		case PPT_HEAD:
		{
			VectorAdd( entity->lerpOrigin, position_offset_torso, position );
			VectorAdd( position, position_offset_head, position );

			return true;
		}
	}

	return false;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
bool Game::GetTagPositionOffset( qhandle_t model, const char* tag_name, vec3_t& position_offset ) const
{
	VectorClear( position_offset );

	orientation_t tag;
	if ( !GetCGImport( )->R_LerpTag( &tag, model, INT_MAX, INT_MAX, 1.0, tag_name ) )
		return false;

	VectorRotate( tag.origin, tag.axis, position_offset );

	return true;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
bool Game::WorldToScreen( vec3_t v, float& sx, float& sy ) const
{
	// Clear results.
	sx = 0;
	sy = 0;

	// Setup constants.
	const int sw = GetCG( )->refdef.width;
	const int sh = GetCG( )->refdef.height;

	// Fetch ModelView and Projection Matrices.
	float* mvm = GetModelViewMatrix( );
	float* pm = GetProjectionMatrix( );

	// Calculate eye matrix.
	vec4_t eye = { 0, 0, 0, 0 };
	for ( int i = 0; i < 4 ; i++ )
		eye[ i ] = v[ 0 ] * mvm[ i + 0 * 4 ] + v[ 1 ] * mvm[ i + 1 * 4 ] + v[ 2 ] * mvm[ i + 2 * 4 ] + 1 * mvm[ i + 3 * 4 ];

	// Calculate clip matrix.
	vec4_t clip = { 0, 0, 0, 0 };
	for ( int i = 0; i < 4 ; i++ )
		clip[ i ] = eye[ 0 ] * pm[ i + 0 * 4 ] + eye[ 1 ] * pm[ i + 1 * 4 ] + eye[ 2 ] * pm[ i + 2 * 4 ] + eye[ 3 ] * pm[ i + 3 * 4 ];

	// Transform to screen.
	vec3_t norm = { 0, 0, 0 };
	norm[ 0 ] = clip[ 0 ] / clip[ 3 ];
	norm[ 1 ] = clip[ 1 ] / clip[ 3 ];
	norm[ 2 ] = ( clip[ 2 ] + clip[ 3 ] ) / ( 2 * clip[ 3 ] );

	// Calculate screen position.
	sx = floor( ( 0.5 * ( 1.0 + norm[ 0 ] ) * sw ) + 0.5 );
	sy = sh - floor( ( 0.5 * ( 1.0 + norm[ 1 ] ) * sh ) + 0.5 );

	// Check if result is valid.
	return norm[ 2 ] < 1.0;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
bool Game::TraceLine( vec3_t start, vec3_t end )
{
	trace_t trace;
	GetCGImport( )->CM_BoxTrace( &trace, start, end, nullptr, nullptr, 0, CONTENTS_SOLID, 0 );

	return trace.fraction == 1.0;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void Game::DrawString( int x, int y, const char* text, vec4_t color, qhandle_t font, bool centered, int size ) const
{
	vec5_t text_width_height = { 0, 0, 0, 0, 0 };
	GetCGImport( )->CG_Text_Width_Height( text, 0, font, size, -1, text_width_height );

	y += ( text_width_height[ 3 ] - text_width_height[ 1 ] );

	if ( centered )
	{
		x -= ( text_width_height[ 2 ] - text_width_height[ 0 ] ) / 2.0f;
	}

	GetCGImport( )->R_SetColor( color );
	GetCGImport( )->CG_Text_Paint( x, y, text, font, size, -1, 0, 1 );
	GetCGImport( )->R_SetColor( nullptr );
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
int Game::DrawString_Width( const char* text, qhandle_t font, int size )
{
	vec5_t text_width_height = { 0, 0, 0, 0, 0 };
	GetCGImport( )->CG_Text_Width_Height( text, 0, font, size, -1, text_width_height );

	return text_width_height[ 2 ] - text_width_height[ 0 ];
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
int Game::DrawString_Height( const char* text, qhandle_t font, int size )
{
	vec5_t text_width_height = { 0, 0, 0, 0, 0 };
	GetCGImport( )->CG_Text_Width_Height( text, 0, font, size, -1, text_width_height );

	return text_width_height[ 3 ] - text_width_height[ 1 ];
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void Game::DrawFilledRectangle( int x, int y, int width, int height, vec4_t color ) const
{
	GetCGImport( )->R_SetColor( color );
	GetCGImport( )->R_DrawStretchPic( x, y, width, height, 0, 0, 1, 1, GetCGS( )->media.whiteShader );
	GetCGImport( )->R_SetColor( nullptr );
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void Game::DrawOutlinedRectangle( int x, int y, int width, int height, vec4_t color, int line_size ) const
{
	DrawFilledRectangle( x, y, line_size, height, color );
	DrawFilledRectangle( x, y, width, line_size, color );
	DrawFilledRectangle( x + width - line_size, y, line_size, height, color );
	DrawFilledRectangle( x, y + height - line_size, width, line_size, color );
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
int Game::GetWeaponSpeed( int weapon_id )
{
	switch ( weapon_id )
	{
		case 1:		return 0;		// Gauntlet
		case 2:		return 0;		// Chaingun
		case 3:		return 0;		// Shotgun
		case 4:		return 700;		// Grenade Launcher
		case 5:		return 1000;	// Rocket Launcher
		case 6:		return 0;		// Lightning Gun
		case 7:		return 0;		// Rail Gun
		case 8:		return 2000;	// Plasma Gun
		case 9:		return 1800;	// BFG
		case 10:	return 0;		// Heavy Machine Gun
		default:	return 0;
	}
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
int Game::GetWeaponRange( int weapon_id )
{
	switch ( weapon_id )
	{
		case 1:		return 32;		// Gauntlet
		case 2:		return 0;		// Chaingun
		case 3:		return 0;		// Shotgun
		case 4:		return 0;		// Grenade Launcher
		case 5:		return 0;		// Rocket Launcher
		case 6:		return 768;		// Lightning Gun
		case 7:		return 0;		// Rail Gun
		case 8:		return 0;		// Plasma Gun
		case 9:		return 0;		// BFG
		case 10:	return 0;		// Heavy Machine Gun
		default:	return 0;
	}
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
const char* Game::GetWeaponName( int weapon_id )
{
	switch ( weapon_id )
	{
		case 1:		return "Gauntlet";
		case 2:		return "Chaingun";
		case 3:		return "Shotgun";
		case 4:		return "Grenade Launcher";
		case 5:		return "Rocket Launcher";
		case 6:		return "Lightning Gun";
		case 7:		return "Rail Gun";
		case 8:		return "Plasma Gun";
		case 9:		return "BFG";
		case 10:	return "Heavy Machine Gun";
		default:	return "";
	}
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
DWORD Game::FindPattern( HMODULE module, const char* mask, const BYTE* pattern )
{
	// 
	MODULEINFO module_info;
	if ( !GetModuleInformation( GetCurrentProcess( ), module, &module_info, sizeof( module_info ) ) )
		return 0;

	// 
	DWORD address = 0;

	// 
	int mask_length = strlen( mask );
	
	// 
	BYTE* data = new BYTE[ mask_length ];

	// 
	for ( DWORD current_address = ( DWORD )module_info.lpBaseOfDll; current_address <= ( DWORD )module_info.lpBaseOfDll + module_info.SizeOfImage; current_address++ )
	{
		if ( !ReadProcessMemory( GetCurrentProcess( ), ( void* )( current_address ), data, mask_length, nullptr ) )
			continue;

		bool pattern_match = true;
		for ( int i = 0; i < mask_length; i++ )
		{
			if ( mask[ i ] == '?' )
				continue;

			if ( data[ i ] != pattern[ i ] )
			{
				pattern_match = false;

				break;
			}
		}

		if ( pattern_match )
		{
			address = current_address;

			break;
		}
	}

	delete data;

	// 
	return address;
}

