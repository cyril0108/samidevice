#include <Bridge.h>
#include <Process.h>
#include <YunClient.h>
#include <YunServer.h>
#include <ArduinoJson.h>

#define PORT 3479
YunClient client;
YunServer server(PORT);

void setup() {
  // Initialize Bridge
  Bridge.begin();
  // Initialize Serial
  Serial.begin(9600);
  while (!Serial);
  server.begin();
  runCpuInfo();
}

void loop() {
  YunClient bridgeClient = server.accept();
  if (bridgeClient) {
    process(bridgeClient);
    bridgeClient.stop();
  }
  delay (50);
}

void process(YunClient bridgeClient) {
  StaticJsonBuffer<200> jsonBuffer;
  // read the command
  String command = bridgeClient.readString();
  Serial.println(command);
  Serial.flush();
  JsonObject& root = jsonBuffer.parseObject(command);

  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  String rcvCmd = root["command"];
  Serial.println("received command : " + rcvCmd);

  if (rcvCmd == "queryDisplayInfo") {
    char *res = "{\"status\" : 200,\"DisplayInfo\" : [\"Temperature\", \"WindStrength\", \"Humidity\"],\"Temperature\": \"30\",\"WindStrength\": \"Medium\",\"Humidity\" : \"60\"}";
    Serial.println(res);
    Serial.flush();
    tcpSendCommand(res);
  } else if (rcvCmd == "supportCmds") {
    char *res = "{\"status\":200,\"SupportCmds\":[{\"command\":\"adjustTemperature\",\"command_displayName\":\"Set Temperature\",\"param_type\":\"integer\",\"param_max\":100,\"param_min\":20},{\"command\":\"query\",\"command_displayName\":\"Query Status\",\"param_type\":\"none\"}]";
    Serial.println(res);
    Serial.flush();
    tcpSendCommand(res);
  }
  return;
}

void runUdp() {
  Process p;
  p.begin("python");
  p.addParameter("/mnt/sda1/udp.py");
  p.run();
  Serial.println("UDP runing ....");
  Serial.flush();
}

void tcpSendCommand(String sendCmd) {
  // Make the client connect to the desired server and port
  IPAddress addr(192, 168, 0, 62);
  client.connect(addr, PORT);
  //delay (250);
  if (client.connected())
  {
    Serial.println("Connected to the server.");
    // Send something to the client
    client.println(sendCmd);
    // Cheap way to give the server time to respond.
    // A real application (as opposed to this simple example) will want to be more intelligent about this.
    delay (250);
    // Read all incoming bytes available from the server and print them
    while (client.available())
    {
      char c = client.read();
      Serial.print(c);
    }
    Serial.flush();
    // Close the connection
    client.stop();
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

