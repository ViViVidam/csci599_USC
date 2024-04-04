#include "central_server.h"
// #include "central_server.h"

#include <iostream>
#include <assert.h>


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

std::bool CentralServer::send_info_to_node(uint32_t node_id) {
    // Implement the logic to send info to the node
    return true;
}

std::void CentralServer::process() {
    // Implement the logic to process the info received from the nodes
}

// AequitasFlow::AequitasFlow(uint32_t id, double start_time, uint32_t size, Host *s, Host *d,
//     uint32_t flow_priority) : Flow(id, start_time, size, s, d, flow_priority) {}