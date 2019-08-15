#include "pch.h"
#include <uv.h>
#include <iostream>


void on_connect(uv_connect_t* req, int status)
{
	int r;
	uv_buf_t buf;
	do {
		buf = uv_buf_init((char*)"PING", 4);
		r = uv_try_write(req->handle, &buf, 1);
		if (r > 0) {
			break;
		}
	} while (true);
	uv_close((uv_handle_t*)req->handle, [](uv_handle_t*) {});
}
//int main(int argc, char** argv)
//{
//	uv_loop_t* loop = uv_default_loop();
//	uv_tcp_t *socket = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
//	uv_tcp_init(loop, socket);
//	uv_connect_t *connect = (uv_connect_t*)malloc(sizeof(uv_connect_t));
//
//	struct sockaddr_in dest;
//	uv_ip4_addr("127.0.0.1", 80, &dest);
//	uv_tcp_connect(connect, socket, (const struct sockaddr*)&dest, on_connect);
//	uv_run(loop, UV_RUN_DEFAULT);
//	return 0;
//}