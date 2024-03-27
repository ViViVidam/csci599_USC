#ifndef CENTRAL_SERVER_H
#define CENTRAL_SERVER_H

#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <tuple>

// maintain the information for nodes
std::vector<std::tuple<double, double, double>> node_info_list;

// listen to all nodes (by registering or pre-configure)
// collect and maintain all the information
/* Determine what the information should be, such as Alaph and Beta, OTHERS? 
   For now, if it is to just run the basic Aequitas Algo on server to update Probs, then codes from simulator can be reused */

void listen(int port);
void sendMessage(const char* serverIp, int port, const char* message);

// convert the received message into correct format
void convert_format();
void add_to_list(std::tuple<double, double, double> info, std::vector<std::tuple<double, double, double>> node_info_list);

// run Aequitas algorithm, modify the node_info tuple to contain more information
void run_Aequitas(std::tuple<double, double, double> node_info);






// For the week 3/25-3/31, the work below could be ignored and leaves it to YZ ZHONG only. The priority work is the ones above
/*  TODO: RPC latencies could be obtained from the Aequitas codes. 
    Since it's a responsive method, Network Load should be obtained by our own code, see the following:
    Premise: with out the influence of Aequitas (don't use it)
    STEPs: 1. Based on the packet size and sending rate, actively probe the network to find its maximum bandwidth (remember the optimal operating point)
           2. After reaching the stable point, update the bandwidth to the server
           3. Keep probing to detect any bandwidth change
    
    Possible Alternative: Use Aequitas, get the bandwidth based on all QoS's rate (but what is the reasoning behind this?) */


#endif



