#include <WiFi.h>

// Configuración del punto de acceso
const char* ssid = "ESP32_Access_Point";  // Nombre de la red
const char* password = "12345678";  // Contraseña de la red

WiFiServer server(80);  // Inicia el servidor web en el puerto 80

void setup() {
  Serial.begin(115200);

  // Configurar el ESP32 como un punto de acceso
  WiFi.softAP(ssid, password);
  
  Serial.println("Punto de acceso configurado");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.softAPIP());

  server.begin();  // Inicia el servidor
  Serial.println("Servidor iniciado. Esperando conexiones de clientes...");
}

void loop() {
  WiFiClient client = server.available();  // Espera a que se conecte un cliente
  
  if (client) {
    Serial.println("Cliente conectado.");
    
    // Tiempo de espera para la solicitud del cliente
    unsigned long timeout = millis();
    while (!client.available()) {
      if (millis() - timeout > 2000) {  // Tiempo de espera de 2 segundos
        Serial.println("Tiempo de espera agotado. Cliente desconectado.");
        client.stop();
        return;
      }
    }

    String request = client.readStringUntil('\r');  // Lee la solicitud HTTP
    Serial.println("Solicitud recibida: ");
    Serial.println(request);  // Muestra la solicitud HTTP recibida

    if (request.indexOf("GET / ") >= 0) {
      Serial.println("Intento de ingresar a la página principal detectado.");
    } else {
      Serial.println("Solicitud diferente recibida.");
    }

    client.flush();
    
    // Genera un número aleatorio usando el TRNG del ESP32
    uint32_t random_number = esp_random();

    // Responde con una página web que muestra el número aleatorio
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html><h1>Numero Aleatorio Generado: " + String(random_number) + "</h1></html>");
    
    client.stop();  // Cierra la conexión
    Serial.println("Cliente desconectado.");
  }
}
