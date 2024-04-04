#ifdef CENTRAL_SERVER_H
#define CENTRAL_SERVER_H

#include <cstdint>
#include <vector>
#include <queue>
#include <map>

class Flow;
#include "../coresim/node.h"

/* this class might inherit host as well depending on the requirement*/

std::Struct PerSrcDestDetails{
  std::double QOS_H_fct_curr;
  std::double QOS_M_fct_curr;
  std::double QOS_L_fct_curr;
  
  // default values for alpha and beta will be given.
  std::double alpha_H, beta_H;
  std::double alpha_M, beta_M;
  std::double alpha_L, beta_L;

  std:: double admit_prob_H;
  std:: double admit_prob_M;
  std:: double admit_prob_L;

  std::uint32_t num_downgrades = 0 ;
  std:: Time last
  std:: double incremen_window;

};

class CentralServer:public Node{
  /*
    the central server is responsible for the following tasks:
     1 - SLO's for each Qos class
     2 - failure modes handled by switch to switch basis
     3 - 
  */


  public:
    std:: map<uint32_t, double> SLO; // per QOS class SLO latency
    std:: map<unit32_t, queue<int>> receive_queue; // per node a queue is maintained in the central server
    std:: map<uint32_t, queue<int>> send_queue; // per node the failure status is maintained

    /*
      central server needs to know about all the data in the setup
      like hosts, switches, queues, bandwidth etc
    */

    CentralServer(
      std::uint32_t id,
      std::uint32_t type,
      std::uint32_t num_hosts,
      std::uint32_t num_agg_switches,
      std::uint32_t num_core_switches,
      std::double bandwidth,
      std::uint32_t queue_type,
      // 0 - high latency, 1 - medium latency, 2 - low latency
      std::vector<uint32_t> SLO_values);
      std::map<pair<uint32_t, uint32_t>, PerNodeDetails> per_node_info; 

    
    std::bool receive_info_from_node(uint32_t node_id);
    std::void send_info_to_node(uint32_t node_id, string, qos_class, double qos_latency,double time);
    std::process();

};

#endif