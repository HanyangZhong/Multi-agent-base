/*
 * 测速实现:
 *  阶段1:脉冲数统计
 *  阶段2:速度计算
 * 
 * 阶段1:
 *  1.定义所使用的中断引脚,以及计数器(使用 volatile 修饰)
 *  2.setup 中设置波特率，将引脚设置为输入模式
 *  3.使用 attachInterupt() 函数为引脚添加中断出发时机以及中断函数
 *  4.中断函数编写计算算法，并打印
 *    A.单频统计只需要统计单相上升沿或下降沿
 *    B.2倍频统计需要统计单相的上升沿和下降沿
 *    C.4倍频统计需要统计两相的上升沿和下降沿
 *  5.上传并查看结果
 *  
 * 
 */
// 电机1
int motor_A = 21;//中端口是2
int motor_B = 20;//中断口是3
//电机2
//int motor_A = 18;//中端口是5
//int motor_B = 19;//中断口是4
volatile int count = 0;//如果是正转，那么每计数一次自增1，如果是反转，那么每计数一次自减1 


void count_A(){
  //单频计数实现
  //手动旋转电机一圈，输出结果为 一圈脉冲数 * 减速比
  /*if(digitalRead(motor_A) == HIGH){

    if(digitalRead(motor_B) == LOW){//A 高 B 低
      count++;  
    } else {//A 高 B 高
      count--;  
    }


  }*/

  //2倍频计数实现
  //手动旋转电机一圈，输出结果为 一圈脉冲数 * 减速比 * 2
  if(digitalRead(motor_A) == HIGH){

    if(digitalRead(motor_B) == HIGH){//A 高 B 高
      count++;  
    } else {//A 高 B 低
      count--;  
    }


  } else {

    if(digitalRead(motor_B) == LOW){//A 低 B 低
      count++;  
    } else {//A 低 B 高
      count--;  
    }  

  }

}

//与A实现类似
//4倍频计数实现
//手动旋转电机一圈，输出结果为 一圈脉冲数 * 减速比 * 4
void count_B(){
  if(digitalRead(motor_B) == HIGH){

    if(digitalRead(motor_A) == LOW){//B 高 A 低
      count++;
    } else {//B 高 A 高
      count--;
    }


  } else {

    if(digitalRead(motor_A) == HIGH){//B 低 A 高
      count++;
    } else {//B 低 A 低
      count--;
    }

  }

}

void setup() {
  Serial.begin(57600);//设置波特率  
  pinMode(motor_A,INPUT);
  pinMode(motor_B,INPUT);
  attachInterrupt(2,count_A,CHANGE);//当电平发生改变时触发中断函数
//  //四倍频统计需要为B相也添加中断
  attachInterrupt(3,count_B,CHANGE);

//  attachInterrupt(5,count_A,CHANGE);//当电平发生改变时触发中断函数
  //四倍频统计需要为B相也添加中断
//  attachInterrupt(4,count_B,CHANGE);
}


void loop() {
  //测试计数器输出
  delay(2000);
  Serial.println(count);

}
