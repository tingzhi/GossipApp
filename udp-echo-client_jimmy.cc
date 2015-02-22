#include <stdlib.h>

namespace ns3 {
idea_temp{
	time = local_time
	switch(message):
		case x:
			if ((local_time - time)% 1 milsecond) == 0):
				find random neighbor 
				send x to neighbor
				listen ports for every 0.5 milsecond:
					if receive == ACK:
						while(1):
							listen ports:
								if listen == solic:
									send X
								if listen == data and mesaage == x:
									send ack
		case null:
			listen ports:
				if listen == data:
					message = x
					continue
				every 5 second:
					find random neighbor
					send solicate to neightbor
					listen packet for 0.5 milsecond:
						if receive == data:
							message = x
		
	idea_temp();			
}

Ptr< NetDevice >
findOneNeighbor(Node node_number){
	Ptr< NetDevice > deviceList = node_number.GetNDevices();
	Ptr< NetDevice >device  = deviceList[rand() % sizeOf(deviceList)];	
	if (device != device.GetChannel()[0]){
		return device.GetChannel()[0];
	}else:
		return device.GetChannel()[1];
	}
	
//need to find who connect the device	

}


}
