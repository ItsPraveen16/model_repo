#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

int main(){
NodeContainer nodes;
nodes.Create(2);

PointToPointHelper p2p;
p2p.SetDeviceAttribute("DataRate",StringValue("5Mbps"));
p2p.SetChannelAttribute("Delay",StringValue("2ms"));
NetDeviceContainer devices=p2p.Install(nodes);

InternetStackHelper internet;
internet.Install(nodes);

Ipv4AddressHelper ip;
ip.SetBase("10.1.1.0","255.255.255.0");
Ipv4InterfaceContainer interfaces=ip.Assign(devices);

UdpServerHelper server(5000);
ApplicationContainer s=server.Install(nodes.Get(1));
s.Start(Seconds(1.0));
s.Stop(Seconds(10.0));

UdpClientHelper client(interfaces.GetAddress(1),5000);
client.SetAttribute("MaxPackets",UintegerValue(5));
client.SetAttribute("Interval",TimeValue(Seconds(1.0)));
client.SetAttribute("PacketSize",UintegerValue(512));
ApplicationContainer c=client.Install(nodes.Get(0));
c.Start(Seconds(2.0));
c.Stop(Seconds(9.0));

Simulator::Stop(Seconds(10.0));
Simulator::Run();
Simulator::Destroy();
return 0;
}


Ptr<UdpServer> srv=DynamicCast<UdpServer>(s.Get(0));
Simulator::Schedule(Seconds(9.5),[srv](){
std::cout<<"Packets received: "<<srv->GetReceived()
<<std::endl;
});


