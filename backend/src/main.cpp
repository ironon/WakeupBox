#include <Arduino.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <string>
#include <unordered_map>
#include <FS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#define FORMAT_SPIFFS_IF_FAILED true
#define MULTI_LINE_STRING(...) #__VA_ARGS__
// Replace with your network credentials
// const char* ssid = "Kompsci LAN";
// const char* password = "$nowOnTheBeach5";

const char* ssid = "Wiremore";
const char* password = "antiwireless789!";
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;

bool manuallyLocked = false;
IPAddress local_IP(192, 168, 1, 184);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
AsyncWebServer server(80);
String header;
unsigned long currentTime = millis();
// std::hash<String> hasher;
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;


String wakeupTime = "06:31";
int snoozePin = 26;
int soundPin = 12;
//for other board not screen (for moms house one i think)
// int soundPin = 22;

int GMT_OFFSET = 5;
int delayBetweenBeeps = 30;
bool snooze = false;
bool disabled = false;
int snoozeTime = 1000 * 30;
unsigned long startupTimeD = 1000 * 60 * 10;
unsigned long initTime = millis();
unsigned long startupTime = initTime + startupTimeD;
int beepsRemaining = 0;
void beep(int times) {
 
  beepsRemaining = times;
}
 // get wakeup time from SPIFFS
String getWakeupTime() {
  File file = SPIFFS.open("/wakeupTime.txt", "r");
  if (!file) {
    Serial.println("There was an error opening the file for reading");
    return "06:15";
  }
  String time = file.readStringUntil('\n');
  file.close();
  return time;
}

String processor(const String& var){
  //Serial.println(var);
 
  return String();
}

const char index_html[] PROGMEM = R"rawliteral(<!doctype html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <!-- <link rel="icon" type="image/svg+xml" href="/vite.svg" /> -->
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>David's Amazing Alarm Clock of Doom</title>
    <script type="module">
      var ie=Object.defineProperty;var se=(e,t,n)=>t in e?ie(e,t,{enumerable:!0,configurable:!0,writable:!0,value:n}):e[t]=n;var G=(e,t,n)=>(se(e,typeof t!="symbol"?t+"":t,n),n);(function(){const t=document.createElement("link").relList;if(t&&t.supports&&t.supports("modulepreload"))return;for(const l of document.querySelectorAll('link[rel="modulepreload"]'))r(l);new MutationObserver(l=>{for(const o of l)if(o.type==="childList")for(const c of o.addedNodes)c.tagName==="LINK"&&c.rel==="modulepreload"&&r(c)}).observe(document,{childList:!0,subtree:!0});function n(l){const o={};return l.integrity&&(o.integrity=l.integrity),l.referrerPolicy&&(o.referrerPolicy=l.referrerPolicy),l.crossOrigin==="use-credentials"?o.credentials="include":l.crossOrigin==="anonymous"?o.credentials="omit":o.credentials="same-origin",o}function r(l){if(l.ep)return;l.ep=!0;const o=n(l);fetch(l.href,o)}})();function E(){}function re(e){return e()}function X(){return Object.create(null)}function I(e){e.forEach(re)}function le(e){return typeof e=="function"}function ue(e,t){return e!=e?t==t:e!==t||e&&typeof e=="object"||typeof e=="function"}function fe(e){return Object.keys(e).length===0}function s(e,t){e.appendChild(t)}function K(e,t,n){e.insertBefore(t,n||null)}function P(e){e.parentNode&&e.parentNode.removeChild(e)}function g(e){return document.createElement(e)}function H(e){return document.createTextNode(e)}function w(){return H(" ")}function A(e,t,n,r){return e.addEventListener(t,n,r),()=>e.removeEventListener(t,n,r)}function m(e,t,n){n==null?e.removeAttribute(t):e.getAttribute(t)!==n&&e.setAttribute(t,n)}function ae(e){return Array.from(e.childNodes)}function oe(e,t){t=""+t,e.data!==t&&(e.data=t)}function j(e,t){e.value=t??""}function Y(e,t,n,r){n==null?e.style.removeProperty(t):e.style.setProperty(t,n,r?"important":"")}let z;function q(e){z=e}const O=[],Z=[];let C=[];const ee=[],de=Promise.resolve();let F=!1;function pe(){F||(F=!0,de.then(ce))}function U(e){C.push(e)}const V=new Set;let T=0;function ce(){if(T!==0)return;const e=z;do{try{for(;T<O.length;){const t=O[T];T++,q(t),he(t.$$)}}catch(t){throw O.length=0,T=0,t}for(q(null),O.length=0,T=0;Z.length;)Z.pop()();for(let t=0;t<C.length;t+=1){const n=C[t];V.has(n)||(V.add(n),n())}C.length=0}while(O.length);for(;ee.length;)ee.pop()();F=!1,V.clear(),q(e)}function he(e){if(e.fragment!==null){e.update(),I(e.before_update);const t=e.dirty;e.dirty=[-1],e.fragment&&e.fragment.p(e.ctx,t),e.after_update.forEach(U)}}function _e(e){const t=[],n=[];C.forEach(r=>e.indexOf(r)===-1?t.push(r):n.push(r)),n.forEach(r=>r()),C=t}const me=new Set;function ge(e,t){e&&e.i&&(me.delete(e),e.i(t))}function be(e,t,n){const{fragment:r,after_update:l}=e.$$;r&&r.m(t,n),U(()=>{const o=e.$$.on_mount.map(re).filter(le);e.$$.on_destroy?e.$$.on_destroy.push(...o):I(o),e.$$.on_mount=[]}),l.forEach(U)}function ye(e,t){const n=e.$$;n.fragment!==null&&(_e(n.after_update),I(n.on_destroy),n.fragment&&n.fragment.d(t),n.on_destroy=n.fragment=null,n.ctx=[])}function ke(e,t){e.$$.dirty[0]===-1&&(O.push(e),pe(),e.$$.dirty.fill(0)),e.$$.dirty[t/31|0]|=1<<t%31}function $e(e,t,n,r,l,o,c=null,b=[-1]){const y=z;q(e);const i=e.$$={fragment:null,ctx:[],props:o,update:E,not_equal:l,bound:X(),on_mount:[],on_destroy:[],on_disconnect:[],before_update:[],after_update:[],context:new Map(t.context||(y?y.$$.context:[])),callbacks:X(),dirty:b,skip_bound:!1,root:t.target||y.$$.root};c&&c(i.root);let p=!1;if(i.ctx=n?n(e,t.props||{},(h,k,...S)=>{const $=S.length?S[0]:k;return i.ctx&&l(i.ctx[h],i.ctx[h]=$)&&(!i.skip_bound&&i.bound[h]&&i.bound[h]($),p&&ke(e,h)),k}):[],i.update(),p=!0,I(i.before_update),i.fragment=r?r(i.ctx):!1,t.target){if(t.hydrate){const h=ae(t.target);i.fragment&&i.fragment.l(h),h.forEach(P)}else i.fragment&&i.fragment.c();t.intro&&ge(e.$$.fragment),be(e,t.target,t.anchor),ce()}q(y)}class we{constructor(){G(this,"$$");G(this,"$$set")}$destroy(){ye(this,1),this.$destroy=E}$on(t,n){if(!le(n))return E;const r=this.$$.callbacks[t]||(this.$$.callbacks[t]=[]);return r.push(n),()=>{const l=r.indexOf(n);l!==-1&&r.splice(l,1)}}$set(t){this.$$set&&!fe(t)&&(this.$$.skip_bound=!0,this.$$set(t),this.$$.skip_bound=!1)}}const ve="4";typeof window<"u"&&(window.__svelte||(window.__svelte={v:new Set})).v.add(ve);function te(e){let t;return{c(){t=g("h2"),t.textContent="DISABLED"},m(n,r){K(n,t,r)},d(n){n&&P(t)}}}function ne(e){let t,n=e[4]?"Enable":"Disable",r,l,o;return{c(){t=g("button"),r=H(n),Y(t,"background-color","#ffcc66"),Y(t,"color","black"),m(t,"class","svelte-rq7w07")},m(c,b){K(c,t,b),s(t,r),l||(o=A(t,"click",e[7]),l=!0)},p(c,b){b&16&&n!==(n=c[4]?"Enable":"Disable")&&oe(r,n)},d(c){c&&P(t),l=!1,o()}}}function Ee(e){return{c:E,m:E,d:E}}function Se(e){let t;return{c(){t=g("div"),t.innerHTML="",m(t,"id","lockables")},m(n,r){K(n,t,r)},d(n){n&&P(t)}}}function Le(e){let t,n,r,l,o,c,b,y,i,p,h,k,S,$,B,L,M,a,f,J,R,Q,_=e[4]&&te(),d=e[0].length>3&&ne(e);function W(u,D){return u[2]?Ee:Se}let x=W(e),v=x(e);return{c(){t=g("div"),n=g("div"),r=g("h1"),r.textContent="DAVID-ALARM 3000",l=w(),_&&_.c(),o=w(),c=g("p"),b=H("Current Time: "),y=H(e[3]),i=w(),p=g("input"),h=w(),k=g("button"),k.textContent="Test Beep",S=w(),$=g("button"),$.textContent="Lock",B=w(),L=g("button"),L.textContent="Restart",M=w(),d&&d.c(),a=w(),f=g("input"),J=w(),v.c(),m(p,"type","time"),m(p,"class","svelte-rq7w07"),m(k,"class","svelte-rq7w07"),m($,"class","svelte-rq7w07"),m(L,"class","svelte-rq7w07"),m(f,"type","text"),m(f,"placeholder","Admin Key"),m(f,"class","svelte-rq7w07"),m(n,"class","alarm-clock svelte-rq7w07"),m(t,"class","alarm-clock-container svelte-rq7w07")},m(u,D){K(u,t,D),s(t,n),s(n,r),s(n,l),_&&_.m(n,null),s(n,o),s(n,c),s(c,b),s(c,y),s(n,i),s(n,p),j(p,e[1]),s(n,h),s(n,k),s(n,S),s(n,$),s(n,B),s(n,L),s(n,M),d&&d.m(n,null),s(n,a),s(n,f),j(f,e[0]),s(n,J),v.m(n,null),R||(Q=[A(p,"input",e[10]),A(p,"change",e[11]),A(k,"click",e[6]),A($,"click",e[9]),A(L,"click",e[8]),A(f,"input",e[12])],R=!0)},p(u,[D]){u[4]?_||(_=te(),_.c(),_.m(n,o)):_&&(_.d(1),_=null),D&8&&oe(y,u[3]),D&2&&j(p,u[1]),u[0].length>3?d?d.p(u,D):(d=ne(u),d.c(),d.m(n,a)):d&&(d.d(1),d=null),D&1&&f.value!==u[0]&&j(f,u[0]),x!==(x=W(u))&&(v.d(1),v=x(u),v&&(v.c(),v.m(n,null)))},i:E,o:E,d(u){u&&P(t),_&&_.d(),d&&d.d(),v.d(),R=!1,I(Q)}}}let N="7cb5d18ae409a37c10d3fdf3b68408ea3503d0335796bf949914e400ae2cc27d48ffa59034c01d033a6a854847561c5f2a1f19567e4f569635862d2550a6a1a50de09a20ee3b6b3946157353d568b3ee";function De(e,t,n){let r="",l=!0,o=new Date().toLocaleTimeString(),c=localStorage.getItem("secret")||"",b=!1;const y=async()=>{let f=await(await fetch(`/getTime?key=${N}`)).text();return console.log(f),f},i=async a=>{let f=N;c.length>3&&(f=c),await fetch(`/setTime?time=${a}&key=${f}ENDMESSAGE`,{method:"GET"})};y().then(a=>{a.includes("D")&&(a=a.replace("D",""),n(4,b=!0)),n(1,r=a)});const p=()=>{fetch(`/beep?key=${N}`)},h=()=>{fetch(`/disable?key=${c}ENDMESSAGE`)},k=()=>{fetch(`/restart?key=${N}`)},S=()=>{fetch(`/setLocked?key=${N}`)},$=()=>{const a=new Date().getHours();return a<9||a>18};setInterval(()=>{n(3,o=new Date().toLocaleTimeString()),n(2,l=$()),l&&new Date().getHours()},1e3);function B(){r=this.value,n(1,r)}const L=()=>i(r);function M(){c=this.value,n(0,c)}return e.$$.update=()=>{e.$$.dirty&1&&localStorage.setItem("secret",c)},[c,r,l,o,b,i,p,h,k,S,B,L,M]}class Ae extends we{constructor(t){super(),$e(this,t,De,Le,ue,{})}}new Ae({target:document.getElementById("app")});
    </script>
    <style>
      :root{font-family:Inter,system-ui,Avenir,Helvetica,Arial,sans-serif;line-height:1.5;font-weight:400;color-scheme:light dark;color:#000;background-color:#242424;font-synthesis:none;text-rendering:optimizeLegibility;-webkit-font-smoothing:antialiased;-moz-osx-font-smoothing:grayscale}a{font-weight:500;color:#646cff;text-decoration:inherit}a:hover{color:#535bf2}html{width:100%;height:100%}body{margin:0;display:flex;place-items:center;min-width:320px;min-height:100vh;width:100%;height:100%}h1{font-size:3.2em;line-height:1.1}.card{padding:2em}#app{height:100%%;width:100%%;text-align:center}button{border-radius:8px;border:1px solid transparent;padding:.6em 1.2em;font-size:1em;font-weight:500;font-family:inherit;background-color:#1a1a1a;cursor:pointer;transition:border-color .25s}button:hover{border-color:#646cff}button:focus,button:focus-visible{outline:4px auto -webkit-focus-ring-color}@media (prefers-color-scheme: light){:root{color:#213547;background-color:#fff}a:hover{color:#747bff}button{background-color:#f9f9f9}}.alarm-clock-container.svelte-rq7w07{display:flex;flex-direction:column;justify-content:center;align-items:center;height:100%;width:100%;background:linear-gradient(135deg,#ede2ad,#f7c9a1)}.alarm-clock.svelte-rq7w07{background:#fff;padding:2rem;border-radius:15px;box-shadow:0 4px 6px #0000001a;display:flex;flex-direction:column;align-items:center}input.svelte-rq7w07,button.svelte-rq7w07{margin:.5rem 0;padding:.5rem;border:1px solid #ccc;border-radius:5px;min-width:6rem}button.svelte-rq7w07{cursor:pointer;background-color:#646cff;color:#fff;border:none}button.svelte-rq7w07:hover{background-color:#535bf2}
    </style>
  </head>
  <body>
    <div id="app"></div>
  </body>
</html>

  )rawliteral";





// String getDefaultPage() {

// }
bool isRequest(String header, String url) {
  // if (header.indexOf("key=7cb5d18ae409a37c10d3fdf3b68408ea3503d0335796bf949914e400ae2cc27d48ffa59034c01d033a6a854847561c5f2a1f19567e4f569635862d2550a6a1a50de09a20ee3b6b3946157353d568b3ee") >= 0) {
    if (header.indexOf(url) >= 0) {
      return true;
    }
    
  
  return false;
}
const int PRIME_CONST = 31;

int hashFunction(String key) { //literal black magjic (acutally i kinda get it)
    unsigned long hashCode = 0;
    for (int i = 0; i < key.length(); i++) {
        hashCode += long (key[i] * pow(PRIME_CONST, i));
    }
    return hashCode;
}

bool isAuthorizedRequest(String header) {
  int startIndex = header.indexOf("key=") + 4;
  int endIndex = header.indexOf("ENDMESSAGE");
  Serial.println(header.substring(startIndex, endIndex));
  int hash = hashFunction(header.substring(startIndex, endIndex));
  Serial.print("HASH: ");
  Serial.println(hash);
  // header.indexOf("key=")
  // if (header.indexOf("key=") >= 0) {
  //   return true;
  // }
  return hash == 1465416556;
}
int lastHour = -1;
int getHours() {
  // formattedDate = timeClient.getHours();
  int newHour = timeClient.getHours();


  lastHour =newHour;
  return lastHour;
}
int getMinutes() {
  return timeClient.getMinutes();
}
int getWakeupHour() {
  // uses wakeupTime, the string, and turns it into just the hour component
  return wakeupTime.substring(0, 2).toInt();
}
int getWakeupMinutes() {
  // uses wakeupTime, the string, and turns it into just the minute component
  return wakeupTime.substring(3, 5).toInt();
}
bool getShouldBeep() {
  if (getHours() < 4) { //once, this alarm clock bugged out and woke me up at like 1am. i was not amused.
    return false;
  }
  float currentTime = getHours() + (getMinutes() / 60.0);
  float wakeupTime = getWakeupHour() + (getWakeupMinutes() / 60.0);
  bool beeping = (currentTime - wakeupTime < 0.5) && (currentTime - wakeupTime >= 0);
  Serial.println(currentTime);
  Serial.println(wakeupTime);
  Serial.println(getHours());
  Serial.println(timeClient.getHours());
  Serial.println(((timeClient.getEpochTime()  % 86400L) / 3600));
  Serial.println("=========");
  return beeping && !disabled;
}
bool IsLocked() {
  // returns false if and only if the time is between 10am and 6pm {
  // return true;
  // if millis() is before startup time, return false
  if (millis() < startupTime) {
    return false;
  }
  if (getShouldBeep()) {
    return true;
  }
  Serial.println(manuallyLocked);
  if (manuallyLocked) {
    Serial.println("I AM LOCKED.");
    return manuallyLocked;
  }

  int currentHour = getHours();
  // int currentMinute = getMinutes();
  return !(currentHour >= 10 && currentHour < 23.9 || currentHour < 4);

}
String getQueryStringFromURL(String url, String query) {
  int startIndex = url.indexOf(query + "=");
  if (startIndex != -1) {
    int endIndex = url.indexOf("&", startIndex);
    if (endIndex == -1) { // If there's no other parameter after this one
      endIndex = url.length();
    }
    return url.substring(startIndex + query.length() + 1, endIndex);
  }
  return "";
}

bool isFailureZone() {
  return getHours() < 12;  // anything after midnight is failure
}

// use SPIFFS to save the wakeup time
void saveWakeupTime(String time) {
  File file = SPIFFS.open("/wakeupTime.txt", "w");
  if (!file) {
    Serial.println("There was an error opening the file for writing");
    return;
  }
  file.println(time);
  file.close();
}
// void doWifiStuff() { 
//   WiFiClient client = server.available();   // Listen for incoming clients

//   if (client) {                             // If a new client connects,
//     currentTime = millis();
//     previousTime = currentTime;
//     Serial.println("New Client.");          // print a message out in the serial port
//     String currentLine = "";                // make a String to hold incoming data from the client
//     while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
//       currentTime = millis();
//       if (client.available()) {             // if there's bytes to read from the client,
//         char c = client.read();             // read a byte, then
//         Serial.write(c);                    // print it out the serial monitor
//         header += c;
//         if (c == '\n') {                    // if the byte is a newline character
//           // if the current line is blank, you got two newline characters in a row.
//           // that's the end of the client HTTP request, so send a response:
//           if (currentLine.length() == 0) {
//             // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
//             // and a content-type so the client knows what's coming, then a blank line:
//             client.println("HTTP/1.1 200 OK");
//             client.println("Content-type:text/html");
//             client.println("Connection: close");
//             client.println();
//             if (isRequest(header, "/setLocked")) {
//               manuallyLocked = true;
//               Serial.println("Manually locked unit!");
//               // there's no way to unlock an manual lock, you just gotta wait until the next day.
//             }
//             if (isRequest(header, "/setTime")) {
//               String time = getQueryStringFromURL(header, "time");
//                if (!IsLocked()) {
//                  Serial.println(time);
//                 wakeupTime = time;
//                 saveWakeupTime(time);
//               } else if (isAuthorizedRequest(header)) {
//                 Serial.println(time);
//                 wakeupTime = time;
//                 saveWakeupTime(time);
//               }
             
//             }
//             if (isRequest(header, "/getTime")) {
//               if (disabled) {
//                 client.print("D");
//               }
//               if (getWakeupHour() < 10) {
//                 client.print("0");
//               }
//               client.print(getWakeupHour());
//               client.print(":");
//               if (getWakeupMinutes() < 10) {
//                 client.print("0");
//               }
//               client.print(getWakeupMinutes());
//               break;

//             }
//             if (isRequest(header, "/beep")) {

//               if (disabled) {
//                 beep(10);
//                 delay(20);
//                 beep(10);
//                 delay(20);
           
//               }
//               beep(15);
//               Serial.println("Beeped!");
//             }
//             if (isRequest(header, "/restart")) {
//               ESP.restart();
//             }
//             if (isRequest(header, "/disable")) {

//              if (isAuthorizedRequest(header)) {
//               disabled = !disabled;
//             }
//             }
           
//             client.println(getDefaultPage());
//             // Break out of the while loop
//             break;
//           } else { // if you got a newline, then clear currentLine
//             currentLine = "";
//           }
//         } else if (c != '\r') {  // if you got anything else but a carriage return character,
//           currentLine += c;      // add it to the end of the currentLine
//         }
//       }
//     }
//     // Clear the header variable
//     header = "";
//     // Close the connection
//     client.stop();
//     Serial.println("Client disconnected.");
//     Serial.println("");
//   }
// }

int lastBeep = 0;
int nextBeep = 0;
bool firstten = true;
void checkBeeping() {
  // Serial.println("Checking beeping!");
  // check if the time is <30 after the wakeup time
  
 
  if (timeClient.getEpochTime() > 2000000000) {
    return;
  }
  if (getHours() == 1) {
    disabled = false;
  }
  // Serial.println(currentTime-wakeupTime);

  // the first beep waits 10 minutes, which is a 6th of an hour, so I'm subtracting it off so it doesn't affect how long this thing beeps for.
  if (getShouldBeep()) {
    Serial.println("I should be beeping!");
   
      beep(75);
      nextBeep = millis() + (1000 * 5);

  } 


}
int lastMinute = -34;
unsigned long disconnectTime = 0;
  void loop() {
  // put your main code here, to run repeatedly:
  

  while(!timeClient.update()) {
    // Serial.println("Doing Wifi Stuff")
    // doWifiStuff(); //deprecated 10/11/24 in favor of better async webserver
    if (getHours() > 20 && getHours() < 21 && getMinutes() < 10) { /// at 8pm it locks to 6am wakeup time
      if (!manuallyLocked){
        wakeupTime = "06:00";
        saveWakeupTime(wakeupTime);
        manuallyLocked = true;
      }
    }
      timeClient.forceUpdate();

    // int snoozeRead = digitalRead(snoozePin);
    // Serial.println(snoozeRead);
    // if(snoozeRead == HIGH) {
      // snooze = true;
      // Serial.println("Snoozing!");
    // }
    if (millis() > startupTime) { //if it's been 10 minutes since startup, start beeping
      checkBeeping();
    }
    while (beepsRemaining > 0) {
   
      digitalWrite(soundPin, HIGH);
      delay(20);
      digitalWrite(soundPin, LOW);
      delay(random(20, 50));
      beepsRemaining--;
    }
    
  }
  // delay(500);
  // if wifi is not connected, wait an hour, if its still not connected, restart esp. do not execution by using delay()
  if (!WiFi.isConnected()) {
    disconnectTime = millis();
  }
  if (millis() - disconnectTime > 1000 * 60 * 60) {
    if (!WiFi.isConnected()) {
     ESP.restart();
    }
  } else if (millis() - initTime > (1000 * 60 * 60 * 24) ) {
    ESP.restart();
  }

 


}

void setup() {
  // put your setup code here, to run once:
  pinMode(soundPin, OUTPUT);
  pinMode(snoozePin, INPUT);
  Serial.begin(9600);
  Serial.println("Starting...");
  Serial.println("Sound pin: " + String(soundPin));
  Serial.println("Delay between beeps: " + String(delayBetweenBeeps));
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();
  Serial.println(WiFi.dnsIP());
  WiFi.config(local_IP, WiFi.gatewayIP(), WiFi.subnetMask(), IPAddress(8,8,8,8)); 
  delay(10);
  Serial.println(WiFi.dnsIP());
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  int gmtOffset = 4;
  // configTime(-3600*gmtOffset, 3600, "pool.ntp.org");
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  // timeClient
  
  // if ()
  timeClient.setTimeOffset(-3600*GMT_OFFSET);
 
  Serial.println("Ready to play!");
  wakeupTime = getWakeupTime();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/setLocked", HTTP_GET, [](AsyncWebServerRequest *request){
    manuallyLocked = true;
    request->send_P(200, "text/html", "ok", processor);
  });
   server.on("/getTime", HTTP_GET, [](AsyncWebServerRequest *request){
    String time = "";
    if (disabled) {
      time = time + "D";
    }
    if (getWakeupHour() < 10) {
       time = time + "0";
    }
    time = time + getWakeupHour();
    time = time + ":";
    if (getWakeupMinutes() < 10) {
      time = time + "0";
    }
    time = time + getWakeupMinutes();
    char timeChar[time.length() + 1]; 
    time.toCharArray(timeChar, time.length() + 1);
    request->send_P(200, "text/html", timeChar, processor);

    // request->send_P(200, "text/html", time, processor);
  });
   server.on("/setTime", HTTP_GET, [](AsyncWebServerRequest *request){
    // get query string from request
    Serial.println("help me");
  
    String time = request->getParam("time")->value();
    Serial.println(time);
    if (!IsLocked()) {
      Serial.println(time);
      wakeupTime = time;
      saveWakeupTime(time);
    } else if (isAuthorizedRequest(header)) {
      Serial.println(time);
      wakeupTime = time;
      saveWakeupTime(time);
    }

    // convert time to char
    char timeChar[time.length() + 1];
    time.toCharArray(timeChar, time.length() + 1);

    request->send_P(200, "text/html", timeChar, processor);
  });
  server.on("/beep", HTTP_GET, [](AsyncWebServerRequest *request){
   
    beep(50);
   
    request->send_P(200, "text/html", "ok", processor);
  });
  server.on("/disable", HTTP_GET, [](AsyncWebServerRequest *request){
    if (isAuthorizedRequest(request->url())) {
      disabled = true;
    }
    request->send_P(200, "text/html", "ok", processor);
  });
  server.on("/restart", HTTP_GET, [](AsyncWebServerRequest *request){
    if (isAuthorizedRequest(request->url())) {
      ESP.restart();
    }
    // request->send_P(200, "text/html", "ok", processor);
  });
  server.on("/test", HTTP_GET, [](AsyncWebServerRequest *request){
    beep(1);
    request->send_P(200, "text/html", "ok", processor);
  });

  

  

}

// put function definitions here:
