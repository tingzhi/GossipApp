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
  m_socket = 0;
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
GossipGenerator::Receive (Ptr<Socket> socket)
{
  NS_LOG_INFO ("Packet received");
  NS_LOG_FUNCTION (this << socket);
  while (m_socket->GetRxAvailable () > 0)
    {
      Address from;
      Ptr<Packet> p = m_socket->RecvFrom (0xffffffff, 0, from);
      NS_LOG_DEBUG ("recv " << p->GetSize () << " bytes");
      NS_ASSERT (InetSocketAddress::IsMatchingType (from));
      InetSocketAddress realFrom = InetSocketAddress::ConvertFrom (from);
      NS_ASSERT (realFrom.GetPort () == 1); // protocol should be icmp.
      Ipv4Header ipv4;
      p->RemoveHeader (ipv4);
      // uint32_t recvSize = p->GetSize ();
      NS_ASSERT (ipv4.GetProtocol () == 1); // protocol should be icmp.
      Icmpv4Header icmp;
      p->RemoveHeader (icmp);
      /*
      if (icmp.GetType () == Icmpv4Header::ECHO_REPLY)
        {
          Icmpv4Echo echo;
          p->RemoveHeader (echo);
          std::map<uint16_t, Time>::iterator i = m_sent.find (echo.GetSequenceNumber ());

          if (i != m_sent.end () && echo.GetIdentifier () == 0)
            {
              uint32_t * buf = new uint32_t [m_size];
              uint32_t dataSize = echo.GetDataSize ();
              uint32_t nodeId;
              uint32_t appId;
              if (dataSize == m_size)
                {
                  echo.GetData ((uint8_t *)buf);
                  Read32 ((const uint8_t *) &buf[0], nodeId);
                  Read32 ((const uint8_t *) &buf[1], appId);

                  if (nodeId == GetNode ()->GetId () &&
                      appId == GetApplicationId ())
                    {
                      Time sendTime = i->second;
                      NS_ASSERT (Simulator::Now () >= sendTime);
                      Time delta = Simulator::Now () - sendTime;

                      m_sent.erase (i);
                      m_avgRtt.Update (delta.GetMilliSeconds ());
                      m_recv++;
                      m_traceRtt (delta);

                      if (m_verbose)
                        {
                          std::cout << recvSize << " bytes from " << realFrom.GetIpv4 () << ":"
                                    << " icmp_seq=" << echo.GetSequenceNumber ()
                                    << " ttl=" << (unsigned)ipv4.GetTtl ()
                                    << " time=" << delta.GetMilliSeconds () << " ms\n";
                        }
                    }
                }
              delete[] buf;
            }
        }
       */
    }
}

void
GossipGenerator::SendMessage_public(Ipv4Address src, Ipv4Address dest, int type)
{
  SendMessage( src,  dest,  type);
}

Ptr< NetDevice >
GossipGenerator::ChooseRandomNeighbor(){
  Ptr< Node > node = this->GetNode();
  Ptr< NetDevice >device = node->GetDevice(rand() % node->GetNDevices());
  if (device != device->GetChannel()->GetDevice(0)){
    return device->GetChannel()->GetDevice(0);
  }else{
    return device->GetChannel()->GetDevice(1);
  }
  //need to find who connect the device
}

void
GossipGenerator::SendMessage(Ipv4Address src, Ipv4Address dest, int type)
{
  NS_LOG_FUNCTION (this << dest << type);

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
GossipGenerator::SendPayload(Ipv4Address dest)
{
  NS_LOG_FUNCTION (this << dest );
}

void
GossipGenerator::SendMessage(Ptr<NetDevice> target, int type)
{
  NS_LOG_FUNCTION (this << target << type);

  Ptr<NetDevice> source;
  if (target != target->GetChannel()->GetDevice(0)){
    source = target->GetChannel()->GetDevice(0);
  }else{
    source = target->GetChannel()->GetDevice(1);
  }

  Ipv4Header header = Ipv4Header ();
  // header.SetDestination (target->GetAddress()); //TODO
  header.SetPayloadSize (0);
  // header.SetSource (source->GetAddress()); //TODO
  
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
GossipGenerator::SendPayload(Ptr<NetDevice> target)
{
  NS_LOG_FUNCTION (this << target );
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
GossipGenerator::StartApplication ( void )
{
  NS_LOG_FUNCTION (this);
  // Create the socket if not already
  if (!m_socket)
    {
      m_socket = Socket::CreateSocket (GetNode (), UdpSocketFactory::GetTypeId ());
      m_socket->Bind ();
      m_socket->Listen ();
    }
/*
  m_socket->SetRecvCallback (MakeCallback (&PacketSink::HandleRead, this));
  m_socket->SetAcceptCallback (
    MakeNullCallback<bool, Ptr<Socket>, const Address &> (),
    MakeCallback (&PacketSink::HandleAccept, this));
  m_socket->SetCloseCallbacks (
    MakeCallback (&PacketSink::HandlePeerClose, this),
    MakeCallback (&PacketSink::HandlePeerError, this));
*/
}

void
GossipGenerator::StopApplication ()
{
  NS_LOG_FUNCTION (this);
/*
  while(!m_socketList.empty ()) //these are accepted sockets, close them
    {
      Ptr<Socket> acceptedSocket = m_socketList.front ();
      m_socketList.pop_front ();
      acceptedSocket->Close ();
    }
*/
  if (m_socket) 
    {
      m_socket->Close ();
     // m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
    }
}

} // Namespace ns3
