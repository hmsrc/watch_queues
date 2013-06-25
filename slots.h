struct slot_info{
  int free_12;
  int free_8;
  int free_4;
  int total_slots;
  int total_hosts;
};


struct slot_info slots(){

  int j;

    int TOTAL_SLOTS,TOTAL_PERCENT,TOTAL_HOSTS,TOTAL_RUN;
    int SHARED_SLOTS,SHARED_PERCENT,SHARED_RUN;
    int FREE_SLOTS1,FREE_SLOTS2;
    int FREE_12,FREE_8,FREE_4;
    int FREE2_12,FREE2_8,FREE2_4;

    struct hostInfoEnt *hInfo;
    char *hostname ;
    int numHosts = 1;
    struct hostInfo *lsHostInfo;
    int lsNumHosts;


    hostname="shared_hosts";
    
    TOTAL_HOSTS=0;
    FREE_12=FREE_8=FREE_4=0;
    FREE2_12=FREE2_8=FREE2_4=0;

    TOTAL_SLOTS = TOTAL_PERCENT = 0;
    SHARED_SLOTS = SHARED_PERCENT = 0;


    /* all hosts total job slots */
    numHosts=0;
    hInfo = lsb_hostinfo(&hostname, &numHosts);
    if (hInfo == NULL) {
      lsb_perror("simbhosts: lsb_hostinfo() failed");
      exit (-1);
    }

    /* save for printing later */
    TOTAL_HOSTS=numHosts;

    /* loop over hosts, exclude unavail state */
    for(j = 0; j < numHosts; j++) {
      if ( hInfo[j].hStatus & HOST_STAT_UNAVAIL ) {}
      else {
	TOTAL_SLOTS = TOTAL_SLOTS + hInfo[j].maxJobs;

	FREE_SLOTS1=hInfo[j].maxJobs - hInfo[j].numRUN;
	FREE_SLOTS2=hInfo[j].maxJobs - hInfo[j].numJobs;

	if ( FREE_SLOTS1 == 12 ){
	  FREE_12++;
	}
	else if ( FREE_SLOTS1 > 8 ){
	  FREE_8++;
	}
	else if ( FREE_SLOTS1 > 4 ){
	  FREE_4++;
	}


	if ( FREE_SLOTS2 == 12 ){
	  FREE2_12++;
	}
	else if ( FREE_SLOTS2 > 8 ){
	  FREE2_8++;
	}
	else if ( FREE_SLOTS2 > 4 ){
	  FREE2_4++;
	}
      }
    }

    struct slot_info retval = {FREE_12,FREE_8,FREE_4,TOTAL_SLOTS,TOTAL_HOSTS};
    return retval;


}
