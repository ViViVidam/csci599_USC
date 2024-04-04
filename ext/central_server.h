#ifdef CENTRAL_SERVER_H
#define CENTRAL_SERVER_H

#include <cstdint>
#include <vector>
#include <queue>
#include <map>

class Flow;
#include "../coresim/node.h"

/* this class might inherit host as well depending on the requirement*/
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
      uint32_t id,
      uint32_t type
      uint32_t num_hosts,
      uint32_t num_agg_switches,
      uint32_t num_core_switches,
      double bandwidth,
      uint32_t queue_type,
      std::vector<uint32_t> SLO_values);
    
    std:: vector<uint32_t> receive_info_from_node(uint32_t node_id);
    std:: bool send_info_to_node(uint32_t node_id);
    std:: process();

};

#endif