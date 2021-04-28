# 创新实验的详细记录

## 一、系统示意图

<img src="http://r.photo.store.qq.com/psc?/V52npUMi34iSk33hv9bD0cfpZY0kIx2o/45NBuzDIW489QBoVep5mcanzrTOGIktMxCecS8ju.G3AUZaXpk5oe4nQztgnNN7FdT8ROXYgqeS6vdOdrslO*OzIEuHzv9Ulgkqj9VRqDtk!/r"/>



## 二、树莓派与外设的连接和驱动

树莓派的引脚有不同的编码方式，由于我主要用的是C语言，所以用的wiringPi库，故应该使用WiringPi下的编码。

<img src="http://r.photo.store.qq.com/psc?/V52npUMi34iSk33hv9bD0cfpZY0kIx2o/45NBuzDIW489QBoVep5mce2u21c3eM.cSyXw.s9yquInY3T2DoaoT*nw0M.I56NU3yQAjdIf1daj6bcJjG2R.rKPUmHSY0607DEoCKLFaCQ!/r" style="zoom:80%;" />



引脚约定：

| 树莓派引脚（WiringPi Pin） | 外设引脚      | 导线颜色 |
| -------------------------- | ------------- | -------- |
| 22                         | ULN2003_IN1   | 橙色     |
| 23                         | ULN2003_IN2   | 黄色     |
| 24                         | ULN2003_IN3   | 绿色     |
| 25                         | ULN2003_IN4   | 蓝色     |
| 21                         | 光敏模块的DO  | 黄色     |
| 4                          | HC-SR04_Trig  | 黄色     |
| 5                          | HC-SR04_Echo  | 绿色     |
| 26                         | 舵机的PWM输入 | 橙色     |

### 1-光敏电阻触发相机拍照

对应文件：

- `photosensitive_camera.cpp`和`photosensitive_camera.h`

功能：

- 当人把垃圾放到托盘上后，相机自动拍照，得到垃圾的照片。

实现原理：

- 当光照正常时，光敏电阻模块DO输出为0，一旦被遮挡住（人把垃圾放上去），DO输出为1。利用死循环将程序阻塞在DO==0上，并在后面进行消抖判断。
- 人把垃圾放上去，光敏电阻输出DO==1后，跳出死循环，进行拍照。
- 由于我并没有找到如何用C语言控制树莓派拍照的函数调用（可能并没有），但是有控制树莓派拍照的shell命令，故采取了一种相对折中的做法：fork出一个子进程，然后用execl函数替换掉子进程的进程映像为`/usr/bin/raspistill`（控制树莓派拍照的shell命令），后面加上合适的参数，这样就相当于在程序中执行shell命令，实现了拍照功能。

### 2-步进电机驱动模块

对应文件：

- `step_motor.cpp`和`step_motor.h`

功能：

- 得到垃圾类型（映射为4个整数）后，控制步进电机带动四个桶转动一定角度，使对应的垃圾类型的桶对准托盘下方。

实现原理：

- 向ULN2003的IN1~IN4分时写入对应的电平即可，具体见步进电机的驱动原理。另外，步进电机最好用12V驱动（3根锂电池串联，与信号共地），扭矩大，转速高，又快又猛。

### 3-舵机驱动模块



### 4-超声波测距模块HC-SR04

对应文件：

- `ultrasonic_ranging.cpp`和`ultrasonic_ranging.h`

功能：

- 测距。

实现原理：

- 参考源码和HS-SR04的原理



## 三、图像识别部分

对应文件：

- `waste_sorting.cpp`和`waste_sorting.h`

### 1-实现思路概述

When it comes to 图像识别，首先想到的肯定是深度学习，但是我并不会，只懂个其中的皮毛，所以采用了一种更为简单的解决方案：使用<a href="https://ai.baidu.com">BaiduAI开放平台</a>提供的图像识别接口同时配合<a href="https://www.tianapi.com/">天行数据</a>提供的垃圾分类查询接口实现图像识别并分类。因此这部分的功能就是读入一张垃圾的图片，输出它属于什么垃圾（用一个整数表示）。

代码实现在`waste_sorting.cpp`文件中，下面结合头文件`waste_sorting.h`中的函数声明简单说明其原理：

```cpp
#ifndef __WASTE_SORTING_H__
#define __WASTE_SORTING_H__

#include <vector>
#include <string>
#include <iostream>

/*
   获取垃圾类型的函数：传入识别出来的物体种类（包含所有可能结果的string数组），返回其中最有可能所属的垃圾类型，约定垃圾类型如下：

   0-可回收
   1-有害
   2-厨余(湿)
   3-其他(干)

   -1表示出错，并打印到stderr中
*/
int getGarbageCategoryByNames(std::vector<std::string>* objectNames);
int getGarbageNamesByImage(const char* fileName, std::vector<std::string>* objectNames);

#endif
```

函数`getGarbageNamesByImage`的第一个参数`fileName`是图片的文件名，这个函数会调用BaiduAI的图像识别SDK中的`aip::Imageclassify`类下的`advanced_general`方法来获取图像中的物体类型，返回结果是一个JSON对象，格式如下（我输入了一张蓄电池的图片）：

```json
{
	"error_code" : null,
	"log_id" : 1376707039807406080,
	"result" : 
	[
		{
			"keyword" : "蓄电池",
			"root" : "商品-电子原器件",
			"score" : 0.82018500000000005
		},
		{
			"keyword" : "车载电源",
			"root" : "商品-机器设备",
			"score" : 0.58906800000000004
		},
		{
			"keyword" : "包装袋/盒",
			"root" : "非自然图像-图像素材",
			"score" : 0.38386300000000001
		},
		{
			"keyword" : "冷冻油",
			"root" : "商品-原材料",
			"score" : 0.2089
		},
		{
			"keyword" : "电能表",
			"root" : "商品-仪表",
			"score" : 0.039449999999999999
		}
	],
	"result_num" : 5
}
```

相关字段含义应该很清楚，可参阅api文档。

接下来解析这个JSON对象，拿到所有的"keyword"（实验中发现"score"最高结果的也可能是错的，出于容错性的考虑，所有的"keyword"都带走），存到第二个参数`objectNames`指向的一个字符串数组中去（传出参数）。这样函数调用者就能拿到图片中物体的所有可能类型了。

接下来将`objectNames`传给函数`getGarbageCategoryByNames`，它会将其中的每个字符串一一取出，写到post请求的参数中去，然后逐一向天行数据api发送HTTPpost请求查询这个物体的垃圾类型，一旦查询成功，就break，返回垃圾类型。四种垃圾类型用四个整数表示，映射关系在上面的注释里。

### 2-BaiduAI开放平台的使用

#### 1-开发包使用步骤

1. 在[官方网站](http://ai.baidu.com/sdk)下载C++ SDK压缩包。
2. 将下载的`aip-cpp-sdk-version.zip`解压，其中文件为包含实现代码的头文件。
3. 安装依赖库libcurl（需要支持https），openssl，jsoncpp(>1.6.2版本，0.x版本将不被支持)。
4. 编译工程时添加 C++11 支持 (gcc/clang 添加编译参数 -std=c++11)，添加第三方库链接参数 lcurl, lcrypto, ljsoncpp。
5. 在源码中include imageclassify.h ，引入压缩包中的头文件以使用aip命名空间下的类和方法。

#### 2.安装相关的库

如果库的安装或使用有错误，直接给树莓派重装系统吧。

###### libcurl库

这是一个可以发送很多网络请求的C库，利用它可以发送http请求，模拟浏览器的行为；

安装方法：

首先到官网下载源码包：https://curl.haxx.se/download.html

解压、进入主目录进行安装（经典三部曲）：

```bash
tar -zxvf curl-7.51.0.tar.gz
 
sudo ./configure
 
sudo make
 
sudo make install
```

头文件目录：

<img src="http://r.photo.store.qq.com/psc?/V52npUMi34iSk33hv9bD0cfpZY0kIx2o/45NBuzDIW489QBoVep5mcT64CO8B1yC8SdhkTxeJnfaWL8Hn1pKHNPGTafFqXu9HMY9deIPaW5LlP032PuwQlcNLGQ0VRNtE1TrgJFBiFf0!/r" align="left"/>

动态库目录：

<img src="http://r.photo.store.qq.com/psc?/V52npUMi34iSk33hv9bD0cfpZY0kIx2o/45NBuzDIW489QBoVep5mcT64CO8B1yC8SdhkTxeJnfaHslxq4faTN4yyGn9wdCKW2q764ohSU1E0lCzklEzjboZSe7TNcAQ1gBGTPV7iR.8!/r" align="left"/>



关于如何使用curl库，这里有详细的视频教程：https://www.bilibili.com/video/BV1o7411w75c



###### jsoncpp库

这是用C++编写的json库，可以用它来解析json字符串，操纵json对象等；

安装方法：直接命令`sudo apt-get install libjsoncpp-dev`

头文件目录：

<img src="http://r.photo.store.qq.com/psc?/V52npUMi34iSk33hv9bD0cfpZY0kIx2o/45NBuzDIW489QBoVep5mcRqE0kWwQAq2brZ63uQun2H679HyocvIkCFInejJtyhkcp*jWRtKzKG4p5ZCX4J*YuBpxvIYG1tEGZta9yCW99U!/r" align="left"/>

动态库目录：

<img src="http://r.photo.store.qq.com/psc?/V52npUMi34iSk33hv9bD0cfpZY0kIx2o/45NBuzDIW489QBoVep5mcRqE0kWwQAq2brZ63uQun2Hn37FYb8kBfij2w6hvRYjZ.tvoKVl*oYlzAKjGmr.VR3whq.jYHmIwIfCCExSUJBI!/r" align="left"/>



有一说一，他这个BaiduAI提供的SDK中json头文件引用的目录是错的，他直接`#include<json/json.h>`，少了一层jsoncpp，在前面都加上就好了。顺便记录一下gcc的头文件搜索顺序：

- 由参数-I指定的路径(指定路径有多个路径时，按指定路径的顺序搜索)
- 然后找gcc的环境变量 C_INCLUDE_PATH, CPLUS_INCLUDE_PATH, OBJC_INCLUDE_PATH
- 再找内定目录
  `/usr/include`
  `/usr/local/include`
  `/usr/lib/gcc-lib/i386-linux/2.95.2/include`
  `/usr/lib/gcc-lib/i386-linux/2.95.2/../../../../include/g++-3`
  `/usr/lib/gcc-lib/i386-linux/2.95.2/../../../../i386-linux/include`



###### openssl库

负责https的吧，跟安全有关；

安装方法：

```bash
sudo apt-get install openssl

sudo apt-get install libssl-dev
```

头文件目录：<img src="http://r.photo.store.qq.com/psc?/V52npUMi34iSk33hv9bD0cfpZY0kIx2o/45NBuzDIW489QBoVep5mcRqE0kWwQAq2brZ63uQun2F4LoocHbggT4mZKbtuAGjusy*BJavicduEi85phvJyy67gOloZVQEMUAdLAlpOiXo!/r" align="left"/>







#### 3-图像识别部分源码



## 四、服务端部分

这部分不是跑在树莓派上的，是跑在本地机器，或者正儿八经服务器上的（我没有域名和公网IP，只在局域网里玩玩），**TCPServer**和**WebServer**这两个服务程序相配合：TCPServer接收来自树莓派端封装了**垃圾桶最新使用余量状态信息（usage）**的报文，启动一个线程，将其解析为JSON对象，并写入到`allTrashesInfo.json`文件中去。

WebServer在前端js脚本调用<a href="http://lbsyun.baidu.com/">百度地图api</a>，负责向用户在地图上展示垃圾桶的**位置**和**usage**以及**是否已满**，后端基于NodeJS实现，很简单。

### 1-目录结构

![image-20210426184750631](/home/daniel/InnovationLab/Pic/image-20210426184750631.png)

### 2-目录中各个文件作用

#### 1-allTrashesInfo.json

这个文件存储了一个json数组（对象），里面的每个对象都包装了一个垃圾桶的信息，格式是这样的：

```json
   {
      "id" : 0,
      "location" : [ 120.3502, 30.3202 ],
      "recyleBitMap" : 1,
      "usage" : [ 0.2999, 0.5, 0.1000, 0.9000 ]
   }
```

各个键值对儿的解读：

- id：垃圾桶的编号。
- location：垃圾桶的GPS定位，考虑到成本，并不打算给垃圾桶安装GPS模块，每当移动位置时修改这个垃圾桶里的GPS参数即可。
- recyleBitMap：一个四位的bitmap，每个二进制位标志着对应的垃圾桶满了，从高位到低位依次对应“可回收”，“有害”，“厨余”，“其他”。
- usage：一个trash的每个subtrash的用量情况，从0到1共分11个分度，当大于等于0.9时对应的recyleBitMap置1，意味着此subtrash需要被回收。出现一长串数字的原因是jsoncpp库对小数的解析有点小问题，不能很好的表示非(1/2)^n的浮点数。

#### 2-TCPServer

- make.sh是一个编译TCPServer.cpp的一个shell脚本，懒得写makefile了，里面就一行编译命令。

- TCPServer是编译出来的elf可执行文件。

- TCPServer.cpp是TCP服务器源码，这个TCP服务器的作用是接受来自垃圾桶（树莓派）发过来的一段TCP报文，里面的信息是上面json文件中的那个对象，它包含了这个垃圾桶最新的状态。服务器收到这个报文后，按照其中的内容更新allTrashesInfo.json文件（读入-修改-写回）。服务器程序是**多线程**的，每当有一个树莓派建立连接都会pthread_create一个子线程去接收报文更新文件。由于各个线程共享打开的文件，文件属于临界资源，出于**线程安全**的考虑，应用**锁**的机制**对线程访问文件时上锁**，保证每个线程有序更改文件，原理示例如下：

  ```cpp
  /*进入临界区，加锁*/
  pthread_mutex_lock(&mutex);
  
  /*临界区：操作文件*/
  
  pthread_mutex_unlock(&mutex);
  /*退出临界区，解锁*/
  ```

  

- 剩下的两个wrap文件是把用于网络通信的系统调用函数进行了**错误处理封装**，提供给服务器程序使用，不赘述。

- StressTest.sh是**测试TCPServer程序**用的，测试时在一两秒内迭代了请求512次，还挺快的。

#### 3-WebServer

这是一个提供给管理者（环卫部门）或者用户的Web服务器，用于在地图上展示垃圾桶的使用情况，效果如下：

<img src="http://r.photo.store.qq.com/psc?/V52npUMi34iSk33hv9bD0cfpZY0kIx2o/45NBuzDIW489QBoVep5mcT64CO8B1yC8SdhkTxeJnfa*JFw*ZfYwJ*xSKBh3ff**CiJf0t8QUrlENdN7ot*6xeb9pHi9mWtGs2HIobibyX0!/r"/>

界面很丑不要在意，毕竟直男审美。

这部分比较简单：

- img目录存放了一个favicon.ico图标和16张垃圾桶的图片。在地图上显示不同subtrash满的原理就是bitmap，上面提到过，4个binary位从高到低分别是“可回收（蓝色）”，“有害（红色）”，“厨余（绿色）”，“其他（黄色）”。比如说都没满就是0H（0D，0000B），表现在图上就是四个空条；都满了就是FH（16D，1111B），表现在图上就是“蓝红绿黄”四个条；厨余垃圾和有害满了就是6H（6D，0110B），表现在图上就是红条和绿条，以此类推。
- index.html是主页代码，没啥东西，body里就一个div容器。
- map.js是index.html页面的脚本文件，主要负责发送ajax请求获取allTrashesInfo中的json对象，然后创建一张地图，最后根据allTrashInfo中的垃圾桶信息挑选合适的垃圾桶图标绑定上click事件放地图上去，具体见源码。
- http.js是基于nodejs的一个后端程序，功能很简单，就是前端请求啥发给它啥，没啥大逻辑，几个if-else，具体见源码。

### 3-服务端源码

https://github.com/Daniel741312/InnovationLab/tree/master/Servers

## 五、开发环境

主机系统是**Ubuntu18.04**；

树莓派跑的官方的是**Debian**系统；

一般用**VSCode**通过**ssh**连接到树莓派上编辑（VSCode天下第一）；

