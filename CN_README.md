# 1.小车驱动板硬件连接
![d442afaf809981fc043d6bcae772b21](https://github.com/HanyangZhong/Multi-agent-base/assets/119017394/8f8ac297-c808-4125-8f53-bb74e785f04c)
![821f4247668aef92b34aded4d3898d7](https://github.com/HanyangZhong/Multi-agent-base/assets/119017394/952c200e-f2a1-4eb7-bf70-e2648e03a5a4)


## 连接驱动板 TB220A1

正面拿驱动板，开关对着左手位置    
以此为正面    
![image](https://github.com/HanyangZhong/self-driving-high-precision-map-study/assets/119017394/e28d370d-d351-4139-bba7-9d6e07931518)

左边的电机是 PWMB 对应encoder1代码里面的right电机    PWMB正时，默认方向向前转    
右边的电机是 PWMA 对应encoder1代码里面的left电机      PWMA正时，默认方向向后转    

## 接线：
驱动板输入端接15v以内电源，最好12v    
对应电机是MG513P30_12V    

左边白色插槽连小车物理位置左电机    
右边白色插槽连小车物理位置右电机    
    
编码器IO：
```
GND -- arduino mega GND
E2A  -- arduino mega 19
E2B  -- arduino mega 18
E1A  -- arduino mega 21
E1B  -- arduino mega 20
5V  不接
```

![c3236ec6e28d0965030df91bf430cfb](https://github.com/HanyangZhong/Multi-agent-base/assets/119017394/cac0c97d-bebb-4227-b61d-17be73e3c035)
![7beadca7b2e9b9cac09d8948b6bce38](https://github.com/HanyangZhong/Multi-agent-base/assets/119017394/4dedd042-d5e0-4062-91e9-68017d839615)


![image](https://github.com/HanyangZhong/self-driving-high-precision-map-study/assets/119017394/764c4b5e-2b0f-4239-9a12-b7b1c36029ba)

驱动IO：        
```
PWMB -- arduino mega 10
BIN2  -- arduino mega 9
BIN1  -- arduino mega 8
STBY  -- arduino mega 7
AIN1 -- arduino mega 5
AIN2 -- arduino mega 4
PWMA -- arduino mega 3
```

# 2.arduino测试
## 1. 电机转动测试
用 TB6612motor这个arduino工程分别测试两个电机是否能正常转动        

## 2. 编码器测试
用 encoder1这个arduino工程分别测试两个编码器的读数        

```
// 电机1
//int motor_A = 21;//中端口是2
//int motor_B = 20;//中断口是3
//电机2
int motor_A = 18;//中端口是5
int motor_B = 19;//中断口是4

//使用其中一个的时候注释另一个
...


void setup() {
  Serial.begin(57600);//设置波特率  
  pinMode(motor_A,INPUT);
  pinMode(motor_B,INPUT);
//使用其中一个的时候注释另一个
//  attachInterrupt(2,count_A,CHANGE);//当电平发生改变时触发中断函数
//  //四倍频统计需要为B相也添加中断
//  attachInterrupt(3,count_B,CHANGE);

  attachInterrupt(5,count_A,CHANGE);//当电平发生改变时触发中断函数
  //四倍频统计需要为B相也添加中断
  attachInterrupt(4,count_B,CHANGE);
}

```

# 3.arduino bridge测试
通过已经配置好TB220A1的接线和IO口,可以直接烧写arduino bridge文件到mega 2560中,然后开始进行测试        
## 1.测试编码器转动
e是测试编码器数量        
r是reset编码器计数        
测试好左轮和右轮的左右以及正方向正确后可以进行下一步测试        

![1689542401212](https://github.com/HanyangZhong/self-driving-high-precision-map-study/assets/119017394/ce1a72d5-582a-4b7c-806a-bb5b705be93f)
![image](https://github.com/HanyangZhong/self-driving-high-precision-map-study/assets/119017394/a0212356-5c85-44a9-810f-185c1ec98e7d)

## 2.测试电机驱动
m是电机速度控制        
m 100 0 是左电机前向运动 100速度 1s        
然后设置 m 0 0        

![image](https://github.com/HanyangZhong/self-driving-high-precision-map-study/assets/119017394/e4f6d4d9-9224-4957-a49c-c3a394f598fc)


## 3.PID调试
在电机转动和编码器功能都测试完后,开始测试PID
在diff_controller.h下,更改测试PID参数        
一个个分开测试doPID左轮和右轮,记得在测试好后重新取消注释还原        
```

SetPointInfo leftPID, rightPID;

/* PID Parameters */
int Kp = 17;
int Kd = 13;
int Ki = 0;
int Ko = 50;

...

void doPID(SetPointInfo * p) {
  long Perror;
  long output;
  int input;

  //Perror = p->TargetTicksPerFrame - (p->Encoder - p->PrevEnc);
  //实时速度 = 当前编码器值 - 上次编码器结束值 （1/30秒）
  input = p->Encoder - p->PrevEnc;
  // 误差 = 目标值 - 当前速度值
  Perror = p->TargetTicksPerFrame - input;
    //打印 然后用串口绘图器测试调pid
//  Serial.println(input);/

....

//分别调试左右轮的pid
  //doPID(&rightPID);
  doPID(&leftPID);


```

## 4.ros arduino bridge yaml文件配置
在ros_arduino_bridge/ros_arduino_python/launch 里面是主要的启动文件        
其中my_arduino_params 是默认文件名称        
到config下复制改名,输入下面参数
```
# For a direct USB cable connection, the port name is typically
# /dev/ttyACM# where is # is a number such as 0, 1, 2, etc
# For a wireless connection like XBee, the port is typically
# /dev/ttyUSB# where # is a number such as 0, 1, 2, etc.

port: /dev/ttyACM0 #视情况设置，一般设置为 /dev/ttyACM0 或 /dev/ttyUSB0
baud: 57600 #波特率
timeout: 0.1 #超时时间

rate: 50
sensorstate_rate: 10

use_base_controller: True  #启用基座控制器
base_controller_rate: 10   

# For a robot that uses base_footprint, change base_frame to base_footprint
base_frame: base_footprint #base_frame 设置

# === Robot drivetrain parameters
wheel_diameter: 0.085 #车轮直径
wheel_track: 0.212    #0.244 -0.032 #轮间距
encoder_resolution: 1560 #编码器精度(一圈的脉冲数 * 倍频 * 减速比)
#gear_reduction: 1 #减速比
#motors_reversed: False #转向取反

# === PID parameters PID参数，需要自己调节
Kp: 17
Kd: 13
Ki: 0
Ko: 50
accel_limit: 1.0

# === Sensor definitions.  Examples only - edit for your robot.
#     Sensor type can be one of the follow (case sensitive!):
#      * Ping
#      * GP2D12
#      * Analog
#      * Digital
#      * PololuMotorCurrent
#      * PhidgetsVoltage
#      * PhidgetsCurrent (20 Amp, DC)

sensors: {
  #motor_current_left:   {pin: 0, type: PololuMotorCurrent, rate: 5},
  #motor_current_right:  {pin: 1, type: PololuMotorCurrent, rate: 5},
  #ir_front_center:      {pin: 2, type: GP2D12, rate: 10},
  #sonar_front_center:   {pin: 5, type: Ping, rate: 10},
  arduino_led:          {pin: 13, type: Digital, rate: 5, direction: output}
}

```

运行bridge的launch文件前，需要将ros_arduino_python/nodes/arduino_node.py下的文件设置可执行权限
```
sudo chmod +x arduino_node.py
```

# 4.分布式通信配置
## 0.wifi连接设置
1.硬件准备：使用显示屏或 HDMI采集卡连接树莓派并启动；        
2.为树莓派连接无线网络；        
3.为树莓派配置静态IP，具体操作如图        
![image](https://github.com/HanyangZhong/self-driving-high-precision-map-study/assets/119017394/c0083ae4-d1ba-4718-89dc-9b6a90a15a66)
![image](https://github.com/HanyangZhong/self-driving-high-precision-map-study/assets/119017394/3ae99666-860b-4e96-824a-8814bdb9ea72)
![image](https://github.com/HanyangZhong/self-driving-high-precision-map-study/assets/119017394/147a9526-c8c1-436a-9171-faf2131f6ab4)
![image](https://github.com/HanyangZhong/self-driving-high-precision-map-study/assets/119017394/5a1cef9e-d461-4407-88bf-8ebf778731a4)

## 1.系统文件配置
### 必要依赖安装
SSH实现架构上分为客户端和服务器端两大部分，客户端是数据的发送方，服务端是数据的接收方，当前场景下，我们需要从PC端发送数据到树莓派，那么PC端属于客户端，而树莓派属于服务端，整个实现具体流程是:        
1.分别安装SSH客户端与服务端        
2.服务端启动SSH服务        
3.客户端远程登陆服务端        
4.实现数据传输        
#### 1.安装SSH客户端与服务端
默认情况下，Ubuntu系统已经安装了SSH客户端，因此只需要在树莓派安装服务端即可(如果树莓派安装的是服务版的Ubuntu，默认会安装SSH服务并已设置成了开机自启动):        
```
sudo apt-get install openssh-server
```
如果客户端需要自行安装，那么调用如下命令:        
```
sudo apt-get install openssh-client
```
#### 2.服务端启动SSH服务
树莓派启动 ssh 服务:
```
sudo /etc/init.d/ssh start
```
启动后查看服务是否正常运行:        
```
ps -e | grep ssh
```
如果启动成功，会包含 sshd 与 ssh 两个程序。        
以后需要频繁的使用ssh登录树莓派，为了简化实现，可以将树莓派的ssh服务设置为开机自启动，命令如下:        
```
sudo systemctl enable ssh
```
#### 3.客户端远程登陆服务端
登陆树莓派可以调用如下命令:        
```
ssh 账号@ip地址
```
然后根据提示，录入登陆密码，即可成功登陆。        
如果退出登陆，可以调用exit命令:        
```
exit
```
#### 4.实现数据传输
上传文件:        
```
scp 本地文件路径 账号@ip:树莓派路径
```
上传文件夹:        
```
scp -r 本地文件夹路径 账号@ip:树莓派路径
```
下载文件:        
```
scp 账号@ip:树莓派路径 本地文件夹路径
```
下载文件夹:        
```
scp -r 账号@ip:树莓派路径 本地文件夹路径
```
#### 3.安装ros—arduino-bridge
ros_arduino_bridge是依赖于python-serial功能包的，请先在树莓派端安装该功能包，安装命令:        
```
sudo apt-get install python-serial
```
或        
```
sudo pip install --upgrade pyserial
```
或
```
sudo easy_install -U pyserial
```

### 配置文件
将树梅派或者jetson nano与电脑端连接到同一wifi网络下        
如PC:192.168.0.101        
如树梅派:192.168.0.110        
电脑端和嵌入式控制器端都设置静态IP,确保在同一网段下        
输入        
```
sudo gedit /etc/hosts
```
在从机和主机都要设置        
配置对方IP地址        
```
127.0.0.1	localhost
127.0.1.1	zhy
192.168.0.110	raspberrypi

# The following lines are desirable for IPv6 capable hosts
::1     ip6-localhost ip6-loopback
fe00::0 ip6-localnet
ff00::0 ip6-mcastprefix
ff02::1 ip6-allnodes
ff02::2 ip6-allrouters
```

然后配置.bashrc文件对应的ros主机和从机
输入`sudo gedit .bashrc`        
PC设置为从机,取主机树梅派地址为主                
```
export ROS_MASTER_URI=http://192.168.0.110:11311
export ROS_HOSTNAME=192.168.0.101
```
树梅派设置为主机,取主机树梅派地址为主                
```
export ROS_MASTER_URI=http://192.168.0.110:11311
export ROS_HOSTNAME=192.168.0.110
```

## 2.ssh连接控制
控制命令如:        
```
ssh -X wheeltec@192.168.0.110
```
输入密码,然后开始控制        

主机端运行roscore,从机端不需要运行roscore就能发送信息        
注意,所有命令行需要source .bashrc 否则不刷新环境变量        
主机端运行测试:        
```
roscore
rosrun turtlesim turtlesim_node 
```
从机端运行控制:
```
rosrun turtlesim turtlesim_node 
```
都测试通过可以接收消息后,可以开始测试arduino bridge控制

## 3.遥控小车测试
### 1.代码传输及编译
请先在树莓派端创建工作空间，在PC端进入本地工作空间的src目录，调用程序上传命令:        
```
scp -r ros_arduino_bridge/ 树莓派用户名@树莓派ip:~/工作空间/src
```
在树莓派端进入工作空间并编译:        
```
catkin_make
```
### 2.测试
现启动树莓派端程序，再启动PC端程序。        
#### 树莓派端
启动 ros_arduino_bridge 节点:        
```
roslaunch ros_arduino_python arduino.launch
```
#### PC端
启动键盘控制节点：        
```
rosrun teleop_twist_keyboard teleop_twist_keyboard.py
```
如无异常，现在就可以在PC端通过键盘控制小车运动了，并且PC端还可以使用rviz查看小车的里程计信息。        
