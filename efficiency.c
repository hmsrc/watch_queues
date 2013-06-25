#include <stdio.h>
#include <stdlib.h>
#include "/opt/lsf/7.0/include/lsf/lsf.h"
#include "/opt/lsf/7.0/include/lsf/lsbatch.h"

/* number of jobs returns doesn't match bjobs output? not getting "pending jobs" */
/* calculate efficiency and print */

/*
Efic %%: The entire cluster’s CPU efficiency for running jobs. 
Efficiency is calculated with this formula:
cpu_time / (run_time × #_of_cpus).
 */
int main(argc,argv)
     int  argc;
     char *argv[];
{
  /* initialization */

  char *user = "all";
  //int  options = PEND_JOB ;
  // int  options = RUN_JOB ;
  int  options = 0 ;
  struct jRusage runUsage;
  struct jobInfoHead *jInfoH =  (struct jobInfoHead *) malloc(sizeof(struct jobInfoHead));
  struct jobInfoEnt *job; 
  int more,i;

  /* printf("initialize\n"); */
  if (lsb_init(argv[0]) < 0) {
    lsb_perror("lsb_init()");
    exit(-1);
  }

  printf("User   - JobID      -  Efficiency\n");
  printf("---------------------------------\n");
  /* open the job information connection with mbatchd */
  /* options set to only retund jobs in pending state */
  jInfoH = lsb_openjobinfo_a(0, NULL, user, NULL, NULL, options);

  /* gets the total number of pending job, exits if failure */
  if (jInfoH !=NULL) {
    /* indices->name = getindexlist(11); */
    for (i=0; i < jInfoH->numJobs ;i++) {
      job = lsb_readjobinfo(&more);
      if (job == NULL) {
	printf("no job \n");
	lsb_perror("lsb_readjobinfo");
	exit(-1);
      }

      if (job->runTime > 0) {

	/* job->cputime is always 0, WTF! */
	/* use user + system time from rusage as the total cputime */
	runUsage = job->runRusage;

        /* put an if statement here to only print range */
        /* say > 1 and less than .5 or whatever we are looking for */

	/* printf("\t%d processes, mem <%d> swap <%d> utime <%d> stime <%d>\n", runUsage.npids, runUsage.mem, runUsage.swap, runUsage.utime, runUsage.stime); */

	/* caclulate efficiency and print */
	/* printf("Job <%d>  efficiency %.4f\n",job->jobId, ((float)(runUsage.utime + runUsage.stime))/ ((float)(job->runTime * job->submit.numProcessors)) ); */
    float efficiency;
    efficiency = ((float)(runUsage.utime + runUsage.stime))/ ((float)(job->runTime * job->submit.numProcessors)); 
        if ( efficiency > 1.05) {
	    printf("%-6s - %-10d -  %.2f\n",job->user, job->jobId, ((float)(runUsage.utime + runUsage.stime))/ ((float)(job->runTime * job->submit.numProcessors)) ); 
        }
      } /* rutime > 0 */
      
    } /*all jobs loop */

    lsb_closejobinfo();

  } /* jInfo */

} /* main */
