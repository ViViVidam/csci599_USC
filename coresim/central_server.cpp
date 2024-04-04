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
        double bandwidth, uint32_t queue_type, std::vector <double> SLO_values, Topology* topology): Node(id, type) {
    this->SLO_values = std::vector<double>(SLO_values);
    for (uint32_t i = 0; i < params.weights.size(); i++) {
        for (uint32_t j = 0; j < params.num_hosts - 1; j++) {
            auto src_dst_pair = std::make_pair(j, params.num_hosts - 1);
            this->channels[i][src_dst_pair] = new AggChannel(count_channel, topology->hosts[j], topology->hosts[params.num_hosts - 1], i);
            //std::cout << "creating channel[" << count_channel << "], src: " << topology->hosts[j]->id << ", dst: " << topology->hosts[params.num_hosts - 1]->id << ", prio: " << i  <<std::endl;
            this->count_channel++;
            // set agg_channel to NICs
        }
    }
    // empty constructor
}

void CentralServer::send_info_to_central(uint32_t flow_id,int priority, double qos_latency, uint32_t src_id, uint32_t dst_id,int flow_size) {
    // Implement the logic to send info to the node

    if(priority == 0)
        this->per_node_info[{src_id, dst_id}].QOS_H_fct_curr = qos_latency;
    else if(priority == 1)
        this->per_node_info[{src_id, dst_id}].QOS_M_fct_curr = qos_latency;
    else if(priority == 2)
        this->per_node_info[{src_id, dst_id}].QOS_L_fct_curr = qos_latency;

    AggChannel* agg_channel = this->channels[priority][std::make_pair(src_id,dst_id)];
    agg_channel->process_latency_signal(qos_latency,flow_id,flow_size);
    //this->process(src_id, dst_id, qos_class, qos_latency, time);
    return;
}

void CentralServer::process(uint32_t src_id,uint32_t dst_id, int qos_class,double qos_latency, double time) {
    // Implement the logic to process the info received from the nodes
    if(this->per_node_info.count({src_id, dst_id}) > 0){
      // check if the info is present
      //update the values for alpha and beta per each class
      
      if(qos_class == 1){
        if(qos_latency < this->SLO_values[0]){
          if(time - this->increment_window){
            double admit_prob_h = this->per_node_info[{src_id, dst_id}].admit_prob_H;
            admit_prob_h = std::min(admit_prob_h+this->dp_alpha, 1.0);
            this->per_node_info[{src_id, dst_id}].admit_prob_H = admit_prob_h;
            this->per_node_info[{src_id, dst_id}].last = time;
          }
        }else{
            double admit_prob_h = this->per_node_info[{src_id, dst_id}].admit_prob_H;
            admit_prob_h = std::max(admit_prob_h-dp_beta, 0.1);
        }
      }
      else if(qos_class == 2){
        if(qos_latency < this->SLO_values[1]){
          if(time - this->per_node_info[{src_id, dst_id}].last> this->increment_window){
            double admit_prob_m = this->per_node_info[{src_id, dst_id}].admit_prob_M;
            admit_prob_m = std::min(admit_prob_m+dp_alpha, 1.0);
            this->per_node_info[{src_id, dst_id}].admit_prob_M = admit_prob_m;
            this->per_node_info[{src_id, dst_id}].last = time;
          }
        }else{
            double admit_prob_m = this->per_node_info[{src_id, dst_id}].admit_prob_M;
            admit_prob_m = std::max(admit_prob_m-dp_beta, 0.1);
        }
      }
    }else{
      // create a struct object and populate the values
      
    }
}

bool CentralServer::receive_info_from_central_node(uint32_t src_id, uint32_t dst_id, int qos_class) {
    // Implement the logic to process the info received from the nodes
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
    if(qos_class < (params.weights.size() - 1) && val <= this->channels[qos_class][{src_id, dst_id}]->admit_prob) {
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