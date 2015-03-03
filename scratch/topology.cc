#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <typeinfo>

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
        int number_nodes_2 = 5;
        NS_LOG_INFO ("Create " << number_nodes_2 << " nodes to test GossipGenerator");
        NodeContainer nodes2;
        nodes2.Create(number_nodes_2);

        NS_LOG_INFO ("Install Internet Stack and GossipGenerator to those nodes.");
        internet.Install (nodes2);

        GossipGeneratorHelper ggh ;
        ApplicationContainer nodeApps = ggh.Install(nodes2);
        nodeApps.Start(Seconds(0.0));
        nodeApps.Stop(Seconds(10.0));

        NS_LOG_INFO ("Assign Addresses to Nodes and Create Links Between Nodes.");
	Ipv4InterfaceContainer interfaceA = ipv4_n.Assign (p2p.Install (NodeContainer (nodes2.Get (0), nodes2.Get(1))));
	ipv4_n.NewNetwork ();

	Ipv4InterfaceContainer interfaceB = ipv4_n.Assign (p2p.Install (NodeContainer (nodes2.Get (1), nodes2.Get(2))));
	ipv4_n.NewNetwork ();

//  	NS_LOG_INFO ("Initialize Global Routing.");
//  	Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

        Ptr<Application> ApplicationOne = nodeApps.Get(0);
        Ptr<Application> *testApplication =&ApplicationOne;

        Ptr<GossipGenerator>*  PtrOneGossipApp =(Ptr<GossipGenerator>*) testApplication;
        GossipGenerator OneGossipApp = PtrOneGossipApp->operator*();
        OneGossipApp.SetCurrentValue( 1 );

        OneGossipApp.SendMessage_debug( interfaceA.GetAddress(0), interfaceA.GetAddress(1), TYPE_ACK );
        /* */

        Simulator::Run ();
        Simulator::Destroy ();

	return 0;
}
