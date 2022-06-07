// Copyright 2022, Kyota Tashiro, Mengheng Lee, and Latona Inc.
// License MIT

#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
#include <stdio.h>
#include "rabbitmq-client.h"

rabbitmq_cli new_rabbitmq_client(char *hostname, int port, 
             char *vhost, char *user, char *pass) {
  int status;
  amqp_rpc_reply_t reply;
  amqp_socket_t *socket = NULL;
  rabbitmq_cli cli;
  amqp_connection_state_t conn;

  cli.connection = amqp_new_connection();
  socket = amqp_tcp_socket_new(cli.connection);
  status = amqp_socket_open(socket, hostname, port);

  if (status){printf("Error opening TCP socket\n");};
  reply = amqp_login(cli.connection, vhost, 0, 131072, 0, 
        AMQP_SASL_METHOD_PLAIN, user, pass);  amqp_channel_open(cli.connection, 1);
  reply = amqp_get_rpc_reply(cli.connection);
  // handle reply error
  printf("Connection established at %s\n", hostname);
  return cli;
}

// TO-DO implement error handler for amqp_rpc_reply_t

// TO-DO implement message content_type
int rabbitmq_cli_publish(rabbitmq_cli cli, char *queuename, char *message) {
  int reply;
  amqp_basic_properties_t props;
  amqp_queue_declare_ok_t *queue_ok;
  props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
  props.content_type = amqp_cstring_bytes("text/json");
  props.delivery_mode = 2; /* persistent delivery mode */


  queue_ok = amqp_queue_declare(cli.connection, 1, amqp_cstring_bytes(queuename),
                     0, 0, 0, 1, amqp_empty_table);
  reply = amqp_basic_publish(cli.connection, 1, amqp_cstring_bytes(""),
                              queue_ok->queue, 0, 0, &props,
                              amqp_cstring_bytes(message));
  return reply;
}

char *rabbitmq_cli_receive(rabbitmq_cli cli, char *queuename) {
  amqp_rpc_reply_t res;
  amqp_envelope_t envelope;
  amqp_basic_consume(cli.connection, 1, amqp_cstring_bytes(queuename), amqp_empty_bytes,
                    0, 0, 0, amqp_empty_table);
  amqp_get_rpc_reply(cli.connection);
  res = amqp_consume_message(cli.connection, &envelope, NULL, 0);
  return (char *) envelope.message.body.bytes;
}