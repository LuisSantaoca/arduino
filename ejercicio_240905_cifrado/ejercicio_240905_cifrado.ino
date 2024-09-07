#include <WiFi.h>
#include <AESLib.h>

const char* ssid = "ESP32_Access_Point";
const char* password = "12345678";

WiFiServer server(80);

byte aes_key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
byte aes_iv[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                   0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

AESLib aesLib;

void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);
  
  Serial.println("Punto de acceso configurado");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.softAPIP());

  server.begin();
  Serial.println("Servidor iniciado. Esperando conexiones de clientes...");
}

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("Cliente conectado.");
    
    unsigned long timeout = millis();
    while (!client.available()) {
      if (millis() - timeout > 2000) {
        Serial.println("Tiempo de espera agotado. Cliente desconectado.");
        client.stop();
        return;
      }
    }

    String request = client.readStringUntil('\r');
    Serial.println("Solicitud recibida: ");
    Serial.println(request);

    client.flush();

    if (request.indexOf("GET /favicon.ico") >= 0) {
      Serial.println("Solicitud de favicon ignorada.");
      client.stop();
      return;
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<body>");
    client.println("<h1>Cifrado AES-128</h1>");
    client.println("<form action=\"/\" method=\"GET\">");
    client.println("Texto a cifrar: <input type=\"text\" name=\"text\" /><br><br>");
    client.println("<input type=\"submit\" value=\"Encriptar\" />");
    client.println("</form>");

    if (request.indexOf("GET /?text=") >= 0) {
      int startIdx = request.indexOf("text=") + 5;
      int endIdx = request.indexOf(' ', startIdx);  // Buscar el siguiente espacio después de 'text='
      if (endIdx == -1) {
        endIdx = request.length();  // Si no hay espacio, tomar hasta el final
      }
      
      String inputText = request.substring(startIdx, endIdx);
      inputText.replace("%20", " ");
      inputText.trim();  // Elimina espacios en blanco iniciales y finales

      if (inputText.length() == 0) {
        Serial.println("Escriba texto a cifrar");
        client.println("<h2>Escriba texto a cifrar</h2>");
      } else {
        Serial.println("Texto a cifrar: " + inputText);
        client.println("<h2>Texto a cifrar: " + inputText + "</h2>");

        byte plainText[inputText.length() + 1];
        inputText.getBytes(plainText, inputText.length() + 1);

        Serial.print("Arreglo de bytes: ");
        String byteArrayString = "";
        for (int i = 0; i < inputText.length(); i++) {
          Serial.print("0x");
          Serial.print(plainText[i], HEX);
          Serial.print(" ");
          byteArrayString += "0x" + String(plainText[i], HEX) + " ";
        }
        Serial.println();
        client.println("<h2>Arreglo de bytes: " + byteArrayString + "</h2>");

        char encrypted[512];
        int encrypted_length = aesLib.encrypt64(plainText, inputText.length(), encrypted, aes_key, 128, aes_iv);

        Serial.println("Texto cifrado (Base64): " + String(encrypted));
        client.println("<h2>Texto cifrado (Base64): " + String(encrypted) + "</h2>");
      }
    } else {
      Serial.println("Escriba texto a cifrar");
      client.println("<h2>Escriba texto a cifrar</h2>");
    }

    client.println("</body>");
    client.println("</html>");
    
    client.stop();
    Serial.println("Cliente desconectado.");
  }
}

