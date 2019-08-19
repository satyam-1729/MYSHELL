#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<time.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#define MAX 1024
#define RED     "\x1b[31m"
#define DEFAULT   "\x1b[0m"
#define GREEN   "\x1b[32m"
#define BLUE    "\x1b[34m"

char oldpwd[100];
char pwd[100];
char *course = "CS_744";
char *assign = "ASSIGNMENT_1";

int apply(int command_no,char* string)
{
	clock_t t;
	if(command_no==1)
	{
	int flag;
	char* p;
	p = strtok(NULL," ");
	setenv("OLDPWD",getcwd(oldpwd,100),1);
	t =clock();
	  flag=chdir(p);
	  t = clock()-t;
	  setenv("PWD",getcwd(pwd,100),1);
	  double fi = ((double)t)/CLOCKS_PER_SEC;
	  printf("Time:%f \n",fi);
	  if(flag !=0)
		  perror("ERROR:");
	  else
	  {
		  printf("OLDPWD:%s\n",getenv("OLDPWD"));
		  printf("PWD:%s\n",getenv("PWD"));
	  }
	  return 1;
	}
	else if(command_no ==2)
	{
		char* p;
		// p = strtok(string," ");
		p = strtok(NULL," ");
		char str[10] = "ls -al ";
		if(p!=NULL)
			strcat(str,p);
		t = clock();
		system(str);
		t = clock()-t;
		double fi = ((double)t)/CLOCKS_PER_SEC;
	    printf("Time:%f \n",fi);
		return 1;
	}
	else if(command_no== 3)
	{
		exit(0);
	}
	else if(command_no==4)
	{
		extern char **environ;
		int i;
		t = clock();
		for(i=0;environ[i] != NULL; i++)
			printf("%s\n",environ[i]);
		t=clock()-t;
		double ti = ((double)t)/CLOCKS_PER_SEC;
	    printf("Time:%f \n",ti);
	}
	else if(command_no ==5)
	{
		t = clock();
		printf("\e[1;1H\e[2J");
		t = clock()-t;
		double ti = ((double)t)/CLOCKS_PER_SEC;
	    printf("Time:%f \n",ti);
	}

}
int main(int argc,char* arg[])
{
	int length;
	FILE * fp;
	fp=NULL;
	clock_t t;
	char* command_list[5]={"cd","dir","quit","env","clr"};
	int i;
	system("clear");
	setenv("COURSE",course,0);
	setenv("ASSIGNMENT",assign,0);
	if(argc==2)
	{
		fp = fopen(arg[1],"r");
	}
	printf(   RED "  MY SHELL>          \n" DEFAULT);
	while(1)
	{
		char buf[1024];
        int indicator=0;
    	const char* s = getenv("USER");
    	char current_dir[100];
    	printf(GREEN "%s@%s $" DEFAULT ,s,getcwd(current_dir,100));
		int status;
		pid_t x;
    	char* command = (char *) malloc(MAX*sizeof(char));
        if(fp!=NULL)
        {
        	if(fgets (buf, 1024 , fp)!=NULL)
        	{
        	length=strlen(buf);
        	buf[length-1]=0;
        	strcpy(command,buf);
        	printf(BLUE "%s" DEFAULT "\n",command );
        	}
        	else{
        		fp=NULL;
        		continue;
        	}
        }
        else
	{
        	fgets(command,1024,stdin);
		length = strlen(command);
		command[length-1]=0;
	}
        x = waitpid(-1,&status,WNOHANG);
        if(x>0)
        	printf("background process is completed%d\n",x );


    	if(strcmp(command,"")!=0)
        {
         char* temp;
    	char org[1024];
		char orginal[1024];
    	char* arg1=NULL;
    	char* arg2=NULL;
    	char* arg3=NULL;
    	strcpy(org,command);
        strcpy(orginal,command);
    	temp = strtok(command," ");
     	for(i=0;i<5;i++)
    	{
	    	if(strcmp(temp,command_list[i])==0)
	    	{
		    	indicator=1;
		    	break;
	    	}
    	}

    	if(indicator==1)
		    apply(i+1,command);
    	else
    	{


		    	if(strstr(org,"&&&")!= NULL)
		    	{   
		    		if(strstr(org,">>")!=NULL)
		    		{
		    			pid_t pid;
		    			char *part;
		    			char *part1;
		    			char *part2;
		    			char *file;
		    			part = strtok_r(org,">>",&arg1);
		    			file = strtok_r(NULL,">>",&arg1);
		    			part1 = strtok_r(part,"&&&",&arg2);
		    			while(part1!=NULL)
		    			{
		    				part2 = strtok_r(part1," ",&arg3);
		    				char* temp = (char *) malloc(MAX*sizeof(char));
							strcpy(temp,"/bin/");
							strcat(temp,part2);
							char* temp2 = (char *) malloc(MAX*sizeof(char));
							strcpy(temp2,"/usr/bin/");
							strcat(temp2,part2);

							char *args[20];
		           			int index;
		            		args[0]=temp;
		  		   			index=1;
		  					part2 = strtok_r(NULL," ",&arg3);
		  					 while(part2 != NULL)
		        			 {
			        			args[index]=part2;
			  		 			index++;
			  		 			part2 = strtok_r(NULL," ",&arg3);
		            		 }
		  					args[index] = NULL;

		  					pid =fork();
		  					if(pid==0)
		  					{
		  						int fd;
		  						fd = open(file+1,O_CREAT | O_APPEND | O_RDWR,0600);
		  						close(STDOUT_FILENO);
		  						dup2(fd,STDOUT_FILENO);
		  						if(execvp(temp,args)<0)
			       				{
				       				args[0]=temp2;
				       				if(execvp(temp2,args)<0)
										perror("ERROR:");
			        			}
		  					}
		  					free(temp);
		  					free(temp2);
		  					part1 = strtok_r(NULL,"&&&",&arg2);

		    			}
		    			waitpid(pid,NULL,0);


		    		}
		    		else if(strstr(org,">")!=NULL)
		    		{
		    			pid_t pid;
		    			char *part;
		    			char *part1;
		    			char *part2;
		    			char *file;
		    			part = strtok_r(org,">",&arg1);
		    			file = strtok_r(NULL,">",&arg1);
		    			part1 = strtok_r(part,"&&&",&arg2);
		    			while(part1!=NULL)
		    			{
		    				part2 = strtok_r(part1," ",&arg3);
		    				char* temp = (char *) malloc(MAX*sizeof(char));
							strcpy(temp,"/bin/");
							strcat(temp,part2);
							char* temp2 = (char *) malloc(MAX*sizeof(char));
							strcpy(temp2,"/usr/bin/");
							strcat(temp2,part2);

							char *args[20];
		           			int index;
		            		args[0]=temp;
		  		   			index=1;
		  					part2 = strtok_r(NULL," ",&arg3);
		  					 while(part2 != NULL)
		        			 {
			        			args[index]=part2;
			  		 			index++;
			  		 			part2 = strtok_r(NULL," ",&arg3);
		            		 }
		  					args[index] = NULL;

		  					pid =fork();
		  					if(pid==0)
		  					{
		  						int fd;
		  						fd = open(file+1,O_CREAT | O_TRUNC | O_RDWR,0600);
		  						close(STDOUT_FILENO);
		  						dup2(fd,STDOUT_FILENO);
		  						if(execvp(temp,args)<0)
			       				{
				       				args[0]=temp2;
				       				if(execvp(temp2,args)<0)
										perror("ERROR:");
			        			}
		  					}
		  					free(temp);
		  					free(temp2);
		  					part1 = strtok_r(NULL,"&&&",&arg2);

		    			}
		    			waitpid(pid,NULL,0);
		    		}
		    		else if(strstr(org,"<")!=NULL)
		    		{}
		    		else
		    		{
		    			pid_t pid,stop;
		    		char *part;
			    	char *part2;
			    	part = strtok_r(org,"&&&",&arg1);
			    	while(part != NULL)
			    	{
					part2 = strtok_r(part," ",&arg3);
					char* temp = (char *) malloc(MAX*sizeof(char));
					strcpy(temp,"/bin/");
					strcat(temp,part2);
					char* temp2 = (char *) malloc(MAX*sizeof(char));
					strcpy(temp2,"/usr/bin/");
					strcat(temp2,part2);

					char *args[20];
		            int index;
		            args[0]=temp;
		  		    index=1;
		  			part2 = strtok_r(NULL," ",&arg3);
		            while(part2 != NULL)
		           {
			          args[index]=part2;
			  		  index++;
			  		  part2 = strtok_r(NULL," ",&arg3);
		            }
		  			args[index] = NULL;

					pid = fork();
					if(pid==0)
					{		
					if(execvp(temp,args)<0)
			        {
				       args[0]=temp2;
				       if(execvp(temp2,args)<0)
						perror("ERROR:");
			        }
					}

			        free(temp);
			        free(temp2);
			        part = strtok_r(NULL,"&&&",&arg1);

				  }
					while ((stop = wait(&status)) > 0);
		    		}
		    	}
		    	//end of &&&
		    	else if(strstr(org,"&&")!=NULL)
		    	{
			    	if(strstr(org,">>")!=NULL)
		    		{
		    			pid_t pid;
		    			char *part;
		    			char *part1;
		    			char *part2;
		    			char *file;
		    			part = strtok_r(org,">>",&arg1);
		    			file = strtok_r(NULL,">>",&arg1);
		    			part1 = strtok_r(part,"&&",&arg2);
		    			while(part1!=NULL)
		    			{
		    				part2 = strtok_r(part1," ",&arg3);
		    				char* temp = (char *) malloc(MAX*sizeof(char));
							strcpy(temp,"/bin/");
							strcat(temp,part2);
							char* temp2 = (char *) malloc(MAX*sizeof(char));
							strcpy(temp2,"/usr/bin/");
							strcat(temp2,part2);

							char *args[20];
		           			int index;
		            		args[0]=temp;
		  		   			index=1;
		  					part2 = strtok_r(NULL," ",&arg3);
		  					 while(part2 != NULL)
		        			 {
			        			args[index]=part2;
			  		 			index++;
			  		 			part2 = strtok_r(NULL," ",&arg3);
		            		 }
		  					args[index] = NULL;

		  					pid =fork();
		  					if(pid==0)
		  					{
		  						int fd;
		  						fd = open(file+1,O_CREAT | O_APPEND | O_RDWR,0600);
		  						close(STDOUT_FILENO);
		  						dup2(fd,STDOUT_FILENO);
		  						if(execvp(temp,args)<0)
			       				{
				       				args[0]=temp2;
				       				if(execvp(temp2,args)<0)
										perror("ERROR:");
			        			}
		  					}
		  					else
		  						waitpid(pid,NULL,0);
		  					free(temp);
		  					free(temp2);
		  					part1 = strtok_r(NULL,"&&",&arg2);

		    			}
		    		}

		    		if(strstr(org,">")!=NULL)
		    		{
		    			pid_t pid;
		    			char *part;
		    			char *part1;
		    			char *part2;
		    			char *file;
		    			part = strtok_r(org,">",&arg1);
		    			file = strtok_r(NULL,">",&arg1);
		    			part1 = strtok_r(part,"&&",&arg2);
		    			while(part1!=NULL)
		    			{
		    				part2 = strtok_r(part1," ",&arg3);
		    				char* temp = (char *) malloc(MAX*sizeof(char));
							strcpy(temp,"/bin/");
							strcat(temp,part2);
							char* temp2 = (char *) malloc(MAX*sizeof(char));
							strcpy(temp2,"/usr/bin/");
							strcat(temp2,part2);

							char *args[20];
		           			int index;
		            		args[0]=temp;
		  		   			index=1;
		  					part2 = strtok_r(NULL," ",&arg3);
		  					 while(part2 != NULL)
		        			 {
			        			args[index]=part2;
			  		 			index++;
			  		 			part2 = strtok_r(NULL," ",&arg3);
		            		 }
		  					args[index] = NULL;

		  					pid =fork();
		  					if(pid==0)
		  					{
		  						int fd;
		  						fd = open(file+1,O_CREAT | O_TRUNC | O_RDWR,0600);
		  						close(STDOUT_FILENO);
		  						dup2(fd,STDOUT_FILENO);
		  						if(execvp(temp,args)<0)
			       				{
				       				args[0]=temp2;
				       				if(execvp(temp2,args)<0)
										perror("ERROR:");
			        			}
		  					}
		  					else
		  						waitpid(pid,NULL,0);
		  					free(temp);
		  					free(temp2);
		  					part1 = strtok_r(NULL,"&&",&arg2);

		    			}
		    		}

		    		else
		    		{
		    			char *part;
			    	char *part2;
			    	part = strtok_r(org,"&&",&arg1);
			    	while(part != NULL)
			    	{
					part2 = strtok_r(part," ",&arg3);
					char* temp = (char *) malloc(MAX*sizeof(char));
					strcpy(temp,"/bin/");
					strcat(temp,part2);
					char* temp2 = (char *) malloc(MAX*sizeof(char));
					strcpy(temp2,"/usr/bin/");
					strcat(temp2,part2);

					char *args[20];
		            int index;
		            args[0]=temp;
		  		    index=1;
		  			part2 = strtok_r(NULL," ",&arg3);
		            while(part2 != NULL)
		           {
			          args[index]=part2;
			  		  index++;
			  		  part2 = strtok_r(NULL," ",&arg3);
		            }
		  			args[index] = NULL;

					int pid;
					pid = fork();
					if(pid==0)
					{		
					if(execvp(temp,args)<0)
			        {
				       args[0]=temp2;
				       if(execvp(temp2,args)<0)
						perror("ERROR:");
			        }
					}
					else
						waitpid(pid,NULL,0);

			        free(temp);
			        free(temp2);
			        part = strtok_r(NULL,"&&",&arg1);

				  }
		    		}

				}
       				// end of &&
	    		else if(strstr(org, "&")!=NULL)
		    	{
		    		if(strstr(org,">>")!=NULL)
		    		{
		    			pid_t pid;
		    			char *part;
		    			char *part1;
		    			char *part2;
		    			char *file;
		    			part = strtok_r(org,">>",&arg1);
		    			file = strtok_r(NULL,">>",&arg1);
		    			part1 = strtok_r(part,"&",&arg2);
		    			part2 = strtok_r(part1," ",&arg3);

		    			char libpath[100] = "/bin/";
						strcat(libpath,part2);
	     				char libpath2[100] = "/usr/bin/";
						strcat(libpath2,part2);
	    	       		char *args[20];
		        		int index;
		       			args[0]=libpath;
		        		index=1;
		        		char *p;
		        		p = strtok_r(NULL," ",&arg3);
		       			while(p != NULL)
		       			{
			   			args[index]=p;
			   			index++;
			   			p= strtok_r(NULL," ",&arg3);
		       			}
		      			args[index] = NULL;
		      			pid =fork();
			
		  				if(pid==0)
		  				{
				//printf("%ld\n",getpid());
		  						int fd;
		  						fd = open(file+1,O_CREAT | O_APPEND | O_RDWR,0600);
		  						close(STDOUT_FILENO);
		  						dup2(fd,STDOUT_FILENO);
		    	         if(execvp(libpath,args)<0)
		    	        {
						args[0]=libpath2;
						if (execvp(libpath2,args)<0)
								perror("ERROR:");
						}
						}
						else
				   			printf("%d\n",pid);
		    	}
		    		if(strstr(org,">")!=NULL)
		    		{
		    			pid_t pid;
		    			char *part;
		    			char *part1;
		    			char *part2;
		    			char *file;
		    			part = strtok_r(org,">",&arg1);
		    			file = strtok_r(NULL,">",&arg1);
		    			part1 = strtok_r(part,"&",&arg2);
		    			part2 = strtok_r(part1," ",&arg3);

		    			char libpath[100] = "/bin/";
						strcat(libpath,part2);
	     				char libpath2[100] = "/usr/bin/";
						strcat(libpath2,part2);
	    	       		char *args[20];
		        		int index;
		       			args[0]=libpath;
		        		index=1;
		        		char *p;
		        		p = strtok_r(NULL," ",&arg3);
		       			while(p != NULL)
		       			{
			   			args[index]=p;
			   			index++;
			   			p= strtok_r(NULL," ",&arg3);
		       			}
		      			args[index] = NULL;
		      			pid =fork();
			
		  				if(pid==0)
		  				{
				//printf("%ld\n",getpid());
		  						int fd;
		  						fd = open(file+1,O_CREAT | O_TRUNC | O_RDWR,0600);
		  						close(STDOUT_FILENO);
		  						dup2(fd,STDOUT_FILENO);
		    	         if(execvp(libpath,args)<0)
		    	        {
						args[0]=libpath2;
						if (execvp(libpath2,args)<0)
								perror("ERROR:");
						}
						}
						else
				   			printf("%d\n",pid);

		    		}
		    		else
		    		{
		    	pid_t pid;
				char *part;
				part = strtok_r(org,"&",&arg1);
				char *part2;
				part2 = strtok_r(part," ",&arg3);
		    		
				char libpath[100] = "/bin/";
				strcat(libpath,part2);
	     		char libpath2[100] = "/usr/bin/";
				strcat(libpath2,part2);
	    	        char *args[20];
		        int index;
		        args[0]=libpath;
		        index=1;
		        char *p;
		        p = strtok_r(NULL," ",&arg3);
		       while(p != NULL)
		       {
			   args[index]=p;
			   index++;
			   p= strtok_r(NULL," ",&arg3);
		       }
		      args[index] = NULL;
		      pid =fork();
			
		  		if(pid==0)
		  		{
				//printf("%ld\n",getpid());
		    	         if(execvp(libpath,args)<0)
		    	        {
				args[0]=libpath2;
				if (execvp(libpath2,args)<0)
					perror("ERROR:");
				}
				}
				else
				   printf("%d\n",pid);


		    		}
				
		    	}
		    	//end of &
		    	else
		    	{
		    		if (strstr(org,">>")!=NULL)
		    		{
		    			pid_t pid;
		    			char* part;
		    			char* file;
		    			char* part1;
		    			part=strtok_r(org,">>",&arg1);
		    			file=strtok_r(NULL,">>",&arg1);
		    			part1=strtok_r(part," ",&arg2);

		    			char libpath[100] = "/bin/";
						strcat(libpath,part1);
	     				char libpath2[100] = "/usr/bin/";
						strcat(libpath2,part1);
	    	        	char *args[20];
		        		int index;
		        		args[0]=libpath;
		        		index=1;
		        		char *p;
		        		p = strtok_r(NULL," ",&arg2);
		              	while(p != NULL)
		              	{
			   				args[index]=p;
			   				index++;
			   				p= strtok_r(NULL," ",&arg2);
		       			}
		      			args[index] = NULL;
		      			pid =fork();
		      			if(pid==0)
		  				{
				//printf("%ld\n",getpid());
		  						int fd;
		  						fd = open(file+1,O_CREAT | O_APPEND | O_RDWR,0600);
		  						close(STDOUT_FILENO);
		  						dup2(fd,STDOUT_FILENO);
		    	         if(execvp(libpath,args)<0)
		    	        {
						args[0]=libpath2;
						if (execvp(libpath2,args)<0)
								perror("command:");
						}
						}
						else
							waitpid(pid,NULL,0);
			
		    		}
		    		else if(strstr(org,">")!=NULL)
		    		{
		    			pid_t pid;
		    			char* part;
		    			char* file;
		    			char* part1;
		    			part=strtok_r(org,">",&arg1);
		    			file=strtok_r(NULL,">",&arg1);
		    			part1=strtok_r(part," ",&arg2);

		    			char libpath[100] = "/bin/";
						strcat(libpath,part1);
	     				char libpath2[100] = "/usr/bin/";
						strcat(libpath2,part1);
	    	        	char *args[20];
		        		int index;
		        		args[0]=libpath;
		        		index=1;
		        		char *p;
		        		p = strtok_r(NULL," ",&arg2);
		              	while(p != NULL)
		              	{
			   				args[index]=p;
			   				index++;
			   				p= strtok_r(NULL," ",&arg2);
		       			}
		      			args[index] = NULL;
		      			pid =fork();
		      			if(pid==0)
		  				{
				//printf("%ld\n",getpid());
		  						int fd;
		  						fd = open(file+1,O_CREAT | O_TRUNC | O_RDWR,0600);
		  						close(STDOUT_FILENO);
		  						dup2(fd,STDOUT_FILENO);
		    	         if(execvp(libpath,args)<0)
		    	        {
						args[0]=libpath2;
						if (execvp(libpath2,args)<0)
								perror("ERROR:");
						}
						}
						else
							waitpid(pid,NULL,0);
			

		    		}

		    		else if(strstr(org,"<")!=NULL)
		    		{

		    			pid_t pid;
		    			char* part;
		    			char* file;
		    			char* part1;
		    			part=strtok_r(org,"<",&arg1);
		    			file=strtok_r(NULL,"<",&arg1);
		    			part1=strtok_r(part," ",&arg2);

		    			char libpath[100] = "/bin/";
						strcat(libpath,part1);
	     				char libpath2[100] = "/usr/bin/";
						strcat(libpath2,part1);
	    	        	char *args[20];
		        		int index;
		        		args[0]=libpath;
		        		index=1;
		        		char *p;
		        		p = strtok_r(NULL," ",&arg2);
		              	while(p != NULL)
		              	{
			   				args[index]=p;
			   				index++;
			   				p= strtok_r(NULL," ",&arg2);
		       			}
		      			args[index] = file+1;
		      			args[++index] = NULL;
		      			pid =fork();

		      			if(pid==0)
		  		        {
		    	        if(execvp(libpath,args)<0)
		    			{
							args[0]=libpath2;
							if(execvp(libpath2,args)<0)
							{
								if(system(orginal)<0)
                                            perror("ERROR:");
							}
						
						}
						}
						else
							waitpid(pid,NULL,0);

		    		}
		    	    else
		    	    {
		    	    	char libpath[100] = "/bin/";
				strcat(libpath,temp);
	     		char libpath2[100] = "/usr/bin/";
				strcat(libpath2,temp);
	    	        char *args[20];
		        int index;
		        args[0]=libpath;
		        index=1;
		        char *p;
		        p = strtok(NULL," ");
		       while(p != NULL)
		       {
			   args[index]=p;
			   index++;
			   p= strtok(NULL," ");
		       }
		      args[index] = NULL;
		      int pid;
		      pid =fork();
		  		if(pid==0)
		  		{
		    	if(execvp(libpath,args)<0)
		    	{
				args[0]=libpath2;
				if(execvp(libpath2,args)<0)
					{
					if(system(orginal)<0)
					{perror("ERROR:");
					}
                             
					}
						
				}
				}
				else
					waitpid(pid,NULL,0);

		    	    }

		    	
		    	}




	}
        }
	free(command);


    
	}

return 0;
}
