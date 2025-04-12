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

#include "sample_trainer.h"

#include <rcsc/common/abstract_client.h>
#include <rcsc/param/cmd_line_parser.h>

#include <iostream>
#include <cstdlib> // exit
#include <cerrno> // errno
#include <cstring> // strerror
#include <csignal> // sigaction

namespace {

SampleTrainer agent;
std::shared_ptr< rcsc::AbstractClient > client;

/*-------------------------------------------------------------------*/
void
sig_exit_handle( int )
{
    std::cerr << "Killed. Exiting trainer..." << std::endl;
    agent.finalize();
    std::exit( EXIT_FAILURE );
}

}


/*-------------------------------------------------------------------*/
int
main( int argc, char ** argv )
{
    struct sigaction sig_action ;
    sig_action.sa_handler = &sig_exit_handle ;
    sig_action.sa_flags = 0;
    if ( sigaction( SIGINT, &sig_action , NULL ) != 0
         || sigaction( SIGTERM, &sig_action , NULL ) != 0
         || sigaction( SIGHUP, &sig_action , NULL ) != 0 )
        /*if ( signal(SIGINT, &sigExitHandle) == SIG_ERR
          || signal(SIGTERM, &sigExitHandle) == SIG_ERR
          || signal(SIGHUP, &sigExitHandle) == SIG_ERR )*/
    {
        std::cerr << __FILE__ << ": " << __LINE__
                  << ": could not set signal handler: "
                  << std::strerror( errno ) << std::endl;
        std::exit( EXIT_FAILURE );
    }

    {
        rcsc::CmdLineParser cmd_parser( argc, argv );
                int grpc_port = 50051;
        bool use_same_grpc_port = true;
        bool add_20_to_grpc_port_if_right_side = false;
        std::string grpc_ip = "localhost";
        std::string rpc_type = "thrift";

        for (int i = 0; i < argc; ++i) {
            if (std::string(argv[i]) == "--rpc-port") {
                grpc_port = std::stoi(argv[i+1]);
            }
            if (std::string(argv[i]) == "--rpc-port-step") {
                use_same_grpc_port = false;
            }
            if (std::string(argv[i]) == "--rpc-add-20-to-port-for-right") {
                add_20_to_grpc_port_if_right_side = true;
            }
            if (std::string(argv[i]) == "--rpc-host") {
                grpc_ip = argv[i+1];
            }
        if (std::string(argv[i]) == "--rpc-type") {
                rpc_type = argv[i+1];
            }
        }

        agent.SetFirstRpcPort(grpc_port);
        agent.SetUseSameRpcPort(use_same_grpc_port);
        agent.SetAdd20ToRpcPortIfRightSide(add_20_to_grpc_port_if_right_side);
        agent.SetRpcIp(grpc_ip);

        bool use_thrift = rpc_type=="thrift";
#ifndef USE_GRPC
        if (!use_thrift) {
            std::cerr << "This program does not support gRPC. Please build with gRPC support." << std::endl;
            return EXIT_FAILURE;
        }
#endif
#ifndef USE_THRIFT
        if (use_thrift) {
            std::cerr << "This program does not support Thrift. Please build with Thrift support." << std::endl;
            return EXIT_FAILURE;
        }
#endif
        agent.SetRpcType(use_thrift);
        
        if ( ! agent.init( cmd_parser ) )
        {
            return EXIT_FAILURE;
        }
    }

    client = agent.createConsoleClient();
    agent.setClient( client );

    /*
      You should add your copyright message here.
     */
    // std::cout << "*****************************************************************\n"
    //           << " This program is modified by <Team Name>\n"
    //           << " Copyright 20xx. <Your name>.\n"
    //           << " <Affiliation>\n"
    //           << " All rights reserved.\n"
    //           << "*****************************************************************\n"
    //           << std::flush;

    /*
      Do NOT remove the following copyright notice!
     */
    std::cout << "*****************************************************************\n"
              << " This program is based on agent2d created by Hidehisa Akiyama.\n"
              << " Copyright 2006 - 2011. Hidehisa Akiyama.\n"
              << " All rights reserved.\n"
              << "*****************************************************************\n"
              << std::flush;

    client->run( &agent );

    return EXIT_SUCCESS;
}
