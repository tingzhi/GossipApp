/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 Oregon State University
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

#ifndef GOSSIP_GENERATOR_HELPER_H
#define GOSSIP_GENERATOR_HELPER_H

#include <stdint.h>
#include "ns3/application-container.h"
#include "ns3/node-container.h"
#include "ns3/object-factory.h"
#include "ns3/ipv4-address.h"
#include "ns3/gossip-generator.h"

namespace ns3 {
/**
 * \ingroup 
 * \class GossipGeneratorHelper
 *
 * \brief The helper class for GossipGenerator
 */
class GossipGeneratorHelper
{
public:
  /**
   * \brief Constructor
   *
   * \param 
   */
  GossipGeneratorHelper ();

  /**
   * \brief Record an attribute to be set in each Application after it is is created.
   *
   * \param name the name of the attribute to set
   * \param value the value of the attribute to set
   */
  void SetAttribute (std::string name, const AttributeValue &value);

  /**
   * \brief Create a GossipGeneratorApplication on the specified Node.
   *
   * \param c The nodes on which to create the Applications.
   * \returns The applications created, one Application per Node in the
   */
  const ApplicationContainer Install (NodeContainer c);

  /**
   * \brief Return the last created generator.
   *
   * \returns a Ptr to the last created generator application
   */
  Ptr<GossipGenerator> GetGenerator (void);
private:
  ObjectFactory m_factory; //!< Object factory.
  Ptr<GossipGenerator> m_generator; //!< The last created generator application
};

} // namespace ns3

#endif /* GOSSIP_GENERATOR_HELPER_H */
