'use strict'

var websocket_tester = {
	_ws:null,
	_googAddlibList:[
	],
	_scriptList:[
		"libprotobuf-js/map.js",
		"libprotobuf-js/message.js",
		"libprotobuf-js/binary/constants.js",
		"libprotobuf-js/binary/utils.js",
		"libprotobuf-js/binary/arith.js",
		"libprotobuf-js/binary/encoder.js",
		"libprotobuf-js/binary/decoder.js",
		"libprotobuf-js/binary/reader.js",
		"libprotobuf-js/binary/writer.js",
		"proto-js/c2sm_echologinrequest.js",
		"proto-js/sm2c_echologinresponse.js",
		"proto-js/c2sm_echologoutrequest.js",
		"proto-js/sm2c_echologoutresponse.js",
		"proto-js/c2sm_echosendrequest.js",
		"proto-js/sm2c_echosendresponse.js",
		"proto-js/c2sm_echobroadcastrequest.js",
		"proto-js/sm2c_echoerror.js",
		"proto-js/sm2c_echoreceivenotice.js",
		"proto-js/sm2c_echobroadcastresponse.js",
	],
	
	echoServerRequiring: function(){
		for(let i=0; i<websocket_tester._scriptList.length; ++i){
			libArmyAnt.importScript(websocket_tester._scriptList[i])
			//goog.require(websocket_tester._scriptList[i])
		}
	},
	
	socketConnect: function(addr, port){
		websocket_tester._ws = new WebSocket("ws://"+addr+":"+port+"/")
		ws.onopen = function(evt) { 
		  console.log("Connection open ...");
		  ws.send("Hello WebSockets!")
		};

		ws.onmessage = function(evt) {
		  console.log( "Received Message: " + evt.data)
		  ws.close()
		}

		ws.onclose = function(evt) {
		  console.log("Connection closed.")
		}
	}
}

websocket_tester.echoServerRequiring();

export default websocket_tester;
