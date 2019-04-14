#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>


#define BUF_LEN 80
#define ESC 27

char info_from_driver[9];
int file_desc;

void clear_screen(void);

volatile char done = 0x0;

void* function(void* param)
{    
    while(done != ESC)
    {	
	file_desc = open("/dev/gpio_driver", O_RDWR);

	if(file_desc < 0)
	{
	    printf("'/dev/gpio_driver' device isn't open\n");
	    return -1;
	}
	read(file_desc, info_from_driver, 8);
	clear_screen();
	printf("%s", info_from_driver);
	fflush(stdout);
	close(file_desc);
	nanosleep((const struct timespec[]){{0, 50000000L}}, NULL);
    }
      
    return NULL;
}

void* get_input(void* param)
{
    while(done != ESC)
    {
	done = getch();
    }
}

int main()
{
    pthread_t t;
    pthread_t quit_thread;

    pthread_create(&t, NULL, function, NULL);
    pthread_create(&quit_thread, NULL, get_input, NULL);
    
    //pthread_join(t, NULL);
    pthread_join(quit_thread, NULL);

    return 0;
}


void clear_screen(void)
{
    const char* CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
    write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}
