struct host_slots{
  int shared;
  int total;
  int sixtyfour;
  int thirtytwo;
  int sixteen;
};

/* find shared and total slot usage, and return hosts memory availability information */
struct host_slots hosts(){

  int j;

    /* bhost related variables */
    struct hostInfoEnt *hInfo;
    char *hostname ;
    int numHosts = 1;
    struct hostInfo *lsHostInfo;
    int free_range[3];
    int TOTAL_SLOTS;
    int SHARED_SLOTS;

    int options = 0;
    
    TOTAL_SLOTS = 0;
    SHARED_SLOTS = 0;
    free_range[0]=free_range[1]=free_range[2]=0;

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
      /* too many possible states, probably missing case */

      if ( (hInfo[j].hStatus & HOST_STAT_UNAVAIL) || (hInfo[j].hStatus & HOST_STAT_UNREACH) || (hInfo[j].hStatus & HOST_CLOSED_BY_ADMIN) ) {
	//printf("%s host unavail or unreach  or admin closed \n", hInfo[j].host );
      }
      
      else {
	if  ( (hInfo[j].hStatus & HOST_STAT_FULL) || hInfo[j].hStatus == 0 || hInfo[j].hStatus & HOST_STAT_BUSY || hInfo[j].hStatus & HOST_STAT_EXCLUSIVE ) {

	  SHARED_SLOTS = SHARED_SLOTS + hInfo[j].maxJobs;

	  /* free memory counts */
	  if ( hInfo[j].load[10] > 65536){
	    free_range[0]++;}
	  else if ( hInfo[j].load[10] > 32768){
	    free_range[1]++;}
	  else if ( hInfo[j].load[10] > 16384){
	    free_range[2]++;}


	} /* if is ok busy full or exclusive */
	else {
	  // printf("%s host not ok busy full or exclusive \n", hInfo[j].host );
	}

      } /* if unreach or unavail */

    }

    struct host_slots retval = {SHARED_SLOTS,TOTAL_SLOTS,free_range[0],free_range[1],free_range[2]};
    return retval;
}
