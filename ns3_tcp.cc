
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

int main() {
    NodeContainer nodes;
    nodes.Create(2);

    PointToPointHelper p2p;
    p2p.SetDeviceAttribute("DataRate",
                           StringValue("5Mbps"));
    p2p.SetChannelAttribute("Delay",
                            StringValue("2ms"));

    NetDeviceContainer devices = p2p.Install(nodes);

    InternetStackHelper internet;
    internet.Install(nodes);

    Ipv4AddressHelper ip;
    ip.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces = ip.Assign(devices);

    // TCP Server
    uint16_t port = 5000;

    PacketSinkHelper server(
        "ns3::TcpSocketFactory",
        InetSocketAddress(Ipv4Address::GetAny(), port)
    );

    ApplicationContainer s = server.Install(nodes.Get(1));
    s.Start(Seconds(1.0));
    s.Stop(Seconds(10.0));

    // TCP Client
    BulkSendHelper client(
        "ns3::TcpSocketFactory",
        InetSocketAddress(interfaces.GetAddress(1), port)
    );

    client.SetAttribute("MaxBytes", UintegerValue(2560));

    ApplicationContainer c = client.Install(nodes.Get(0));
    c.Start(Seconds(2.0));
    c.Stop(Seconds(9.0));

    Simulator::Stop(Seconds(10.0));
    Simulator::Run();

    // Display received bytes
    Ptr<PacketSink> sink =
        DynamicCast<PacketSink>(s.Get(0));

    std::cout << "Total bytes received: "
              << sink->GetTotalRx()
              << std::endl;

    Simulator::Destroy();

    return 0;
}
