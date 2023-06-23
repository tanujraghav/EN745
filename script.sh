#!/usr/bin/env bash

RollNo=27


echo "Simulating Task 1 - Part A (i). Full Buffer Case"

echo "Case 1. Round Robin - TDMA"
for i in {0..4}
do
	./ns3 run "Group6-Asg1 --scheduler=ns3::NrMacSchedulerTdmaRR --RngRun=$(($RollNo+$i))"
done

echo "Case 2. Round Robin - OFDMA"
for i in {0..4}
do
	./ns3 run "Group6-Asg1 --scheduler=ns3::NrMacSchedulerOfdmaRR --RngRun=$(($RollNo+$i))"
done

echo "Case 3. Proportional Fair - TDMA"
for i in {0..4}
do
	./ns3 run "Group6-Asg1 --scheduler=ns3::NrMacSchedulerTdmaPF --RngRun=$(($RollNo+$i))"
done

echo "Case 4. Proportional Fair - OFDMA"
for i in {0..4}
do
	./ns3 run "Group6-Asg1 --scheduler=ns3::NrMacSchedulerOfdmaPF --RngRun=$(($RollNo+$i))"
done

echo "Case 5. Maximum Rate - TDMA"
for i in {0..4}
do
	./ns3 run "Group6-Asg1 --scheduler=ns3::NrMacSchedulerTdmaMR --RngRun=$(($RollNo+$i))"
done

echo "Case 6. Maximum Rate - OFDMA"
for i in {0..4}
do
	./ns3 run "Group6-Asg1 --scheduler=ns3::NrMacSchedulerOfdmaMR --RngRun=$(($RollNo+$i))"
done

echo "Done..."


echo "Simulating Task 1 - Part A (ii). Non Full Buffer Case"

echo "Case 1. Round Robin - TDMA"
for i in {0..4}
do
	./ns3 run "Group6-Asg1 --scheduler=ns3::NrMacSchedulerTdmaRR --RngRun=$(($RollNo+$i)) --fullBufferFlag=false"
done

echo "Case 2. Round Robin - OFDMA"
for i in {0..4}
do
	./ns3 run "Group6-Asg1 --scheduler=ns3::NrMacSchedulerOfdmaRR --RngRun=$(($RollNo+$i)) --fullBufferFlag=false"
done

echo "Case 3. Proportional Fair - TDMA"
for i in {0..4}
do
	./ns3 run "Group6-Asg1 --scheduler=ns3::NrMacSchedulerTdmaPF --RngRun=$(($RollNo+$i)) --fullBufferFlag=false"
done

echo "Case 4. Proportional Fair - OFDMA"
for i in {0..4}
do
	./ns3 run "Group6-Asg1 --scheduler=ns3::NrMacSchedulerOfdmaPF --RngRun=$(($RollNo+$i)) --fullBufferFlag=false"
done

echo "Case 5. Maximum Rate - TDMA"
for i in {0..4}
do
	./ns3 run "Group6-Asg1 --scheduler=ns3::NrMacSchedulerTdmaMR --RngRun=$(($RollNo+$i)) --fullBufferFlag=false"
done

echo "Case 6. Maximum Rate - OFDMA"
for i in {0..4}
do
	./ns3 run "Group6-Asg1 --scheduler=ns3::NrMacSchedulerOfdmaMR --RngRun=$(($RollNo+$i)) --fullBufferFlag=false"
done

echo "Done..."


echo "No need to simulate Task 1 - Part B, as relevant results have already been obtained in Task 1 - Part A"

echo "Done..."


echo "Simulating Task 2 - Part A"

echo "Case 1. Numerology 0"
for i in {0..4}
do
	./ns3 run "Group6-Asg1 --RngRun=$(($RollNo+$i)) --numerology=0"
done

echo "No need to simulate Task 2 - Part A - Case 2. Numerology , as relevant results have already been obtained in Task 1 - Part A"

echo "Case 3. Numerology 2"
for i in {0..4}
do
	./ns3 run "Group6-Asg1 --RngRun=$(($RollNo+$i)) --numerology=2"
done

echo "Case 4. Numerology 3"
for i in {0..4}
do
	./ns3 run "Group6-Asg1 --RngRun=$(($RollNo+$i)) --numerology=3"
done

echo "Done..."


echo "No need to simulate Task 2 - Part B, as relevant results have already been obtained in Task 2 - Part A"

echo "Done..."


echo "Simulating Task 3"

echo "Case 1. Round Robin - Speed: 10 m/s"
for i in {0..4}
do
	./ns3 run "Group6-Asg1 --scheduler=ns3::NrMacSchedulerTdmaRR --speed=10 --RngRun=$(($RollNo+$i)) --mobile=true"
done

echo "Case 2. Round Robin - Speed: 50 m/s"
for i in {0..4}
do
	./ns3 run "Group6-Asg1 --scheduler=ns3::NrMacSchedulerTdmaRR --speed=50 --RngRun=$(($RollNo+$i)) --mobile=true"
done

echo "Case 3. Proportional Fair - Speed: 10 m/s"
for i in {0..4}
do
	./ns3 run "Group6-Asg1 --scheduler=ns3::NrMacSchedulerTdmaPF --speed=10 --RngRun=$(($RollNo+$i)) --mobile=true"
done

echo "Case 4. Proportional Fair - Speed: 50 m/s"
for i in {0..4}
do
	./ns3 run "Group6-Asg1 --scheduler=ns3::NrMacSchedulerTdmaPF --speed=50 --RngRun=$(($RollNo+$i)) --mobile=true"
done

echo "Case 5. Maximum Rate - Speed: 10 m/s"
for i in {0..4}
do
	./ns3 run "Group6-Asg1 --scheduler=ns3::NrMacSchedulerTdmaMR --speed=10 --RngRun=$(($RollNo+$i)) --mobile=true"
done

echo "Case 6. Maximum Rate - Speed: 50 m/s"
for i in {0..4}
do
	./ns3 run "Group6-Asg1 --scheduler=ns3::NrMacSchedulerTdmaMR --speed=50 --RngRun=$(($RollNo+$i)) --mobile=true"
done

echo "Done..."
