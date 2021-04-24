#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

int array_size = 10;
int random_ints[10];
int thelindex = 0;
int therindex = 0;
int num_children = 0;
int kid_pids[1000];

void sig_handler_parent(int signum)
{
    printf("\nParent: My child has reached a 1 list\n");
    for(int i = 0; i < num_children; i++)
    {
        kill(kid_pids[i], SIGUSR2);
    }
    
    exit(0);
}

void sig_handler_child(int signum)
{
    printf("\nwe're done\n");
    exit(0);
    
}

void distribute(int lindex, int rindex)
{
    thelindex = lindex;
    therindex = rindex;

    if(lindex != rindex)
    {
        int m = lindex+(rindex-lindex)/2;
        int pid1 = fork();
        if(pid1 == 0)
        {
            signal(SIGUSR2, sig_handler_child);
            sleep(1);
            distribute(lindex, m);
            pause();
            
        }
        else
        {
            signal(SIGUSR2, sig_handler_parent);
            kid_pids[num_children] = pid1;
            num_children++;
            printf("\nPID: %d [", getpid());
            for(int i = lindex; i < rindex; i++)
            {
                printf("%d,", random_ints[i]);
            }
            printf("%d]",random_ints[rindex]);
            int pid2 = fork();
            if(pid2 == 0)
            {
                signal(SIGUSR2, sig_handler_child);
                sleep(1);
                distribute(m+1,rindex);
                pause();
            }
            else
            {
                signal(SIGUSR2, sig_handler_parent);
                kid_pids[num_children] = pid2;
                num_children++;
                pause();
                

            }
            
        }

    }
    else
    {
        printf("\nPID: %d [", getpid());
        for(int i = lindex; i <= rindex; i++)
        {
            printf("%d]\n", random_ints[i]);
        }
        kill(getppid(), SIGUSR2);
        exit(0);
    }
}


int main()
{
    time_t t;
    srand((unsigned) time(&t));
    
    for(int i = 0; i < array_size; i++)
    {
        random_ints[i] = rand() % 300 + 1;
    }
    distribute(0, (array_size - 1));
    return 0;

}
