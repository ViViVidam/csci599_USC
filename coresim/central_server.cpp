#include "central_server.h"
// #include "central_server.h"

#include <iostream>
#include <assert.h>
#include <string.h>

#include "event.h"
#include "queue.h"
#include "../run/params.h"



CentralServer::CentralServer(uint32_t id, int type,
        uint32_t num_hosts,
        uint32_t num_agg_switches,
        uint32_t num_core_switches,
        double bandwidth,
        uint32_t queue_type,
        std::vector <double> slo_val): Node(id, type) {
          this->SLO_values = slo_val;
}

void CentralServer::send_info_to_central_node(uint32_t node_id,int qos_class, double qos_latency, uint32_t src_id, uint32_t dst_id, double time) {
    // Implement the logic to send info to the node

    if(qos_class == 'H')
        this->per_node_info[{src_id, dst_id}].QOS_H_fct_curr = qos_latency;
    else if(qos_class == 'M')
        this->per_node_info[{src_id, dst_id}].QOS_M_fct_curr = qos_latency;
    else if(qos_class == 'L')
        this->per_node_info[{src_id, dst_id}].QOS_L_fct_curr = qos_latency;
    
    this->process(src_id, dst_id, qos_class, qos_latency, time);
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