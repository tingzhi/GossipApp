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
GossipGenerator::SendMessage_public(Ipv4Address src, Ipv4Address dest, int type)
{
  SendMessage( src,  dest,  type);
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
GossipGenerator::SetCurrentValue ( int val )
{
  NS_LOG_FUNCTION (this << val);
  CurrentValue = val;
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
  /*if (!m_socket)
    {
      m_socket = Socket::CreateSocket (GetNode (), m_tid);
      m_socket->Bind (m_local);
      m_socket->Listen ();
      m_socket->ShutdownSend ();
      if (addressUtils::IsMulticast (m_local))
        {
          Ptr<UdpSocket> udpSocket = DynamicCast<UdpSocket> (m_socket);
          if (udpSocket)
            {
              // equivalent to setsockopt (MCAST_JOIN_GROUP)
              udpSocket->MulticastJoinGroup (0, m_local);
            }
          else
            {
              NS_FATAL_ERROR ("Error: joining multicast on a non-UDP socket");
            }
        }
    }

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
