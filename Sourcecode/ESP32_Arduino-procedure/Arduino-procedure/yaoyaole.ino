#include <spartan-edge-esp32-boot.h>
#include "ESP32IniFile.h"
#include "sea_esp32_qspi.h"
#include "SimpleDHT.h"
#include "string.h"

#include "AWS_IOT.h"
#include "WiFi.h"

#define LIMIT 500
#define ZERO 120
#define lt 5000

// initialize the spartan_edge_esp32_boot library
spartan_edge_esp32_boot esp32Cla;
AWS_IOT hornbill;

char WIFI_SSID[]="ChinaNet-501";
char WIFI_PASSWORD[]="13547676586";
char HOST_ADDRESS[]="a23phc9x6d5323-ats.iot.us-east-1.amazonaws.com";
char CLIENT_ID[]= "HAPPYlife";
char TOPIC_NAME[]= "$aws/things/Happytodo/shadow/update";

const char aws_root_ca_pem[]="-----BEGIN CERTIFICATE-----\n\
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\
rqXRfboQnoZsG4q5WTP468SQvvG5\
-----END CERTIFICATE-----\n";

const char certificate_pem_crt[]="-----BEGIN CERTIFICATE-----\n\
MIIDWjCCAkKgAwIBAgIVAOunlxTpKtF1wjkQNlSh0vnIzc0XMA0GCSqGSIb3DQEB\
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMDA3MjkwNzQ1\
MzFaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDVziS5naF3fuVkQPQM\
j4SQOG41bz+3qoIyyw3bi7miLUBD2z/zcfSpZg1JZLlPN4m70UWzz7HBPsegzzyb\
3ZIsv0A+359J+APHmnwZx+Q3sPz0SJ+TR7NtQgQz7u4IzuIbGHAxfa/ao/9xSRua\
KX/lOxjaSa6gsvL1Lfgu1IPlNYAW7tM2sXwB7z+Z/E77n+x9Vnd9cQ9f2Ih9UvDw\
FbRb71enQFS6Har3hriMS/ZEvfGjeOK/fmnRkaCAGpHaUwKX1f6GctT0o5bRraWT\
T/W3trNpp5CUNjWvam4Etk09zH14Wy3alGEaVMWiH6b/IDHZikwzpFrj3ttDKXGu\
YuybAgMBAAGjYDBeMB8GA1UdIwQYMBaAFPzRz6l4WnQfUAlg2gePXpeCEwmQMB0G\
A1UdDgQWBBSdKCoEvOHLgplZG+eyvWeRB0X7QjAMBgNVHRMBAf8EAjAAMA4GA1Ud\
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEApNSJIsknuXcbIskfhfw1eQnQ\
iddFNtu0wn1uvfl6cfq0AMXWE/DpgOtwRBGP8P+cgMroFNTGWlmkQKcVW4kLXTWf\
ad4Qx+eXql15jeOTZd+1qracuwUpd6iOoIPm21nEy4ImSSUfG3je30iZGAusY7qj\
XIHMpNEXvUqvhymj1cTrSzEhOYNXBkKGkuPatVYxoaST+a/2+6jER45LeK1NF59K\
94rn+sSlHZ3HmKdel4Zh8q4aeuY7+NY9ELaT6nT5ZzMj5k2ppYkjmBCYvFjVz0qG\
tUbo7PRpeRU0lxKadwmKxMyHuMeN6EwX8S60IY0uCkugbHxJShSg5M9nwM449Q==\
-----END CERTIFICATE-----\n";


const char private_pem_key[]="-----BEGIN RSA PRIVATE KEY-----\n\
MIIEpAIBAAKCAQEA1c4kuZ2hd37lZED0DI+EkDhuNW8/t6qCMssN24u5oi1AQ9s/\
83H0qWYNSWS5TzeJu9FFs8+xwT7HoM88m92SLL9APt+fSfgDx5p8GcfkN7D89Eif\
k0ezbUIEM+7uCM7iGxhwMX2v2qP/cUkbmil/5TsY2kmuoLLy9S34LtSD5TWAFu7T\
NrF8Ae8/mfxO+5/sfVZ3fXEPX9iIfVLw8BW0W+9Xp0BUuh2q94a4jEv2RL3xo3ji\
v35p0ZGggBqR2lMCl9X+hnLU9KOW0a2lk0/1t7azaaeQlDY1r2puBLZNPcx9eFst\
2pRhGlTFoh+m/yAx2YpMM6Ra497bQylxrmLsmwIDAQABAoIBAQDABBVDhmf/E74c\
naEQrswVYjjIUJNe9glcaLSXWhNrrUwsJPp4Pv3GLVr9cTJkDLLsNPRumrc/oIxU\
xgneyc1/1ntyfA9otQ9f8kRGOxmhUpi7I4X9FcX09/ciVRD+ZB74yqfoKsMGj4bY\
TRo3B9JPIxYc5VOqC0zcGV7tX/rcm909Q+aa+6kazJLB3zPl2SyRW0USCi01E8DP\
VPO9u3WMox182a88LoS5fkpxQUzWrE9CBae6a/4qJiXSGnRwbh7M1UrrqFRqqnNM\
V/yIUoiglkDQ2BHFSoCzj1uOeokk+sJDkmER0Jz5uahVsFJe7UeBm3nq5fk/C2ou\
wY5OVxKRAoGBAPR6QhSDFpAThWYI+JXtm+gY3dIieHmSIQbsV8KHtniAiYyVMDdJ\
kHxFziabnyqZJyfyhSlU5XowTCbER56wWY4BnicdCWC4/2M3bQdnbyNEyuY6GLkI\
8tQm8FO7ozrBMYk2XYecioVWPK4modrCbm9yMFrJk3KybsodnjRlPUT3AoGBAN/h\
zwciLP5CBSCQochuCchgXbiQxurvLWTmEysOPwQmGdV5fxqauY9tDCQF1PMGM1Mt\
CbUeaePMgOpOGoNsNtIhcZO/YLPerCcL7JC0Q3+Plr8JXa6KR29JKoUv3VuucRS2\
Qe7XsEhM/Glb+yqHGzbcWyMH/r45pYrg28clecB9AoGAYXPKm3qVp+DgMAgrbzhE\
CYTAB1+OPoeteqEDapvGdMpoCg3+JkvAuIjq4CDwlX5vSVE3G36n6skN0fYzxmHt\
kNm8PUMp1dBonRge8fCdMKXfa16FNF0IzuCms/n72xPftx5gsgoIfZYeohzsL9OJ\
SgtukwH/MJ1H5HFh+ujQP4kCgYEAgHTM19f5Hu38iSHTYfAZNtpsC/Et4TDYx/Lm\
/qZO6+98lSsADeDHJY85DxY1iDd/jXLjBgPsV+NH+KhVuCCIF+tOgqqlLq9N8Z16\
39A+hlt58CTMltAKisTdW8wJ44fkpDCrBgLFx8Y8catwIoegysqGlEQLG1RyMoww\
LkTHZfUCgYALbPFYodmU3wICvMrSEQEci+IlHimKPfdoRszFRBcYdil+S0MlhmOx\
xN7W3/D/cl6zC7FMbCtdaqMj50uYSmntEp1g+w3oCGyJA5pjrKfNycLNt36VjxGq\
PFn3k2uHs+/AgqHOP00ihjIkYg7eeIzSIgPaU7lcACmeM6zOHIQO5g==\
-----END RSA PRIVATE KEY-----\n";



const size_t bufferLen = 80;
char buffer[bufferLen];
char buffer1[bufferLen];
// the setup routine runs once when you press reset:


int status = WL_IDLE_STATUS;
int tick=0,msgCount=0,msgReceived = 0,tick1=0;
char payload[512];
char rcvdPayload[512];
uint8_t data1[4] = {41,42,43,44};
uint8_t data2[32];
void mySubCallBackHandler (char *topicName, int payloadLen, char *payLoad)
{
    strncpy(rcvdPayload,payLoad,payloadLen);
    rcvdPayload[payloadLen] = 0;
    msgReceived = 1;
}
//////////////////////////////////////////////////////
//读取数据成16字节
    int16_t read(int ad)
    {
        
        uint8_t c[4];
        int16_t *b;
        SeaTrans.read(ad,c,4);
       uint8_t a[2]={c[2],c[3]};
        b=(int16_t*)a;
        return *b;
    }

    //设置状态
    int8_t state()
    {
        int16_t dat=read(0);
        if(dat-ZERO>LIMIT)
            return 1;
        else if(dat+ZERO<-LIMIT)
            return -1;
        else
            return 0; 
    }

/////////////////////////////////////////////////////


void setup()
{
 // initialization 
  esp32Cla.begin();

  // check the .ini file exist or not
  const char *filename = "/board_config.ini";
  IniFile ini(filename);
  if (!ini.open()) {
    Serial.print("Ini file ");
    Serial.print(filename);
    Serial.println(" does not exist");
    return;
  }
  Serial.println("Ini file exists");

  // check the .ini file valid or not
  if (!ini.validate(buffer, bufferLen)) {
    Serial.print("ini file ");
    Serial.print(ini.getFilename());
    Serial.print(" not valid: ");
    return;
  }

  // Fetch a value from a key which is present
  if (ini.getValue("Overlay_List_Info", "Overlay_Dir", buffer, bufferLen)) {
    Serial.print("section 'Overlay_List_Info' has an entry 'Overlay_Dir' with value ");
    Serial.println(buffer);
  }
  else {
    Serial.print("Could not read 'Overlay_List_Info' from section 'Overlay_Dir', error was ");
  }

  // Fetch a value from a key which is present
  if (ini.getValue("Board_Setup", "overlay_on_boot", buffer1, bufferLen)) {
    Serial.print("section 'Board_Setup' has an entry 'overlay_on_boot' with value ");
    Serial.println(buffer1);
  }
  else {
    Serial.print("Could not read 'Board_Setup' from section 'overlay_on_boot', error was ");
  }

  // Splicing characters
  strcat(buffer,buffer1);
  
  // XFPGA pin Initialize
  esp32Cla.xfpgaGPIOInit();

  // loading the bitstream
  esp32Cla.xlibsSstream(buffer);
/////////////////////////////////////////////////////////////
  //jiekou
  Serial.begin(115200);
  SeaTrans.begin();
  delay(2000);

    while (status != WL_CONNECTED)
    {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(WIFI_SSID);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

        // wait 5 seconds for connection:
        delay(5000);
    }

    Serial.println("Connected to wifi");

    if(hornbill.connect(HOST_ADDRESS,CLIENT_ID,aws_root_ca_pem,certificate_pem_crt,private_pem_key)== 0)
    {
        Serial.println("Connected to AWS");
        delay(1000);

        if(0==hornbill.subscribe(TOPIC_NAME,mySubCallBackHandler))
        {
            Serial.println("Subscribe Successfully");
        }
        else
        {
            Serial.println("Subscribe Failed, Check the Thing Name and Certificates");
            while(1);
        }
    }
    else
    {
        Serial.println("AWS connection failed, Check the HOST Address");
        while(1);
    }

    delay(2000);
  
}

void loop()
{
    ////////////////////////////new count
    //主函数
    uint8_t i;
    int count2=0; //次数（一上一下算两次）
    int count=0;
    int ps=0;    //前一个状态
    int cs=0;    //后一个状态
    Serial.println("请开始摇动");
    unsigned long st=millis();    //st为前一刻时间
    while(millis()-st<lt)
    {
      cs=state();
      if(((cs==1)||(cs==-1))&&(cs!=ps))    count2++;
      ps=cs;
      
    }
    count=count2/2;

    sprintf(payload,"%d秒内摇动次数为%d",lt/1000,count);
   
   if(hornbill.publish(TOPIC_NAME,payload) == 0)
        {        
            Serial.println("Publish successfully!");
            Serial.println(payload);
        }
        else
        {
            Serial.println("Publish failed!");
        }
    
    msgReceived = 0;
    vTaskDelay(1000 / portTICK_RATE_MS); 
   
    tick++;
}

    
