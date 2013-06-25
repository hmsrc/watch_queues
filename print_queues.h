struct run_values{
  int shared;
  int total;
};

struct run_values print_queues()
{
    /* bqueues related variables */
    struct queueInfoEnt *qInfo;
    int  numQueues = 0;
    char **queue;
    int  i,j,k,c;


    int TOTAL_JOBS,TOTAL_SUSP,TOTAL_RUN,TOTAL_PEND;

    int SHARED_RUN,SHARED_PEND;
    int CONTRIB_RUN;

    TOTAL_JOBS = TOTAL_SUSP = TOTAL_RUN = TOTAL_PEND = 0;
    SHARED_RUN = SHARED_PEND = 0;
    CONTRIB_RUN = 0;

    printf("%-20s\t\tNJOBS\tPEND\tRUN\tSUSP\n","ORCHESTRA");

    /* bqueues equivalent, get infomation on all queues */
    /* see: https://wiki.med.harvard.edu/doc/lsf/api_ref/lsb_queueinfo.html */
    qInfo = lsb_queueinfo(queue, &numQueues, NULL, NULL, 0); 
    if (qInfo == NULL) { 
        lsb_perror("lsb_queueinfo()");
        exit(-1);
    }


    /* loop over all queues in qInfo */
    for(j = 0; j < numQueues; j++) {	

      /* print if queue has jobs in it */
      if (qInfo[j].numJobs != 0 ) {
	/* printf("%-20s\t\t%-d\t%-d\t%-d\t%-d\n", qInfo[j].queue, qInfo[j].numJobs,
	   qInfo[j].numPEND, qInfo[j].numRUN, qInfo[j].numSSUSP + qInfo[j].numUSUSP); */
      }

      /* contrib is everthing that is not the below things */
      /* shared queues: interactive long short mini parallel */
      /* internal queues: tmp_unlimited training testing, highmem_unlimited */
      /* all_unlimited until it goes away */

      /* shared queue running job totals */
      /* note: !strcmp returns true when it matches "value" */
      if ( !strcmp(qInfo[j].queue,"short") || !strcmp(qInfo[j].queue,"long") || !strcmp(qInfo[j].queue,"interactive") || !strcmp(qInfo[j].queue,"mini") || !strcmp(qInfo[j].queue,"parallel") ) {
	SHARED_RUN = SHARED_RUN + qInfo[j].numRUN;                 
	SHARED_PEND = SHARED_PEND + qInfo[j].numPEND;                 

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

    printf("-------------------------------------------------------------------\n");
    printf("%-20s\t\t%-d\t%-d\t%-d\t%-d\n","TOTALS",TOTAL_JOBS,TOTAL_PEND,TOTAL_RUN,TOTAL_SUSP);

    struct run_values retval = {SHARED_RUN,TOTAL_RUN};
    return retval;

} /* end print queues */
