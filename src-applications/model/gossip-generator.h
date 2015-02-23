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

#include "ns3/application.h"

namespace ns3 {

class GossipGenerator : public Application
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  GossipGenerator ();
  virtual ~GossipGenerator ();
  void SetCurrentValue (int val);
  int GetCurrentValue ( void);

protected:
  virtual void DoDispose (void);
  int CurrentValue;

private:
  virtual void StartApplication (void);
  virtual void StopApplication (void);

};

} // Namespace ns3

#endif // GOSSIP_GENERATOR_H
