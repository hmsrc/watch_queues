/*
  queue and percent utilzation for shared, contributed and all nodes
  -u and -q to see utilization or queue information only
   compiled with:
      gcc -o watch_queues watch_queues.c -I /opt/lsf/7.0/include/ /opt/lsf/7.0/linux2.6-glibc2.3-x86_64/lib/libbat.a /opt/lsf/7.0/linux2.6-glibc2.3-x86_64/lib/liblsf.a -ldl -lnsl
   greg cavanagh
   12 December 2012
*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "/opt/lsf/7.0/include/lsf/lsf.h"
#include "/opt/lsf/7.0/include/lsf/lsbatch.h"

int main (argc, argv)
    int  argc;
    char *argv[];
{

    /* bqueues related variables */
    struct queueInfoEnt *qInfo;
    int  numQueues = 0;
    char **queue;
    int  i,j,c;
    int TOTAL_JOBS,TOTAL_SUSP,TOTAL_RUN,TOTAL_PEND,TOTAL_SLOTS,TOTAL_PERCENT;
    int SHARED_RUN,SHARED_SLOTS,SHARED_PERCENT;
    int CONTRIB_RUN,CONTRIB_SLOTS,CONTRIB_PERCENT;

    /* bhost related variables */
    struct hostInfoEnt *hInfo;
    char *hostname ;
    /* char *contribhosts[] = { "bpf_hosts","cbi_hosts","church_hosts","danuser_hosts","freedberg_hosts","i2b2_hosts","kreiman_hosts","megason_hosts","merfeld_hosts","nezafat_hosts","nowak_hosts","park_hosts","sorger_hosts","sysbio_hosts","usheva_hosts"}; */
    char *contribhosts[] = { "bpf_hosts","i2b2_hosts","park_hosts","reich_hosts","seidman_hosts"};
    int numHosts = 1;
    int show_time,show_queues,show_memory,show_utilization,show_dependency;
    time_t mytime;
    mytime = time(NULL);

    show_time=show_queues=show_memory=show_utilization=show_dependency=1;

    /* parse arguments */
    while ((c = getopt (argc, argv, "quDt")) != -1){
      switch (c)
	{
        case 't':
          show_time=0;
          break;
	case 'u':
	  show_queues=show_memory=show_dependency=0;
	  break;
	case 'D':
	  show_queues=show_memory=show_utilization=0;
	  break;
	case 'q':
	  show_memory=show_utilization=show_dependency=0;
	  break;
	default:
	  printf("urecognized argument \n");
	  exit(1);
	}
    }

    /* initialize variables */
    TOTAL_JOBS = TOTAL_SUSP = TOTAL_RUN = TOTAL_PEND = TOTAL_SLOTS = TOTAL_PERCENT = 0;
    SHARED_RUN = SHARED_SLOTS = SHARED_PERCENT = 0;
    CONTRIB_RUN = CONTRIB_SLOTS = CONTRIB_PERCENT = 0;

    if ( show_time == 1 ){
	printf("\n");
	printf(ctime(&mytime));
	printf("\n");
    }

    if (lsb_init(argv[0]) < 0) {
        lsb_perror("lsb_init()");
        exit(-1);
    }

    /* bqueues equivalent, get infomation on all queues */
    /* see: https://wiki.med.harvard.edu/doc/lsf/api_ref/lsb_queueinfo.html */
    qInfo = lsb_queueinfo(queue, &numQueues, NULL, NULL, 0);
    if (qInfo == NULL) {
        lsb_perror("lsb_queueinfo()");
        exit(-1);
    }

    if ( show_queues == 1 ){
      printf("%-20s\t\tNJOBS\tPEND\tRUN\tSUSP\n","QUEUE_NAME");
      printf("-------------------------------------------------------------------\n");
    }
    /* loop over all queues in qInfo */
    for(j = 0; j < numQueues; j++) {

      /* print if queue has jobs in it */
      if (qInfo[j].numJobs != 0 && show_queues == 1 ) {
	  printf("%-20s\t\t%-d\t%-d\t%-d\t%-d\n", qInfo[j].queue, qInfo[j].numJobs,
		 qInfo[j].numPEND, qInfo[j].numRUN, qInfo[j].numSSUSP + qInfo[j].numUSUSP);
      }

      /* contrib is everthing that is not the below things */
      /* shared queues: interactive long short mini parallel */
      /* internal queues: tmp_unlimited training testing, highmem_unlimited */
      /* all_unlimited until it goes away */

      /* shared queue running job totals */
      /* note: !strcmp returns true when it matches "value" */
      if ( !strcmp(qInfo[j].queue,"short") || !strcmp(qInfo[j].queue,"long") || !strcmp(qInfo[j].queue,"interactive") || !strcmp(qInfo[j].queue,"mini") || !strcmp(qInfo[j].queue,"parallel") ) {
	SHARED_RUN = SHARED_RUN + qInfo[j].numRUN;
      }
      else {

	/* contributed queue running job totals , not shared and not internal*/
	if (!strcmp(qInfo[j].queue,"tmp_unlimited") || !strcmp(qInfo[j].queue,"training") || !strcmp(qInfo[j].queue,"testing") || !strcmp(qInfo[j].queue,"highmem_unlimited") || !strcmp(qInfo[j].queue,"all_unlimited") ) {}
	else{
	  CONTRIB_RUN  = CONTRIB_RUN + qInfo[j].numRUN;
	}

      }

      /* totals for all queues */
      TOTAL_JOBS = TOTAL_JOBS + qInfo[j].numJobs;
      TOTAL_RUN = TOTAL_RUN + qInfo[j].numRUN;
      TOTAL_PEND = TOTAL_PEND + qInfo[j].numPEND;
      TOTAL_SUSP = TOTAL_SUSP + qInfo[j].numSSUSP + qInfo[j].numUSUSP;



    } /* end loop over all queues in qInfo */

    if ( show_queues == 1 ){
      printf("-------------------------------------------------------------------\n");
      printf("%-20s\t\t%-d\t%-d\t%-d\t%-d\n","TOTALS",TOTAL_JOBS,TOTAL_PEND,TOTAL_RUN,TOTAL_SUSP);
    }


    /* calculate and print the percent utilization of the job slots */
  if ( show_utilization == 1 ){

    /* bhost equivalent to get total of shared job slots */
    /* see: https://wiki.med.harvard.edu/doc/lsf/api_ref/lsb_hostinfo.html */
    hostname="shared_hosts";
    hInfo = lsb_hostinfo(&hostname, &numHosts);
    if (hInfo == NULL) {
      lsb_perror("simbhosts: lsb_hostinfo() failed");
      exit (-1);
    }
    /* loop over hosts, exclude unavail state */
    for(j = 0; j < numHosts; j++) {
      if ( hInfo[j].hStatus & ( HOST_STAT_UNAVAIL | HOST_CLOSED_BY_ADMIN )) {}
      else {
	SHARED_SLOTS = SHARED_SLOTS + hInfo[j].maxJobs;
      }
    }

    /* all hosts total job slots */
    numHosts=0;
    hInfo = lsb_hostinfo(&hostname, &numHosts);
    if (hInfo == NULL) {
      lsb_perror("simbhosts: lsb_hostinfo() failed");
      exit (-1);
    }
    /* loop over hosts, exclude unavail state */
    for(j = 0; j < numHosts; j++) {
      if ( hInfo[j].hStatus & ( HOST_STAT_UNAVAIL | HOST_CLOSED_BY_ADMIN )) {}
      else {
	TOTAL_SLOTS = TOTAL_SLOTS + hInfo[j].maxJobs;
      }
    }

    /* contributed job slots */
    numHosts=15;
    hInfo = lsb_hostinfo(contribhosts, &numHosts);
    if (hInfo == NULL) {
      lsb_perror("simbhosts: lsb_hostinfo() failed");
      exit (-1);
    }
    /* loop over hosts, exclude unavail state */
    for(j = 0; j < numHosts; j++) {
      if ( hInfo[j].hStatus & ( HOST_STAT_UNAVAIL | HOST_CLOSED_BY_ADMIN )) {}
      else {
	CONTRIB_SLOTS = CONTRIB_SLOTS + hInfo[j].maxJobs;
      }
    }


    printf("\n\nHOST_TYPE\t\tJOBS/SLOTS\tUTILIZATION\n");
    printf("-------------------------------------------------------------------\n");
    printf("%s\t\t%d/%d\t%0.4g\%\n","shared_hosts", SHARED_RUN,SHARED_SLOTS,((double)SHARED_RUN)/SHARED_SLOTS*100);
    printf("%s\t\t%d/%d\t%0.4g\%\n","hms_orchestra", TOTAL_RUN,TOTAL_SLOTS,((double)TOTAL_RUN)/TOTAL_SLOTS*100);
    printf("%s\t\t%d/%d\t%0.4g\%\n","contrib_hosts", CONTRIB_RUN,CONTRIB_SLOTS,((double) CONTRIB_RUN)/CONTRIB_SLOTS*100);
  } /* end show utilization */


  exit(0);

}
