#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define WIFI_SSID "Adora_X"
#define WIFI_PASS "b10nomor11"

#define IN1 15
#define IN2 13
#define IN3 12
#define IN4 14

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>Nodsoket</title>
  <style>
    #buttons {
      display: grid;
      column-gap: 20px;
      row-gap: 10px;
      grid-template-columns: auto auto;
    }
  </style>
</head>
<body>
  <p>Press theese buttons or keys to control</p>
    <div id="buttons">
      <button id="b_w">/\<br>W</button>
      <button id="b_i">/\<br>I</button>
      <button id="b_a">A<br>\/</button>
      <button id="b_l">L<br>\/</button>
    </div>
  <script>
    const gateway = `ws://${window.location.hostname}/ws`;
    let websocket;
    function initWebSocket() {
      console.log("Opening WebSocket Connection...");
      websocket = new WebSocket(gateway);
      websocket.onopen = onOpen;
      websocket.onclose = onClose;
      websocket.onmessage = onMessage;
    }

    function onOpen(event) {
      console.log("WebSocket Connection Opened");
    }

    function onClose(event) {
      console.log("WebSocket Connection Closed");
      setTimeout(initWebSocket, 2000);
    }

    function onMessage(event) {
      const state = event.data == "1" ? "ON" : "OFF";
      document.getElementById("state").innerHTML = state;
    }

    window.addEventListener("load", event => {
      initWebSocket();
      initButtons();
    });

    function initButtons() {
      document.getElementById("b_w").addEventListener("mousedown", () => websocket.send("left_forward"));
      document.getElementById("b_i").addEventListener("mousedown", () => websocket.send("right_forward"));
      document.getElementById("b_a").addEventListener("mousedown", () => websocket.send("left_backward"));
      document.getElementById("b_l").addEventListener("mousedown", () => websocket.send("right_backward"));

      document.getElementById("b_w").addEventListener("mouseup", () => websocket.send("stop_left"));
      document.getElementById("b_i").addEventListener("mouseup", () => websocket.send("stop_right"));
      document.getElementById("b_a").addEventListener("mouseup", () => websocket.send("stop_left"));
      document.getElementById("b_l").addEventListener("mouseup", () => websocket.send("stop_right"));

      window.addEventListener("keydown", e => {
        switch (e.code) {
          case "KeyW":
            websocket.send("left_forward");
            break;
          case "KeyI":
            websocket.send("right_forward");
            break;
          case "KeyA":
            websocket.send("left_backward");
            break;
          case "KeyL":
            websocket.send("right_backward");
            break;
        }
      });

      window.addEventListener("keyup", e => {
        switch (e.code) {
          case "KeyW":
            websocket.send("stop_left");
            break;
          case "KeyI":
            websocket.send("stop_right");
            break;
          case "KeyA":
            websocket.send("stop_left");
            break;
          case "KeyL":
            websocket.send("stop_right");
            break;
        }
      });
    }
  </script>
</body>
</html>
)rawliteral";

void handleWebSocketMessage(void* arg, uint8_t* data, size_t len) {
  AwsFrameInfo* info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "left_forward") == 0) {
      Serial.println("left_forward");
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
    } else if (strcmp((char*)data, "left_backward") == 0) {
      Serial.println("left_backward");
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
    } else if (strcmp((char*)data, "right_forward") == 0) {
      Serial.println("right_forward");
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    } else if (strcmp((char*)data, "right_backward") == 0) {
      Serial.println("right_backward");
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    } else if (strcmp((char*)data, "stop_left") == 0) {
      Serial.println("stop_left");
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
    } else if (strcmp((char*)data, "stop_right") == 0) {
      Serial.println("stop_right");
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
    }
  }
}

void onEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u diconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  initWebSocket();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", index_html);
  });

  server.begin();
}

void loop() {
  ws.cleanupClients();
}
