#!/bin/sh


LIBPATH=/usr/local/lib
if [ x"$LIBPATH" != x ]; then
  if [ x"$LD_LIBRARY_PATH" = x ]; then
    LD_LIBRARY_PATH=$LIBPATH
  else
    LD_LIBRARY_PATH=$LIBPATH:$LD_LIBRARY_PATH
  fi
  export LD_LIBRARY_PATH
fi

DIR=`dirname $0`

player="${DIR}/sample_player"
coach="${DIR}/sample_coach"
trainer="${DIR}/sample_trainer"
teamname="TRAINER_MODE"
host="localhost"
g_port=50051
diff_g_port="false"
gp20="false"
config="${DIR}/player.conf"
config_dir="${DIR}/formations-dt"

number=11
usecoach="true"

sleepprog=sleep
goaliesleep=1
sleeptime=0

debugopt="--offline_logging --debug --debug_server_connect"

usage()
{
  (echo "Usage: $0 [options]"
   echo "Possible options are:"
   echo "      --help                print this"
   echo "  -h, --host HOST           specifies server host"
   echo "  --g_port GRPC PORT           specifies grpc port (default: 50051)"
   echo "  --diff_g_port                specifies different grpc port for each player (default: false)"
   echo "  --gp20                       add 20 to GRPC Port if team run on right side (default: false)"
   echo "  -t, --teamname TEAMNAME   specifies team name") 1>&2
}

while [ $# -gt 0 ]
do
  case $1 in

    --help)
      usage
      exit 0
      ;;

    -h|--host)
      if [ $# -lt 2 ]; then
        usage
        exit 1
      fi
      host=$2
      shift 1
      ;;

    -t|--teamname)
      if [ $# -lt 2 ]; then
        usage
        exit 1
      fi
      teamname=$2
      shift 1
      ;;

    *)
      usage
      exit 1
      ;;
  esac

  shift 1
done

OPT="-h ${host} -t ${teamname}"
OPT="${OPT} --player-config ${config} --config_dir ${config_dir}"
OPT="${OPT} ${debugopt}"
opt="${opt} --g-port ${g_port}"
if [ "${same_g_port}" = "true" ]; then
  OPT="${OPT} --diff-g-port"
fi
if [ "${gp20}" = "true" ]; then
  OPT="${OPT} --gp20"
fi
#if [ $number -gt 0 ]; then
#  $player ${OPT} -g &
#  $sleepprog $goaliesleep
#fi

#for (( i=2; i<=${number}; i=$i+1 )) ; do
  $player ${OPT} -n 11 &
  $sleepprog $sleeptime
#done

