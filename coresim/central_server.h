#ifndef CENTRAL_SERVER_H
#define CENTRAL_SERVER_H

#include <cstdint>
#include <vector>
#include <queue>
#include <map>
class AggChannel;
class Topology;
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

class CentralServer{
protected:
    std::vector<double> SLO_values; // per QOS class SLO latency
    std::vector<std::map<std::pair<uint32_t, uint32_t>, AggChannel *>> channels;
    std::vector<std::vector<double>> channels_PL;
    uint32_t use_endp_ctl = 0;
    uint32_t count_channel = 0;
    double alpha = 0.8;
    double beta = 0.2;
public:
    /*
      central server needs to know about all the data in the setup
      like hosts, switches, queues, bandwidth etc
    */

    CentralServer(uint32_t id, int type, uint32_t num_hosts, uint32_t num_agg_switches, uint32_t num_core_switches,
                  double bandwidth, uint32_t queue_type, std::vector <double> SLO_values, Topology* topology);

    bool receive_info_from_central_node(uint32_t src_id, uint32_t dst_id, int qos_class);

    void send_info_to_central(uint32_t flow_id,int priority, double PL,double NL, uint32_t src_id, uint32_t dst_id,int flow_size);

    void process(uint32_t src_id,uint32_t dst_id, int qos_class,double qos_latency, double time);

};

#endif