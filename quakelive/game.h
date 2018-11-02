// ----------------------------------------------------------------------------
// Provides an interface for accessing / using game variables and functions.
// ----------------------------------------------------------------------------

#pragma once

// ===== Project includes =====
#include "engine.h"


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
class Game
{
public:

	enum PlayerPositionType
	{
		PPT_BASE,
		PPT_LEGS,
		PPT_TORSO,
		PPT_HEAD,

		PPT_MAX
	};

	// Constructor & Destructor
	Game( DWORD main_thread_id, HMODULE module_game_exe, HMODULE module_cgame );
	~Game( );

	// Helper
	centity_t*			GetEntity( int entity_index ) const;
	centity_t*			GetEntityByClientNumber( int client_number ) const;
	usercmd_t*			GetClientCommand( int command_index ) const;
	usercmd_t*			GetCurrentClientCommand( ) const;
	usercmd_t*			GetPreviousClientCommand( ) const;
	clientInfo_t*		GetClientInfo( int client_number ) const;
	clientInfo_t*		GetLocalPlayerClientInfo( ) const;
	gametype_t			GetGameType( ) const;

	// Complex Helpers
	bool				Player_IsFriend( int client_number ) const;
	bool				Player_GetPosition( int client_number, vec3_t& position, PlayerPositionType player_position_type = PPT_TORSO ) const;
	bool				GetTagPositionOffset( qhandle_t model, const char* tag_name, vec3_t& position ) const;
	bool				WorldToScreen( vec3_t v, float& sx, float& sy ) const;
	bool				TraceLine( vec3_t start, vec3_t end );

	// Drawing
	void				DrawString( int x, int y, const char* text, vec4_t color, qhandle_t font = 0, bool centered = false, int size = 20 ) const;
	int					DrawString_Width( const char* text, qhandle_t font, int size );
	int					DrawString_Height( const char* text, qhandle_t font, int size );
	void				DrawFilledRectangle( int x, int y, int width, int height, vec4_t color ) const;
	void				DrawOutlinedRectangle( int x, int y, int width, int height, vec4_t color, int line_size = 1 ) const;

	// 
	int					GetWeaponSpeed( int weapon_id );
	int					GetWeaponRange( int weapon_id );
	const char*			GetWeaponName( int weapon_id );

	// Getter - 
	DWORD				GetMainThreadID( ) const					{ return main_thread_id_; }

	// Getter - 
	DWORD				GetAddress_CG_Init( ) const					{ return address_cg_init_; }
	DWORD				GetAddress_R_Init( ) const					{ return address_r_init_; }
	DWORD				GetAddress_CG_DrawActiveFrame( ) const		{ return address_cg_draw_active_frame_; }
	DWORD				GetAddress_RE_RenderScene( ) const			{ return address_re_render_scene_; }
	DWORD				GetAddress_CL_CreateCmd( ) const			{ return address_cl_create_cmd_; }
	DWORD				GetAddress_CL_CreateNewCommands( ) const	{ return address_cl_create_new_commands_; }

	// Getter - 
	cgameImport_t*		GetCGImport( ) const						{ return *( cgameImport_t** )address_cg_import_pointer_; }
	trGlobals_t*		GetTRGlobals( ) const						{ return ( trGlobals_t* )address_tr_globals_; }
	cg_t*				GetCG( ) const								{ return ( cg_t* )address_cg_; }
	cgs_t*				GetCGS( ) const								{ return ( cgs_t* )address_cgs_; }
	centity_t*			GetCGEntities( ) const						{ return ( centity_t* )address_cg_entities_; }
	weaponInfo_t*		GetCGWeapons( ) const						{ return ( weaponInfo_t* )address_cg_weapons_; }
	gitem_t*			GetBGItemList() const						{ return ( gitem_t* )address_bg_itemlist_; }

	// Getter - 
	vec3_t*				GetClientViewAngles( ) const				{ return ( vec3_t* )address_cl_viewangles_; }
	int*				GetClientCommandNumber( ) const				{ return ( int* )address_cl_command_number_; }
	usercmd_t*			GetClientCommands( ) const					{ return ( usercmd_t* )address_cl_commands_; }
	float*				GetModelViewMatrix( ) const					{ return ( *( unknown_001_t** )address_unknown_001_ )->modelview_matrix; }
	float*				GetProjectionMatrix( ) const				{ return ( *( unknown_001_t** )address_unknown_001_ )->projection_matrix; }

private:

	// 
	DWORD				FindPattern( HMODULE module, const char* mask, const BYTE* pattern );

	// 
	DWORD				main_thread_id_;
	HMODULE				module_game_exe_;
	HMODULE				module_cgame_;

	// 
	DWORD				address_cg_init_;
	DWORD				address_r_init_;
	DWORD				address_re_render_scene_;
	DWORD				address_cg_draw_active_frame_;
	DWORD				address_cl_create_cmd_;
	DWORD				address_cl_create_new_commands_;
	DWORD				address_cg_;
	DWORD				address_cgs_;
	DWORD				address_cg_entities_;
	DWORD				address_cg_weapons_;
	DWORD				address_bg_itemlist_;
	DWORD				address_cg_import_pointer_;
	DWORD				address_tr_globals_;
	DWORD				address_cl_viewangles_;
	DWORD				address_cl_command_number_;
	DWORD				address_cl_commands_;
	DWORD				address_unknown_001_;
};

