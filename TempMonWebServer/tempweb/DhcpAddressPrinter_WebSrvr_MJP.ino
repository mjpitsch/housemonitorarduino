/*
  DHCP-based IP printer
 
 This sketch uses the DHCP extensions to the Ethernet library
 to get an IP address via DHCP and print the address obtained.
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 created 12 April 2011
 by Tom Igoe
 
 */

//#include <SPI.h>
//#include <Ethernet.h>
//#include "WebServer.h"

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  
  0x90, 0xA2, 0xDA, 0x00, 0xA9, 0xD6 };

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

/* This creates an instance of the webserver.  By specifying a prefix
 * of "/", all pages will be at the root of the server. */
#define PREFIX "/"
WebServer webserver(PREFIX, 80);

/* commands are functions that get called by the webserver framework
 * they can read any posted data from client, and they output to the
 * server to send data back to the web browser. */
void helloCmd(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
  /* this line sends the standard "we're all OK" headers back to the
     browser */
  server.httpSuccess();

  /* if we're handling a GET or POST, we can output our data here.
     For a HEAD request, we just stop after outputting headers. */
  if (type != WebServer::HEAD)
  {
    /* this defines some HTML text in read-only memory aka PROGMEM.
     * This is needed to avoid having the string copied to our limited
     * amount of RAM. */
    P(helloMsg) = "<h1>Hello, World!  Says Mike</h1>";

    /* this is a special form of print that outputs from PROGMEM */
    server.printP(helloMsg);
    server << "<p>The freezer temperature 1 is: " << "<c1>" <<temps[0] << "</c1>" << "C.</p>";
    server << "<p>The freezer temperature 2 is: " << "<c2>" <<temps[1] << "</c2>" << "C.</p>";
    server << "<p>The basement temperature is: " << "<c3>" <<temps[2] << "</c3>" << "C.</p>";
    server << "<p>The outside temperature is: " << "<c4>" <<temps[3] << "</c4>" << "C.</p>";
  }
}

void setupEth() {
  // start the serial library:
  Serial.begin(9600);
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  // print your local IP address:
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println();

  /* setup our default command that will be run when the user accesses
   * the root page on the server */
  webserver.setDefaultCommand(&helloCmd);

  /* run the same command if you try to load /index.html, a common
   * default page name */
  webserver.addCommand("index.html", &helloCmd);

  /* start the webserver */
  webserver.begin();
}

void loopEth() {
  char buff[64];
  int len = 64;

  /* process incoming connections one at a time forever */
  webserver.processConnection(buff, &len);
}

void loopPost() {
  Serial.println("try to connect");
  Serial.println(client.connected());
  while(!client.connected())
  {
    Serial.println("connecting");
    int result;
    char serverName[] = "http://mjphomelog.appspot.com";
    result = client.connect(serverName, 80);
    Serial.println(result);
  }
  Serial.println("posting");
  //client.println("GET /?value=999 HTTP/1.0");
  client << "GET /?value1=" << temps[0] << "&value2=" << temps[1] <<
                 "&value3=" << temps[2] << "&value4=" << temps[3] << 
                 " HTTP/1.1" << "\n";
  //  client.println("Host: 091-labs.appspot.com");
  client.println("Host: mjphomelog.appspot.com");
  client.println("Connection: keep-alive");
  client.println();
  Serial.println("Posted 999");
  //  client.print("Content-Length: ");
  //  client.println(data.length());
  //  client.println("Cache-Control: max-age=0");
  //  client.println("Content-Type: application/x-www-form-urlencoded");
  //  client.println("Accept-Language: en-US,en;q=0.8");
  //  client.println();
  //  client.print(data);
  //  client.println();
  Serial.println(client.available());
  char c;
  while(client.available())
  {
    c = client.read();
    Serial.println(c);
  }
  client.stop();
}


