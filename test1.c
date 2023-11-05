#include "test1.h"
typedef void (*sighandler_t)(int);
const int times = 4;
int main(int argc, char *argv[])
{
    // 如果在命令行上没输入子进程要执行的命令 //则执行缺省的命令

    int i;
    int pid1, pid2;                       // 存放子进程号
    int status;                           // 存放子进程返回状态
    char *ls[] = {"/bin/ls", "-a", NULL}; // 子进程要缺省执行的命令
    char *ps[] = {"/bin/ps", NULL};
    signal(SIGINT, (sighandler_t)sigcat); // 注册一个本进程处理键盘中断的函数
    perror("SIGINT");                     // 如果系统调用signal成功执行，输出”SIGINT”，否则，
    // 输出”SIGINT”及出错原因

    for (i = 1; i < times; i++)
    {
        pid1 = fork();
        if (pid1 < 0) // 建立子进程失败?
        {
            printf("Create Process fail!\n");
            exit(EXIT_FAILURE);
        }
        if (pid1 == 0) // 子进程执行代码段
        {
            // 报告父子进程进程号
            printf("I am FirstChild process %d\nMy father is %d\n", getpid(), getppid());
            pause(); // 暂停，等待键盘中断信号唤醒
            // 子进程被键盘中断信号唤醒继续执行help
            printf("%d child will Running: \n", getpid());

            for (i = 0; ls[i] != NULL; i++)
                printf("%s ", ls[i]);
            printf("\n");
            // 装入并执行新的程序
            char
                status = execve(ls[0], ls, NULL);
        }
        else // 父进程执行代码段
        {
            pid2 = fork();
            if (pid2 < 0) // 建立子进程失败?
            {
                printf("Create Process fail!\n");
                exit(EXIT_FAILURE);
            }
            if (pid2 == 0)
            {
                printf("I am SecChild process %d\nMy father is %d\n", getpid(), getppid());
                pause(); // 暂停，等待键盘中断信号唤醒
                // 子进程被键盘中断信号唤醒继续执行help
                printf("%d child will Running: \n", getpid());
                for (i = 0; ps[i] != NULL; i++)
                    printf("%s ", ps[i]);
                printf("\n");
                // 装入并执行新的程序
                char
                    status = execve(ps[0], ps, NULL);
            }
            else//父进程
            {
                printf("\nI am Parent process	%d\n", getpid()); // 报告父进程进程号
                // 如果在命令行上没输入子进程要执行的命令
                // 唤醒子进程，与子进程并发执行不等待子进程执行结束
                sleep(3); // 思考：如果去掉这条语句，可能会出现什么现象

                if (kill(pid2, SIGINT) >= 0)
                    printf("%d Wakeup %d SecChild.\n", getpid(), pid2);
                waitpid(pid2, &status, 0);
                printf("\nMy SecChild exit! status = %d\n\n", status);


                if (kill(pid1, SIGINT) >= 0)
                    printf("%d Wakeup %d Firstchild.\n", getpid(), pid1);
                waitpid(pid1, &status, 0);
                printf("\nMy FirstChild exit! status = %d\n\n", status);
            }
        }
        sleep(1);
    }
    return EXIT_SUCCESS;
}
