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
}

void
GossipGenerator::StopApplication ()
{
  NS_LOG_FUNCTION (this);
}

} // Namespace ns3
