#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "/opt/lsf/7.0/include/lsf/lsf.h"
#include "/opt/lsf/7.0/include/lsf/lsbatch.h"


static struct resItem * getResourceDef(char *); 
static struct lsInfo  * lsInfo; 
 

int get_matlab_resource() 
{ 
  struct lsSharedResourceInfo *resLocInfo; 
  int numRes = 0; 
  int i, j, k; 
 
  lsInfo = ls_info(); 
  if (lsInfo == NULL) { 
    ls_perror("ls_info"); 
    exit(-1); 
  } 
 
  resLocInfo = ls_sharedresourceinfo (NULL, &numRes, NULL, 0); 
 
  if (resLocInfo == NULL) { 
    ls_perror("ls_sharedresourceinfo"); 
    exit(-1); 
  } 
 

  for (k = 0; k < numRes; k++) { 
    struct resItem *resDef; 
    resDef = getResourceDef(resLocInfo[k].resourceName); 
    if (! (resDef->flags & RESF_DYNAMIC))  
      continue; 

    if (strcmp(resLocInfo[k].resourceName, "matlab_dc_lic") == 0){

	struct lsSharedResourceInstance *instance; 
	instance = &resLocInfo[k].instances[0]; 
	return(atoi(instance->value)); 

    } /* if matlab_lic */

  } /* for */ 

} /* get_matlab_resource */


static struct resItem * 
getResourceDef(char *resourceName) 
{ 
  int i; 
 
  for (i = 0; i < lsInfo->nRes; i++) { 
    if (strcmp(resourceName, lsInfo->resTable[i].name) == 0)  
      return &lsInfo->resTable[i]; 
  } 
 
  /* Fail to find the matching resource */ 
  fprintf(stderr, "Cannot find resource definition for            <%s>\n", resourceName); 
 
  exit (-1); 
 
} 
