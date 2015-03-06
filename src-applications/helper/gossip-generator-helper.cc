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

#include "gossip-generator-helper.h"
#include "ns3/gossip-generator.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"

namespace ns3 {

GossipGeneratorHelper::GossipGeneratorHelper ()
{
  m_factory.SetTypeId (GossipGenerator::GetTypeId ());
}

void
GossipGeneratorHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

const ApplicationContainer
GossipGeneratorHelper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;

      m_generator = m_factory.Create<GossipGenerator> ();
      node->AddApplication (m_generator);
      apps.Add (m_generator);

    }
  return apps;
}

Ptr<GossipGenerator>
GossipGeneratorHelper::GetGenerator (void)
{
  return m_generator;
}

} // namespace ns3
