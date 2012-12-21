
#!/bin/bash

function pcdsum
{
    awk '{ OFMT = "%.0f" ; sum+=$1}END{print sum}'
}

SHARED_JOBS="$(bqueues short long parallel mini interactive | tail -n +2 |awk '{print $10}'| pcdsum)"
SHARED_SLOTS="$(bhosts shared_hosts | grep -v unavail | awk '{print $4}' | pcdsum)"
SHARED_PERCENT=$(echo "scale=2;($SHARED_JOBS*100)/$SHARED_SLOTS" | bc)

CONTRIB_JOBS="$(bqueues church_int_15m danuser_int_15m freedberg_int_15m hcra_int_15m i2b2_int_15m megason_int_15m merfeld_int_15m mgh-ita_int_15m nezafat_int_15m nowak_int_15m park_int_15m reich_int_15m sorger_int_15m sysbio_int_15m usheva_int_15m church_int_2h danuser_int_2h freedberg_int_2h hcra_int_2h i2b2_int_2h kreiman_int_2h megason_int_2h merfeld_int_2h mgh-ita_int_2h nezafat_int_2h nowak_int_2h park_int_2h reich_int_2h sorger_int_2h sysbio_int_2h usheva_int_2h bpf_int_12h cbdm_int_12h cbi_int_12h church_int_12h danuser_int_7d danuser_int_12h erdogmus_int_12h freedberg_int_12h hcra_int_12h i2b2_int_12h kreiman_int_12h kung_int_12h megason_int_12h merfeld_int_12h mgh-ita_int_12h nezafat_int_12h nowak_int_12h park_int_12h reich_int_12h sorger_int_2d sysbio_int_2d usheva_int_12h bpf_15m church_15m danuser_15m freedberg_15m hcra_15m i2b2_15m kreiman_15m megason_15m merfeld_15m mgh-ita_15m nezafat_15m nowak_15m park_15m reich_15m sorger_15m sysbio_15m usheva_15m cbdm_2h church_2h danuser_2h freedberg_2h hcra_2h i2b2_2h kreiman_2h megason_2h merfeld_2h mgh-ita_2h nezafat_2h nowak_2h park_2h reich_2h sorger_2h sysbio_2h usheva_2h bpf_12h cbdm_12h cbi_12h church_12h danuser_12h erdogmus_12h freedberg_12h hcra_12h i2b2_12h kreiman_12h kung_12h megason_12h merfeld_12h mgh-ita_12h nezafat_12h nowak_12h park_12h reich_12h sorger_12h sysbio_12h usheva_12h church_1d danuser_1d erdogmus_1d freedberg_1d hcra_1d i2b2_1d kreiman_1d megason_1d merfeld_1d mgh-ita_1d nezafat_1d nowak_1d park_1d reich_1d sorger_1d sysbio_1d usheva_1d church_7d danuser_7d freedberg_7d hcra_7d i2b2_7d megason_7d merfeld_7d mgh-ita_7d nezafat_7d nowak_7d park_7d reich_7d sorger_7d sysbio_7d usheva_7d bpf_unlimited cbdm_unlimited cbi_unlimited church_unlimited danuser_unlimited deisboeck_unlimited erdogmus_unlimited freedberg_unlimited hcra_unlimited i2b2_unlimited kreiman_unlimited kung_unlimited megason_unlimited merfeld_unlimited mgh-ita_unlimited nezafat_unlimited nowak_unlimited park_unlimited reich_unlimited sorger_unlimited sorger_par_unlimited sorger_par_1d sysbio_unlimited sysbio_par_1d usheva_unlimited rodeo_15m rodeo_12h rodeo_unlimited | tail -n +2 |awk '{print $10}'| pcdsum)"
CONTRIB_SLOTS="$(bhosts bpf_hosts cbdm_hosts cbi_hosts church_hosts danuser_hosts deisboeck_hosts freedberg_hosts hcra_hosts i2b2_hosts kreiman_hosts kung_hosts megason_hosts merfeld_hosts mgh-ita_hosts nezafat_hosts nowak_hosts park_hosts reich_hosts sorger_hosts sysbio_hosts usheva_hosts | grep -v unavail | awk '{print $4}' | pcdsum)"
CONTRIB_PERCENT=$(echo "scale=2;($CONTRIB_JOBS*100)/$CONTRIB_SLOTS" | bc)

TOTAL_JOBS="$(bqueues | tail -n +2 |awk '{print $10}'| pcdsum)"
TOTAL_SLOTS="$(bhosts | grep -v unavail | awk '{print $4}' | pcdsum)"
TOTAL_PERCENT=$(echo "scale=2;($TOTAL_JOBS*100)/$TOTAL_SLOTS" | bc)


/opt/lsf/7.0/linux2.6-glibc2.3-x86_64/bin/bqueues  | grep -v "0     0     0     0" 2>&1 ; /opt/lsf/7.0/linux2.6-glibc2.3-x86_64/bin/bqueues | grep -v "0     0     0     0" | tail -n+1 | awk '{ njob += $ 8 } {pend += $9 } {run += $10 } {susp += $11} END { print "njob", njob, "pend", pend, "run", run, "susp", susp}' 2>&1
echo
echo "Utilization of shared_hosts: $SHARED_JOBS/$SHARED_SLOTS = $SHARED_PERCENT%"
echo "Utilization of hms_orchestra: $TOTAL_JOBS/$TOTAL_SLOTS = $TOTAL_PERCENT%"
echo "Utilization of contrib_hosts: $CONTRIB_JOBS/$CONTRIB_SLOTS = $CONTRIB_PERCENT%"
