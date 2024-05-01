#include "central_server.h"
// #include "central_server.h"

#include <iostream>
#include <assert.h>
#include <string.h>
#include "event.h"
#include "queue.h"
#include "topology.h"
#include "agg_channel.h"
#include "../run/params.h"
#include "../ext/factory.h"

extern DCExpParams params;
CentralServer::CentralServer(uint32_t id, int type, uint32_t num_hosts, uint32_t num_agg_switches, uint32_t num_core_switches,
        double bandwidth, uint32_t queue_type, std::vector <double> SLO_values, Topology* topology):use_endp_ctl(params.use_recend_ctl) {
    this->channels.resize(params.weights.size());
    this->channels_PL.resize(params.weights.size());
    this->SLO_values = std::vector<double>(SLO_values);
    for (uint32_t i = 0; i < params.weights.size(); i++) {
        for (uint32_t j = 0; j < params.num_hosts; j++) {
            for (uint32_t k = 0; k < params.num_hosts; k++) {
                if (j != k) {
                    //auto src_dst_pair = std::make_pair(topology->hosts[j], topology->hosts[k]);
                    auto src_dst_pair = std::make_pair(j, k);
                    this->channels[i][src_dst_pair] = new AggChannel(count_channel, topology->hosts[j],
                                                                     topology->hosts[k], i);
                    //std::cout << "creating channel[" << count_channel << "], src: " << topology->hosts[j]->id << ", dst: " << topology->hosts[k]->id << ", prio: " << i  <<std::endl;
                    this->count_channel++;
                }
            }
        }
    }
    for(auto i = 0; i < this->channels_PL.size(); i++){
        this->channels_PL[i].resize(params.num_hosts,0);
    }
    // empty constructor
}

void CentralServer::send_info_to_central(uint32_t flow_id,int priority, double PL,double NL, uint32_t src_id, uint32_t dst_id,int flow_size) {
    // Implement the logic to send info to the node
    AggChannel *agg_channel = this->channels[priority][std::make_pair(src_id, dst_id)];
    if(this->use_endp_ctl) {
        // if not enable use endp, PL will also be zero and NL = RNL
        // if not set up PL yet, set it up
        if (this->channels_PL[priority][dst_id] == 0) this->channels_PL[priority][dst_id] = PL;
        else this->channels_PL[priority][dst_id] += this->alpha * this->channels_PL[priority][dst_id] + this->beta * PL;
    }
    agg_channel->process_latency_signal(this->channels_PL[priority][dst_id] + NL, flow_id, flow_size);

    //this->process(src_id, dst_id, qos_class, qos_latency, time);
    return;
}

//true: send at same priority
bool CentralServer::receive_info_from_central_node(uint32_t src_id, uint32_t dst_id, int qos_class) {
    // Implement the logic to process the info received from the nodes
    // std::cout << "receive_info_from_central_node" << std::endl;
    double val = (double)rand() / (RAND_MAX);
    // if ((double)rand() / (RAND_MAX) > admit_prob) {
    //             run_priority = params.weights.size() - 1;
    //             num_downgrades++;
    //             num_downgrades_per_host[src->id]++;
    //             if (flow_priority == 0) {
    //                 per_host_QoS_H_downgrades[src->id]++;
    //                 num_qos_h_downgrades[1]++;
    //                 per_pctl_downgrades[0]++;
    //             } else {
    //                 num_qos_m_downgrades++;
    //                 per_pctl_downgrades[1]++;
    //             }
    //  }

    if(val <= this->channels[qos_class][{src_id, dst_id}]->admit_prob) {
        return true;
    }
    else{
        return false;
    }
}