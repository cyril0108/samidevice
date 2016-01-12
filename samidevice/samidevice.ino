#include <Process.h>
#include <YunClient.h>
#include <YunServer.h>

#define PORT 3479

YunClient client;
YunServer server;

void setup() {
  // Initialize Bridge
  Bridge.begin();

  // Initialize Serial
  Serial.begin(9600);

  // Wait until a Serial Monitor is connected.
  while (!Serial);

  // run various example processes
  runUdp();
  runCpuInfo();
}

void loop() {
  //tcpConnect();
  // Make the client connect to the desired server and port
  //  IPAddress addr(192, 168, 1, 59);

  // Or define it using a single unsigned 32 bit value
  // IPAddress addr(0xc0a8sab9); // same as 192.168.42.185

  // Or define it using a byte array
  // const uint8 addrBytes = {192, 168, 42, 185};
  // IPAddress addr(addrBytes);

  //  client.connect(addr, PORT);

  // Or connect by a server name and port.
  // Note that the Yun doesn't support mDNS by default, so "Yun.local" won't work
  // client.connect("ServerName.com", PORT);

  // Give some time before trying again
  //  delay (10000);
}

void runUdp() {
  // Launch "curl" command and get Arduino ascii art logo from the network
  // curl is command line program for transferring data using different internet protocols
  Process p;		// Create a process and call it "p"
  p.begin("python");	// Process that launch the "curl" command
  p.addParameter("/mnt/sda1/udp.py"); // Add the URL parameter to "curl"
  p.run();		// Run the process and wait for its termination

  Serial.println("UDP runing ....");
  Serial.flush();
}

void tcpConnect() {

  // Make the client connect to the desired server and port
  IPAddress addr(192, 168, 0, 62);

  // Or define it using a single unsigned 32 bit value
  // IPAddress addr(0xc0a8sab9); // same as 192.168.42.185

  // Or define it using a byte array
  // const uint8 addrBytes = {192, 168, 42, 185};
  // IPAddress addr(addrBytes);

  client.connect(addr, PORT);

  if (client.connected())
  {
    Serial.println("Connected to the server.");

    // Send something to the client
    client.println("Something...");

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
  // Or connect by a server name and port.
  // Note that the Yun doesn't support mDNS by default, so "Yun.local" won't work
  // client.connect("ServerName.com", PORT);

  // Give some time before trying again
  delay (1000);
}

void runTcp() {
  if (client.connected())
  {
    Serial.println("Connected to the server.");

    // Send something to the client
    client.println("Something...");

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

