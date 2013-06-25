#include <stdio.h>
#include <stdlib.h>
#include "/opt/lsf/7.0/include/lsf/lsf.h"
#include "/opt/lsf/7.0/include/lsf/lsbatch.h"

/* Report runlimit (bsub -W) for all jobs */

int main(argc,argv)
     int  argc;
     char *argv[];
{
  /* initialization */

  char *user = "all";
  //int  options = PEND_JOB ;
  // int  options = RUN_JOB ;
  //int  options = 0 ;
  int options = RUN_JOB|SUSP_JOB; // not DONE or PEND
  struct jRusage runUsage;
  struct jobInfoHead *jInfoH =  (struct jobInfoHead *) malloc(sizeof(struct jobInfoHead));
  struct jobInfoEnt *job; 
  struct submit submit;
  char *jobQ;
  int index;
  int more,i;

  /* printf("initialize\n"); */
  if (lsb_init(argv[0]) < 0) {
    lsb_perror("lsb_init()");
    exit(-1);
  }

  printf("JobID\tJobIndex\tUser\tQueue\tRunlimit\n");
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

      // Exclude jobs that got USUSP without ever running, e.g.
      if (job->runTime > 0) {

	submit = job->submit;
	jobQ = submit.queue;
	index = LSB_ARRAY_IDX(job->jobId);

	/* Could do limiting here, but do it in calling program instead */
	/* if (!strcmp(jobQ, "long") || !strcmp(jobQ, "parallel")) { */

	  /* == bjob -l's RUNLIMIT in seconds. Same for 1 or multi-thread */
	  int runtimeEstimation;
	  runtimeEstimation = submit.rLimits[LSF_RLIMIT_RUN];

	  printf("%-10d\t%-6d\t%-10s\t%-15s\t%8d\n",job->jobId, index, job->user, jobQ, runtimeEstimation ); 
	/*}*/
      } /* rutime > 0 */
      
    } /*all jobs loop */

    lsb_closejobinfo();

  } /* jInfo */

} /* main */
