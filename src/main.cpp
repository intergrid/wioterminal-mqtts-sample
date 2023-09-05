#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <SPI.h>
#define MOSQUITO
const char *ssid = "underhill";      // your network SSID
const char *password = "yoga-mat"; // your network password
String MQstate(int state);
int port = 8883;
#ifdef MOSQUITO
 char *server = "test.mosquitto.org"; // Server URL
#else
const char *server = "a3k5r1hk2ir232-ats.iot.us-east-1.amazonaws.com"; // Server URL
#endif

//const char *server = "c68b630709884839a87be09740cdc595.s2.eu.hivemq.cloud";
const char *test_root_ca =
 /*   "-----BEGIN CERTIFICATE-----\n"
    "MIIEAzCCAuugAwIBAgIUBY1hlCGvdj4NhBXkZ/uLUZNILAwwDQYJKoZIhvcNAQEL\n"
    "BQAwgZAxCzAJBgNVBAYTAkdCMRcwFQYDVQQIDA5Vbml0ZWQgS2luZ2RvbTEOMAwG\n"
    "A1UEBwwFRGVyYnkxEjAQBgNVBAoMCU1vc3F1aXR0bzELMAkGA1UECwwCQ0ExFjAU\n"
    "BgNVBAMMDW1vc3F1aXR0by5vcmcxHzAdBgkqhkiG9w0BCQEWEHJvZ2VyQGF0Y2hv\n"
    "by5vcmcwHhcNMjAwNjA5MTEwNjM5WhcNMzAwNjA3MTEwNjM5WjCBkDELMAkGA1UE\n"
    "BhMCR0IxFzAVBgNVBAgMDlVuaXRlZCBLaW5nZG9tMQ4wDAYDVQQHDAVEZXJieTES\n"
    "MBAGA1UECgwJTW9zcXVpdHRvMQswCQYDVQQLDAJDQTEWMBQGA1UEAwwNbW9zcXVp\n"
    "dHRvLm9yZzEfMB0GCSqGSIb3DQEJARYQcm9nZXJAYXRjaG9vLm9yZzCCASIwDQYJ\n"
    "KoZIhvcNAQEBBQADggEPADCCAQoCggEBAME0HKmIzfTOwkKLT3THHe+ObdizamPg\n"
    "UZmD64Tf3zJdNeYGYn4CEXbyP6fy3tWc8S2boW6dzrH8SdFf9uo320GJA9B7U1FW\n"
    "Te3xda/Lm3JFfaHjkWw7jBwcauQZjpGINHapHRlpiCZsquAthOgxW9SgDgYlGzEA\n"
    "s06pkEFiMw+qDfLo/sxFKB6vQlFekMeCymjLCbNwPJyqyhFmPWwio/PDMruBTzPH\n"
    "3cioBnrJWKXc3OjXdLGFJOfj7pP0j/dr2LH72eSvv3PQQFl90CZPFhrCUcRHSSxo\n"
    "E6yjGOdnz7f6PveLIB574kQORwt8ePn0yidrTC1ictikED3nHYhMUOUCAwEAAaNT\n"
    "MFEwHQYDVR0OBBYEFPVV6xBUFPiGKDyo5V3+Hbh4N9YSMB8GA1UdIwQYMBaAFPVV\n"
    "6xBUFPiGKDyo5V3+Hbh4N9YSMA8GA1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQEL\n"
    "BQADggEBAGa9kS21N70ThM6/Hj9D7mbVxKLBjVWe2TPsGfbl3rEDfZ+OKRZ2j6AC\n"
    "6r7jb4TZO3dzF2p6dgbrlU71Y/4K0TdzIjRj3cQ3KSm41JvUQ0hZ/c04iGDg/xWf\n"
    "+pp58nfPAYwuerruPNWmlStWAXf0UTqRtg4hQDWBuUFDJTuWuuBvEXudz74eh/wK\n"
    "sMwfu1HFvjy5Z0iMDU8PUDepjVolOCue9ashlS4EB5IECdSR2TItnAIiIwimx839\n"
    "LdUdRudafMu5T5Xma182OC0/u/xRlEm+tvKGGmfFcN0piqVl8OrSPBgIlb+1IKJE\n"
    "m/XriWr/Cq4h/JfB7NTsezVslgkBaoU=\n"
    "-----END CERTIFICATE-----\n"; */
"-----BEGIN CERTIFICATE-----\n"
"MIIDWTCCAkGgAwIBAgIUIVPCxirO3kcd1qy0y5xOpakNHiswDQYJKoZIhvcNAQEL\n"
"BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n"
"SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIzMDkwNTAxNDE0\n"
"OFoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n"
"ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANxtjVoO/I9w75HNxo5s\n"
"vJzbnnoqxeDyqiOmqOWpJw5lRyQdoQGA8zuEj2J3P53QameZOAdecF7L8IQn2Ppr\n"
"7hVmhks/ZQuvApPUtRDxkFi8L07DwBm/zoJ4ZEPzt47WiHl7pmadk3UO9F1JSALy\n"
"tG3aFS+OKVF36HJ2SXHgCVVuZCijpVTwQGsjXWBFD2SwOwPcFsYw+fJHUhiCAjDl\n"
"K4Q3xdcaopTnwJEVXzY/Q8ixVPKofntA4LoKrLd3Ha+CblfobWtY2BCTuUT7kXn2\n"
"rmvJNFEOy4Nv2wE3zkR0rFR06XoYNyFbka+K3n/7UKB/jo1fUWzn2/V6b0REWWdf\n"
"W4kCAwEAAaNgMF4wHwYDVR0jBBgwFoAU6YbwTx5+xlORRJDx5anoBNcRLx8wHQYD\n"
"VR0OBBYEFBcNWOrwM1OcqeQ+iUlrzRjgvkDXMAwGA1UdEwEB/wQCMAAwDgYDVR0P\n"
"AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQAuhy1eRGNJfDSjTQvLjbf+smCG\n"
"KVf/1vNFxB0goaiPyNvw7n+8bmNaFJTOk4jLk5Q+nTBvUccDLfHzxYCNIQm9N3NI\n"
"yZXmER+G/Kp6CW2048wMDJW3AJ6PyZDLIfuUca0nPdACU0Ru+igutnUfx7j0ANOZ\n"
"RmtX6m7UAA9tzMeXMRncb/LeA8BcFd5XGdZiTwXcZn8r/hsspNAlWmfHuDHe/RpG\n"
"kQmYTbVLOnPJSllepoc+oR816uVW4xcM2/IV8hgTXhuG61PEvkj3Fibq+7pJGRYD\n"
"9k7nnOdGY1Qt1cleew53Aj5fT85DPkK72yQqogEaJCAW8SnbijItBcyY4pNX\n"
"-----END CERTIFICATE-----\n";

WiFiClientSecure wifiClient;
PubSubClient client(wifiClient);

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Trying MQTT connection...");
    client.setServer(server, port);
    // try connect
#ifdef MOSQUITO
    if (client.connect("intergrid")) {
 #else
    if (client.connect("intergrid","WIO","Bunyip")) {
#endif
    
      Serial.println("MQTT connected");
      // Once connected, publish an announcement...
      client.publish("WindMon", "Hello World!");
      // ... and resubscribe
      client.subscribe("WindMon");
    }
    else {
      int state = client.state();
      Serial.print("failed, rc=");
      Serial.print(state);
      Serial.println(MQstate(state));
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
//-------------------------------------------------------------
void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  //while (!Serial) ; // Wait for Serial to be ready
  delay(1000);
  Serial.println("MQTT Test V1.0");

  Serial.print("Wifi - trying SSID: ");
  Serial.println(ssid);
  WiFi.begin("underhill", "yoga-mat");
  Serial.println("Wifi begin ok");
  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("."); // wait 1 second for re-trying
    delay(1000);
  }
  Serial.print("Wifi Connected to ");
  Serial.println(ssid);

  wifiClient.setCACert(test_root_ca);

  client.setServer(server, port);
  client.setCallback(callback);
}
int LastSeconds = 0;
//---------------------------------------------------------
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(1);
  int Interval = millis() / 5000;
  if(Interval != LastSeconds) {
    LastSeconds = Interval;
    String S = "T="+String(Interval);
    client.publish("WindMon", S.c_str());
  }
}
//--------------------------------------------------------
String MQstate(int state) {
switch(state) {
  case MQTT_CONNECTION_TIMEOUT: return " Connection Timeout";
  case MQTT_CONNECTION_LOST: return " Connection Timeout";
  case MQTT_CONNECT_FAILED: return " Connection Failed";
  case MQTT_DISCONNECTED: return " Disconnected";
  case MQTT_CONNECTED: return " Connected";
  case MQTT_CONNECT_BAD_PROTOCOL: return " Bad Protocol";
  case MQTT_CONNECT_BAD_CLIENT_ID: return " Bad Client ID";
  case MQTT_CONNECT_UNAVAILABLE: return " Connect Unavailable";
  case MQTT_CONNECT_BAD_CREDENTIALS: return " Bad Credentials";
  case MQTT_CONNECT_UNAUTHORIZED: return  "Unauthorized";
  default: return "Unknown State";
}
}