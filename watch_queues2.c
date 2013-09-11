/*
  queue and percent utilzation for shared, contributed and all nodes 
  -u and -q to see utilization or queue information only
   compiled with: 
      gcc -o watch_queues watch_queues.c -I /opt/lsf/7.0/include/ /opt/lsf/7.0/linux2.6-glibc2.3-x86_64/lib/libbat.a /opt/lsf/7.0/linux2.6-glibc2.3-x86_64/lib/liblsf.a -ldl -lnsl 
   greg cavanagh    
   12 December 2012
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "/opt/lsf/7.0/include/lsf/lsf.h"
#include "/opt/lsf/7.0/include/lsf/lsbatch.h"
#include "./resource.h"
#include "./dispatch_time.h"
#include "./print_queues.h"
#include "./hosts.h"
#include "./slots.h"


int main (argc, argv)
    int  argc;
    char *argv[];
{

    char* user=getenv("USER");
    int k;
    int total_users,user_position,user_slots;
     
    /* host partition variables */
    int  numHostParts = 1;
    char *hostParts[] = {"shared_hosts"};
    struct hostPartInfoEnt *hostPartStruct;

    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

    if (lsb_init(argv[0]) < 0) {
        lsb_perror("lsb_init()");
        exit(-1);
    }
    
    /* use structure to pass return values from print queues function */
    /* values referenced as run.xxxx, hosts.xxxx, shared.xxxxx */
    struct run_values run = print_queues();
    struct host_slots hslots = hosts();
    struct slot_info hmemory =slots();

  
    printf("\n\nHOST GROUP\t\tJOBS/SLOTS\tUTILIZATION\n");
    printf("-------------------------------------------------------------------\n");
    printf("%s\t\t%d/%d\t%0.4g\%\n","shared_hosts", run.shared,hslots.shared,((double)run.shared)/hslots.shared*100);
    printf("%s\t\t%d/%d\t%0.4g\%\n","hms_orchestra", run.total,hmemory.total_slots,((double)run.total)/hmemory.total_slots*100);

  /* fairshare host partition information */
  hostPartStruct = lsb_hostpartinfo(hostParts,&numHostParts);
  total_users=0;
  user_position=0;
  user_slots=0;

  /* find username in user string then print jobs and position */
  /* count total number of users */
  for (k=0; k < hostPartStruct[0].numUsers; k++){
    total_users++;

    if ( strstr(hostPartStruct[0].users[k].user,user) != 0){
      user_position=total_users;
      user_slots=hostPartStruct[0].users[k].numStartJobs;
    }

   
  }

    printf("\n\nAVAILABLE RESOURCES\n");

    printf("-------------------------------------------------------------------\n");
    printf("%s\t\t\t%d\n","Compute nodes:",hmemory.total_hosts);
    printf("%s\t%d\n","Nodes with 12 free slots:",hmemory.free_12);
    printf("%s\t%d\n","Nodes with 8  free slots:",hmemory.free_8);
    printf("%s\t%d\n","Nodes with 4  free slots:",hmemory.free_4);
    printf("%s\t%d\n","Nodes with NO free slots:",hmemory.free_0);
    printf("\n");

    printf("%s\t%d\n","Nodes with more than 64GB available memory:",hslots.sixtyfour);
    printf("%s\t%d\n","Nodes with more than 32GB available memory:",hslots.thirtytwo);
    printf("%s\t%d\n","Nodes with more than 16GB available memory:",hslots.sixteen);
    printf("%s\t%d\n","Nodes with LESS than 2 GB available memory:",hslots.less_than_two);
    printf("\n");
    printf("%s%d\n","Available Matlab Distributed Computing licences: ",get_matlab_resource() );
    printf("\n");
    printf("\n");

    dispatch_times();

    printf("FAIRSHARE\n");
    printf("-------------------------------------------------------------------\n");
    printf("There are currently %d users in the LSF fairshare table.\n",total_users);
    printf("User %s position is #%d\n",user,user_position);
    printf("User %s is currently using %d job slots\n",user, user_slots);
    printf("\n");
    printf("-------------------------------------------------------------------\n");
    printf("DISCLAIMER:  blah blah blah queues, priority, memory, unknown variables\n");
    printf("\n");
    printf("\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
 
    exit(0);

}
