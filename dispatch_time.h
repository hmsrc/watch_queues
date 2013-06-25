void dispatch_times()
{

  FILE *myfile;            /* declare the file pointer */
  int i = 0;    
  int line_index = 0;     
  char *p;    
  char queue[80],dispatch_month[10],dispatch_time[8];


  int queue_time;

  myfile = fopen ("/scratch/last_dispatch_test.txt", "rt");  /* open the file for reading */
  fscanf(myfile,"%s %s %s\n",queue,&dispatch_month,&dispatch_time);

  printf("%s\t%s %s\n","Last queue dispatch test was:",dispatch_month,dispatch_time);  
  printf("-------------------------------------------------------------------\n");
  printf("%s\t\t%s\n","QUEUE","TIME (seconds)");

  while( (i = fscanf(myfile,"%s %d\n",queue,&queue_time)) != EOF ) //loop to read the file
    {
      printf("%-15s\t%d\n",queue,queue_time);
    } 

  fclose(myfile);
  printf("\n");
}





