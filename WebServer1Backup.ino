//Thư viện ESP
#include <ESP8266WiFi.h>


const char* ssid = "AoiRey"; //Tên mạng Wifi đang kết nối
const char* password = "kiochinsu"; //Pass mạng Wifi


// Cổng server là 80
WiFiServer server(80);

//Biến lưu trữ mà HTTP yêu cầu
String header;

//Tạo các biến phụ để lưu trữ trạng thái hiện tại
String relay1State = "off";
String relay2State = "off";
String relay3State = "off";
String relay4State = "off";

// Gán biến đầu ra cho chân tín hiệu GPIO
const int relay1 = 5; // GPIO5 gán cho chân D1 ứng với đèn IN1
const int relay2 = 4; // GPIO4 gán cho chân D2 ứng với đèn IN2
const int relay3 = 0; // GPIO0 gán cho chân D3 ứng với đèn IN3
const int relay4 = 2; // GPIO2 gán cho chân D4 ứng với đèn IN4

void setup() {
  Serial.begin(115200);
  //Khởi động một cổng Serial với baudrate 115200
  //Khởi tạo biến đầu ra
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  // Kích trạng thái. Nếu đầu ra là CAO. thì relay sẽ ở trạng thái THẤP
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);

  // Kết nối wifi
  //Lấy tài khoản và pass của wifi đẻ kết nối vào
  WiFi.begin(ssid, password);
  Serial.print("Ket noi den mang: ");
  Serial.println(ssid);
  
  while (WiFi.status() != WL_CONNECTED) { //Trường hợp trạng thái kết nối sai
    delay(500);//Mỗi 500ms thì in 1 dấu (.)
    Serial.print("."); //Nếu không thể kết nối wifi sẽ in ra dấu (.)
  }
    // Trường hợp kết nối wifi thành công. In ra địa chỉ IP Local và bắt đầu kết nối
  Serial.println("");
  Serial.println("Ket noi thanh cong !");

  server.begin();
  Serial.println ("Bat dau Server");
  Serial.println("Dia chi IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  
  WiFiClient client = server.available();   // Lắng nghe yêu cầu từ phía Client
  
 //Check xem có Client nào kết nối với Server của mình không ?
  if (client) {                             //Nếu 1 client mới kết nối đến
    Serial.println("Xin Chao Client.");     //Gửi lời chào đến Client
    String currentLine = "";                //Tạo một chuỗi để giữ dữ liệu gửi đến từ máy khách
    while (client.connected()) {            //Tạo vòng lặp while Client được kết nối
      if (client.available()) {             //Khi Client có thể kết nối
        char c = client.read();             
        Serial.write(c);                     // in ra số seri của màn hình
        header += c;
        if (c == '\n') {                    // Ngược lại byte là giá trị mới
          
         // Trường hợp Client muốn kết thúc yêu cầu HTTP
          if (currentLine.length() == 0) {
            
            client.println("HTTP/1.1 200 OK");  // Tiêu đề HTTP luôn bắt đầu bằng mã phản hồi (vd: HTTP/1.1 200 OK)
            client.println("Kieu noi dung:text/html");
            client.println("Trang thai ket noi: DONG");
            client.println();

            // Bật tắt chân tín hiệu GPIO
            if (header.indexOf("GET /5/on") >= 0)
            {
              Serial.println("Den IN1 BAT");
              relay1State = "on";
              digitalWrite(relay1, LOW);
            }
            else if (header.indexOf("GET /5/off") >= 0)
            {
              Serial.println("Den IN1 TAT");
              relay1State = "off";
              digitalWrite(relay1, HIGH);
            }
            else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("Den IN2 BAT");
              relay2State = "on";
              digitalWrite(relay2, LOW);
            }
            else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("Den IN1 TAT");
              relay2State = "off";
              digitalWrite(relay2, HIGH);
            }
            else if (header.indexOf("GET /0/on") >= 0)
            {
              Serial.println("Den IN3 BAT");
              relay3State = "on";
              digitalWrite(relay3, LOW);
            }
            else if (header.indexOf("GET /0/off") >= 0)
            {
              Serial.println("Den IN3 TAT");
              relay3State = "off";
              digitalWrite(relay3, HIGH);
            }
            else if (header.indexOf("GET /2/on") >= 0) {
              Serial.println("Den IN4 BAT");
              relay4State = "on";
              digitalWrite(relay4, LOW);
            }
            else if (header.indexOf("GET /2/off") >= 0) {
              Serial.println("Den IN4 TAT");
              relay4State = "off";
              digitalWrite(relay4, HIGH);
            }

            // Tao cho server 1 trang web
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // Sử dụng CSS để tạo nút ON/OFF
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 12px 24px;");
            client.println("text-decoration: none; font-size: 20px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");

            // Tiêu đề trang
            client.println("<body><h1>DIEU KHIEN WEB SERVER</h1>");

            //  Hiển thị trạng thái hiện tại và các nút ON/OFF cho chân tín hiệu GPIO 5
            client.println("<p>Den 1 - Trang thai: " + relay1State + "</p>");
            // Khi relay1State ở trạng thái OFF, trên màn hình sẽ hiển thị ON
            if (relay1State == "off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // //  Hiển thị trạng thái hiện tại và các nút ON/OFF cho chân tín hiệu GPIO 4
            client.println("<p>Den 2 - Trang thai: " + relay2State + "</p>");
            // Khi relay2State ở trạng thái OFF, trên màn hình sẽ hiển thị ON
            if (relay2State == "off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // //  Hiển thị trạng thái hiện tại và các nút ON/OFF cho chân tín hiệu GPIO 0
            client.println("<p>Den 3 - Trang thai: " + relay3State + "</p>");
            // Khi relay3State ở trạng thái OFF, trên màn hình sẽ hiển thị ON
            if (relay3State == "off") {
              client.println("<p><a href=\"/0/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/0/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // //  Hiển thị trạng thái hiện tại và các nút ON/OFF cho chân tín hiệu GPIO 2
            client.println("<p>Den 4 - Trang thai: " + relay4State + "</p>");
            // Khi relay4State ở trạng thái OFF, trên màn hình sẽ hiển thị ON
            if (relay4State == "off") {
              client.println("<p><a href=\"/2/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/2/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("</body></html>");

            
            client.println();
            
            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      
        }
      }
    }
    // Xóa giá tri ban đầu
    header = "";
    // Đóng kết nối
    client.stop();
    Serial.println("Ket thuc ket noi voi Client !");
    Serial.println("");
  }
}
