// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------

// ===== Platform includes =====
#include <algorithm>

// ===== Project includes =====
#include "feature_esp.h"


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
Feature_ESP::Feature_ESP( Game* game ) : 
	Feature( game )
{
	memset( &cvar_esp_enabled_, 0, sizeof( cvar_esp_enabled_ ) );
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
Feature_ESP::~Feature_ESP( )
{

}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void Feature_ESP::RegisterCVars( )
{
	game_->GetCGImport( )->Cvar_Register( &cvar_esp_enabled_, "esp_enabled", "1", 1 );
	game_->GetCGImport()->Cvar_Register( &cvar_esp_items_, "esp_items", "1", 1 );
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void Feature_ESP::UpdateCVars( )
{
	game_->GetCGImport( )->Cvar_Update( &cvar_esp_enabled_ );
	game_->GetCGImport()->Cvar_Update( &cvar_esp_items_ );
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void Feature_ESP::Render( )
{
	if ( cvar_esp_enabled_.integer == 0 )
		return;

	for ( int i = 0; i < MAX_ENTITIES; i++ )
	{
		centity_t* cent = game_->GetEntity( i );

		if ( !cent->currentValid )
			continue;

		if ( cvar_esp_items_.integer != 0 )
		{
			if ( cent->currentState.eType == ET_ITEM )
			{
				vec4_t color = { 1, 1, 1, 1 };
				if ( !game_->TraceLine( game_->GetCG( )->refdef.vieworg, cent->lerpOrigin ) )
				{
					color[ 3 ] *= 0.5;
				}

				float screen_x, screen_y;
				if ( game_->WorldToScreen(cent->lerpOrigin, screen_x, screen_y ) )
				{
					char text[ 256 ] = { 0 };
					sprintf( text, "[%s]", game_->GetBGItemList()[cent->currentState.modelindex].pickup_name );

					game_->DrawString( screen_x, screen_y, text, color, 0, true, 16 );
				}
			}
		}

		if ( cent->currentState.eType != ET_PLAYER || cent->currentState.eFlags & EF_DEAD )
			continue;

		if ( !game_->GetCGS( )->clientinfo[ cent->currentState.clientNum ].infoValid )
			continue;

		if ( game_->GetCGS( )->clientinfo[ cent->currentState.clientNum ].team == TEAM_SPECTATOR )
			continue;
		
		vec4_t friend_color = { 0, 0, 1, 1 };
		vec4_t enemy_color = { 1, 0, 0, 1 };
		vec4_t text_color = { 1, 1, 1, 1 };

		vec4_t color;
		if ( game_->Player_IsFriend(cent->currentState.clientNum ) )
			Vector4Copy( friend_color, color );
		else
			Vector4Copy( enemy_color, color );

		if ( !game_->TraceLine( game_->GetCG( )->refdef.vieworg, cent->lerpOrigin ) )
		{
			color[ 3 ] *= 0.5;
			text_color[ 3 ] *= 0.5;
		}

		vec3_t world_bottom = { 0, 0, 0 };
		vec3_t world_top = { 0, 0, 0 };

		if ( !game_->Player_GetPosition( cent->currentState.clientNum, world_bottom, Game::PPT_BASE ) )
			continue;

		if ( !game_->Player_GetPosition( cent->currentState.clientNum, world_top, Game::PPT_HEAD ) )
			continue;

		world_bottom[ 2 ] -= 40;

		world_top[ 0 ] = world_bottom[ 0 ];
		world_top[ 1 ] = world_bottom[ 1 ];
		world_top[ 2 ] += 40;

		vec2_t screen_bottom = { 0, 0 };
		vec2_t screen_top = { 0, 0 };

		if ( game_->WorldToScreen( world_bottom, screen_bottom[ 0 ], screen_bottom[ 1 ] ) && game_->WorldToScreen( world_top, screen_top[ 0 ], screen_top[ 1 ] ) )
		{
			char text[ 256 ] = { 0 };
			sprintf( text, "%s", game_->GetCGS( )->clientinfo[ cent->currentState.clientNum ].name );

			int min_width = game_->DrawString_Width( text, game_->GetCGS( )->media.charsetShader, 16 ) + 4;

			int height = screen_bottom[ 1 ] - screen_top[ 1 ];
			int width = max( height / 2, min_width );

			game_->DrawFilledRectangle( screen_top[ 0 ] - width / 2, screen_top[ 1 ] - 20, width, 20, color );
			game_->DrawString( screen_top[ 0 ] + 1, screen_top[ 1 ] - 20 + 2 - 1, text, text_color, 0, true, 16 );
			game_->DrawOutlinedRectangle( screen_top[ 0 ] - width / 2, screen_top[ 1 ], width, height, color, 2 );
		}
	}
}

