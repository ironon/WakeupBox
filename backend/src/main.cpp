#include <Arduino.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <FS.h>
#include <SPIFFS.h>
#define FORMAT_SPIFFS_IF_FAILED true
#define MULTI_LINE_STRING(...) #__VA_ARGS__
// Replace with your network credentials
const char* ssid = "Kompsci LAN";
const char* password = "$nowOnTheBeach5";
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;

IPAddress local_IP(192, 168, 1, 184);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
WiFiServer server(80);
String header;
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

String wakeupTime = "06:31";
// int soundPin = 12;
// int snoozePin = 26;

//for other board not screen
int soundPin = 22;
int snoozePin = 26;

int delayBetweenBeeps = 30;
bool snooze = false;
int snoozeTime = 1000 * 30;
unsigned long startupTimeD = 1000 * 60 * 10;
unsigned long startupTime = millis() + startupTimeD;

void beep(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(soundPin, HIGH);
    delay(delayBetweenBeeps);
    digitalWrite(soundPin, LOW);
    delay(delayBetweenBeeps);
  }
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
  timeClient.setTimeOffset(-3600*4);
  Serial.println("Ready to play!");
  wakeupTime = getWakeupTime();

}
String getDefaultPage() {
  return MULTI_LINE_STRING(<!doctype html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <!-- <link rel="icon" type="image/svg+xml" href="/vite.svg" /> -->
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>David's Amazing Alarm Clock of Doom</title>
    <script type="module">var W=Object.defineProperty;var X=(e,t,n)=>t in e?W(e,t,{enumerable:!0,configurable:!0,writable:!0,value:n}):e[t]=n;var P=(e,t,n)=>(X(e,typeof t!="symbol"?t+"":t,n),n);(function(){const t=document.createElement("link").relList;if(t&&t.supports&&t.supports("modulepreload"))return;for(const o of document.querySelectorAll('link[rel="modulepreload"]'))r(o);new MutationObserver(o=>{for(const c of o)if(c.type==="childList")for(const l of c.addedNodes)l.tagName==="LINK"&&l.rel==="modulepreload"&&r(l)}).observe(document,{childList:!0,subtree:!0});function n(o){const c={};return o.integrity&&(c.integrity=o.integrity),o.referrerPolicy&&(c.referrerPolicy=o.referrerPolicy),o.crossOrigin==="use-credentials"?c.credentials="include":o.crossOrigin==="anonymous"?c.credentials="omit":c.credentials="same-origin",c}function r(o){if(o.ep)return;o.ep=!0;const c=n(o);fetch(o.href,c)}})();function L(){}function z(e){return e()}function R(){return Object.create(null)}function O(e){e.forEach(z)}function G(e){return typeof e=="function"}function Y(e,t){return e!=e?t==t:e!==t||e&&typeof e=="object"||typeof e=="function"}function Z(e){return Object.keys(e).length===0}function f(e,t){e.appendChild(t)}function M(e,t,n){e.insertBefore(t,n||null)}function I(e){e.parentNode&&e.parentNode.removeChild(e)}function m(e){return document.createElement(e)}function T(e){return document.createTextNode(e)}function x(){return T(" ")}function N(e,t,n,r){return e.addEventListener(t,n,r),()=>e.removeEventListener(t,n,r)}function $(e,t,n){n==null?e.removeAttribute(t):e.getAttribute(t)!==n&&e.setAttribute(t,n)}function ee(e){return Array.from(e.childNodes)}function J(e,t){t=""+t,e.data!==t&&(e.data=t)}function V(e,t){e.value=t??""}let j;function E(e){j=e}const v=[],F=[];let S=[];const U=[],te=Promise.resolve();let B=!1;function ne(){B||(B=!0,te.then(Q))}function H(e){S.push(e)}const q=new Set;let k=0;function Q(){if(k!==0)return;const e=j;do{try{for(;k<v.length;){const t=v[k];k++,E(t),re(t.$$)}}catch(t){throw v.length=0,k=0,t}for(E(null),v.length=0,k=0;F.length;)F.pop()();for(let t=0;t<S.length;t+=1){const n=S[t];q.has(n)||(q.add(n),n())}S.length=0}while(v.length);for(;U.length;)U.pop()();B=!1,q.clear(),E(e)}function re(e){if(e.fragment!==null){e.update(),O(e.before_update);const t=e.dirty;e.dirty=[-1],e.fragment&&e.fragment.p(e.ctx,t),e.after_update.forEach(H)}}function oe(e){const t=[],n=[];S.forEach(r=>e.indexOf(r)===-1?t.push(r):n.push(r)),n.forEach(r=>r()),S=t}const ce=new Set;function ie(e,t){e&&e.i&&(ce.delete(e),e.i(t))}function le(e,t,n){const{fragment:r,after_update:o}=e.$$;r&&r.m(t,n),H(()=>{const c=e.$$.on_mount.map(z).filter(G);e.$$.on_destroy?e.$$.on_destroy.push(...c):O(c),e.$$.on_mount=[]}),o.forEach(H)}function se(e,t){const n=e.$$;n.fragment!==null&&(oe(n.after_update),O(n.on_destroy),n.fragment&&n.fragment.d(t),n.on_destroy=n.fragment=null,n.ctx=[])}function ue(e,t){e.$$.dirty[0]===-1&&(v.push(e),ne(),e.$$.dirty.fill(0)),e.$$.dirty[t/31|0]|=1<<t%31}function fe(e,t,n,r,o,c,l=null,g=[-1]){const d=j;E(e);const i=e.$$={fragment:null,ctx:[],props:c,update:L,not_equal:o,bound:R(),on_mount:[],on_destroy:[],on_disconnect:[],before_update:[],after_update:[],context:new Map(t.context||(d?d.$$.context:[])),callbacks:R(),dirty:g,skip_bound:!1,root:t.target||d.$$.root};l&&l(i.root);let y=!1;if(i.ctx=n?n(e,t.props||{},(u,b,...h)=>{const s=h.length?h[0]:b;return i.ctx&&o(i.ctx[u],i.ctx[u]=s)&&(!i.skip_bound&&i.bound[u]&&i.bound[u](s),y&&ue(e,u)),b}):[],i.update(),y=!0,O(i.before_update),i.fragment=r?r(i.ctx):!1,t.target){if(t.hydrate){const u=ee(t.target);i.fragment&&i.fragment.l(u),u.forEach(I)}else i.fragment&&i.fragment.c();t.intro&&ie(e.$$.fragment),le(e,t.target,t.anchor),Q()}E(d)}class ae{constructor(){P(this,"$$");P(this,"$$set")}$destroy(){se(this,1),this.$destroy=L}$on(t,n){if(!G(n))return L;const r=this.$$.callbacks[t]||(this.$$.callbacks[t]=[]);return r.push(n),()=>{const o=r.indexOf(n);o!==-1&&r.splice(o,1)}}$set(t){this.$$set&&!Z(t)&&(this.$$.skip_bound=!0,this.$$set(t),this.$$.skip_bound=!1)}}const de="4";typeof window<"u"&&(window.__svelte||(window.__svelte={v:new Set})).v.add(de);function pe(e){let t,n,r,o;return{c(){t=m("p"),n=T("Alarm will be unlocked in "),r=T(e[3]),o=T(" hours")},m(c,l){M(c,t,l),f(t,n),f(t,r),f(t,o)},p(c,l){l&8&&J(r,c[3])},d(c){c&&I(t)}}}function he(e){let t;return{c(){t=m("div"),t.innerHTML="",$(t,"id","lockables")},m(n,r){M(n,t,r)},p:L,d(n){n&&I(t)}}}function me(e){let t,n,r,o,c,l,g,d,i,y,u,b,h,s,_,A;function K(a,w){return a[1]?pe:he}let C=K(e),p=C(e);return{c(){t=m("div"),n=m("div"),r=m("h1"),r.textContent="DAVID-ALARM 3000",o=x(),c=m("p"),l=T("Current Time: "),g=T(e[2]),d=x(),i=m("input"),y=x(),u=m("button"),u.textContent="Test Beep",b=x(),h=m("button"),h.textContent="Dampen",s=x(),p.c(),$(i,"type","time"),$(i,"class","svelte-rq7w07"),$(u,"class","svelte-rq7w07"),$(h,"class","svelte-rq7w07"),$(n,"class","alarm-clock svelte-rq7w07"),$(t,"class","alarm-clock-container svelte-rq7w07")},m(a,w){M(a,t,w),f(t,n),f(n,r),f(n,o),f(n,c),f(c,l),f(c,g),f(n,d),f(n,i),V(i,e[0]),f(n,y),f(n,u),f(n,b),f(n,h),f(n,s),p.m(n,null),_||(A=[N(i,"input",e[7]),N(i,"change",e[8]),N(u,"click",e[5]),N(h,"click",e[6])],_=!0)},p(a,[w]){w&4&&J(g,a[2]),w&1&&V(i,a[0]),C===(C=K(a))&&p?p.p(a,w):(p.d(1),p=C(a),p&&(p.c(),p.m(n,null)))},i:L,o:L,d(a){a&&I(t),p.d(),_=!1,O(A)}}}let D="7cb5d18ae409a37c10d3fdf3b68408ea3503d0335796bf949914e400ae2cc27d48ffa59034c01d033a6a854847561c5f2a1f19567e4f569635862d2550a6a1a50de09a20ee3b6b3946157353d568b3ee";function _e(e,t,n){let r="",o=!0,c=new Date().toLocaleTimeString(),l="0";const g=async()=>{let _=await(await fetch(`/getTime?key=${D}`)).text();return console.log(_),_},d=async s=>{await fetch(`/setTime?time=${s}&key=${D}`+s,{method:"POST"})};g().then(s=>{n(0,r=s)});const i=()=>{fetch(`/beep?key=${D}`)},y=()=>{const s=new Date().getHours();return s<9||s>18},u=()=>{fetch(`/dampen?key=${D}`)};setInterval(()=>{if(n(2,c=new Date().toLocaleTimeString()),n(1,o=y()),o){const s=new Date().getHours(),_=(6-s)*60,A=(9-s)*60;n(3,l=Math.min(_,A).toString()),n(3,l=(l/60).toString())}},1e3);function b(){r=this.value,n(0,r)}return[r,o,c,l,d,i,u,b,()=>d(r)]}class ge extends ae{constructor(t){super(),fe(this,t,_e,me,Y,{})}}new ge({target:document.getElementById("app")});
</script>
    <style>
      :root{font-family:Inter,system-ui,Avenir,Helvetica,Arial,sans-serif;line-height:1.5;font-weight:400;color-scheme:light dark;color:#000;background-color:#242424;font-synthesis:none;text-rendering:optimizeLegibility;-webkit-font-smoothing:antialiased;-moz-osx-font-smoothing:grayscale}a{font-weight:500;color:#646cff;text-decoration:inherit}a:hover{color:#535bf2}html{width:100%;height:100%}body{margin:0;display:flex;place-items:center;min-width:320px;min-height:100vh;width:100%;height:100%}h1{font-size:3.2em;line-height:1.1}.card{padding:2em}#app{height:100%;width:100%;text-align:center}button{border-radius:8px;border:1px solid transparent;padding:.6em 1.2em;font-size:1em;font-weight:500;font-family:inherit;background-color:#1a1a1a;cursor:pointer;transition:border-color .25s}button:hover{border-color:#646cff}button:focus,button:focus-visible{outline:4px auto -webkit-focus-ring-color}@media (prefers-color-scheme: light){:root{color:#213547;background-color:#fff}a:hover{color:#747bff}button{background-color:#f9f9f9}}.alarm-clock-container.svelte-rq7w07{display:flex;flex-direction:column;justify-content:center;align-items:center;height:100%;width:100%;background:linear-gradient(135deg,#89f7fe,#66a6ff)}.alarm-clock.svelte-rq7w07{background:#fff;padding:2rem;border-radius:15px;box-shadow:0 4px 6px #0000001a;display:flex;flex-direction:column;align-items:center}input.svelte-rq7w07,button.svelte-rq7w07{margin:.5rem 0;padding:.5rem;border:1px solid #ccc;border-radius:5px;min-width:6rem}button.svelte-rq7w07{cursor:pointer;background-color:#646cff;color:#fff;border:none}button.svelte-rq7w07:hover{background-color:#535bf2}

      </style>
  </head>
  <body>
    <div id="app"></div>
  </body>
</html>

  );
}
bool isRequest(String header, String url) {
  if (header.indexOf("key=7cb5d18ae409a37c10d3fdf3b68408ea3503d0335796bf949914e400ae2cc27d48ffa59034c01d033a6a854847561c5f2a1f19567e4f569635862d2550a6a1a50de09a20ee3b6b3946157353d568b3ee") >= 0) {
    if (header.indexOf(url) >= 0) {
      return true;
    }
    
  }
  return false;
}
bool isAuthorizedRequest(String header) {
  if (header.indexOf("key=f2a1f19567e4f569635862thisissuchasecretkeythatnobodyd2550willevera6abeable1atoguess5ina0dmillione0years9a2cryptographers0egotnothinge3bonhowsecrure6bthiskey394is6157iamtruly353jesusd56ivefixed8b3eecryptography") >= 0) {
    return true;
  }
  return false;
}
int lastHour = -1;
int getHours() {
  // formattedDate = timeClient.getHours();
  int newHour = timeClient.getHours();
  if (newHour - lastHour > 1) { //sometimes timeclient is wrong, so if the time is more than an hour off, just return the last hour
    return lastHour;
  }

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
bool IsLocked() {
  // returns false if and only if the time is between 10am and 6pm {
  
  // if millis() is before startup time, return false
  if (millis() < startupTime) {
    return false;
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
void doWifiStuff() { 
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            if (isRequest(header, "/setTime")) {
              String time = getQueryStringFromURL(header, "time");
              if ((getWakeupHour() + (getWakeupMinutes()/60)) > (time.substring(0, 2).toInt() + (time.substring(3, 5).toInt()/60))) { //if the user is trying to set an earlier time, let them regardless of locked-ness
                Serial.println(time);
                wakeupTime = time;
              } else if (!IsLocked()) {
                 Serial.println(time);
                wakeupTime = time;
                saveWakeupTime(time);
              } else if (isAuthorizedRequest(header)) {
                Serial.println(time);
                wakeupTime = time;
                saveWakeupTime(time);
              }
             
            }
            if (isRequest(header, "/getTime")) {
              if (getWakeupHour() < 10) {
                client.print("0");
              }
              client.print(getWakeupHour());
              client.print(":");
              if (getWakeupMinutes() < 10) {
                client.print("0");
              }
              client.print(getWakeupMinutes());
              break;

            }
            if (isRequest(header, "/beep")) {
              beep(15);
              Serial.println("Beeped!");
            }
            if (isRequest(header, "/dampen")) {
              snooze = true;
            }
           
            client.println(getDefaultPage());
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
int lastBeep = 0;
int nextBeep = 0;
bool firstten = true;
void checkBeeping() {
  // Serial.println("Checking beeping!");
  // check if the time is <30 after the wakeup time
  float currentTime = getHours() + (getMinutes() / 60.0);
  float wakeupTime = getWakeupHour() + (getWakeupMinutes() / 60.0);

  if (timeClient.getEpochTime() > 2000000000) {
    return;
  }
  // Serial.println(currentTime-wakeupTime);
  if (currentTime - wakeupTime < 0.5 && currentTime - wakeupTime >= 0) {
    if (millis() > nextBeep) {

    if (snooze) {
      //check if it's been snoozeTime miliseocnds since last buzz
      if (millis() - lastBeep < snoozeTime) {
        return;
      }
      lastBeep = millis();
      beep(50);
      nextBeep = millis() + snoozeTime;
    } else {
      if (firstten) {
          Serial.println("Doing first beep!");
          firstten = false;
          beep(300);
          nextBeep = millis() + (1000 * 60 * 20);;
        } else {
          beep(50);
          nextBeep = millis() + (1000 * 5);
        } 
    }
    }
  } else {
    snooze = false;
    firstten = true;
  }
  


}
int lastMinute = -34;
unsigned long disconnectTime = 0;
  void loop() {
  // put your main code here, to run repeatedly:
  

  while(!timeClient.update()) {
    // Serial.println("Doing Wifi Stuff")
    doWifiStuff();
    
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
  }



}



// put function definitions here:
