/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <typeinfo>

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
#include "ns3/ipv4-global-routing-helper.h"

#include "ns3/gossip-generator.h"
#include "ns3/gossip-generator-helper.h"

using namespace std;
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("GenericTopologyCreation");

Ptr<GossipGenerator> GetGossipApp(Ptr <Node> node)
{
  Ptr< Application > gossipApp = node->GetApplication (0) ;
  return DynamicCast<GossipGenerator>(gossipApp);
}

int main (int argc, char *argv[])
{
  Time::SetResolution (Time::NS);
  LogComponentEnable ("GossipGeneratorApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("GenericTopologyCreation", LOG_LEVEL_INFO);
  LogComponentEnable ("Icmpv4L4Protocol"       , LOG_LEVEL_INFO);

  std::string LinkRate ("100Mbps");
  std::string LinkDelay ("2ms");
  int n_nodes = 3;

  NS_LOG_INFO ("Create Nodes.");

  NodeContainer nodes;   // Declare nodes objects
  nodes.Create (n_nodes);

  NS_LOG_INFO ("Create P2P Link Attributes.");

  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue (LinkRate));
  p2p.SetChannelAttribute ("Delay", StringValue (LinkDelay));

  NS_LOG_INFO ("Install Internet Stack to Nodes.");

  InternetStackHelper internet;
  internet.Install (nodes);

  NS_LOG_INFO ("Assign Addresses to Nodes.");

  Ipv4AddressHelper ipv4_n;
  ipv4_n.SetBase ("10.0.0.0", "255.255.255.252");

  NS_LOG_INFO ("Create Links Between Nodes.");

  // uint32_t linkCount = 0;

  NodeContainer n_links1 = NodeContainer (nodes.Get (0), nodes.Get(1));
  NetDeviceContainer n_devs1 = p2p.Install (n_links1);
  Ipv4InterfaceContainer interface0 = ipv4_n.Assign (n_devs1);
  ipv4_n.NewNetwork ();
  //linkCount++;

  NodeContainer n_links2 = NodeContainer (nodes.Get (1), nodes.Get(2));
  NetDeviceContainer n_devs2 = p2p.Install (n_links2);
  Ipv4InterfaceContainer interface1 = ipv4_n.Assign (n_devs2);
  ipv4_n.NewNetwork ();
  //linkCount++;

  NS_LOG_INFO ("Initialize Global Routing.");
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install ( nodes.Get (0) );
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (interface0.GetAddress (0), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (2));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  /* */
  NodeContainer nodes2;
  int NodeNumber, Edge1, Edge2;
  bool ParseNodes = false;
  bool ParseEdges = false;
  string Line;
  string NodePrefix = "#Nodes";
  string EdgePrefix = "#Edges";
  ifstream Topology;
  Topology.open ("scratch/TopologyFile");
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

      GossipGeneratorHelper ggh ;
      ApplicationContainer nodeApps = ggh.Install(nodes2);
      nodeApps.Start(Seconds(0.0));
      nodeApps.Stop(Seconds(10.0));

      NS_LOG_INFO ("Assign Addresses to Nodes and Create Links Between Nodes.");
      ParseEdges = true;
      continue;
    }
    if (ParseNodes) {
      istringstream(Line) >> NodeNumber; // Just update til end
      NodeNumber++; // convert index -> length
    }
    if (ParseEdges) { // Format: (12, 14)
      istringstream(Line.substr(1)) >> Edge1;
      istringstream(Line.substr(1+Line.find(","))) >> Edge2;

      NS_LOG_INFO("Parsed Edge: (" << Edge1 << ", " << Edge2 << ")");

      Ipv4InterfaceContainer InterfaceCont = ipv4_n.Assign (p2p.Install (NodeContainer (nodes2.Get (Edge1), nodes2.Get(Edge2))));
      ipv4_n.NewNetwork ();
      GetGossipApp(nodes2.Get(Edge1))->AddNeighbor(InterfaceCont.GetAddress(0),InterfaceCont.GetAddress(1));
      GetGossipApp(nodes2.Get(Edge2))->AddNeighbor(InterfaceCont.GetAddress(1),InterfaceCont.GetAddress(0));

//  	NS_LOG_INFO ("Initialize Global Routing.");
//  	Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

    }
  }
  Topology.close();
  Ptr<GossipGenerator> a = GetGossipApp(nodes2.Get(0));
  a->SetCurrentValue( 1 );
  //a->SendMessage_debug( src,dest, TYPE_ACK );
  /* */

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}


