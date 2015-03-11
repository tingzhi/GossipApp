/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright 2015 Marco Falke
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
 */

#include <string>

#include "ns3/log.h"

#include "gossip-generator.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("GossipGeneratorApplication");

TypeId
GossipGenerator::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::GossipGenerator")
    .SetParent<Application> ()
    .AddConstructor<GossipGenerator> ()
  ;
  return tid;
}

GossipGenerator::GossipGenerator ()
{
  NS_LOG_FUNCTION (this);
  CurrentValue = 0;
  SentMessages = 0;
  ReceivedData = Seconds(0);
  halt = false;
  gossip_delta_t = Seconds(0.001);
  solicit_delta_t = Seconds(5);
}

GossipGenerator::~GossipGenerator ()
{
  NS_LOG_FUNCTION (this);
}

void
GossipGenerator::DoDispose ( void )
{
  NS_LOG_FUNCTION (this);
  Application::DoDispose ();
}

/*
void
GossipGenerator::SendMessage_debug(Ipv4Address src, Ipv4Address dest, int type)
{
  SendPayload( src,  dest);
}
*/

void
GossipGenerator::AddNeighbor(Ipv4Address own,Ipv4Address neighbor)
{ //TODO duplicates?
  neighbours[0].push_back(own);
  neighbours[1].push_back(neighbor);
  NS_LOG_INFO("Added to neighbors; New size: " << neighbours[0].size());
  /*
  for(uint8_t i=0; i<neighbours[0].size();i++){
    NS_LOG_INFO(neighbours[0].at(i));
    NS_LOG_INFO(" -> " << neighbours[1].at(i));
  }
  */
}

void
GossipGenerator::HandleAck(void)
{
  NS_LOG_INFO("GossipGenerator::HandleAck");
  NS_LOG_INFO(" Time: " << Simulator::Now ().GetSeconds () << "s");

  halt = true;
}

void
GossipGenerator::HandleSolicit(Ipv4Address src,Ipv4Address dest)
{
  NS_LOG_INFO("GossipGenerator::HandleSolicit " << src << " -> " << dest);
  NS_LOG_INFO(" Time: " << Simulator::Now ().GetSeconds () << "s");
  if (CurrentValue != 0)
  {
    SendPayload(dest,src);
  }
}

void
GossipGenerator::HandlePayload(Ipv4Address src,Ipv4Address dest,uint8_t payload_in[])
{
  int payload = (int) payload_in[0];
  int hops = (int) payload_in[1];
  NS_LOG_INFO("GossipGenerator::HandlePayload " << src << " -> " << dest << " Value:" << payload);
  NS_LOG_INFO(" Time: " << Simulator::Now ().GetSeconds () << "s");
  if( payload == CurrentValue)
  {
    SendMessage(dest, src, TYPE_ACK);
  }
  else
  {
    CurrentValue = payload;
    PacketHops = hops;
    ReceivedData = Simulator::Now ();
  }
}

void
GossipGenerator::ChooseRandomNeighbor(Ipv4Address ipv4array[2]){
  NS_LOG_INFO("ChooseRandomNeighbor from " << neighbours[0].size());
  int temp_rnd = rand() % neighbours[0].size(); // Note: Seed, if desired! // TODO may not be random enough.
  ipv4array[0] = neighbours[0].at(temp_rnd);
  ipv4array[1] = neighbours[1].at(temp_rnd);
}

void
GossipGenerator::SendMessage(Ipv4Address src, Ipv4Address dest, int type)
{
  NS_LOG_FUNCTION (this << src << dest << type);
  NS_LOG_INFO("GossipGenerator::SendMessage " << src << " -> " << dest << " Type:" << type);
  NS_LOG_INFO(" Time: " << Simulator::Now ().GetSeconds () << "s");

  SentMessages++;
  Ipv4Header header = Ipv4Header ();
  header.SetDestination (dest);
  header.SetPayloadSize (0);
  header.SetSource (src);
  
  Ptr<Icmpv4L4Protocol> icmp = this->GetNode()->GetObject<Icmpv4L4Protocol>();

  switch(type) {
    case TYPE_ACK : 
      icmp->SendAck(header);
      break;
    case TYPE_SOLICIT :
      icmp->SendRequest(header);
      break;
  }
}

void
GossipGenerator::SendPayload(Ipv4Address src, Ipv4Address dest)
{
  NS_LOG_FUNCTION (this << dest );
  NS_LOG_INFO("GossipGenerator::SendPayload " << src << " -> " << dest << " Value:" << CurrentValue );
  NS_LOG_INFO(" Time: " << Simulator::Now ().GetSeconds () << "s");

  SentMessages++;
  Ipv4Header header = Ipv4Header ();
  header.SetDestination (dest);
  header.SetPayloadSize (0);
  header.SetSource (src);

  uint8_t NewPacketHops = (uint8_t) PacketHops + 1; // TODO no cast!

  uint8_t data[8];
  for (uint8_t j = 0; j < 8; j++)
  {
    data[j] = 0;
  }
  data[0] = (uint8_t) CurrentValue; // ONLY use first 8 bits to store data. // TODO May be extended...  
  data[1] = NewPacketHops;

  Ptr<Icmpv4L4Protocol> icmp = this->GetNode()->GetObject<Icmpv4L4Protocol>(); 
  icmp->SendData(header, data);
}

void
GossipGenerator::SetCurrentValue ( int val )
{
  NS_LOG_FUNCTION (this << val);
  CurrentValue = val;
  PacketHops = 0; // Fresh data -> no hops
  NS_LOG_INFO ("Value of node set to " << CurrentValue);
}

void
GossipGenerator::SetSolicitInterval ( Time val )
{
  NS_LOG_FUNCTION (this << val);
  solicit_delta_t = val;
}

void
GossipGenerator::SetGossipInterval ( Time val )
{
  NS_LOG_FUNCTION (this << val);
  gossip_delta_t = val;
}

int
GossipGenerator::GetCurrentValue ( void )
{
  NS_LOG_FUNCTION (this);
  return CurrentValue;
}

int
GossipGenerator::GetPacketHops ( void )
{
  NS_LOG_FUNCTION (this);
  return PacketHops;
}

int
GossipGenerator::GetSentMessages ( void )
{
  NS_LOG_FUNCTION (this);
  return SentMessages;
}

Time
GossipGenerator::GetReceivedDataTime ( void )
{
  NS_LOG_FUNCTION (this);
  return ReceivedData;
}

void
GossipGenerator::GossipProcess(void)
{
  if(!halt)
  {
    //reschedule
    Simulator::Schedule (gossip_delta_t, &GossipGenerator::GossipProcess,this);
    if (  CurrentValue != 0 )
    {
      Ipv4Address ipv4array[2];
      ChooseRandomNeighbor(ipv4array);
      SendPayload(ipv4array[0],ipv4array[1]);
    }
  }
}

void
GossipGenerator::Solicit(void)
{
  if(CurrentValue == 0)
  {
    //reschedule
    Simulator::Schedule (solicit_delta_t, &GossipGenerator::Solicit,this);
    Ipv4Address ipv4array[2];
    ChooseRandomNeighbor(ipv4array);
    SendMessage(ipv4array[0],ipv4array[1], TYPE_SOLICIT);
  }
}

void
GossipGenerator::StartApplication ( void )
{
  NS_LOG_FUNCTION (this);

  Simulator::Schedule (gossip_delta_t, &GossipGenerator::GossipProcess, this);
  Simulator::Schedule (solicit_delta_t, &GossipGenerator::Solicit, this);
}

void
GossipGenerator::StopApplication ()
{
  NS_LOG_FUNCTION (this);
}

} // Namespace ns3
