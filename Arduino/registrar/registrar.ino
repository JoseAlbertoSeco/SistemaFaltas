#include <HardwareSerial.h>
#include <FPM.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

HardwareSerial fserial(2);
FPM finger(&fserial);
FPM_System_Params params;
#define BUFF_SZ     768
uint8_t template_buffer[BUFF_SZ];
String DIR = "--- DIRECCIÓN DEL SERVIDOR WEB ---";
WiFiMulti wifiMulti;
String huella = "";

void setup()
{
  Serial.begin(115200);
  fserial.begin(57600);
  wifiMulti.addAP("SSID", "PASSWORD");
  if (finger.begin()) {
    finger.readParams(&params);
    Serial.println("[ESP32] SENSOR ENCONTRADO");
  } else {
    Serial.println("[ESP32] SENSOR NO ENCONTRADO");
    while (1) yield();
  }
  /* just to find out if your sensor supports the handshake command */
  if (finger.handshake()) {
    Serial.println("Handshake command is supported.");
  } else {
    Serial.println("Handshake command not supported.");
  }
  wifiMulti.addAP("SSID", "PASSWORD");
}

void loop()
{
  Serial.println("\n\n----------------------------------------------------------");
  Serial.println("[ESP32] ALMACENAMIENTO DE HUELLA");
  empty_database();
  enroll_finger(1);
  delay(20000);
}

bool control() {
  // wait for WiFi connection
  if ((wifiMulti.run() == WL_CONNECTED)) {
    Serial.println("[JSON] SE VAN A LEER LAS HUELLAS DEL ARCHIVO");
    HTTPClient http;
    //Serial.print("\n\n[HTTP] begin...\n");
    http.begin("http://" + DIR + "/addIndex/control_reiniciar.php"); //HTTP
    //Serial.print("[HTTP] GET...\n");
    int httpCode = http.GET();
    if (httpCode > 0) {
      //Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println("[CONTROL] "+payload);
        //Serial.println("------ JSON CODE ------\n"+payload);
        if (payload == "true") {
          return true;
        } else {
          return false;
        }
      } else {
        Serial.println("[CONTROL] CONECTANDO DE NUEVO...");
      }
    } else {
      Serial.println("[CONTROL] CONECTANDO DE NUEVO...");
    }
    http.end();
  } else {
    Serial.println("[CONTROL] CONECTANDO DE NUEVO...");
  }
  delay(1000);
}

int16_t enroll_finger(int16_t fid) {
  int16_t p = -1;
  sendHTTP("ponerDedo");
  huella = "";
  Serial.println("[ENROLL] ESPERANDO...");
  while (p != FPM_OK) {
    p = finger.getImage();
    switch (p) {
      case FPM_OK:
        Serial.println("[ENROLL] HUELLA DETECTADA");
        break;
      case FPM_NOFINGER:
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
    yield();
  }
  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FPM_OK:
      Serial.println("[ENROLL] HUELLA CONVERTIDA");
      break;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("[ENROLL] QUITE EL DEDO");
  sendHTTP("quitarDedo");
  delay(2000);
  p = 0;
  while (p != FPM_NOFINGER) {
    p = finger.getImage();
    yield();
  }

  p = -1;
  Serial.println("[ENROLL] PONGA EL MISMO DEDO");
  Serial.println("[ENROLL] ESPERANDO...");
  sendHTTP("ponerDedo");
  while (p != FPM_OK) {
    p = finger.getImage();
    switch (p) {
      case FPM_OK:
        Serial.println("[ENROLL] HUELLA DETECTADA");
        break;
      case FPM_NOFINGER:
        Serial.print(".");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
    yield();
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FPM_OK:
      Serial.println("[ENROLL] HUELLA CONVERTIDA");
      break;
    default:
      Serial.println("Unknown error");
      return p;
  }

  sendHTTP("quitarDedo");
  delay(2000);

  // OK converted!
  p = finger.createModel();
  if (p == FPM_OK) {
    Serial.println("[ENROLL] LAS HUELLAS SON CORRECTAS.");
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(fid);
  p = finger.storeModel(fid);
  if (p == FPM_OK) {
    Serial.println("[ENROLL] HUELLA ALMACENADA!");
    uint16_t total_read = read_template(1, template_buffer, BUFF_SZ);
    huella = "";
    for (int i = 0; i < total_read - 1; i++) {
      huella += template_buffer[i];
      huella += ",";
    }
    huella += template_buffer[total_read - 1];
    Serial.println(huella);
    sendHUELLA("almacenado");
    delay(5000);
    return 0;
  } else {
    Serial.println("Unknown error");
    return p;
  }
}


void sendHTTP(String option) {
  if ((wifiMulti.run() == WL_CONNECTED)) {
    HTTPClient http;
    Serial.println("[SEND] ENVIANDO DATOS A INSERTAR_ALUMNO");
    String url = "http://";
    url += DIR ;
    url += "/addIndex/control_setup.php?option=";
    url += option;
    Serial.println("[SEND] CON URL: " + url);
    http.begin(url); //HTTP

    int httpCode = http.GET();
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.println("[SEND] DATOS ENVIADOS");
    } else {
      Serial.println("[SEND] ¡¡ERROR!!");
    }
    http.end();
  } else {
    sendHTTP(option);
  }
}
void sendHUELLA(String option) {
  if ((wifiMulti.run() == WL_CONNECTED)) {
    HTTPClient http;
    Serial.println("[SEND] ENVIANDO DATOS A INSERTAR_ALUMNO");
    String url = "http://";
    url += DIR ;
    url += "/addIndex/control_setup.php?option=";
    url += option;
    url += "&huella=";
    url += huella;
    Serial.println("[SEND] CON URL: " + url);
    http.begin(url); //HTTP

    int httpCode = http.GET();
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.println("[SEND] DATOS ENVIADOS");
    } else {
      Serial.println("[SEND] ¡¡ERROR!!");
    }
    http.end();
  } else {
    sendHTTP(option);
  }
}
void sendCONTROL(String option, String reinicio) {
  if ((wifiMulti.run() == WL_CONNECTED)) {
    HTTPClient http;
    Serial.println("[SEND] ENVIANDO DATOS A INSERTAR_ALUMNO");
    String url = "http://";
    url += DIR ;
    url += "/addIndex/control_setup.php?option=";
    url += option;
    url += "&reiniciar=";
    url += reinicio;
    Serial.println("[SEND] CON URL: " + url);
    http.begin(url); //HTTP

    int httpCode = http.GET();
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.println("[SEND] DATOS ENVIADOS");
    } else {
      Serial.println("[SEND] ¡¡ERROR!!");
    }
    http.end();
  } else {
    sendHTTP(option);
  }
}

uint16_t read_template(uint16_t fid, uint8_t * buffer, uint16_t buff_sz)
{
  int16_t p = finger.loadModel(fid);
  switch (p) {
    case FPM_OK:
      //Serial.print("Template "); Serial.print(fid); Serial.println(" loaded");
      break;
    default:
      Serial.print("Unknown error: "); Serial.println(p);
      return 0;
  }

  p = finger.downloadModel();
  switch (p) {
    case FPM_OK:
      break;
    default:
      Serial.print("Unknown error: "); Serial.println(p);
      return 0;
  }

  bool read_finished;
  int16_t count = 0;
  uint16_t readlen = buff_sz;
  uint16_t pos = 0;

  while (true) {
    bool ret = finger.readRaw(FPM_OUTPUT_TO_BUFFER, buffer + pos, &read_finished, &readlen);
    if (ret) {
      count++;
      pos += readlen;
      readlen = buff_sz - pos;
      if (read_finished)
        break;
    }
    else {
      Serial.print("Error receiving packet ");
      Serial.println(count);
      return 0;
    }
    yield();
  }

  uint16_t total_bytes = count * FPM::packet_lengths[params.packet_len];
  return total_bytes;
}
void empty_database(void) {
  int16_t p = finger.emptyDatabase();
  if (p == FPM_OK) {
    Serial.println("[EMPTY] LECTOR VACIADO!");
  } else {
    Serial.println("[EMPTY] HA OCURRIDO UN ERROR");
  }
}
