# List of Thrift source files
set(THRIFT_SOURCES
        ../thrift-generated/Game.cpp
        ../thrift-generated/soccer_service_types.cpp
        ../thrift-client/thrift_state_generator.cpp
        ../thrift-client/thrift_client.cpp
        ../thrift-client/thrift_client_player.cpp
)

# List of gRPC source files
set(GRPC_SOURCES
        ../grpc-generated/service.pb.cc
        ../grpc-generated/service.grpc.pb.cc
        ../grpc-client/state_generator.cpp
        ../grpc-client/grpc_client.cpp
        ../grpc-client/grpc_client_player.cpp
)

set (COMMON_SOURCES
  basic_actions/basic_actions.cpp
  basic_actions/bhv_before_kick_off.cpp
  basic_actions/bhv_emergency.cpp
  basic_actions/bhv_go_to_point_look_ball.cpp
  basic_actions/bhv_scan_field.cpp
  basic_actions/body_advance_ball2009.cpp
  basic_actions/body_clear_ball2009.cpp
  basic_actions/body_dribble2008.cpp
  basic_actions/body_go_to_point.cpp
  basic_actions/body_go_to_point_dodge.cpp
  basic_actions/body_hold_ball2008.cpp
  basic_actions/body_intercept2018.cpp
  basic_actions/body_kick_one_step.cpp
  basic_actions/body_kick_to_relative.cpp
  basic_actions/body_pass.cpp
  basic_actions/body_smart_kick.cpp
  basic_actions/body_stop_ball.cpp
  basic_actions/body_stop_dash.cpp
  basic_actions/focus_move_to_point.cpp
  basic_actions/focus_reset.cpp
  basic_actions/intention_dribble2008.cpp
  basic_actions/intention_time_limit_action.cpp
  basic_actions/intercept_evaluator.cpp
  basic_actions/kick_table.cpp
  basic_actions/neck_scan_field.cpp
  basic_actions/neck_scan_players.cpp
  basic_actions/neck_turn_to_ball_and_player.cpp
  basic_actions/neck_turn_to_ball_or_scan.cpp
  basic_actions/neck_turn_to_goalie_or_scan.cpp
  basic_actions/neck_turn_to_player_or_scan.cpp
  basic_actions/neck_turn_to_low_conf_teammate.cpp
  basic_actions/view_synch.cpp
  planner/actgen_cross.cpp
  planner/actgen_direct_pass.cpp
  planner/actgen_self_pass.cpp
  planner/actgen_shoot.cpp
  planner/actgen_short_dribble.cpp
  planner/actgen_simple_dribble.cpp
  planner/actgen_strict_check_pass.cpp
  planner/action_chain_graph.cpp
  planner/action_chain_holder.cpp
  planner/bhv_planned_action.cpp
  planner/bhv_normal_dribble.cpp
  planner/bhv_pass_kick_find_receiver.cpp
  planner/bhv_strict_check_shoot.cpp
  planner/body_force_shoot.cpp
  planner/clear_ball.cpp
  planner/clear_generator.cpp
  planner/cooperative_action.cpp
  planner/cross_generator.cpp
  planner/dribble.cpp
  planner/field_analyzer.cpp
  planner/hold_ball.cpp
  planner/neck_turn_to_receiver.cpp
  planner/pass.cpp
  planner/predict_state.cpp
  planner/self_pass_generator.cpp
  planner/shoot.cpp
  planner/shoot_generator.cpp
  planner/short_dribble_generator.cpp
  planner/simple_pass_checker.cpp
  planner/strict_check_pass_generator.cpp
  planner/tackle_generator.cpp
  setplay/bhv_go_to_placed_ball.cpp
  setplay/bhv_prepare_set_play_kick.cpp
  setplay/bhv_set_play.cpp
  setplay/bhv_set_play_free_kick.cpp
  setplay/bhv_set_play_goal_kick.cpp
  setplay/bhv_set_play_indirect_free_kick.cpp
  setplay/bhv_set_play_kick_in.cpp
  setplay/bhv_set_play_kick_off.cpp
  setplay/bhv_their_goal_kick_move.cpp
  setplay/intention_wait_after_set_play_kick.cpp
  bhv_basic_move.cpp
  bhv_basic_tackle.cpp
  bhv_custom_before_kick_off.cpp
  bhv_goalie_basic_move.cpp
  bhv_goalie_chase_ball.cpp
  bhv_goalie_free_kick.cpp
  bhv_penalty_kick.cpp
  neck_default_intercept_neck.cpp
  neck_goalie_turn_neck.cpp
  neck_offensive_intercept_neck.cpp
  view_tactical.cpp
  intention_receive.cpp
  intercept_evaluator_sample.cpp
  soccer_role.cpp
  role_center_back.cpp
  role_center_forward.cpp
  role_defensive_half.cpp
  role_goalie.cpp
  role_offensive_half.cpp
  role_sample.cpp
  role_side_back.cpp
  role_side_forward.cpp
  role_side_half.cpp
  role_keepaway_keeper.cpp
  role_keepaway_taker.cpp
  keepaway_communication.cpp
  sample_communication.cpp
  sample_field_evaluator.cpp
  sample_freeform_message_parser.cpp
  sample_player.cpp
  strategy.cpp
  main_player.cpp
  ../rpc-client/rpc-player-client.cpp
  )

if (USE_THRIFT AND USE_GRPC)
  add_executable(sample_player
          ${GRPC_SOURCES}
          ${THRIFT_SOURCES}
          ${COMMON_SOURCES}
  )
elseif (USE_THRIFT)
  add_executable(sample_player
          ${THRIFT_SOURCES}
          ${COMMON_SOURCES}
  )
elseif (USE_GRPC)
  add_executable(sample_player
          ${GRPC_SOURCES}
          ${COMMON_SOURCES}
  )
else()
  message(FATAL_ERROR "Neither USE_GRPC nor USE_THRIFT is defined. One must be enabled.")
endif()

target_include_directories(sample_player
  PRIVATE
        # Include gRPC directories only if gRPC is enabled
        $<$<BOOL:${USE_GRPC}>:${PROJECT_SOURCE_DIR}/../idl/grpc/>
        ${PROJECT_SOURCE_DIR}/src/
        ${PROJECT_SOURCE_DIR}/src/player
        ${PROJECT_SOURCE_DIR}/src/player/planner
        ${PROJECT_SOURCE_DIR}/src/player/setplay
        ${PROJECT_BINARY_DIR}
  PUBLIC
        ${Boost_INCLUDE_DIRS}
        ${LIBRCSC_INCLUDE_DIR}
  )

target_link_libraries(sample_player
  PRIVATE
        # Link Thrift library only if Thrift is enabled
        $<$<BOOL:${USE_THRIFT}>:${THRIFT_LIBRARY}>

        # Link gRPC++ library only if gRPC is enabled
        $<$<BOOL:${USE_GRPC}>:${_GRPC_GRPCPP}>
        pthread
  PUBLIC
        ${LIBRCSC_LIB}
        Boost::system
        ZLIB::ZLIB
        Threads::Threads
  )

set_target_properties(sample_player
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
  )
