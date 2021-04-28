#include <stdio.h>
#include <unistd.h>
#include <vector>

#include "photosensitive_camera.h"
#include "steering_engine.h"
#include "step_motor.h"
#include "tcp_send.h"
#include "ultrasonic_ranging.h"
#include "waste_sorting.h"

/*测试函数*/
static void photosensitive_camera_test();
static void steering_engine_test();
static void step_motor_test();
static void tcp_send_test();
static void ultrasonic_ranging_test();
static void waste_sorting_test();

int main(int argc, char* argv[]) {
    photosensitive_camera_test();
    steering_engine_test();
    step_motor_test();
    tcp_send_test();
    ultrasonic_ranging_test();
    waste_sorting_test();
    return 0;
}

static void photosensitive_camera_test(){
    photosensitive_camera::PhotoSentiveCamera* psc0=new photosensitive_camera::PhotoSentiveCamera(21);
    psc0->WaitForDO(nullptr,nullptr);
    psc0->TakeAPic("100");

    delete(psc0);
    psc0=nullptr;

    photosensitive_camera::PhotoSentiveCamera* psc1=new photosensitive_camera::PhotoSentiveCamera(21,true);
    int a=9427;
    psc0->WaitForDO(foo,&a);
    delete(psc1);

    return;
}

static void foo(void* arg){
    printf("foo:%d",*static_cast<int*>(arg));
}

static void steering_engine_test(){
    steering_engine::SteerEngine* se0=new steering_engine::SteerEngine(26);

    se0->RotateByAngle(45);
    se0->RotateByAngle(90);
    se0->RotateByAngle(135);
    se0->RotateByAngle(180);

    delete(se0);
    se0=nullptr;
    return;
}

static void step_motor_test() {
    step_motor::StepMotor* sm0=new step_motor::StepMotor(22,23,24,25,4);
    
    sm0->RotateClockwise(512);
    sm0->Stop();
    sm0->RotateAntiClockwise(512);
    sm0->Stop();

    sm0->interval_=2;
    sm0->setIN1_IN4(22,23,24,25);
    sm0->RotateClockwise(512);
    sm0->Stop();
    sm0->RotateAntiClockwise(512);
    sm0->Stop();

    delete(sm0);
    sm0=nullptr;
    return;
}

static void tcp_send_test() {
    tcp_send::TCPClient* tcpclient0=new tcp_send::TCPClient("127.0.0.7",9526);

    tcpclient0->setBufferSize(128);
    printf("bufferSize=%d\n",tcpclient0->getBufferSize());

    std::string strReturn="";
    std::string strSend="{\"id\":6,\"recyleBitMap\":14,\"location\":[120.3532,30.3242],\"usage\":[0.9, 1, 1, 0.5]}";
    int ret=tcpclient0->SendStrViaTCP(strSend,&strReturn);

    printf("strReturn=%s\n",strReturn.c_str());

    delete(tcpclient0);
    tcpclient0=nullptr;
    return;
}

static void ultrasonic_ranging_test() {
    ultrasonic_ranging::UltraSonicRanging* usr0=new ultrasonic_ranging::UltraSonicRanging(4,5);

    for (int i = 0; i < 10; ++i) {
        float dis = usr0->DisMeasure();
        printf("dis=%f", dis);
        sleep(1);
    }

    delete(usr0);
    usr0=nullptr;
    return;
}

static void waste_sorting_test(){
    waste_sorting::WasteSorting* ws0=new waste_sorting::WasteSorting();
    std::vector<std::string>* objectsNames=new std::vector<std::string>;
    ws0->getGarbageNamesByImage("garbage.jpg",objectsNames);
    int type=ws0->getGarbageCategoryByNames(objectsNames);
    printf("%s:type=%d\n",__func__,type);

    delete(ws0);
    ws0=nullptr;
    return;
}
