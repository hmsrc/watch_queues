all: watch_queues lsf_cpuslots get_runlimits watch_queues2

INCLUDES=-I /opt/lsf/7.0/include/
ARCHIVES=/opt/lsf/7.0/linux2.6-glibc2.3-x86_64/lib/libbat.a /opt/lsf/7.0/linux2.6-glibc2.3-x86_64/lib/liblsf.a 
LIBS=-ldl -lnsl

lsf_cpuslots: efficiency.c
	gcc -o $@ $< $(INCLUDES) $(ARCHIVES) $(LIBS)

watch_queues: watch_queues.c
	gcc -o $@ $< $(INCLUDES) $(ARCHIVES) $(LIBS)

get_runlimits: get_runlimits.c
	gcc -o $@ $< $(INCLUDES) $(ARCHIVES) $(LIBS)

watch_queues2: watch_queues2.c
	gcc -o $@ $< $(INCLUDES) $(ARCHIVES) $(LIBS)

