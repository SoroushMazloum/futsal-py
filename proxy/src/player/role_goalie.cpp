// -*-c++-*-

/*
 *Copyright:

 Copyright (C) Hidehisa AKIYAMA

 This code is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3, or (at your option)
 any later version.

 This code is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this code; see the file COPYING.  If not, write to
 the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

 *EndCopyright:
 */

/////////////////////////////////////////////////////////////////////

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "role_goalie.h"

#include "bhv_goalie_basic_move.h"
#include "bhv_goalie_chase_ball.h"
#include "bhv_goalie_free_kick.h"

#include "basic_actions/basic_actions.h"
#include "basic_actions/neck_scan_field.h"
#include "basic_actions/body_clear_ball.h"

#include <rcsc/player/player_agent.h>
#include <rcsc/player/debug_client.h>
#include <rcsc/player/world_model.h>

#include <rcsc/common/logger.h>
#include <rcsc/common/server_param.h>
#include <rcsc/geom/rect_2d.h>

using namespace rcsc;

const std::string RoleGoalie::NAME( "Goalie" );

/*-------------------------------------------------------------------*/
/*!

 */
namespace {
rcss::RegHolder role = SoccerRole::creators().autoReg( &RoleGoalie::create,
                                                       RoleGoalie::name() );
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
RoleGoalie::execute( PlayerAgent * agent )
{
    static const Rect2D our_penalty( Vector2D( -ServerParam::i().pitchHalfLength(),
                                               -ServerParam::i().penaltyAreaHalfWidth() + 1.0 ),
                                     Size2D( ServerParam::i().penaltyAreaLength() - 1.0,
                                             ServerParam::i().penaltyAreaWidth() - 2.0 ) );

    //////////////////////////////////////////////////////////////
    // play_on play

    // catchable
    if ( agent->world().self().isKickable() )
    {
        doKick( agent );
    }
    else
    {
        doMove( agent );
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
RoleGoalie::doKick( PlayerAgent * agent )
{
    if (Body_ClearBall().execute( agent ))
    {
        agent->setNeckAction( new Neck_ScanField() );
        return true;
    }
    else
    {
        agent->setNeckAction( new Neck_ScanField() );
        return false;
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
RoleGoalie::doMove( PlayerAgent * agent )
{
    if ( Bhv_GoalieChaseBall::is_ball_chase_situation( agent ) )
    {
        return Bhv_GoalieChaseBall().execute( agent );
    }
    else
    {
        return Bhv_GoalieBasicMove().execute( agent );
    }
}
