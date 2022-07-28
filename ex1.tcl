#create simulator obj

set ns [new Simulator]

set nf [open out.nam w]
$ns namtrace-all $nf

#define a finish procedure
proc finish {} {
global ns nf
$ns flush-trace
close $nf
exec nam out.nam &
exit 0
}

$ns color 1 Blue
$ns color 2 Red

set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]

$ns duplex-link $n0 $n1 2Mb 10ms DropTail 
$ns duplex-link $n1 $n2 1Mb 10ms DropTail 
$ns duplex-link $n3 $n1 2Mb 10ms DropTail 


$ns duplex-link-op $n0 $n1 orient right-down
$ns duplex-link-op $n1 $n2 orient right
$ns duplex-link-op $n3 $n1 orient right-up

$ns queue-limit $n1 $n2 20
$ns duplex-link-op $n1 $n2 queuePos 0.5 

set tcp0 [new Agent/TCP]
$ns attach-agent $n0 $tcp0
set ftp0 [new Application/FTP]
$ftp0 attach-agent $tcp0
$tcp0 set packet_size_ 512
$tcp0 set fid_ 1

set tcp1 [new Agent/TCP]
$ns attach-agent $n3 $tcp1
set ftp1 [new Application/FTP]
$ftp1 attach-agent $tcp1
$tcp1 set packet_size_ 512
$tcp1 set fid_ 2

set null0 [new Agent/TCPSink]
$ns attach-agent $n2 $null0
$ns connect $tcp0 $null0
set null1 [new Agent/TCPSink]
$ns attach-agent $n2 $null1
$ns connect $tcp1 $null1

$ns at 1.0 "$ftp0 start"
$ns at 1.0 "$ftp1 start"
$ns at 9.5 "$ftp0 stop"
$ns at 10.0 "finish"

$ns run
