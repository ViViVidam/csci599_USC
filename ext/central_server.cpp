#include "central_server.h"
// #include "central_server.h"

#include <iostream>
#include <assert.h>
#include <string.h>

#include "../coresim/event.h"
#include "../coresim/queue.h"
#include "../run/params.h"



CentralServer::CentralServer(
    uint32_t id,
    uint32_t type,
    uint32_t num_hosts,
    uint32_t num_agg_switches,
    uint32_t num_core_switches,
    double bandwidth,
    uint32_t queue_type,
    std::vector<uint32_t> SLO_values
    ): Node(id, type) {

      // empty constructor

}

std::vector<uint32_t> CentralServer::receive_info_from_node(uint32_t node_id) {
    // Implement the logic to receive info from the node and populate 'info' vector
    return info;
}

std::bool CentralServer::send_info_to_node(uint32_t node_id,string qos_class, double qos_latency, uint32_t src_id, uint32_t dst_id) {
    // Implement the logic to send info to the node

    if(qos_class == 'H')
        this->per_node_info[{src_id, dst_id}].QOS_H_fct_curr = qos_latency;
    else if(qos_class == 'M')
        this->per_node_info[{src_id, dst_id}].QOS_M_fct_curr = qos_latency;
    else if(qos_class == 'L')
        this->per_node_info[{src_id, dst_id}].QOS_L_fct_curr = qos_latency;
    
    this->process(src_id, dst_id, qos_class);
    return;
}

std::void CentralServer::process(uint32_t src_id,uint32_t dst_id, string qos_class) {
    // Implement the logic to process the info received from the nodes
    if(this->per_node_info[{src_id, dst_id}]){
      // check if the info is present
      
      //update the values for alpha and beta per each class


    }else{
      // create a struct object and populate the values
    }
}

std::bool CentralServer::process(uint32_t src_id, uint32_t dst_id, string qos_class) {
    // Implement the logic to process the info received from the nodes
    double val = rand();
    if(qos_class == 'H' && val <= this->per_node_info[{src_id, dst_id}].admit_prob_H){
        return true;
    }
    else{
        return false;
    }

    if(qos_class == 'M' && val <= this->per_node_info[{src_id, dst_id}].admit_prob_M){
        return true;
    }else{
        return false;
    }
}

// AequitasFlow::AequitasFlow(uint32_t id, double start_time, uint32_t size, Host *s, Host *d,
//     uint32_t flow_priority) : Flow(id, start_time, size, s, d, flow_priority) {}