#include "photosensitive_camera.h"

#include <sys/wait.h>
#include <unistd.h>
#include <wiringPi.h>

/*
#define DO 21
*/
namespace photosensitive_camera {

PhotoSentiveCamera::PhotoSentiveCamera(int int DO, bool trigerByA = false) {
    if (wiringPiSetup() == -1) {
        write(STDERR_FILENO, "Setup wiringPi failed\nexit now\n", 32);
        exit(-1);
    }
    this->do_ = DO;
    this->triger_mode_=trigerByA;
    pinMode(do_, INPUT);
    printf("A new SteerEngine:DO-%d\n", do_);
}

PhotoSentiveCamera::~PhotoSentiveCamera() {
    printf("PhotoSentiveCamera has been removed\n");
}

void PhotoSentiveCamera::WaitForDO(void (*func)(void*), void* arg) {
    if (triger_mode_) {
        while (1) {
            /*光照正常时，引脚为低电平，程序阻塞在这里*/
            while (digitalRead(do_) == 0);
            /*类似于消抖，或者说迟滞的作用，跳出循环时，等待0.1秒再去读引脚电平*/
            delay(100);
            /*如果延时后引脚还为高电平，跳出死循环，否则回到死循环*/
            if (digitalRead(do_) == 1) break;
        }
    } else {
        char triger = '\0';
        while (1) {
            /*阻塞等待用户键入a并回车后跳出死循环*/
            scanf("%c", &triger);
            if (triger == 'a')
                break;
            else
                continue;
        }
    }

    if (func != nullptr) {
        func(arg);
    }
    return;
}

int PhotoSentiveCamera::TakeAPic(const char* delay) {
    /*创建一个子进程，然后让子进程去执行/usr/bin/下的拍照程序raspistill*/
    int forkRet = fork();
    if (forkRet < 0) {
        perror("fork error");
        return -1;
    }

    /*子进程使用execl将其进程地址空间的内容替换为raspistill，后面跟的是参数*/
    if (forkRet == 0) {
        int execlRet = execl("/usr/bin/raspistill", "raspistill", "-o", "garbage.jpg", "-t", delay, nullptr);
        if (execlRet < 0) {
            perror("execl error");
            exit(-1);
        }
    } else {
        /*父进程，阻塞在这里，等待子进程退出并回收子进程*/
        int status=0;
        pid_t wpid=wait(&status);
        if(wpid==-1){
            perror("wait error");
            return -1;
        }

        /*子进程正常退出*/
        if(WIFEXITED(status)){
            /*子进程正常退出，且退出值正常*/
            if(WEXITSTATUS(status)==0){
                printf("Child process exit,he should take a picture named 'garbage.jpg'\n");
                return 0;
            }else{
                write(STDERR_FILENO,"Child process exit with -1\n",28);
                return -1;
            }
        }else{
        /*子进程非正常退出，可能被终止了*/
            write(STDERR_FILENO, "Child process unnormally\n",26)
            return -1;
        }
    }
}

}  // namespace photosensitive_camera
