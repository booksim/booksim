#include "booksim.hpp"
#include <string>
#include <stdlib.h>

#include "routefunc.hpp"
#include "traffic.hpp"
#include "booksim_config.hpp"
#include "trafficmanager.hpp"
#include "random_utils.hpp"
#include "network.hpp"
#include "singlenet.hpp"
#include "kncube.hpp"
#include "fly.hpp"
#include "injection.hpp"

void AllocatorSim( const Configuration& config )
{
  Network *net;
  string topo;

  config.GetStr( "topology", topo );

  if ( topo == "torus" ) {
    net = new KNCube( config, true );
  } else if ( topo == "mesh" ) {
    net = new KNCube( config, false );
  } else if ( topo == "fly" ) {
    net = new KNFly( config );
  } else if ( topo == "single" ) {
    net = new SingleNet( config );
  } else {
    cerr << "Unknown topology " << topo << endl;
    exit(-1);
  }

  if ( config.GetInt( "link_failures" ) ) {
    net->InsertRandomFaults( config );
  }

  TrafficManager traffic( config, net );
  traffic.Run( );
}

int main( int argc, char **argv )
{
  BookSimConfig config;

  if ( !ParseArgs( &config, argc, argv ) ) {
    cout << "Usage: " << argv[0] << " configfile" << endl;
    return 0;
  }

  InitializeRoutingMap( );
  InitializeTrafficMap( );
  InitializeInjectionMap( );

  RandomSeed( config.GetInt("seed") );

  AllocatorSim( config );

  return 0;
}
