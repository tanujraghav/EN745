#include "ns3/antenna-module.h"
#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/mobility-module.h"
#include "ns3/nr-module.h"
#include "ns3/point-to-point-module.h"

using namespace ns3;

int main(int argc, char *argv[]) {

  // constant parameters
  const uint16_t gNbNum = 1;
  const uint16_t ueNumPergNb = 6;
  const double txPower = 23;
  const double bandwidth = 50e6;
  const double udpAppStartTime = 0;
  const uint32_t udpPacketSize = 1500;
  const bool useFixedMcs = false;
  const uint8_t numCcPerBand = 1;
  const double gNbHeight = 10;
  const double ueHeight = 1.5;

  // primary parameters, user-defined
  std::string scheduler = "ns3::NrMacSchedulerTdmaPF";
  uint16_t speed = 10;
  uint16_t RngRun = 27;
  uint16_t numerology = 1;
  bool fullBufferFlag = true;
  bool mobile = false;

  // secondary parameters, dependent on primary parameters
  double centralFrequency = 6e9;
  double simTime = 5;
  double lambda = 2500;

  // output directory
  std::string outputDir = "./group6-simulation-results";

  // command-line usage
  CommandLine cmd;
  cmd.AddValue("scheduler", "which MAC Scheduler to use", scheduler);
  cmd.AddValue("speed", "speed (in case of mobile scenario)", speed);
  cmd.AddValue("RngRun", "runtime seed number to be used", RngRun);
  cmd.AddValue("numerology", "network numerology to be used", numerology);
  cmd.AddValue("fullBufferFlag", "whether to set the full buffer traffic", fullBufferFlag);
  cmd.AddValue("mobile", "whether to set mobile UEs scenario", mobile);
  cmd.Parse(argc, argv);

  // output file format
  std::string simTag = "schedulerType_" + scheduler + " - RngRun_" + std::to_string(RngRun) + " - mobile_" + std::to_string(mobile) + " - numerology_" + std::to_string(numerology) + " - fullBufferFlag_" + std::to_string(fullBufferFlag) + " - speed_" + std::to_string(speed);

  // setting up secondary parameters
  if (numerology == 3) {
    centralFrequency = 28e9;
  }

  if (mobile == true) {
    simTime = 20;
  }

  if (fullBufferFlag == false) {
    lambda = 1000;
  }

  // configuring runtime seed count and seed number
  RngSeedManager::SetSeed(5);
  RngSeedManager::SetRun(RngRun);

  Config::SetDefault("ns3::LteRlcUm::MaxTxBufferSize", UintegerValue(999999999));

  // setting up NR helper
  Ptr<NrHelper> nrHelper = CreateObject<NrHelper>();

  BandwidthPartInfo::Scenario scenario = BandwidthPartInfo::UMa_LoS;

  CcBwpCreator ccBwpCreator;
  CcBwpCreator::SimpleOperationBandConf bandConf(centralFrequency, bandwidth, numCcPerBand, scenario);

  OperationBandInfo band = ccBwpCreator.CreateOperationBandContiguousCc(bandConf);

  nrHelper->InitializeOperationBand(&band);

  BandwidthPartInfoPtrVector allBwps = CcBwpCreator::GetAllBwps({band});

  // BandWidth Part
  nrHelper->SetGnbPhyAttribute("TxPower", DoubleValue(txPower));
  nrHelper->SetGnbPhyAttribute("Numerology", UintegerValue(numerology));
  nrHelper->SetGnbPhyAttribute("Pattern", StringValue("F|F|F|F|F|F|F|F|F|F|"));

  nrHelper->SetSchedulerTypeId(TypeId::LookupByName(scheduler));

  nrHelper->SetSchedulerAttribute("FixedMcsDl", BooleanValue(useFixedMcs));
  nrHelper->SetSchedulerAttribute("FixedMcsUl", BooleanValue(useFixedMcs));

  // Antennas for all the UEs
  nrHelper->SetUeAntennaAttribute("NumRows", UintegerValue(2));
  nrHelper->SetUeAntennaAttribute("NumColumns", UintegerValue(4));
  nrHelper->SetUeAntennaAttribute("AntennaElement", PointerValue(CreateObject<IsotropicAntennaModel>()));

  // Antennas for all the gNbs
  nrHelper->SetGnbAntennaAttribute("NumRows", UintegerValue(4));
  nrHelper->SetGnbAntennaAttribute("NumColumns", UintegerValue(8));
  nrHelper->SetGnbAntennaAttribute("AntennaElement", PointerValue(CreateObject<ThreeGppAntennaModel>()));

  // BeamformingMethod
  Ptr<IdealBeamformingHelper> idealBeamformingHelper = CreateObject<IdealBeamformingHelper>();
  idealBeamformingHelper->SetAttribute("BeamformingMethod", TypeIdValue(DirectPathBeamforming::GetTypeId()));
  nrHelper->SetBeamformingHelper(idealBeamformingHelper);

  Config::SetDefault("ns3::ThreeGppChannelModel::UpdatePeriod", TimeValue(MilliSeconds(0)));
  nrHelper->SetPathlossAttribute("ShadowingEnabled", BooleanValue(false));

  // Error Model
  nrHelper->SetUlErrorModel("ns3::NrEesmIrT1");
  nrHelper->SetDlErrorModel("ns3::NrEesmIrT1");

  // AmcModel
  nrHelper->SetGnbDlAmcAttribute("AmcModel", EnumValue(NrAmc::ShannonModel));
  nrHelper->SetGnbUlAmcAttribute("AmcModel", EnumValue(NrAmc::ShannonModel));

  Ptr<NrPointToPointEpcHelper> epcHelper = CreateObject<NrPointToPointEpcHelper>();
  nrHelper->SetEpcHelper(epcHelper);

  // S1-U Link Delay between gNodeB and P-GW
  epcHelper->SetAttribute("S1uLinkDelay", TimeValue(MilliSeconds(2)));

  uint32_t bwpIdForBearer = 0;
  nrHelper->SetGnbBwpManagerAlgorithmAttribute("GBR_CONV_VOICE", UintegerValue(bwpIdForBearer));

  // initializing NR helper
  nrHelper->Initialize();

  NodeContainer gNbNodes;
  NodeContainer ueNodes;

  std::string speedString = std::to_string(speed);

  MobilityHelper gNbmobility;
  gNbmobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");

  // setting up UE mobility based on "mobile" parameter
  MobilityHelper uemobility;
  if (mobile) {
    uemobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel", "Bounds", RectangleValue(Rectangle(-1000, 3000, -300, 300)), "Speed", StringValue("ns3::ConstantRandomVariable[Constant=" + speedString + "]"));
  }
  else {
    uemobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  }

  Ptr<ListPositionAllocator> bsPositionAlloc = CreateObject<ListPositionAllocator>();
  Ptr<ListPositionAllocator> utPositionAlloc = CreateObject<ListPositionAllocator>();

  gNbNodes.Create(gNbNum);
  ueNodes.Create(ueNumPergNb * gNbNum);

  // setting up gNB location 
  bsPositionAlloc->Add(Vector(0.0, 0.0, gNbHeight));

  // setting up UEs location 
  utPositionAlloc->Add(Vector(1000.0, 0.0, ueHeight));
  utPositionAlloc->Add(Vector(-500, 0.0, ueHeight));
  utPositionAlloc->Add(Vector(3000.0, 0.0, ueHeight));
  utPositionAlloc->Add(Vector(-1000.0, 0.0, ueHeight));
  utPositionAlloc->Add(Vector(10.0, 0.0, ueHeight));
  utPositionAlloc->Add(Vector(-10.0, 0.0, ueHeight));

  int64_t randomStream = 1;

  gNbmobility.SetPositionAllocator(bsPositionAlloc);
  gNbmobility.Install(gNbNodes);

  NetDeviceContainer gNbNetDev = nrHelper->InstallGnbDevice(gNbNodes, allBwps);
  randomStream += nrHelper->AssignStreams(gNbNetDev, randomStream);
  for (auto it = gNbNetDev.Begin(); it != gNbNetDev.End(); ++it) {
    DynamicCast<NrGnbNetDevice>(*it)->UpdateConfig();
  }

  uemobility.SetPositionAllocator(utPositionAlloc);
  uemobility.Install(ueNodes);

  NetDeviceContainer ueNetDev = nrHelper->InstallUeDevice(ueNodes, allBwps);
  randomStream += nrHelper->AssignStreams(ueNetDev, randomStream);
  for (auto it = ueNetDev.Begin(); it != ueNetDev.End(); ++it) {
    DynamicCast<NrUeNetDevice>(*it)->UpdateConfig();
  }

  Ptr<Node> pgw = epcHelper->GetPgwNode();
  NodeContainer remoteHostContainer;
  remoteHostContainer.Create(1);

  Ptr<Node> remoteHost = remoteHostContainer.Get(0);
  InternetStackHelper internet;
  internet.Install(remoteHostContainer);

  PointToPointHelper p2ph;
  p2ph.SetDeviceAttribute("DataRate", DataRateValue(DataRate("10Gb/s")));
  p2ph.SetDeviceAttribute("Mtu", UintegerValue(2500));
  p2ph.SetChannelAttribute("Delay", TimeValue(MilliSeconds(5)));
  NetDeviceContainer internetDevices = p2ph.Install(pgw, remoteHost);

  Ipv4AddressHelper ipv4h;
  ipv4h.SetBase("1.0.0.0", "255.0.0.0");
  Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign(internetDevices);
  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting(remoteHost->GetObject<Ipv4>());
  remoteHostStaticRouting->AddNetworkRouteTo(Ipv4Address("7.0.0.0"), Ipv4Mask("255.0.0.0"), 1);
  internet.Install(ueNodes);

  Ipv4InterfaceContainer ueIpIface = epcHelper->AssignUeIpv4Address(NetDeviceContainer(ueNetDev));

  for (uint32_t j = 0; j < ueNodes.GetN(); ++j) {
    Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting(ueNodes.Get(j)->GetObject<Ipv4>());
    ueStaticRouting->SetDefaultRoute(epcHelper->GetUeDefaultGatewayAddress(), 1);
  }

  nrHelper->AttachToClosestEnb(ueNetDev, gNbNetDev);

  uint16_t dlPort = 1234;

  // setting up Client-Server application
  ApplicationContainer serverApps;

  UdpServerHelper dlPacketSinkHelper(dlPort);
  serverApps.Add(dlPacketSinkHelper.Install(ueNodes));

  UdpClientHelper dlClient;
  dlClient.SetAttribute("RemotePort", UintegerValue(dlPort));
  dlClient.SetAttribute("PacketSize", UintegerValue(udpPacketSize));
  dlClient.SetAttribute("MaxPackets", UintegerValue(0xFFFFFFFF));

  if (fullBufferFlag) {
    double bitRate = 75000000;
    bitRate /= ueNumPergNb;
    if (bandwidth > 20e6) {
      bitRate *= bandwidth / 20e6;
    }
    lambda = bitRate / static_cast<double>(udpPacketSize * 8);
  }

  dlClient.SetAttribute("Interval", TimeValue(Seconds(1.0 / lambda)));

  EpsBearer bearer(EpsBearer::GBR_CONV_VOICE);

  Ptr<EpcTft> tft = Create<EpcTft>();
  EpcTft::PacketFilter dlpf;
  dlpf.localPortStart = dlPort;
  dlpf.localPortEnd = dlPort;
  tft->Add(dlpf);

  ApplicationContainer clientApps;

  for (uint32_t i = 0; i < ueNodes.GetN(); ++i) {
    Ptr<Node> ue = ueNodes.Get(i);
    Ptr<NetDevice> ueDevice = ueNetDev.Get(i);
    Address ueAddress = ueIpIface.GetAddress(i);

    dlClient.SetAttribute("RemoteAddress", AddressValue(ueAddress));
    clientApps.Add(dlClient.Install(remoteHost));

    nrHelper->ActivateDedicatedEpsBearer(ueDevice, bearer, tft);
  }

  // starting up client-server architecture
  serverApps.Start(Seconds(udpAppStartTime));
  clientApps.Start(Seconds(udpAppStartTime + 0.005));
  serverApps.Stop(Seconds(simTime));
  clientApps.Stop(Seconds(simTime));

  FlowMonitorHelper flowmonHelper;
  NodeContainer endpointNodes;
  endpointNodes.Add(remoteHost);
  endpointNodes.Add(ueNodes);

  Ptr<ns3::FlowMonitor> monitor = flowmonHelper.Install(endpointNodes);
  monitor->SetAttribute("DelayBinWidth", DoubleValue(0.001));
  monitor->SetAttribute("JitterBinWidth", DoubleValue(0.001));
  monitor->SetAttribute("PacketSizeBinWidth", DoubleValue(20));

  // starting simulation
  Simulator::Stop(Seconds(simTime));
  Simulator::Run();

  // setting up packet flow monitoring
  monitor->CheckForLostPackets();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmonHelper.GetClassifier());
  FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats();

  uint32_t packetLost = 0.0;
  uint32_t totalTxPackets = 0.0;

  double averageFlowThroughput = 0.0;
  double averageFlowDelay = 0.0;

  // output
  std::ofstream outFile;
  std::string filename = outputDir + "/" + simTag;

  outFile.open(filename.c_str(), std::ofstream::out | std::ofstream::trunc);
  if (!outFile.is_open()) {
    std::cerr << "Can't open file " << filename << std::endl;
    return 1;
  }

  outFile.setf(std::ios_base::fixed);

  // simulation metadata
  outFile << "Scheduling Algo: " << scheduler << "\n";
  outFile << "  Speed: " << speed << "\n";
  outFile << "  RngRun: " << RngRun << "\n";
  outFile << "  Numerology: " << numerology << "\n";
  outFile << "  CentralFrequency " << centralFrequency << "\n";
  outFile << "  FullBuffer: " << fullBufferFlag << "\n";
  outFile << "  Mobile: " << mobile << "\n";
  outFile << "  SimTime " << simTime << "\n\n\n";

  double flowDuration = simTime - udpAppStartTime;

  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin(); i != stats.end(); ++i) {
    Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(i->first);

    std::stringstream protoStream;
    protoStream << (uint16_t)t.protocol;
    if (t.protocol == 17) {
      protoStream.str("UDP");
    }

    outFile << "Flow " << i->first << " (" << t.sourceAddress << ":" << t.sourcePort << " -> " << t.destinationAddress << ":" << t.destinationPort << ") proto " << protoStream.str() << "\n";

    packetLost += (i->second.txPackets - i->second.rxPackets);
    totalTxPackets += i->second.txPackets;

    if (i->second.rxPackets > 0) {
      averageFlowThroughput += i->second.rxBytes * 8.0 / flowDuration / 1000 / 1000;
      averageFlowDelay += 1000 * i->second.delaySum.GetSeconds() / i->second.rxPackets;

      outFile << "  Throughput: " << i->second.rxBytes * 8.0 / flowDuration / 1000 / 1000 << " Mbps\n";
      outFile << "  Loss Rate: " << 100 * ((double)i->second.txPackets - (double)i->second.rxPackets) / (double)i->second.txPackets << " %\n";
      outFile << "  Delay: " << 1000 * i->second.delaySum.GetSeconds() / i->second.rxPackets << " ms\n\n";
    }
    else {
      outFile << "  Throughput: 0 Mbps\n";
      outFile << "  Loss Rate: 100%\n";
      outFile << "  Delay: 0 ms\n\n";
    }
  }

  // extracting average packet flow data
  outFile << "\nMean Flow Throughput: " << averageFlowThroughput / stats.size() << " Mbps\n";
  outFile << "Mean Packet Loss Rate: " << 100 * (double)packetLost / (double)totalTxPackets << " %\n";
  outFile << "Mean Flow Delay: " << averageFlowDelay / stats.size() << " ms\n";

  outFile.close();

  std::ifstream f(filename.c_str());
  if (f.is_open()) {
    std::cout << f.rdbuf();
  }

  // stopping simulation and destroying simulation object
  Simulator::Destroy();
  return 0;
}