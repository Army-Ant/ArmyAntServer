# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: ArmyAntMessage/System/SessionStart.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='ArmyAntMessage/System/SessionStart.proto',
  package='ArmyAntMessage.System',
  syntax='proto3',
  serialized_pb=_b('\n(ArmyAntMessage/System/SessionStart.proto\x12\x15\x41rmyAntMessage.System\"\r\n\x0b\x43lientEnterb\x06proto3')
)




_CLIENTENTER = _descriptor.Descriptor(
  name='ClientEnter',
  full_name='ArmyAntMessage.System.ClientEnter',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=67,
  serialized_end=80,
)

DESCRIPTOR.message_types_by_name['ClientEnter'] = _CLIENTENTER
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

ClientEnter = _reflection.GeneratedProtocolMessageType('ClientEnter', (_message.Message,), dict(
  DESCRIPTOR = _CLIENTENTER,
  __module__ = 'ArmyAntMessage.System.SessionStart_pb2'
  # @@protoc_insertion_point(class_scope:ArmyAntMessage.System.ClientEnter)
  ))
_sym_db.RegisterMessage(ClientEnter)


# @@protoc_insertion_point(module_scope)