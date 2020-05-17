/* *****************************************************************
 *
 * Download latest Blinker library here:
 * https://github.com/blinker-iot/blinker-library/archive/master.zip
 * 
 * 
 * Blinker is a cross-hardware, cross-platform solution for the IoT. 
 * It provides APP, device and server support, 
 * and uses public cloud services for data transmission and storage.
 * It can be used in smart home, data monitoring and other fields 
 * to help users build Internet of Things projects better and faster.
 * 
 * Make sure installed 2.5.0 or later ESP8266/Arduino package,
 * if use ESP8266 with Blinker.
 * https://github.com/esp8266/Arduino/releases
 * 
 * Make sure installed 1.0.2 or later ESP32/Arduino package,
 * if use ESP32 with Blinker.
 * https://github.com/espressif/arduino-esp32/releases
 * 
 * Docs: https://doc.blinker.app/
 *       https://github.com/blinker-iot/blinker-doc/wiki
 * 
 * *****************************************************************
 * 
 * Blinker 库下载地址:
 * https://github.com/blinker-iot/blinker-library/archive/master.zip
 * 
 * Blinker 是一套跨硬件、跨平台的物联网解决方案，提供APP端、设备端、
 * 服务器端支持，使用公有云服务进行数据传输存储。可用于智能家居、
 * 数据监测等领域，可以帮助用户更好更快地搭建物联网项目。
 * 
 * 如果使用 ESP8266 接入 Blinker,
 * 请确保安装了 2.5.0 或更新的 ESP8266/Arduino 支持包。
 * https://github.com/esp8266/Arduino/releases
 * 
 * 如果使用 ESP32 接入 Blinker,
 * 请确保安装了 1.0.2 或更新的 ESP32/Arduino 支持包。
 * https://github.com/espressif/arduino-esp32/releases
 * 
 * 文档: https://doc.blinker.app/
 *       https://github.com/blinker-iot/blinker-doc/wiki
 * 
 * *****************************************************************/
#define GPIO0 0
#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT
#define DEBUG 0
#include <Blinker.h>
/*
 GPIO0 就是控制继电器的开关 LOW是打开 HIGH是关闭  
 LED_BUILDIN是板载led灯   LOW是打开 HIGH是关闭
 */

//设置认证码与wifi
char auth[] = "d1910e9fbe99";
char ssid[] = "Miereith_livingroom";
char pswd[] = "03147525183";

// 新建组件对象
// 按钮1-4分别是连续开关继电器1-4次
BlinkerButton Button1("btn-one");
BlinkerButton Button2("btn-two");
BlinkerButton Button3("btn-three");
BlinkerButton Button4("btn-four");
// 灯是否打开的全局变量 0是关 1是开
bool Light = 0;

//全局延时
int d=700;
// 小爱同学默认灯开关的次数
int LIANGDU=3;
// 需要调的亮度
int brightNeed=1;
// 现在的亮度
int brightNow=1;
// 不同亮度级别(0-20,21-40,41-60,61-80,81-100)分别代表的连续开关次数
int b1=1;
int b2=2;
int b3=3;
int b4=4;
int b5=5;

//开关灯，参数是连续开关的次数
void OPEN(int n){
  
  for (int i=1;i<n;i++){
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(GPIO0, LOW);
      Blinker.delay(d);
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(GPIO0, HIGH);
      Blinker.delay(d);
  }
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(GPIO0, LOW);
}

// 开灯
void Light_on(int n){
      OPEN(n);
      Light = 1;
      BlinkerMIOT.powerState("on");
      BlinkerMIOT.print();
      Button1.print("on");
      Button1.color("#00FF00");              
      Button1.text("亮");
}

void Light_off(){
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(GPIO0, HIGH);
      Light = 0;
      BlinkerMIOT.powerState("off");
      BlinkerMIOT.print();
    Button1.print("off");
    Button1.color("#0000FF");              
    Button1.text("灭");
}


// 设置亮度，判断需要的亮度然后关灯，根据设置的b1-b5来进行相应次数的连续开灯。
void setBright(){
  if (brightNeed>=0&&brightNeed<=20){
    Light_off();
    Blinker.delay(d);
    Light_on(b1);
  }
  else if (brightNeed>20&&brightNeed<=40){
    Light_off();
    Blinker.delay(d);
    Light_on(b2);
  }
  else if (brightNeed>40&&brightNeed<=60){
    Light_off();
    Blinker.delay(d);
    Light_on(b3);
  }
  else if (brightNeed>60&&brightNeed<=80){
    Light_off();
    Blinker.delay(d);
    Light_on(b4);
  }
  else{
    Light_off();
    Blinker.delay(d);
    Light_on(b5);
  }
  brightNow = brightNeed;
}



//小爱同学的亮度操作接口
void miotBright(const String & bright)
{   
    brightNeed = bright.toInt();
    if (DEBUG==1){
    BLINKER_LOG("[小爱同学]设置亮度：", brightNeed);
    }
    setBright();
    BlinkerMIOT.powerState("on");
    BlinkerMIOT.brightness(brightNow);
    BlinkerMIOT.print();
}



// 小爱的电源操作接口
void miotPowerState(const String & state)
{ 

    if (DEBUG==1){
    BLINKER_LOG("need set power state: ", state);
    }
    if (state == BLINKER_CMD_ON) {
        Light_on(LIANGDU);
        if (DEBUG==1){
        BLINKER_LOG("[小爱]开灯");
        }
       
    }
    else if (state == BLINKER_CMD_OFF) {
        Light_off();
        if (DEBUG==1){
        BLINKER_LOG("[小爱]关灯");}
    }

}
// 小爱同学的查询函数
void miotQuery(int32_t queryCode)
{
          if (DEBUG==1){
    BLINKER_LOG("MIOT Query codes: ", queryCode);
          }

      if (Light==0){
                if (DEBUG==1){
        BLINKER_LOG("[查询]关");}

        BlinkerMIOT.powerState("off");
        BlinkerMIOT.print();
      }
      else{
              if (DEBUG==1){
        BLINKER_LOG("[查询]开");}
        BlinkerMIOT.brightness(brightNow);
        BlinkerMIOT.powerState("on");
        BlinkerMIOT.print();
      }
    

}
// 亮度4，4次的
void button4_callback(const String & state)
{
          if (DEBUG==1){
    BLINKER_LOG("[按钮按下了]");}
    if (Light==0) {
        Light_on(4);
        if (DEBUG==1){
        BLINKER_LOG("[按钮]开灯");}
    }
    
    else {
        Light_off();
                if (DEBUG==1){
        BLINKER_LOG("[按钮]关灯");}

    }

}
// 亮度3，3次的
void button3_callback(const String & state)
{
          if (DEBUG==1){
    BLINKER_LOG("[按钮按下了]");}
    if (Light==0) {
        Light_on(3);
        if (DEBUG==1){
        BLINKER_LOG("[按钮]开灯");}
    }
    
    else {
        Light_off();
                if (DEBUG==1){
        BLINKER_LOG("[按钮]关灯");}

    }

}

// 亮度2，2次的
void button2_callback(const String & state)
{
          if (DEBUG==1){
    BLINKER_LOG("[按钮按下了]");}
    if (Light==0) {
        Light_on(2);
        if (DEBUG==1){
        BLINKER_LOG("[按钮]开灯");}
    }
    
    else {
        Light_off();
                if (DEBUG==1){
        BLINKER_LOG("[按钮]关灯");}

    }

}

// 亮度1，1次的
void button1_callback(const String & state)
{
          if (DEBUG==1){
    BLINKER_LOG("[按钮按下了]");}
    if (Light==0) {
        Light_on(1);
        if (DEBUG==1){
        BLINKER_LOG("[按钮]开灯");}
    }
    
    else {
        Light_off();
                if (DEBUG==1){
        BLINKER_LOG("[按钮]关灯");}

    }

}




// 如果未绑定的组件被触发，则会执行其中内容
// 其实就是app里输入框输入相应的数据到开发板，开发板读取数据执行的函数
// 数字1-3代表调整默认让小爱同学开灯，灯连续打开的次数
// 1000x,2000x,3000x,4000x,5000x,分别代表把b1或b2或b3或b4或b5调整成连续开关x次。
// 如果是大于3 小于10000的数字，就直接把连续开关灯的延时调成对应的数值（单位毫秒）
// 如果是其他的值，会造成无法预料的bug，请不要尝试。
void dataRead(const String & data)
{
    if (DEBUG==1){
    BLINKER_LOG("Blinker readString: ", data);}
    int dataint = data.toInt();
    
        
        if (dataint<=3&&dataint>=1){
            LIANGDU=dataint;
            if (DEBUG==1){BLINKER_LOG("[亮度改变]",LIANGDU);}
        }
        else if (dataint>=10000&&dataint<20000){
          int door = dataint-10000;
          b1=door;
          if (DEBUG==1){BLINKER_LOG("[b1改变]",b1);}
        }
        else if (dataint>=20000&&dataint<30000){
          int door = dataint-20000;
          b2=door;
          if (DEBUG==1){BLINKER_LOG("[b2改变]",b2);}
        }
        else if (dataint>=30000&&dataint<40000){
          int door = dataint-30000;
          b3=door;
          if (DEBUG==1){BLINKER_LOG("[b3改变]",b3);}
        }
        else if (dataint>=40000&&dataint<50000){
          int door = dataint-40000;
          b4=door;
          if (DEBUG==1){BLINKER_LOG("[b4改变]",b4);}
        }
        else if (dataint>=50000&&dataint<60000){
          int door = dataint-50000;
          b5=door;
          if (DEBUG==1){BLINKER_LOG("[b5改变]",b5);}
        }
        else{
            d=dataint;
            if (DEBUG==1){BLINKER_LOG("[延迟改变]",d);}
        }

    


    
}

void setup()
{
    // 初始化串口
    if (DEBUG==1){
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);
    BLINKER_DEBUG.debugAll();
    }
    // 初始化IO
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    pinMode(GPIO0, OUTPUT);
    digitalWrite(GPIO0, HIGH);
    // 初始化blinker
    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);
    BlinkerMIOT.attachPowerState(miotPowerState);
    BlinkerMIOT.attachQuery(miotQuery);
    BlinkerMIOT.attachBrightness(miotBright);
    // 按钮回调
    Button1.attach(button1_callback);
    Button2.attach(button2_callback);
    Button3.attach(button3_callback);
    Button4.attach(button4_callback);
}

void loop() {

    Blinker.run();
}
