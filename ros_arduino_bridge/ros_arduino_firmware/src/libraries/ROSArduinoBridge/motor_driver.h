/***************************************************************
   Motor driver function definitions - by James Nugen
   *************************************************************/

#ifdef L298_MOTOR_DRIVER
  #define RIGHT_MOTOR_BACKWARD 5
  #define LEFT_MOTOR_BACKWARD  6
  #define RIGHT_MOTOR_FORWARD  9
  #define LEFT_MOTOR_FORWARD   10
  #define RIGHT_MOTOR_ENABLE 12
  #define LEFT_MOTOR_ENABLE 13
  
#elif defined n20_motor
    
  #define DIRA  7
  #define PWMA  9
  #define DIRB  6
  #define PWMB  11

#elif defined MG513_motor
  #define PWMA 3  //3为模拟引脚，用于PWM控制
  #define AIN1 5
  #define AIN2 4
  #define PWMB 10  //10为模拟引脚，用于PWM控制
  #define BIN1 8
  #define BIN2 9
  #define STBY 7  //2、4、8、12、7为数字引脚，用于开关量控制
  
#endif


void initMotorController();
void setMotorSpeed(int i, int spd);
void setMotorSpeeds(int leftSpeed, int rightSpeed);
