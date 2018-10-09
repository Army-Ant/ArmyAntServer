'use strict'

import libArmyAnt from "./ArmyAnt.js/libArmyAnt.js"

var websocket_tester = {
	_ws:null,
	_googAddlibList:[
	],
	_scriptList:[
		{script: "libprotobuf-js/map.js", isAsync: false, isDefer: false},
		{script: "libprotobuf-js/message.js", isAsync: false, isDefer: false},
		{script: "libprotobuf-js/binary/constants.js", isAsync: false, isDefer: false},
		{script: "libprotobuf-js/binary/utils.js", isAsync: false, isDefer: false},
		{script: "libprotobuf-js/binary/decoder.js", isAsync: false, isDefer: false},
		{script: "libprotobuf-js/binary/encoder.js", isAsync: false, isDefer: false},
		{script: "libprotobuf-js/binary/arith.js", isAsync: false, isDefer: false},
		{script: "libprotobuf-js/binary/reader.js", isAsync: false, isDefer: false},
		{script: "libprotobuf-js/binary/writer.js", isAsync: false, isDefer: false},
		{script: "proto-js/c2sm_echologinrequest.js", isAsync: false, isDefer: false},
		{script: "proto-js/sm2c_echologinresponse.js", isAsync: false, isDefer: false},
		{script: "proto-js/c2sm_echologoutrequest.js", isAsync: false, isDefer: false},
		{script: "proto-js/sm2c_echologoutresponse.js", isAsync: false, isDefer: false},
		{script: "proto-js/c2sm_echosendrequest.js", isAsync: false, isDefer: false},
		{script: "proto-js/sm2c_echosendresponse.js", isAsync: false, isDefer: false},
		{script: "proto-js/c2sm_echobroadcastrequest.js", isAsync: false, isDefer: false},
		{script: "proto-js/sm2c_echoerror.js", isAsync: false, isDefer: false},
		{script: "proto-js/sm2c_echoreceivenotice.js", isAsync: false, isDefer: false},
		{script: "proto-js/sm2c_echobroadcastresponse.js", isAsync: false, isDefer: false},
	],
	
	echoServerRequiring: function(){
		let loadScript = (i) => {
			if(i >= websocket_tester._scriptList.length)
				return;
			let scriptName = websocket_tester._scriptList[i].script;
			let isAsync = false
			if(websocket_tester._scriptList[i].isAsync)
				isAsync = websocket_tester._scriptList[i].isAsync
			let isDefer = false
			if(websocket_tester._scriptList[i].isDefer)
				isDefer = websocket_tester._scriptList[i].isDefer
			libArmyAnt.importScript(scriptName, isAsync, isDefer, ()=>{loadScript(i+1);})
			//goog.require(websocket_tester._scriptList[i])
		}
		loadScript(0);
	},
	
	onload: function(){
		websocket_tester._ws = new WebSocket("ws://127.0.0.1:14774/")
		websocket_tester._ws.onopen = function(evt) { 
		  console.log("Websocket opened");
		  document.getElementById('tester').innerHTML += '<br/>' + "WebSocket 服务器已连接"
		  //websocket_tester._ws.send("Hello WebSockets!");
		};

		websocket_tester._ws.onmessage = function(evt) {
		  console.log( "Received Message: " + evt)
		  //websocket_tester._ws.close()
		}

		websocket_tester._ws.onclose = function(evt) {
		  console.log("Websocket closed.")
		  document.getElementById('tester').innerHTML += '<br/>' + "WebSocket 服务器已断开"
		}
	},
	
	connect: function(){
	},
	
	login: function(){
	},
	
	send: function(){
	},
	
	broadcast: function(){
	},
}

websocket_tester.echoServerRequiring();

export default websocket_tester;
