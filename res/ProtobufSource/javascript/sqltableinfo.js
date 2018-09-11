/**
 * @fileoverview
 * @enhanceable
 * @suppress {messageConventions} JS Compiler reports an error if a variable or
 *     field starts with 'MSG_' and isn't a translatable message.
 * @public
 */
// GENERATED CODE -- DO NOT EDIT!

goog.provide('proto.ArmyAntMessage.DBProxy.SqlTableInfo');

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
proto.ArmyAntMessage.DBProxy.SqlTableInfo = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.ArmyAntMessage.DBProxy.SqlTableInfo, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  proto.ArmyAntMessage.DBProxy.SqlTableInfo.displayName = 'proto.ArmyAntMessage.DBProxy.SqlTableInfo';
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
proto.ArmyAntMessage.DBProxy.SqlTableInfo.prototype.toObject = function(opt_includeInstance) {
  return proto.ArmyAntMessage.DBProxy.SqlTableInfo.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Whether to include the JSPB
 *     instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.ArmyAntMessage.DBProxy.SqlTableInfo} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.ArmyAntMessage.DBProxy.SqlTableInfo.toObject = function(includeInstance, msg) {
  var f, obj = {
    tableName: jspb.Message.getFieldWithDefault(msg, 5, ""),
    engine: jspb.Message.getFieldWithDefault(msg, 6, ""),
    comment: jspb.Message.getFieldWithDefault(msg, 7, "")
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
 * @return {!proto.ArmyAntMessage.DBProxy.SqlTableInfo}
 */
proto.ArmyAntMessage.DBProxy.SqlTableInfo.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.ArmyAntMessage.DBProxy.SqlTableInfo;
  return proto.ArmyAntMessage.DBProxy.SqlTableInfo.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.ArmyAntMessage.DBProxy.SqlTableInfo} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.ArmyAntMessage.DBProxy.SqlTableInfo}
 */
proto.ArmyAntMessage.DBProxy.SqlTableInfo.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 5:
      var value = /** @type {string} */ (reader.readString());
      msg.setTableName(value);
      break;
    case 6:
      var value = /** @type {string} */ (reader.readString());
      msg.setEngine(value);
      break;
    case 7:
      var value = /** @type {string} */ (reader.readString());
      msg.setComment(value);
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
proto.ArmyAntMessage.DBProxy.SqlTableInfo.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.ArmyAntMessage.DBProxy.SqlTableInfo.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.ArmyAntMessage.DBProxy.SqlTableInfo} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.ArmyAntMessage.DBProxy.SqlTableInfo.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getTableName();
  if (f.length > 0) {
    writer.writeString(
      5,
      f
    );
  }
  f = message.getEngine();
  if (f.length > 0) {
    writer.writeString(
      6,
      f
    );
  }
  f = message.getComment();
  if (f.length > 0) {
    writer.writeString(
      7,
      f
    );
  }
};


/**
 * optional string table_name = 5;
 * @return {string}
 */
proto.ArmyAntMessage.DBProxy.SqlTableInfo.prototype.getTableName = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 5, ""));
};


/** @param {string} value */
proto.ArmyAntMessage.DBProxy.SqlTableInfo.prototype.setTableName = function(value) {
  jspb.Message.setProto3StringField(this, 5, value);
};


/**
 * optional string engine = 6;
 * @return {string}
 */
proto.ArmyAntMessage.DBProxy.SqlTableInfo.prototype.getEngine = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 6, ""));
};


/** @param {string} value */
proto.ArmyAntMessage.DBProxy.SqlTableInfo.prototype.setEngine = function(value) {
  jspb.Message.setProto3StringField(this, 6, value);
};


/**
 * optional string comment = 7;
 * @return {string}
 */
proto.ArmyAntMessage.DBProxy.SqlTableInfo.prototype.getComment = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 7, ""));
};


/** @param {string} value */
proto.ArmyAntMessage.DBProxy.SqlTableInfo.prototype.setComment = function(value) {
  jspb.Message.setProto3StringField(this, 7, value);
};


