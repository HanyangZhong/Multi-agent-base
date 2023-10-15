# 1.Hardware connection of the burgerbot drive board
![d442afaf809981fc043d6bcae772b21](https://github.com/HanyangZhong/Multi-agent-base/assets/119017394/8f8ac297-c808-4125-8f53-bb74e785f04c)
![821f4247668aef92b34aded4d3898d7](https://github.com/HanyangZhong/Multi-agent-base/assets/119017394/952c200e-f2a1-4eb7-bf70-e2648e03a5a4)


## Connecting the drive board TB220A1

Take the driver board from the front, with the switch facing the left hand position        
Take this as a positive aspect        
![image](https://github.com/HanyangZhong/self-driving-high-precision-map-study/assets/119017394/e28d370d-d351-4139-bba7-9d6e07931518)

The motor on the left is the right motor 'PWMB' timing in the encoder1 code corresponding to 'PWMB', with a default direction of forward rotation
The motor on the right is the left motor 'PWMA' timing in the encoder1 code corresponding to 'PWMA', with the default direction turning backwards

## Wire Connection：
The input terminal of the drive board is connected to a power supply of less than 15V, preferably 12V        
The corresponding motor is 'MG513P30_ 12V'        
        
Left white slot connected to the physical position of the car, left motor        
Right white slot connected to the physical position of the car Right motor        
        
EncoderIO：        
```
GND -- arduino mega GND
E2A  -- arduino mega 19
E2B  -- arduino mega 18
E1A  -- arduino mega 21
E1B  -- arduino mega 20
5V  None
```

![c3236ec6e28d0965030df91bf430cfb](https://github.com/HanyangZhong/Multi-agent-base/assets/119017394/cac0c97d-bebb-4227-b61d-17be73e3c035)
![7beadca7b2e9b9cac09d8948b6bce38](https://github.com/HanyangZhong/Multi-agent-base/assets/119017394/4dedd042-d5e0-4062-91e9-68017d839615)


![image](https://github.com/HanyangZhong/self-driving-high-precision-map-study/assets/119017394/764c4b5e-2b0f-4239-9a12-b7b1c36029ba)

DriverBoardIO：        
```
PWMB -- arduino mega 10
BIN2  -- arduino mega 9
BIN1  -- arduino mega 8
STBY  -- arduino mega 7
AIN1 -- arduino mega 5
AIN2 -- arduino mega 4
PWMA -- arduino mega 3
```

# 2.Arduino wire connection Test
## 1. Motor rotation test
Test whether the two motors can rotate normally using the 'TB6612motor' Arduino project          

## 2. Encoder testing
Use 'encoder1' as the Arduino project to test the readings of two encoders separately        

```
// motor1
//int motor_A = 21;//Interrupt port is 2
//int motor_B = 20;//Interrupt port is 3
//motor2
int motor_A = 18;//Interrupt port is 5
int motor_B = 19;//Interrupt port is 4

//Annotate the other when using pairs
...


void setup() {
  Serial.begin(57600);//
  pinMode(motor_A,INPUT);
  pinMode(motor_B,INPUT);
//
//  attachInterrupt(2,count_A,CHANGE);//Trigger interrupt function when level changes
//  //Quadruple frequency statistics require adding interrupts for phase B as well
//  attachInterrupt(3,count_B,CHANGE);

  attachInterrupt(5,count_A,CHANGE);//Trigger interrupt function when level changes
  //Quadruple frequency statistics require adding interrupts for phase B as well
  attachInterrupt(4,count_B,CHANGE);
}

```

# 3.Arduino Bridge Test
By configuring the wiring and IO port of 'TB220A1', you can directly burn and write the arduino bridge file to 'Mega2560', and then start testing        
## 1.Test encoder rotation
'e' is the number of test encoders        
'r' is the reset encoder count        
After testing the left and right wheels in the correct direction and direction, proceed to the next step of testing        
        
![1689542401212](https://github.com/HanyangZhong/self-driving-high-precision-map-study/assets/119017394/ce1a72d5-582a-4b7c-806a-bb5b705be93f)
![image](https://github.com/HanyangZhong/self-driving-high-precision-map-study/assets/119017394/a0212356-5c85-44a9-810f-185c1ec98e7d)

## 2.Test motor drive
'm' is motor speed control        
'm 100 0' is the forward movement of the left motor at a speed of 100 seconds        
Then set 'm 0 0'        

![image](https://github.com/HanyangZhong/self-driving-high-precision-map-study/assets/119017394/e4f6d4d9-9224-4957-a49c-c3a394f598fc)


## 3.PID debugging
After testing the motor rotation and encoder function, start testing the PID        
In diff_ Under controller. h, change the test PID parameters        
Test doPID left and right wheels separately, remember to uncomment and restore after testing is completed        

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
  //Real time speed=Current encoder value - Last encoder end value (1/30 seconds)
  input = p->Encoder - p->PrevEnc;
  // Error=target value - current speed value
  Perror = p->TargetTicksPerFrame - input;
    //Print and use a serial plotter to test and adjust the pid
//  Serial.println(input);/

....

//Adjust the pid of the left and right wheels separately
  //doPID(&rightPID);
  doPID(&leftPID);


```

## 4.Ros-arduino-bridge yaml File Configuration
The main startup files are in 'ROS_Arduino_Bridge/Ros_Arduino_Python/launch'.        
Among them, 'my_ arduino_params' is the default file name        
Copy and rename under 'config', enter the following parameters        
```
# For a direct USB cable connection, the port name is typically
# /dev/ttyACM# where is # is a number such as 0, 1, 2, etc
# For a wireless connection like XBee, the port is typically
# /dev/ttyUSB# where # is a number such as 0, 1, 2, etc.

port: /dev/ttyACM0 #Set as appropriate, usually set to/dev/ttyACM0 or/dev/ttyUSB0
baud: 57600 #
timeout: 0.1 #

rate: 50
sensorstate_rate: 10

use_base_controller: True  #Enable Base Controller
base_controller_rate: 10   

# For a robot that uses base_footprint, change base_frame to base_footprint
base_frame: base_footprint #base_frame set up

# === Robot drivetrain parameters
wheel_diameter: 0.085 #Wheel diameter
wheel_track: 0.212    #0.244 -0.032 #Wheel spacing
encoder_resolution: 1560 #Encoder accuracy (number of pulses per cycle * frequency doubling * deceleration ratio)
#gear_reduction: 1 #reduction ratio
#motors_reversed: False #Reverse steering

# === PID parameters PID Parameters that need to be adjusted by oneself
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

Before running the bridge's launch file, it is necessary to 'ros_arduino_python/nodes/arduino_node.py' Setting executable permissions for files under 'node.py'        
```
sudo chmod +x arduino_node.py
```

# 4.Distributed communication configuration
## 0.Wifi Connections setting up        
1.Hardware preparation: Use a display screen or HDMI acquisition card to connect to Raspberry Pi and start it;        
2.Connect the Raspberry Pi to a wireless network;        
3.Configure static IP for Raspberry Pi, as shown in the figure        
![image](https://github.com/HanyangZhong/self-driving-high-precision-map-study/assets/119017394/c0083ae4-d1ba-4718-89dc-9b6a90a15a66)
![image](https://github.com/HanyangZhong/self-driving-high-precision-map-study/assets/119017394/3ae99666-860b-4e96-824a-8814bdb9ea72)
![image](https://github.com/HanyangZhong/self-driving-high-precision-map-study/assets/119017394/147a9526-c8c1-436a-9171-faf2131f6ab4)
![image](https://github.com/HanyangZhong/self-driving-high-precision-map-study/assets/119017394/5a1cef9e-d461-4407-88bf-8ebf778731a4)

## 1.System File Configuration
### Necessary dependency installation
The SSH implementation architecture is divided into two parts: client and server. The client is the sender of data, and the server is the receiver of data.         
In the current scenario, we need to send data from the PC to Raspberry Pi. Therefore, the PC belongs to the client, while Raspberry Pi belongs to the server. The specific implementation process is as follows:        
1. Install SSH client and server separately
2. Start SSH service on the server
3. Remote login of the client to the server
4. Implement data transmission
#### 1.Install SSH client and server        
By default, the Ubuntu system already has an SSH client installed, so you only need to install the server on Raspberry Pi (if Raspberry Pi is installing a service version of Ubuntu, the SSH service will be installed by default and has been set to boot automatically):        
```
sudo apt-get install openssh-server
```
If the client needs to install itself, call the following command:        
```
sudo apt-get install openssh-client
```
#### 2.Start SSH service on the server
Raspberry Pi Start SSH Service:        
```
sudo /etc/init.d/ssh start
```
Check if the service is running normally after startup:        
```
ps -e | grep ssh
```
If the startup is successful, it will include two programs: 'sshd' and 'ssh'.        
In the future, it is necessary to frequently use SSH to log in to Raspberry Pi. To simplify implementation, you can set the SSH service of Raspberry Pi to boot automatically. The command is as follows:        
```
sudo systemctl enable ssh
```
#### 3.Remote login of the client to the server
To log in to Raspberry Pi, you can call the following command:        
```
ssh Account@IP_address
```
Then, according to the prompt, enter the login password to successfully log in.        
If you exit the login, you can call the 'exit' command:        
```
exit
```
#### 4.Implement data transmission
Upload file:        
```
scp Local_file_path account@ip:Raspberry_path
```
Upload folder:        
```
scp -r Local_folder_path account@ip:Raspberry_path
```
Download files:                
```
scp account@ip:Raspberry_path Local_file_path    
```
Download folders:        
```
scp -r account@ip:Raspberry_path Local_folder_path
```
#### 3.Install Ros—Arduino-Bridge
'ROS_Arduino_Bridge' relies on the 'Python serial' feature pack. Please install this feature pack on the Raspberry Pi client first. The installation command is:            
```
sudo apt-get install python-serial
```
or            
```
sudo pip install --upgrade pyserial
```
or            
```
sudo easy_install -U pyserial
```

### configuration file    
Connect Raspberry Pi or Jetson Nano to the same WiFi network as the computer            
As 'PC: 192.168.0.101'          
As Raspberry Pi 'Pi: 192.168.0.110'            
Both the computer end and embedded controller end are set with static IPs to ensure they are in the same network segment        
Input        
```
sudo gedit /etc/hosts
```
Both the slave and host must be set        
Configure the other party's IP address        
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

Then configure the ROS host and slave corresponding to the '. bashrc' file        
Enter 'sudo gedit. bashrc'        
Set the PC as a slave and take the main address of the host Raspberry        
```
export ROS_MASTER_URI=http://192.168.0.110:11311
export ROS_HOSTNAME=192.168.0.101
```
Set Raspberry Pi as the host, and take the address of the host Raspberry Pi as the main address        
```
export ROS_MASTER_URI=http://192.168.0.110:11311
export ROS_HOSTNAME=192.168.0.110
```

## 2.sshConnection control
Control commands such as:            
```
ssh -X wheeltec@192.168.0.110
```
Enter password and start controlling        

The host side run 'roscore', while the slave side does not need to run 'roscore' to send messages        
Note that all command lines require 'source. bashrc', otherwise the environment variables will not be refreshed        
Host side running test:        
```
roscore
rosrun turtlesim turtlesim_node 
```
Slave running test:        
```
rosrun turtlesim turtlesim_node 
```
After all tests are passed and messages can be received, you can start testing the arduino bridge control        

## 3.Remote control car test
### 1.Code transmission and compilation
Please first create a workspace on the Raspberry Pi end, enter the 'src' directory of the local workspace on the PC end, and call the program upload command:        
```
scp -r ros_arduino_bridge/ Raspberry_account@Raspberry_ip:~/workspace/src
```
Enter the workspace and compile on the Raspberry Pi end:        
```
catkin_make
```
### 2.test
Now start the Raspberry Pi program, and then start the PC program.        
#### Raspberry Pi
Start ros_arduino_bridge node:        
```
roslaunch ros_arduino_python arduino.launch
```
#### PC
Start keyboard control node：        
```
rosrun teleop_twist_keyboard teleop_twist_keyboard.py
```
如无异常，现在就可以在PC端通过键盘控制小车运动了，并且PC端还可以使用rviz查看小车的里程计信息。        
