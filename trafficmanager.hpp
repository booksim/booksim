#ifndef _TRAFFICMANAGER_HPP_
#define _TRAFFICMANAGER_HPP_

#include <list>
#include <map>
#include <queue>

#include "module.hpp"
#include "config_utils.hpp"
#include "network.hpp"
#include "flit.hpp"
#include "buffer_state.hpp"
#include "stats.hpp"
#include "traffic.hpp"
#include "routefunc.hpp"
#include "outputset.hpp"
#include "injection.hpp"

class flitp_compare {
public:
  bool operator()( const Flit *a, const Flit *b ) const {
    return ( a->sn > b->sn );
  }
};

class TrafficManager : public Module {

  int _sources;
  int _dests;

  Network *_net;

  // ============ Message priorities ============ 

  enum ePriority { class_based, age_based, none };

  ePriority _pri_type;
  int       _classes;

  // ============ Injection VC states  ============ 

  BufferState **_buf_states;

  // ============ Injection queues ============ 

  int          _voqing;
  int          **_qtime;
  bool         **_qdrained;
  list<Flit *> **_partial_packets;

  bool         _use_lagging;

  list<Flit *> **_voq;
  list<int>    *_active_list;
  bool         **_active_vc;

  int           _measured_in_flight;
  int           _total_in_flight;
  map<int,bool> _in_flight;
  bool          _empty_network;

  int          _split_packets; 

  // ============ Reorder queues ============ 
  
  bool _reorder;
  
  int                    **_inject_sqn;
  int                    **_rob_sqn;
  int                    **_rob_sqn_max;
  int                     *_rob_pri;

  priority_queue<Flit *, vector<Flit *>, flitp_compare> **_rob;

  // ============ Statistics ============

  Stats **_latency_stats;     
  Stats **_overall_latency;
  Stats *_rob_latency;
  Stats *_rob_size;

  Stats **_pair_latency;
  Stats *_hop_stats;

  Stats **_accepted_packets;
  Stats *_overall_accepted;
  Stats *_overall_accepted_min;

  int   **_latest_packet;

  bool  _flit_timing;

  // ============ Simulation parameters ============ 

  enum eSimState { warming_up, running, draining, done };
  eSimState _sim_state;

  enum eSimMode { latency, throughput };
  eSimMode _sim_mode;

  int   _warmup_time;
  int   _drain_time;

  float _load;
  int   _packet_size;

  int   _total_sims;
  int   _sample_period;
  int   _max_samples;
  int   _warmup_periods;

  int   _include_queuing;

  double _latency_thres;

  float _internal_speedup;
  float _partial_internal_cycles;

  int _cur_id;
  int _time;

  list<Flit *> _used_flits;
  list<Flit *> _free_flits;

  tTrafficFunction  _traffic_function;
  tRoutingFunction  _routing_function;
  tInjectionProcess _injection_process;

  // ============ Internal methods ============ 

  Flit *_NewFlit( );
  void _RetireFlit( Flit *f, int dest );
  
  void _FirstStep( );
  void _Step( );

  Flit *_ReadROB( int dest );

  bool _PacketsOutstanding( ) const;
  
  int  _IssuePacket( int source, int cl ) const;
  void _GeneratePacket( int source, int size, int cl, int time );

  void _ClassInject( );
  void _VOQInject( );

  void _ClearStats( );

  int  _ComputeAccepted( double *avg, double *min ) const;

  bool _SingleSim( );

  void _DisplayRemaining( ) const;

public:
  TrafficManager( const Configuration &config, Network *net );
  ~TrafficManager( );

  void Run( );
};

#endif
