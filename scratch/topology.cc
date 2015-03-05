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
  LogComponentEnable ("GossipGeneratorApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("GenericTopologyCreation", LOG_LEVEL_INFO);
  LogComponentEnable ("Icmpv4L4Protocol", LOG_LEVEL_INFO);

  std::string LinkRate ("100Mbps");
  std::string LinkDelay ("2ms");
  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue (LinkRate));
  p2p.SetChannelAttribute ("Delay", StringValue (LinkDelay));

  InternetStackHelper internet;

  Ipv4AddressHelper ipv4_n;
  ipv4_n.SetBase ("10.0.0.0", "255.255.255.252");

  NodeContainer nodes2;

  GossipGeneratorHelper ggh ;
  ApplicationContainer nodeApps; // TODO unused

  /* Parser Code - Begin */
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

  Ptr<GossipGenerator> a = GetGossipApp(nodes2.Get(0));
  a->SetCurrentValue( 2 );
  //a->SendMessage_debug( src,dest, TYPE_ACK );

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}


