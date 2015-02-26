/***************************************/
/*				Icmpv4Ack
/***************************************/

NS_OBJECT_ENSURE_REGISTERED (Icmpv4Ack);

TypeId
Imcpv4Ack::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Icmpv4Ack")
	.setParent<Header> ()
	.AddConstructor<Imcpv4Ack> ()
  ;
  return tid;
}
Icmpv4Ack::Icmpv4Ack ()
{
  NS_LOG_FUNCTION (this);
  for (uint8_t j = 0; j < 8; j++)
  {
	m_data[j] = 0;
  }
}

void 
Icmpv4Ack::SetData (Ptr<const Packet> data)
{
  NS_LOG_FUNCTION (this << *data);
  data->CopyData (m_data, 8);
}
void 
Icmpv4Ack::SetHeader (Ipv4Header header)
{
  NS_LOG_FUNCTION (this << header);
  m_header = header;
}
void 
Icmpv4Ack::GetData (uint8_t payload[8]) const
{
  NS_LOG_FUNCTION (this << payload);
  memcpy (payload, m_data, 8);
}
Ipv4Header 
Icmpv4Ack::GetHeader (void) const
{
  NS_LOG_FUNCTION (this);
  return m_header;
}

Icmpv4Ack::~Icmpv4Ack ()
{
  NS_LOG_FUNCTION (this);
}
TypeId
Icmpv4Ack::GetInstanceTypeId (void) const
{
  NS_LOG_FUNCTION (this);
  return GetTypeId ();
}
uint32_t
Icmpv4Ack::GetSerializedSize (void) const
{
  NS_LOG_FUNCTION (this);
  return 4 + m_header.GetSerializedSize () + 8;
}
void
Icmpv4Ack::Serialize (Buffer::Iterator start) const
{
  NS_LOG_FUNCTION (this << &start);
  start.WriteU32 (0);
  uint32_t size = m_header.GetSerializedSize ();
  m_header.Serialize (start);
  start.Next (size);
  start.Write (m_data, 8);
}

uint32_t 
Icmpv4Ack::Deserialize (Buffer::Iterator start)
{
  NS_LOG_FUNCTION (this << &start);
  Buffer::Iterator i = start;
  i.Next (4);
  uint32_t read = m_header.Deserialize (i);
  i.Next (read);
  for (uint8_t j = 0; j < 8; j++)
    {
      m_data[j] = i.ReadU8 ();
    }
  return i.GetDistanceFrom (start);
}
void 
Icmpv4Ack::Print (std::ostream &os) const
{
  NS_LOG_FUNCTION (this << &os);
  m_header.Print (os);
  os << " org data=";
  for (uint8_t i = 0; i < 8; i++)
    {
      os << (uint32_t) m_data[i];
      if (i != 8)
        {
          os << " ";
        }
    }
}

/***************************************/
/*				Icmpv4Request
/***************************************/

NS_OBJECT_ENSURE_REGISTERED (Icmpv4Request);

TypeId
Imcpv4Request::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Icmpv4Request")
	.setParent<Header> ()
	.AddConstructor<Imcpv4Ack> ()
  ;
  return tid;
}
Icmpv4Request::Icmpv4Request ()
{
  NS_LOG_FUNCTION (this);
  for (uint8_t j = 0; j < 8; j++)
  {
	m_data[j] = 0;
  }
}

void 
Icmpv4Request::SetData (Ptr<const Packet> data)
{
  NS_LOG_FUNCTION (this << *data);
  data->CopyData (m_data, 8);
}
void 
Icmpv4Request::SetHeader (Ipv4Header header)
{
  NS_LOG_FUNCTION (this << header);
  m_header = header;
}
void 
Icmpv4Request::GetData (uint8_t payload[8]) const
{
  NS_LOG_FUNCTION (this << payload);
  memcpy (payload, m_data, 8);
}
Ipv4Header 
Icmpv4Request::GetHeader (void) const
{
  NS_LOG_FUNCTION (this);
  return m_header;
}

Icmpv4Request::~Icmpv4Request ()
{
  NS_LOG_FUNCTION (this);
}
TypeId
Icmpv4Request::GetInstanceTypeId (void) const
{
  NS_LOG_FUNCTION (this);
  return GetTypeId ();
}
uint32_t
Icmpv4Request::GetSerializedSize (void) const
{
  NS_LOG_FUNCTION (this);
  return 4 + m_header.GetSerializedSize () + 8;
}
void
Icmpv4Request::Serialize (Buffer::Iterator start) const
{
  NS_LOG_FUNCTION (this << &start);
  start.WriteU32 (0);
  uint32_t size = m_header.GetSerializedSize ();
  m_header.Serialize (start);
  start.Next (size);
  start.Write (m_data, 8);
}

uint32_t 
Icmpv4Request::Deserialize (Buffer::Iterator start)
{
  NS_LOG_FUNCTION (this << &start);
  Buffer::Iterator i = start;
  i.Next (4);
  uint32_t read = m_header.Deserialize (i);
  i.Next (read);
  for (uint8_t j = 0; j < 8; j++)
    {
      m_data[j] = i.ReadU8 ();
    }
  return i.GetDistanceFrom (start);
}
void 
Icmpv4Request::Print (std::ostream &os) const
{
  NS_LOG_FUNCTION (this << &os);
  m_header.Print (os);
  os << " org data=";
  for (uint8_t i = 0; i < 8; i++)
    {
      os << (uint32_t) m_data[i];
      if (i != 8)
        {
          os << " ";
        }
    }
}

/***************************************/
/*				Icmpv4Data
/***************************************/

NS_OBJECT_ENSURE_REGISTERED (Icmpv4Data);

TypeId
Imcpv4Data::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Icmpv4Data")
	.setParent<Header> ()
	.AddConstructor<Imcpv4Data> ()
  ;
  return tid;
}
Icmpv4Data::Icmpv4Data ()
{
  NS_LOG_FUNCTION (this);
  for (uint8_t j = 0; j < 8; j++)
  {
	m_data[j] = 0;
  }
}

void 
Icmpv4Data::SetData (Ptr<const Packet> data)
{
  NS_LOG_FUNCTION (this << *data);
  data->CopyData (m_data, 8);
}
void 
Icmpv4Data::SetHeader (Ipv4Header header)
{
  NS_LOG_FUNCTION (this << header);
  m_header = header;
}
void 
Icmpv4Data::GetData (uint8_t payload[8]) const
{
  NS_LOG_FUNCTION (this << payload);
  memcpy (payload, m_data, 8);
}
Ipv4Header 
Icmpv4Data::GetHeader (void) const
{
  NS_LOG_FUNCTION (this);
  return m_header;
}

Icmpv4Data::~Icmpv4Data ()
{
  NS_LOG_FUNCTION (this);
}
TypeId
Icmpv4Data::GetInstanceTypeId (void) const
{
  NS_LOG_FUNCTION (this);
  return GetTypeId ();
}
uint32_t
Icmpv4Data::GetSerializedSize (void) const
{
  NS_LOG_FUNCTION (this);
  return 4 + m_header.GetSerializedSize () + 8;
}
void
Icmpv4Data::Serialize (Buffer::Iterator start) const
{
  NS_LOG_FUNCTION (this << &start);
  start.WriteU32 (0);
  uint32_t size = m_header.GetSerializedSize ();
  m_header.Serialize (start);
  start.Next (size);
  start.Write (m_data, 8);
}

uint32_t 
Icmpv4Data::Deserialize (Buffer::Iterator start)
{
  NS_LOG_FUNCTION (this << &start);
  Buffer::Iterator i = start;
  i.Next (4);
  uint32_t read = m_header.Deserialize (i);
  i.Next (read);
  for (uint8_t j = 0; j < 8; j++)
    {
      m_data[j] = i.ReadU8 ();
    }
  return i.GetDistanceFrom (start);
}
void 
Icmpv4Data::Print (std::ostream &os) const
{
  NS_LOG_FUNCTION (this << &os);
  m_header.Print (os);
  os << " org data=";
  for (uint8_t i = 0; i < 8; i++)
    {
      os << (uint32_t) m_data[i];
      if (i != 8)
        {
          os << " ";
        }
    }
}