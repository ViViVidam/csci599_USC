#ifndef CENTRAL_SERVER_H
#define CENTRAL_SERVER_H

#include <cstdint>
#include <vector>
#include <queue>
#include <map>
#include "node.h"
#include "agg_channel.h"

class Flow;
/* this class might inherit host as well depending on the requirement*/

struct PerSrcDestDetails{
  double QOS_H_fct_curr;
  double QOS_M_fct_curr;
  double QOS_L_fct_curr;
  
  // default values for alpha and beta will be given.

  double admit_prob_H;
  double admit_prob_M;
  double admit_prob_L;

  uint32_t num_downgrades = 0 ;
  double last;

};

class CentralServer:public Node {
protected:
    std::vector<double> SLO_values; // per QOS class SLO latency
    std::map <uint32_t, std::queue<int>> receive_queue; // per node a queue is maintained in the central server
    std::map <uint32_t, std::queue<int>> send_queue; // per node the failure status is maintained
    std::vector<std::map<std::pair<uint32_t, uint32_t>, AggChannel *>> channels;
    uint32_t count_channel;
    double increment_window;
    double dp_alpha, dp_beta;
public:
    /*
      central server needs to know about all the data in the setup
      like hosts, switches, queues, bandwidth etc
    */

    CentralServer(uint32_t id, int type,
            uint32_t num_hosts,
            uint32_t num_agg_switches,
            uint32_t num_core_switches,
            double bandwidth,
            uint32_t queue_type,
            // 0 - high latency, 1 - medium latency, 2 - low latency
            std::vector <double> slo_val);

    std::map <std::pair<uint32_t, uint32_t>, PerSrcDestDetails> per_node_info;


    bool receive_info_from_central_node(uint32_t src_id, uint32_t dst_id, int qos_class);

    void send_info_to_central(uint32_t flow_id,int priority, double qos_latency, uint32_t src_id, uint32_t dst_id,int flow_size);

    void process(uint32_t src_id,uint32_t dst_id, int qos_class,double qos_latency, double time);

};

#endif