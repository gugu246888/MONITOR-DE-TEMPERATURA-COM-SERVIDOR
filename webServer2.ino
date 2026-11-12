#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

char ssid[] = "Nome_da_rede";                       
char pass[] = "Senha_da_rede";                

ESP8266WebServer server(80);

#define DHTPIN 14 
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE); 

float temperatura; 
float umidade; 

void setup() {
  Serial.begin(115200); 
  delay(50); 
  dht.begin();

  Serial.println("Conectando a Rede: "); 
  Serial.println(ssid); 
  WiFi.begin(ssid, pass); 

  //Verificação da conexão
  while (WiFi.status() != WL_CONNECTED) { 
    delay(1000);
    Serial.print("."); 
  }
  Serial.println("");
  Serial.println("WiFi Conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP()); 

  server.on("/", envia_dados);                      
  server.onNotFound(erro);                          

  server.begin();                                             
  Serial.println("Servidor HTTP inicializado");

}

void loop() {
  server.handleClient();              
}

void envia_dados() {
  
  temperatura = dht.readTemperature();  
  umidade = dht.readHumidity(); 
  Serial.print("Temperatura: ");
  Serial.print(temperatura); 
  Serial.println(" ºC");
  Serial.print("Umidade: ");
  Serial.print(umidade); 
  Serial.println(" %");
  
  server.send(200, "text/html", EnviaHTML(temperatura, umidade));  

}

void erro() {                                                                               
  server.send(404, "text/plain", "Não encontrado");         

}

String EnviaHTML(float Temperaturastat, float Umidadestat) { 
  String ptr = "<!DOCTYPE html> <html>\n";
  
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n"; 
  ptr += "<meta http-equiv='refresh' content='2'>";
  ptr += "<link href=\"https://fonts.googleapis.com/css?family=Open+Sans:300,400,600\" rel=\"stylesheet\">\n";
  ptr += "<title>Estacao Meteorologica</title>\n"; 

 
  ptr += "<style>html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #000000;}\n";
  ptr += "body{margin-top: 50px; background-color: #962bcc; color: white;}\n";
  ptr += "h1 {margin: 50px auto 30px;}\n";
  ptr += "h2 {margin: 40px auto 20px;}\n";
  ptr += "p {font-size: 24px; color: #f8f8f8 ;margin-bottom: 10px;}\n";
  ptr += "b{color: black;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
  ptr += "<h1>Monitoramento de Temperatura e Umidade</h1>\n";

  
  ptr += "<p><b>Temperatura: </b>";
  ptr += (float)Temperaturastat;
  ptr += " Graus Celsius</p>";
  ptr += "<p><b>Umidade: </b>";
  ptr += (float)Umidadestat;
  ptr += " %</p>";

  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;

}
