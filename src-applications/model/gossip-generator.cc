/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright 2015 Oregon State University
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
// #include <thread>

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

void
GossipGenerator::SendMessage_debug(Ipv4Address src, Ipv4Address dest, int type)
{
  SendPayload( src,  dest);
}

void
GossipGenerator::AddNeighbor(Ipv4Address own,Ipv4Address neighbor)
{ //TODO duplicates?
  neighbours[0].push_back(own);
  neighbours[1].push_back(neighbor);
  NS_LOG_INFO("Added to neighbors; New size: " << neighbours[0].size());
}

void
GossipGenerator::HandleAck(void)
{
  NS_LOG_INFO("GossipGenerator::HandleAck");
  halt = true;
}

void
GossipGenerator::HandleSolicit(Ipv4Address src,Ipv4Address dest)
{
  NS_LOG_INFO("GossipGenerator::HandleSolicit " << src << " -> " << dest);
  SendPayload(dest,src);
}

void
GossipGenerator::HandlePayload(Ipv4Address src,Ipv4Address dest,uint8_t payload_in[])
{
  int payload = (int) payload_in[0];
  NS_LOG_INFO("GossipGenerator::HandlePayload " << src << " -> " << dest << " Value:" << payload);
  if( payload == CurrentValue)
  {
    SendMessage(dest, src, TYPE_ACK);
  }
  else
  {
    CurrentValue = payload;
  }
}

void
GossipGenerator::ChooseRandomNeighbor(Ipv4Address ipv4array[2]){
  NS_LOG_INFO("ChooseRandomNeighbor from " << neighbours[0].size());
  int temp_rnd = rand() % neighbours[0].size();
  ipv4array[0] = neighbours[0].at(temp_rnd);
  ipv4array[1] = neighbours[1].at(temp_rnd);
}

void
GossipGenerator::SendMessage(Ipv4Address src, Ipv4Address dest, int type)
{
  NS_LOG_FUNCTION (this << src << dest << type);
  NS_LOG_INFO("GossipGenerator::SendMessage " << src << " -> " << dest << " Type:" << type);

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

  Ipv4Header header = Ipv4Header ();
  header.SetDestination (dest);
  header.SetPayloadSize (0);
  header.SetSource (src);

  uint8_t data[8];
  for (uint8_t j = 0; j < 8; j++)
  {
  data[j] = 0;
  }
  data[0] = (uint8_t) CurrentValue; // ONLY use first 8 bits to store data. May be extended...  

  Ptr<Icmpv4L4Protocol> icmp = this->GetNode()->GetObject<Icmpv4L4Protocol>(); 
  icmp->SendData(header, data);
}

void
GossipGenerator::SetCurrentValue ( int val )
{
  NS_LOG_FUNCTION (this << val);
  CurrentValue = val;
  NS_LOG_INFO ("Value of first node set to " << CurrentValue);
}

int
GossipGenerator::GetCurrentValue ( void )
{
  NS_LOG_FUNCTION (this);
  return CurrentValue;
}

void
GossipGenerator::GossipProcess(void)
{
  //int delta_t = 1;//millisecond
  while(!halt)
  {
    //sleep(delta_t);
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
  //int delta_t = 5000;//millisecond
  while(CurrentValue != 0)
  {
    //sleep(delta_t);
    Ipv4Address ipv4array[2];
    ChooseRandomNeighbor(ipv4array);
    SendMessage(ipv4array[0],ipv4array[1], TYPE_SOLICIT);
  }
}

void
GossipGenerator::StartApplication ( void )
{
  NS_LOG_FUNCTION (this);

  // Schedule... t1(GossipProcess); // TODO
  // Schedule... t2(Solicit);

}

void
GossipGenerator::StopApplication ()
{
  NS_LOG_FUNCTION (this);
}

} // Namespace ns3
