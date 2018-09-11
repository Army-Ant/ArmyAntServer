/**
 * @fileoverview
 * @enhanceable
 * @suppress {messageConventions} JS Compiler reports an error if a variable or
 *     field starts with 'MSG_' and isn't a translatable message.
 * @public
 */
// GENERATED CODE -- DO NOT EDIT!

goog.provide('proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo');

goog.require('jspb.BinaryReader');
goog.require('jspb.BinaryWriter');
goog.require('jspb.Message');


/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo.displayName = 'proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo';
}


if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto suitable for use in Soy templates.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     com.google.apps.jspb.JsClassTemplate.JS_RESERVED_WORDS.
 * @param {boolean=} opt_includeInstance Whether to include the JSPB instance
 *     for transitional soy proto support: http://goto/soy-param-migration
 * @return {!Object}
 */
proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo.prototype.toObject = function(opt_includeInstance) {
  return proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Whether to include the JSPB
 *     instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo.toObject = function(includeInstance, msg) {
  var f, obj = {
    name: jspb.Message.getFieldWithDefault(msg, 1, ""),
    server: jspb.Message.getFieldWithDefault(msg, 2, ""),
    charset: jspb.Message.getFieldWithDefault(msg, 3, ""),
    sortRule: jspb.Message.getFieldWithDefault(msg, 4, "")
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo}
 */
proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo;
  return proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo}
 */
proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {string} */ (reader.readString());
      msg.setName(value);
      break;
    case 2:
      var value = /** @type {string} */ (reader.readString());
      msg.setServer(value);
      break;
    case 3:
      var value = /** @type {string} */ (reader.readString());
      msg.setCharset(value);
      break;
    case 4:
      var value = /** @type {string} */ (reader.readString());
      msg.setSortRule(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getName();
  if (f.length > 0) {
    writer.writeString(
      1,
      f
    );
  }
  f = message.getServer();
  if (f.length > 0) {
    writer.writeString(
      2,
      f
    );
  }
  f = message.getCharset();
  if (f.length > 0) {
    writer.writeString(
      3,
      f
    );
  }
  f = message.getSortRule();
  if (f.length > 0) {
    writer.writeString(
      4,
      f
    );
  }
};


/**
 * optional string name = 1;
 * @return {string}
 */
proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo.prototype.getName = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 1, ""));
};


/** @param {string} value */
proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo.prototype.setName = function(value) {
  jspb.Message.setProto3StringField(this, 1, value);
};


/**
 * optional string server = 2;
 * @return {string}
 */
proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo.prototype.getServer = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 2, ""));
};


/** @param {string} value */
proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo.prototype.setServer = function(value) {
  jspb.Message.setProto3StringField(this, 2, value);
};


/**
 * optional string charset = 3;
 * @return {string}
 */
proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo.prototype.getCharset = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 3, ""));
};


/** @param {string} value */
proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo.prototype.setCharset = function(value) {
  jspb.Message.setProto3StringField(this, 3, value);
};


/**
 * optional string sort_rule = 4;
 * @return {string}
 */
proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo.prototype.getSortRule = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 4, ""));
};


/** @param {string} value */
proto.ArmyAntMessage.DBProxy.SqlDatabaseInfo.prototype.setSortRule = function(value) {
  jspb.Message.setProto3StringField(this, 4, value);
};


