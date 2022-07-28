#create simulator obj
#TCP using Link nDowm
set ns [new Simulator]
$ns rtproto DV
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

for {set i 0} {$i < 7} {incr i} {
	set n($i) [$ns node]
}

for {set i 0} {$i < 7} {incr i} {
	$ns duplex-link $n($i) $n([expr ($i+1)%7]) 1Mb 10ms DropTail
}

set tcp0 [new Agent/TCP]
$ns attach-agent $n(0) $tcp0
set ftp0 [new Application/FTP]
$ftp0 attach-agent $tcp0
$tcp0 set packet_size_ 10


set null1 [new Agent/TCPSink]
$ns attach-agent $n(3) $null1
$ns connect $tcp0 $null1


$ns rtmodel-at 2.0 down $n(1) $n(2)
$ns rtmodel-at 3.0 up $n(1) $n(2)

$ns at 1.0 "$ftp0 start"
$ns at 6.0 "$ftp0 stop"
$ns at 7.0 "finish"

$ns run
