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

#ifndef GOSSIP_GENERATOR_H
#define GOSSIP_GENERATOR_H

#include <vector>

#include "ns3/application.h"
#include "ns3/socket.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"

namespace ns3 {

  const uint8_t TYPE_SOLICIT = 21; //!< constant used to indicate type of msg
  const uint8_t TYPE_ACK     = 23; //!< constant used to indicate type of msg

/**
 * \ingroup 
 * \class GossipGenerator
 *
 * \brief An application implementing the gossip protocol to communicate
 */
class GossipGenerator : public Application
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  /**
   * \brief Constructor.
   */
  GossipGenerator ();

  /**
   * \brief Constructor.
   */
  virtual ~GossipGenerator ();

  /**
   * \brief add a neighbor
   * \param ip of the neighbor
   */
  void AddNeighbor( Ipv4Address own,Ipv4Address neighbor );

  /**
   * \brief Find a random neigbor
   * \param Store a random source and neighbor address in this array
   */
  void ChooseRandomNeighbor(Ipv4Address neighbor[2]);

  /**
   * \brief Seed the internal value of a node to initiate the gossip.
   * \param The internal value.
   */
  void SetCurrentValue (int val);

  /**
   * \brief Return the current internal value.
   * \return The value
   */
  int GetCurrentValue ( void);
  void SendMessage_debug(Ipv4Address src, Ipv4Address dest, int type);
  void HandleAck(void);
  void HandleSolicit(Ipv4Address src,Ipv4Address dest);

  /**
   * \brief Handle payload
   * \param the source address
   * \param the destination address
   * \param the payload
   */
  void HandlePayload(Ipv4Address src,Ipv4Address dest,uint8_t payload[]);

protected:
  /**
   * \brief Dispose method.
   */
  virtual void DoDispose (void);

  /**
   * \brief Send a message to the given destination indicating the type
   * \param the source
   * \param the destination
   * \param the message type
   */
  void SendMessage(Ipv4Address src, Ipv4Address dest, int type);

  /**
   * \brief Send the current value to the given destination
   * \param the source
   * \param the destination
   */
  void SendPayload(Ipv4Address src, Ipv4Address dest);

  int CurrentValue; //!< The current Value
  bool halt; //!< If the gossip is paused
  std::vector<Ipv4Address> neighbours[2]; //!< The own addresses and corresponding neighbors of this node

private:
  /**
   * \brief Start the GossipProcess
   */
  void GossipProcess();

  /**
   * \brief Start the SolicitProcess
   */
  void Solicit(void);

  /**
   * \brief Start the application.
   */
  virtual void StartApplication ();

  /**
   * \brief Stop the application.
   */
  virtual void StopApplication ();
};

} // Namespace ns3

#endif // GOSSIP_GENERATOR_H
