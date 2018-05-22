#!/bin/bash

#Check source files
if [ ! -f ../data/tcp_throughput.dat ]; then
    printf "\nUnable to find tcp_throughput source file\n\n"
    exit 1
fi

if [ ! -f ../data/udp_throughput.dat ]; then
    printf "\nUnable to find udp_throughput source file\n\n"
    exit 1
fi

#Remove old files
rm ../data/BandLatency*.png

#Obtaining first and last line for band-latency model
head=$(head -n 1 ../data/tcp_throughput.dat)
tail=$(tail -n 1 ../data/tcp_throughput.dat)

#Obtain local variables
N1=$(echo $head| cut -d' ' -f 1)
N2=$(echo $tail| cut -d' ' -f 1)
TM1=$(echo $head| cut -d' ' -f 2)
TM2=$(echo $tail| cut -d' ' -f 2)

#Delay calculation
DN1=$(bc -l <<< "$N1/$TM1")
DN2=$(bc -l <<< "$N2/$TM2")

#B and L calculation
B=$(bc -l <<< "($N2-$N1)/($DN2-$DN1)")
L=$(bc -l <<< "(($DN1*$N2)-($DN2*$N1))/($N2-$N1)")

#Creation of TCP graphic
gnuplot <<-eNDgNUPLOTcOMMAND
	set term png size 900, 700
	set output "../data/BandLatencyTCP.png"
	set logscale x 2
	set logscale y 10
	set xlabel "msg size (B)"
	set ylabel "throughput (KB/s)"
	lbf(x)=x/($L+x/$B)
	plot "../data/tcp_throughput.dat" using 1:2 title "TCP Throughput" with linespoints, \
	[0.01:10**(10)] lbf(x) title "Latency-Bandwidth model with L=$L and B=$B" with linespoints
	clear
eNDgNUPLOTcOMMAND

#Obtaining first and last line for band-latency model
head=$(head -n 1 ../data/udp_throughput.dat)
tail=$(tail -n 1 ../data/udp_throughput.dat)

#Obtain local variables
N1=$(echo $head| cut -d' ' -f 1)
N2=$(echo $tail| cut -d' ' -f 1)
TM1=$(echo $head| cut -d' ' -f 2)
TM2=$(echo $tail| cut -d' ' -f 2)

#Delay calculation
DN1=$(bc -l <<< "$N1/$TM1")
DN2=$(bc -l <<< "$N2/$TM2")

#B and L calculation
B=$(bc -l <<< "($N2-$N1)/($DN2-$DN1)")
L=$(bc -l <<< "(($DN1*$N2)-($DN2*$N1))/($N2-$N1)")

#Creation of UDP graphic
gnuplot <<-eNDgNUPLOTcOMMAND
	set term png size 900, 700
	set output "../data/BandLatencyUDP.png"
	set logscale x 2
	set logscale y 10
	set xlabel "msg size (B)"
	set ylabel "throughput (KB/s)"
	lbf(x)=x/($L+x/$B)
	plot "../data/udp_throughput.dat" using 1:2 title "UDP Throughput" with linespoints, \
	[0.01:10**(10)] lbf(x) title "Latency-Bandwidth model with L=$L and B=$B" with linespoints
	clear
eNDgNUPLOTcOMMAND