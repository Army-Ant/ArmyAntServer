
import c2sm_echologinrequest from "c2sm_echologinrequest"
import sm2c_echologinresponse from "sm2c_echologinresponse"
import c2sm_echologoutrequest from "c2sm_echologoutrequest"
import sm2c_echologoutresponse from "sm2c_echologoutresponse"
import c2sm_echosendrequest from "c2sm_echosendrequest"
import c2sm_echobroadcastrequest from "c2sm_echobroadcastrequest"
import sm2c_echoerror from "sm2c_echoerror"
import sm2c_echosendresponse from "sm2c_echosendresponse"
import sm2c_echoreceivenotice from "sm2c_echoreceivenotice"

var ws = new WebSocket("ws://127.0.0.1:14774");

ws.onopen = function(evt) { 
  console.log("Connection open ..."); 
  ws.send("Hello WebSockets!");
};

ws.onmessage = function(evt) {
  console.log( "Received Message: " + evt.data);
  ws.close();
};

ws.onclose = function(evt) {
  console.log("Connection closed.");
};      

