// Copyright 2022, Kyota Tashiro, Mengheng Lee, and Latona Inc.
// License MIT
#ifndef MQ_CLIENT 
#define MQ_CLIENT
#include <rabbitmq-c/amqp.h>

typedef struct rabbitmq_cli {
  amqp_connection_state_t connection;
  amqp_channel_t *channel;
  char *url;
  amqp_bytes_t *receive_queues;
  amqp_bytes_t *send_queues;
  int counter;
  char *tag;
  int is_closed;
} rabbitmq_cli;

rabbitmq_cli new_rabbitmq_client(char *hostname, int port, 
             char *vhost, char *user, char *pass);
// TO-DO implement message content type
int rabbitmq_cli_publish(rabbitmq_cli cli, char *queuename, char *message);

char *rabbitmq_cli_receive(rabbitmq_cli cli, char *queuename);

#endif
