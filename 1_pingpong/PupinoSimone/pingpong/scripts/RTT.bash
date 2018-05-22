#!/bin/bash

#Check the number of parameters provided
if [[ $# != 4 ]]; then
	printf "\nIncorrect parameter provided. Use RTT.bash UDP_min UDP_max TCP_min TCP_max\n\n"
	exit 1
fi

#Check if source files provided are correct
if [ ! -f ../data/udp_${1}.out ]; then
    printf "\nIncorrect parameter provided. Unable to find UDP_min source file\n\n"
    exit 1
fi

if [ ! -f ../data/udp_${2}.out ]; then
    printf "\nIncorrect parameter provided. Unable to find UDP_max source file\n\n"
    exit 1
fi

if [ ! -f ../data/tcp_${3}.out ]; then
    printf "\nIncorrect parameter provided. Unable to find TCP_min source file\n\n"
    exit 1
fi

if [ ! -f ../data/tcp_${4}.out ]; then
    printf "\nIncorrect parameter provided. Unable to find TCP_max source file\n\n"
    exit 1
fi

#Remove old files
rm ../data/RTT_*

#Declaration of script variables
RTT1=0;RTT2=0;RTT3=0;RTT4=0
EstRTT1=0;EstRTT2=0;EstRTT3=0;EstRTT4=0
VarRTT1=0;VarRTT2=0;VarRTT3=0;VarRTT4=0
DiffRTT1=0;DiffRTT2=0;DiffRTT3=0;DiffRTT4=0
UDP_min=$1;UDP_max=$2;TCP_min=$3;TCP_max=$4;

#Obtain number of repetition in current iteration
line=$(awk '/Round/{print $9}' ../data/tcp_${TCP_min}.out | tail -n 1) 


for(( i=1; i<=$line; i++))
do
	#Obtain all lines that start with "Round"
	RTT1=$(grep "Round" ../data/tcp_${TCP_min}.out | awk 'NR=='$i'{print $5}')
	RTT2=$(grep "Round" ../data/tcp_${TCP_max}.out | awk 'NR=='$i'{print $5}')
	RTT3=$(grep "Round" ../data/udp_${UDP_min}.out | awk 'NR=='$i'{print $5}')
	RTT4=$(grep "Round" ../data/udp_${UDP_max}.out | awk 'NR=='$i'{print $5}')
	
	#For the first iteration the value of Estimated RTT is the same as Sample RTT, and the delay is 0
	if [[ $i == 1 ]]; then
		EstRTT1=$RTT1
		EstRTT2=$RTT2
		EstRTT3=$RTT3
		EstRTT4=$RTT4

		#Write data on file
		echo $(grep "Round" ../data/tcp_${TCP_min}.out | awk 'NR=='$i'{print $9,$5}') " " $EstRTT1 " " $VarRTT1 >> ../data/RTT_TCP_${TCP_min}.out
		echo $(grep "Round" ../data/tcp_${TCP_max}.out | awk 'NR=='$i'{print $9,$5}') " " $EstRTT2 " " $VarRTT2 >> ../data/RTT_TCP_${TCP_max}.out
		echo $(grep "Round" ../data/udp_${UDP_min}.out | awk 'NR=='$i'{print $9,$5}') " " $EstRTT3 " " $VarRTT3 >> ../data/RTT_UDP_${UDP_min}.out
		echo $(grep "Round" ../data/udp_${UDP_max}.out | awk 'NR=='$i'{print $9,$5}') " " $EstRTT4 " " $VarRTT4 >> ../data/RTT_UDP_${UDP_max}.out
	else 
		#Computation of Estimated RTT
		EstRTT1=$(bc -l <<< "(0.875*$EstRTT1)+(0.125*$RTT1)")
		EstRTT2=$(bc -l <<< "(0.875*$EstRTT2)+(0.125*$RTT2)")
		EstRTT3=$(bc -l <<< "(0.875*$EstRTT3)+(0.125*$RTT3)")
		EstRTT4=$(bc -l <<< "(0.875*$EstRTT4)+(0.125*$RTT4)")

		#Computation of Delay between Estimated RTT and Sample RTT
		DiffRTT1=$(bc -l <<< "($RTT1-$EstRTT1)")
		DiffRTT2=$(bc -l <<< "($RTT2-$EstRTT2)")
		DiffRTT3=$(bc -l <<< "($RTT3-$EstRTT3)")
		DiffRTT4=$(bc -l <<< "($RTT4-$EstRTT4)")
		
		#Absolute value of the Delay
		DiffRTT1=$(echo $DiffRTT1 | sed 's/-//')
		DiffRTT2=$(echo $DiffRTT2 | sed 's/-//')
		DiffRTT3=$(echo $DiffRTT3 | sed 's/-//')
		DiffRTT4=$(echo $DiffRTT4 | sed 's/-//')

		#Computation of final Delay value
		VarRTT1=$(bc -l <<< "(0.75*$VarRTT1)+(0.125*$DiffRTT1)")
		VarRTT2=$(bc -l <<< "(0.75*$VarRTT2)+(0.125*$DiffRTT2)")
		VarRTT3=$(bc -l <<< "(0.75*$VarRTT3)+(0.125*$DiffRTT3)")
		VarRTT4=$(bc -l <<< "(0.75*$VarRTT4)+(0.125*$DiffRTT4)")

		#Write data on file
		echo $(grep "Round" ../data/tcp_${TCP_min}.out | awk 'NR=='$i'{print $9,$5}') " " $EstRTT1 " " $VarRTT1 >> ../data/RTT_TCP_${TCP_min}.out
		echo $(grep "Round" ../data/tcp_${TCP_max}.out | awk 'NR=='$i'{print $9,$5}') " " $EstRTT2 " " $VarRTT2 >> ../data/RTT_TCP_${TCP_max}.out
		echo $(grep "Round" ../data/udp_${UDP_min}.out | awk 'NR=='$i'{print $9,$5}') " " $EstRTT3 " " $VarRTT3 >> ../data/RTT_UDP_${UDP_min}.out
		echo $(grep "Round" ../data/udp_${UDP_max}.out | awk 'NR=='$i'{print $9,$5}') " " $EstRTT4 " " $VarRTT4 >> ../data/RTT_UDP_${UDP_max}.out
	fi
done
 
#Creation of the 4 graphics
gnuplot <<-eNDgNUPLOTcOMMAND
	set term png size 900, 700
	set output "../data/RTT_TCP_${TCP_min}.png"
	set logscale y 10
	set xlabel "Repetition"
	set ylabel "RTT(ms)"
	plot "../data/RTT_TCP_${TCP_min}.out" using 1:2 title "TCP sample" \
			with linespoints, \
		"../data/RTT_TCP_${TCP_min}.out" using 1:3 title "TCP estimated" \
			with linespoints, \
		"../data/RTT_TCP_${TCP_min}.out" using 1:4 title "TCP variability" \
			with linespoints
	clear

	set term png size 900, 700
	set output "../data/RTT_TCP_${TCP_max}.png"
	set logscale y 10
	set xlabel "Repetition"
	set ylabel "RTT(ms)"
	plot "../data/RTT_TCP_${TCP_max}.out" using 1:2 title "TCP sample" \
			with linespoints, \
		"../data/RTT_TCP_${TCP_max}.out" using 1:3 title "TCP estimated" \
			with linespoints, \
		"../data/RTT_TCP_${TCP_max}.out" using 1:4 title "TCP variability" \
			with linespoints
	clear

	set term png size 900, 700
	set output "../data/RTT_UDP_${UDP_min}.png"
	set logscale y 10
	set xlabel "Repetition"
	set ylabel "RTT(ms)"
	plot "../data/RTT_UDP_${UDP_min}.out" using 1:2 title "UDP sample" \
			with linespoints, \
		"../data/RTT_UDP_${UDP_min}.out" using 1:3 title "UDP estimated" \
			with linespoints, \
		"../data/RTT_UDP_${UDP_min}.out" using 1:4 title "UDP variability" \
			with linespoints
	clear

	set term png size 900, 700
	set output "../data/RTT_UDP_${UDP_max}.png"
	set logscale y 10
	set xlabel "Repetition"
	set ylabel "RTT(ms)"
	plot "../data/RTT_UDP_${UDP_max}.out" using 1:2 title "UDP sample" \
			with linespoints, \
		"../data/RTT_UDP_${UDP_max}.out" using 1:3 title "UDP estimated" \
			with linespoints, \
		"../data/RTT_UDP_${UDP_max}.out" using 1:4 title "UDP variability" \
			with linespoints
	clear
eNDgNUPLOTcOMMAND

