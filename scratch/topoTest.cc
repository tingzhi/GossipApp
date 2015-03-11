/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
 /*
 * Copyright (c) 2015 Marco Falke, Jinming Mu, and Tingzhi Li
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Marco Falke
           Jinming Mu
           Tingzhi Li <vincentltz at gmail dot com>
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <typeinfo>
#include <time.h>
#include <unistd.h>

#include "ns3/object.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/global-route-manager.h"
#include "ns3/mobility-module.h"
#include "ns3/netanim-module.h"
#include "ns3/assert.h"

#include "ns3/gossip-generator.h"
#include "ns3/gossip-generator-helper.h"

using namespace std;
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("GenericTopologyCreation");

class simstats {
    double time;
    int hops;
    double avgMsg;
  public:
    simstats(double, int, double);
    int getHops(void);
    double getTime(void);
    double getAvgMsgs(void);
}; 

simstats::simstats (double t, int h, double a) {
  time = t;
  hops = h;
  avgMsg = a;
}

int simstats::getHops(void){
  return hops;
}

double simstats::getTime(void){
  return time;
}

double simstats::getAvgMsgs(void){
  return avgMsg;
}

Ptr<GossipGenerator> GetGossipApp(Ptr <Node> node)
{
  Ptr< Application > gossipApp = node->GetApplication (0) ;
  return DynamicCast<GossipGenerator>(gossipApp);
}

simstats simulation (char *filename)
{
  NS_LOG_INFO ("Filename : " << filename << " to read from for  GossipGenerator");

  std::string LinkRate ("100Mbps"); // 10kbps. Need to change to wifi channel
  std::string LinkDelay ("2ms");
  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue (LinkRate));
  p2p.SetChannelAttribute ("Delay", StringValue (LinkDelay));

  InternetStackHelper internet;

  Ipv4AddressHelper ipv4_n;
  ipv4_n.SetBase ("10.0.0.0", "255.255.255.252"); //Netmask setting?

  NodeContainer nodes2;

  GossipGeneratorHelper ggh ;
  Time GossipInterval = Seconds(.005); // Must be larger than the round-trip-time! (c.f. LinkDelay)
  Time SolicitInterval = Seconds(5); // Should be larger than the GossipInterval!
  ApplicationContainer nodeApps; // TODO unused

  /* Parser Code - Begin */
  int NodeNumber, Edge1, Edge2;
  bool ParseNodes = false;
  bool ParseEdges = false;
  string Line;
  string NodePrefix = "#Nodes";
  string EdgePrefix = "#Edges";
  ifstream Topology;

  Topology.open (filename);
  while(getline(Topology,Line))
  {
    if (!Line.compare(0, NodePrefix.size(), NodePrefix)) {
      ParseNodes = true;
      continue;
    }
    if (!Line.compare(0, EdgePrefix.size(), EdgePrefix)) {
      ParseNodes = false;
      NS_LOG_INFO ("Create " << NodeNumber << " nodes to test GossipGenerator");
      nodes2.Create(NodeNumber);

      NS_LOG_INFO ("Install Internet Stack and GossipGenerator to those nodes.");
      internet.Install (nodes2);
      nodeApps = ggh.Install(nodes2);

      NS_LOG_INFO ("Assign Addresses to Nodes and Create Links Between Nodes...");
      ParseEdges = true;
      continue;
    }
    if (ParseNodes) {
      istringstream(Line) >> NodeNumber; // Just update til end
      NodeNumber++; // convert max_index -> length
    }
    if (ParseEdges) { // Format: (12, 14)
      istringstream(Line.substr(1)) >> Edge1;
      istringstream(Line.substr(1+Line.find(","))) >> Edge2;

      NS_LOG_INFO("Parsed Edge: (" << Edge1 << ", " << Edge2 << ")");

      Ipv4InterfaceContainer InterfaceCont = ipv4_n.Assign (p2p.Install (NodeContainer (nodes2.Get (Edge1), nodes2.Get(Edge2))));
      ipv4_n.NewNetwork ();
      GetGossipApp(nodes2.Get(Edge1))->AddNeighbor(InterfaceCont.GetAddress(0),InterfaceCont.GetAddress(1));
      GetGossipApp(nodes2.Get(Edge2))->AddNeighbor(InterfaceCont.GetAddress(1),InterfaceCont.GetAddress(0));
    }
  }
  Topology.close();
  /* Parser Code - End */

  for ( int i=0; i<NodeNumber;++i)
  { //TODO use attributes
    Ptr<GossipGenerator> ii = GetGossipApp(nodes2.Get(i));
    ii->SetGossipInterval(GossipInterval);
    ii->SetSolicitInterval(SolicitInterval);
  }

  Ptr<GossipGenerator> a = GetGossipApp(nodes2.Get(0));
  a->SetCurrentValue( 2 );
  //a->SendMessage_debug( src,dest, TYPE_ACK );

  Simulator::Run ();

  NS_LOG_INFO(endl << " ---- Print results ---" << endl);
  int MaxHops = 0;
  Time MaxTime = Seconds(0);
  double AvgMessagesPerNode = 0;
  for ( int i=0; i<NodeNumber;++i)
  {
    Ptr<GossipGenerator> ii = GetGossipApp(nodes2.Get(i));
    if (MaxHops < ii->GetPacketHops()){
      MaxHops = ii->GetPacketHops();
    }
    if (MaxTime.Compare(ii->GetReceivedDataTime()) == -1){
      MaxTime = ii->GetReceivedDataTime();
    }
    AvgMessagesPerNode += ii->GetSentMessages();
    NS_LOG_INFO("Node " << i << ": ");
    NS_LOG_INFO(" * Sent icmp messages   : " << ii->GetSentMessages());
    NS_LOG_INFO(" * Hops of data message : " << ii->GetPacketHops());
    NS_LOG_INFO(" * Time of data received: " << ii->GetReceivedDataTime().GetSeconds() << "s");
  }
  AvgMessagesPerNode /= NodeNumber;
  NS_LOG_INFO(endl << "Simulation terminated after " << Simulator::Now().GetSeconds() << "s");
  NS_LOG_INFO("Max hops: " << MaxHops);
  NS_LOG_INFO("Average amount of sent messages per node: " << AvgMessagesPerNode);
  NS_LOG_INFO("Time until information was spread: " << MaxTime.GetSeconds() << "s" << endl);
  simstats ret(MaxTime.GetSeconds(),MaxHops,AvgMessagesPerNode);
  Simulator::Destroy ();
  return ret;
}

/* 

  function converts std::string types into char * types
  @param string
  @return char *

*/
char* convertStrToChar(std::string str){
  char *newChars = new char[str.size()+1];
  newChars[str.size()]='\0';
  memcpy(newChars,str.c_str(),str.size());
  return newChars;
}

int main(int argc, char *argv[]) {
  // LogComponentEnable ("GossipGeneratorApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("GenericTopologyCreation", LOG_LEVEL_INFO);
  // LogComponentEnable ("Icmpv4L4Protocol", LOG_LEVEL_INFO);

  // srand(time(NULL));

  //These are the default values for the command line
  // if you do not override them via n3 command lines, this is what will run
  std::string inTopologyFile = "scratch/TopologyFile"; //default filenames
  std::string outHopsFile = "scratch/maxhops.txt";
  std::string outTimeFile = "scratch/maxtime.txt";
  std::string outAvgMsgFile = "scratch/avgmsg.txt";

  //This lets you add command line arguments in n3 
  // to view all command line arguments when you run the program type:
  // ./waf --run "topoTest --help"
  //
  // it will run on the defaults provided above if no command line arguments are given
  //
  // otherwise type in :
  // ./waf --run "topoTest --infile=scratch/TopologyFile --outHopsFile=scratch/maxhops.txt --outTimeFile=scratch/maxtime.txt --outAvgMsgFile=scratch/avgmsg.txt"
  // changing the file names to what you want them to be
  // be sure to include the path!
  CommandLine cmd;
  cmd.AddValue("infile", "Topology file read in", inTopologyFile);
  cmd.AddValue("outHopsFile", "Filename that max hops are written out to", outHopsFile);
  cmd.AddValue("outTimeFile", "Filename that max hops are written out to", outTimeFile);
  cmd.AddValue("outAvgMsgFile", "Filename that avg number of messages per node are written out to", outAvgMsgFile);
  cmd.Parse (argc, argv);

  //All of the functions take char * not strings
  //but n3 doesnt have command line entries for char * so they need to be converted from string to char *
  char *newTopoFile = convertStrToChar(inTopologyFile);
  char *newHopsFile = convertStrToChar(outHopsFile);
  char *newTimeFile = convertStrToChar(outTimeFile);
  char *newAvgMsgFile = convertStrToChar(outAvgMsgFile);

  //makes sure nothing weird happened in conversion. comment out or delete later
  std::cout << "Filenames chars *:" << newTopoFile << " " << newHopsFile << " " << newTimeFile << " " << newAvgMsgFile << endl;

  FILE *timefile;
  FILE *hopfile;
  FILE *avgfile;
  timefile = fopen(newTimeFile, "a+");
  hopfile = fopen(newHopsFile, "a+");
  avgfile = fopen(newAvgMsgFile, "a+");

  if (timefile != NULL && hopfile != NULL && avgfile != NULL){
    for (int i = 0; i < 1; i++){
      simstats results = simulation(newTopoFile);
      fprintf(timefile,"%f\n", results.getTime());
      fprintf(hopfile,"%d\n", results.getHops());
      fprintf(avgfile,"%f\n", results.getAvgMsgs());
      sleep(1);
    }
  }
  fclose(timefile);
  fclose(hopfile);
  fclose(avgfile);
  return 0;
  
} 
