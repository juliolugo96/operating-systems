#include <stdio.h>
#include <utils.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void smooth_flush(char words[])
{
    for (int i = 0; i < strlen(words); i++)
    {
        usleep(50000);
        putchar(words[i]);
        fflush(stdout);
    }
}

void usage()
{
    char words[] = "\t\t\t*********Usage**********\n\n\n\n";
    smooth_flush(words);
    char menu[] = "\tPlease choose an option to start your simulations tests\n\n";
    smooth_flush(menu);
    char options[] = "\t[1]\tSync with Critical Section --> ./main.exe 1 [mean value]\n\n\t[2]\tSync with Client Server --> ./sim.exe 2 [mean value]\n\n";
    smooth_flush(options);
}

void successful_exec()
{
    char words[] = "\t\tNice simulation, uh? Hope we see each other again!\n\n\n";
    smooth_flush(words);
}

void failed_exec()
{
    char words[] = "\t\tDamn! It didn't work?! Nowadays programmers suck!\n\n\n";
    smooth_flush(words);
}

int init(int selection, float mean)
{
    while (true)
    {
        if (selection == EXIT)
            return 0;

        int code = handle_option_selected(selection, mean);

        if (code == SUCCESS_CODE)
        {
            successful_exec();
            return SUCCESS_CODE;
        }
            
        else
        {
            failed_exec();
            return ERROR_CODE;    
        }
    }
}

int main(int argc, char *argv[])
{
    enum sync_case menu;

    if (argc != 3)
    {
        usage();
        return -1;
    }

    srand(time(NULL));

    init(atoi(argv[1]), atof(argv[2]));

    return 0;
}
