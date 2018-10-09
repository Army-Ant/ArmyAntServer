'use strict'

import libArmyAnt from "./ArmyAnt.js/libArmyAnt.js"

var websocket_tester = {
	_ws:null,
	_convIndex:1,
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
		{script: "proto-js/socketextendnormal_v0_0_0_1.js", isAsync: false, isDefer: false},
		{script: "proto-js/conversationsteptype.js", isAsync: false, isDefer: false},
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
	
	checkIsBigEnding: function(){
		let buffer = new ArrayBuffer(8)
		let uint32 = new Uint32Array(buffer)
		uint32[0] =1 // 在uint32对应的缓冲区的开始，用四个字节，写入数字1 默认按计算机存储方式， 如果是小端存储，每一个缓冲区byte分别为 1，0，0，0. 大端存储为0,0,0,1
 
		let uint8 = new Uint8Array(buffer, 0, 1) //让uint8对应缓冲区的前1个字节，并按uint8 来呈现缓冲区
		return uint8 == 0
	},
	
	createHead: function(serials, type, extendVersion, extend, content){
		let isBig = this.checkIsBigEnding()
		let buffer = new ArrayBuffer(16)
		var arr32 = new Uint32Array(buffer);
		if(isBig){
			arr32[0] = extend.length
			arr32[1] = extendVersion
			arr32[2] = type
			arr32[3] = serials
			let ret = Array.prototype.slice.call(new Uint8Array(buffer))
			return ret.reverse()
		} else {
			arr32[0] = serials
			arr32[1] = type
			arr32[2] = extendVersion
			arr32[3] = extend.length
			let ret = Array.prototype.slice.call(new Uint8Array(buffer))
			return ret
		}
	},
	
	onload: function(){
	},
	
	connect: function(){
		if(websocket_tester._ws){
			websocket_tester._ws.close();
			return '断开服务器...';
		} else {
			websocket_tester._ws = new WebSocket("ws://127.0.0.1:14774/");
			websocket_tester._ws.onopen = function(evt) { 
			  console.log("Websocket opened");
			  document.getElementById('tester').innerHTML += '<br/>' + "WebSocket 服务器已连接";
			};

			websocket_tester._ws.onmessage = function(evt) {
			  console.log( "Received Message: " + evt);
			};
			websocket_tester._ws.onerror = function(evt) {
			  console.log( "Websocket caused error: " + evt);
			};

			websocket_tester._ws.onclose = function(evt) {
			  console.log("Websocket closed.");
			  document.getElementById('tester').innerHTML += '<br/>' + "WebSocket 服务器已断开";
			  websocket_tester._ws = null;
			};
			return '连接服务器...';
		}
	},
	
	login: function(){
		if(websocket_tester._ws){
			let msg = new proto.ArmyAntMessage.SubApps.C2SM_EchoLoginRequest()
			msg.setUserName("jason")
			let bytes_msg = msg.serializeBinary()
			let extend = new proto.ArmyAntMessage.System.SocketExtendNormal_V0_0_0_1()
			extend.setAppId(1001)
			extend.setContentLength(bytes_msg.length)
			extend.setMessageCode(10011001)
			extend.setConversationCode(++websocket_tester._convIndex)
			extend.setConversationStepIndex(0)
			extend.setConversationStepType(proto.ArmyAntMessage.System.ConversationStepType.ASKFOR)
			let bytes_extend = extend.serializeBinary()
			let byteArray_head = websocket_tester.createHead(0, 1, 1, bytes_extend, bytes_msg)
			let array_all = byteArray_head.concat(Array.prototype.slice.call(bytes_extend), Array.prototype.slice.call(bytes_msg))
			let writer_all = Uint8Array.from(array_all)
			websocket_tester._ws.send(writer_all);
			return "登录中..."
		} else {
			let res = "Websocket 尚未建立连接, 请先连接服务器"
			window.alert(res)
			return res;
		}
	},
	
	send: function(){
	},
	
	broadcast: function(){
	},
}

websocket_tester.echoServerRequiring();

export default websocket_tester;
