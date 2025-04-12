#include "thrift_client.h"

#ifndef ThriftClientCoach_H
#define ThriftClientCoach_H

class ThriftClientCoach : public ThriftAgent {
    rcsc::CoachAgent * M_agent;
    public:
    ThriftClientCoach() ;

    void init(rcsc::SoccerAgent * agent,
              std::string target="localhost",
              int port=50051,
              bool use_same_grpc_port=true,
              bool add_20_to_grpc_port_if_right_side=false) override;

    void getActions();
    soccer::State generateState() const;
};
#endif