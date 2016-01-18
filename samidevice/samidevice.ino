#include "DHT.h"
#include <Bridge.h>
#include <Process.h>
#include <YunClient.h>
#include <YunServer.h>
#include <ArduinoJson.h>

#define DHTPIN A0     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11

// 定義腳位
#define PIN_0 9
#define PIN_g 3
#define PIN_c 4
#define PIN_h 5
#define PIN_d 6
#define PIN_e 7
#define PIN_b 8
#define PIN_1 10
#define PIN_2 13
#define PIN_f 11
#define PIN_a 12

// 共有4個七段顯示器，分別由針腳PIN_0、PIN_1、PIN_2、PIN_3控制
// 七段顯示器裡有8個LED（包含小數點）
#define POS_NUM 4
#define SEG_NUM 8
const byte pos_pins[POS_NUM] = {PIN_0, PIN_1, PIN_2};
const byte seg_pins[SEG_NUM] = {PIN_a, PIN_b, PIN_c, PIN_d, PIN_e, PIN_f, PIN_g, PIN_h};

// 底下定義由七段顯示器顯示數字時所需要的資料
#define t true
#define f false
const boolean data[12][SEG_NUM] = {
  {t, t, t, t, t, t, f, f}, // 0
  {f, t, t, f, f, f, f, f}, // 1
  {t, t, f, t, t, f, t, f}, // 2
  {t, t, t, t, f, f, t, f}, // 3
  {f, t, t, f, f, t, t, f}, // 4
  {t, f, t, t, f, t, t, f}, // 5
  {t, f, t, t, t, t, t, f}, // 6
  {t, t, t, f, f, f, f, f}, // 7
  {t, t, t, t, t, t, t, f}, // 8
  {t, t, t, t, f, t, t, f}, // 9
  {t, f, f, t, t, t, f, f}, // C
  {f, t, t, f, t, t, t, f}, // F
};


#define PORT 3479
BridgeClient client;
BridgeServer server(PORT);
BridgeClient bridgeClient;
Process p; //Use to run a shell command
DHT dht(DHTPIN, DHTTYPE);
int sysTemperature = 20;
float dhtHumidity;
float dhtTemperature;
unsigned long time_previous;
bool torh = true; // t == show temperature; f == show humidity;
bool rcvCommand = false;

void setup() {
  // Initialize Bridge
  Bridge.begin();
  // Initialize Serial
  Serial.begin(9600);
  while (!Serial);

  for (int i = 0; i < POS_NUM; i++) {
    pinMode(pos_pins[i], OUTPUT);
    digitalWrite(pos_pins[i], HIGH);
  }
  for (int i = 0; i < SEG_NUM; i++) {
    pinMode(seg_pins[i], OUTPUT);
    digitalWrite(seg_pins[i], LOW);
  }

  server.begin();
  dht.begin();
  time_previous = millis();
  setNumber(sysTemperature);
  runCpuInfo();
  runUdp();
}

void loop() {
  bridgeClient = server.accept();
  if (bridgeClient) {
    process(bridgeClient);
    bridgeClient.stop();
  }
  //pf("number=%d\n", sysTemperature);
  //  setNumber(sysTemperature);
  //delay (50);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  dhtHumidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  dhtTemperature = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float dhtF = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(dhtHumidity) || isnan(dhtTemperature) || isnan(dhtF)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  //  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  //  float hic = dht.computeHeatIndex(t, h, false);

  unsigned long time_now = millis();
  if (time_now - time_previous > 5000 && rcvCommand == true) {
    rcvCommand = false;
  }

  if (time_now - time_previous > 2000 && rcvCommand == false) {
    if (torh) {
      setHumidity(dhtHumidity);
      torh = false;
    } else {
      setTemperature(dhtTemperature);
      torh = true;
    }
    time_previous += 2000;
  } else {
    if (rcvCommand) {
      setNumber(sysTemperature);
    } else if (torh) {
      setTemperature(dhtTemperature);
    } else {
      setHumidity(dhtHumidity);
    }
  }

}

void process(YunClient bridgeClient) {
  DynamicJsonBuffer jsonBuffer;
  // read the command
  String command = bridgeClient.readString();
  Serial.println(command);
  Serial.flush();
  JsonObject& root = jsonBuffer.parseObject(command);

  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  } else if (root.success() && p.running()) {
    p.close();
  }
  String rcvCmd = root["command"];
  Serial.println("received command : " + rcvCmd);

  if (rcvCmd == "supportCmds") {
    char *res = "{\"status\":200,\"SupportCmds\":[{\"command\":\"adjustTemperature\",\"command_displayName\":\"Set Temperature\",\"param_type\":\"integer\",\"param_max\":\"100\",\"param_min\":\"20\"},{\"command\":\"queryDisplayInfo\",\"command_displayName\":\"Query Status\",\"param_type\":\"none\"}]}";
    Serial.println(res);
    Serial.flush();
    tcpSendCommand(bridgeClient, res);
  } else if (rcvCmd == "queryDisplayInfo") {
    String temperature = String(sysTemperature);
    dhtHumidity = dht.readHumidity();
    Serial.print("Humidity: ");
    Serial.print(dhtHumidity);
    Serial.print(" %\n");
//    String humidity = String(dhtHumidity, 2);
//    Serial.println(humidity);
    //String res = "{\"status\":200,\"DisplayInfo\":[\"Temperature\",\"WindStrength\",\"Humidity\"],\"Temperature\":\"" + temperature + "\",\"WindStrength\":\"Medium\",\"Humidity\":\"" + humidity + "\"}";
//    String res = "{\"status\":200,\"DisplayInfo\":[\"Temperature\",\"WindStrength\",\"Humidity\"],\"Temperature\":\"20\",\"WindStrength\":\"Medium\",\"Humidity\":\"58\"}";
    char *res = new char[200];
    Serial.print(sysTemperature);
    Serial.print(dhtHumidity);
    int nn = sprintf(res, "{\"status\":200,\"DisplayInfo\":[\"Temperature\",\"WindStrength\",\"Humidity\"],\"Temperature\":\"%d\",\"WindStrength\":\"Medium\",\"Humidity\":\"%d\"}", sysTemperature, (int)dhtHumidity);
    Serial.print(nn);
    Serial.println(res);
    Serial.flush();
    tcpSendCommand(bridgeClient, res);
    delete [] res;
  } else if (rcvCmd == "adjustTemperature") {
    char *res = "{\"status\":200}";
    String rcvParam = root["param"];
    sysTemperature = rcvParam.toInt();
    rcvCommand = true;
    Serial.println(res);
    Serial.flush();
    tcpSendCommand(bridgeClient, res);
  }
  return;
}

void runUdp() {
  p.begin("python");
  p.addParameter("/mnt/sda1/udp.py");
  p.runAsynchronously();
  Serial.println("UDP runing ....");
  Serial.flush();
}

void tcpSendCommand(BridgeClient bridgeClient, char *sendCmd) {
  // Make the client connect to the desired server and port
  //  IPAddress addr(192, 168, 1, 59);
  //  client.connect(addr, PORT);
  //delay (250);
  if (bridgeClient.connected())
  {
    Serial.println("Connected to the server.");
    Serial.println(sendCmd);
    // Send something to the client
    bridgeClient.print(sendCmd);
    // Cheap way to give the server time to respond.
    // A real application (as opposed to this simple example) will want to be more intelligent about this.
    delay (250);
    // Read all incoming bytes available from the server and print them
    while (bridgeClient.available())
    {
      char c = client.read();
      Serial.print(c);
    }
    Serial.flush();
    // Close the connection
    //    client.stop();
  }
  else
    Serial.println("Could not connect to the server.");
}

void runCpuInfo() {
  // Launch "cat /proc/cpuinfo" command (shows info on Atheros CPU)
  // cat is a command line utility that shows the content of a file
  Process p;		// Create a process and call it "p"
  p.begin("cat");	// Process that launch the "cat" command
  p.addParameter("/proc/cpuinfo"); // Add the cpuifo file path as parameter to cut
  p.run();		// Run the process and wait for its termination

  // Print command output on the Serial.
  // A process output can be read with the stream methods
  while (p.available() > 0) {
    char c = p.read();
    Serial.print(c);
  }
  // Ensure the last bit of data is sent.
  Serial.flush();
}

// 一支方便的函式，以格式字串輸出到序列埠
void pf(const char *fmt, ... ) {
  char tmp[128]; // max is 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(tmp, 128, fmt, args);
  va_end (args);
  Serial.print(tmp);
}

// 設定某個七段顯示器所顯示的數字，
// 參數pos為0~3，指出想要更新哪一個七段顯示器，
// 參數n為0~9，顯示數字
void setDigit(int pos, int n) {
  if (pos < 0 || 2 < pos) {
    pf("error pos=%d\n", pos);
    return;
  }

  // 控制想要更新哪一個七段顯示器，將其腳位設為LOW
  // 其他腳位則設為HIGH，代表不更新。
  for (int p = 0; p < POS_NUM; p++) {
    if (p == pos)
      digitalWrite(pos_pins[p], LOW);
    else
      digitalWrite(pos_pins[p], HIGH);
  }

  // 寫入數字
  if (0 <= n && n <= 9) {
    for (int i = 0; i < SEG_NUM; i++) {
      digitalWrite(seg_pins[i], data[n][i] == t ? HIGH : LOW);
    }
  }
  else if (n == 10) {
    for (int i = 0; i < SEG_NUM; i++) {
      digitalWrite(seg_pins[i], data[n][i] == t ? HIGH : LOW);
    }
  }
  else if (n == 11) {
    for (int i = 0; i < SEG_NUM; i++) {
      digitalWrite(seg_pins[i], data[n][i] == t ? HIGH : LOW);
    }
  }
  else {
    for (int i = 0; i < SEG_NUM; i++) {
      digitalWrite(seg_pins[i], LOW);
    }
    digitalWrite(PIN_h, HIGH);
    pf("error pos=%d, n=%d\n", pos, n);
  }
}

// 設定整個四合一型七段顯示器想要顯示的數字
// 參數number的範圍應是0~999
void setNumber(int number)
{
  int n0, n1, n2;
  n2 = number / 100;
  number %= 100;
  n1 = number / 10;
  n0 = number % 10;

  // 求出每個位數的值後，分別更新
  // 注意，此處以delay(5)隔開每個位數的更新
  setDigit(0, n0); delay(5);
  setDigit(1, n1); delay(5);
  setDigit(2, n2); delay(5);
}

void setTemperature(float temperature) {
  int n0, n1, n2;
  n2 = int(temperature) / 10;
  n1 = int(temperature) % 10;
  setDigit(2, n2); delay(5);
  setDigit(1, n1); delay(5);
  setDigit(0, 10); delay(5);
}

void setHumidity(float humidity) {
  int n0, n1, n2;
  n2 = int(humidity) / 10;
  n1 = int(humidity) % 10;
  setDigit(2, n2); delay(5);
  setDigit(1, n1); delay(5);
  setDigit(0, 11); delay(5);
}

