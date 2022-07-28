BEGIN{
pkt=0;
time=0;
}
{
if($1 == "r" && $3 =="0" && $4=="2")
{
pkt=pkt+$6;
time=$2;
printf("%f\t%d\n",time,pkt);
}
END {
}
