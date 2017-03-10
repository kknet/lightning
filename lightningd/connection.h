#ifndef LIGHTNING_LIGHTNINGD_CONNECTION_H
#define LIGHTNING_LIGHTNINGD_CONNECTION_H

#include "config.h"
#include <ccan/io/io.h>
#include <ccan/short_types/short_types.h>

struct daemon_conn {
	/* Context to tallocate all things from, possibly the
	 * container of this connection. */
	tal_t *ctx;

	/* Last message we received */
	u8 *msg_in;

	/* Array of queued outgoing messages */
	u8 **msg_out;

	int conn_fd;
	struct io_conn *conn;

	/* Callback for incoming messages */
	struct io_plan *(*daemon_conn_recv)(struct io_conn *conn, struct daemon_conn *);
};

/**
 * daemon_conn_init - Initialize a new daemon connection
 *
 * @ctx: context to allocate from
 * @dc: daemon_conn to initialize
 * @fd: socket file descriptor to wrap
 * @daemon_conn_recv: callback function to be called upon receiving a message
 */
void daemon_conn_init(tal_t *ctx, struct daemon_conn *dc, int fd,
		      struct io_plan *(*daemon_conn_recv)(struct io_conn *,
							  struct daemon_conn *));
/**
 * daemon_conn_send - Enqueue an outgoing message to be sent
 */
void daemon_conn_send(struct daemon_conn *dc, u8 *msg);

/**
 * daemon_conn_read_next - Read the next message
 */
struct io_plan *daemon_conn_read_next(struct io_conn *conn,
				      struct daemon_conn *dc);

#endif /* LIGHTNING_LIGHTNINGD_CONNECTION_H */