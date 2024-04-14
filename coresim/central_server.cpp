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
        double bandwidth, uint32_t queue_type, std::vector <double> SLO_values, Topology* topology, uint32_t failure_init_val) {
    this->channels.resize(params.weights.size());
    this->SLO_values = std::vector<double>(SLO_values);
    this->failure_count = failure_init_val;
    for (uint32_t i = 0; i < params.weights.size(); i++) {
        for (uint32_t j = 0; j < params.num_hosts; j++) {
            for (uint32_t k = 0; k < params.num_hosts; k++) {
                if (j != k) {
                    //auto src_dst_pair = std::make_pair(topology->hosts[j], topology->hosts[k]);
                    auto src_dst_pair = std::make_pair(j, k);
                    this->channels[i][src_dst_pair] = new AggChannel(count_channel, topology->hosts[j], topology->hosts[k], i);
                    //std::cout << "creating channel[" << count_channel << "], src: " << topology->hosts[j]->id << ", dst: " << topology->hosts[k]->id << ", prio: " << i  <<std::endl;
                    this->count_channel++;
                }
            }
        }
    }

    std::cout << "DIVI: CentralServer created count of channels is " << this->count_channel << std::endl;
    // empty constructor
}

void CentralServer::send_info_to_central(uint32_t flow_id,int priority, double qos_latency, uint32_t src_id, uint32_t dst_id,int flow_size) {
    // Implement the logic to send info to the node
    //std::cout << "send_info_to_central" << std::endl;
    //std::cout << "send info to central" << std::endl;
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
//true: send at same priority
bool CentralServer::receive_info_from_central_node(uint32_t src_id, uint32_t dst_id, int qos_class) {
    // Implement the logic to process the info received from the nodes
    // std::cout << "receive_info_from_central_node" << std::endl;
    double val = (double)rand() / (RAND_MAX);

    if(val <= this->channels[qos_class][{src_id, dst_id}]->admit_prob) {
        return true;
    }
    else{
        return false;
    }
}

bool CentralServer::conn_to_central_node(){
    if(!params.test_failure){
        return true;
    }
    double val = (double)rand() / (RAND_MAX);
    if(val < 0.01){
        this->failure_count++;
        return false;
    }
    return true;
}

std::uint32_t CentralServer::get_failure_count(){
    return this->failure_count;
}