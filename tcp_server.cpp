#include <iostream>
#include <uv.h>

const int DEFAULT_PORT = 80;
const int DEFAULT_BACKLOG = 128;
sockaddr_in addr;

void on_new_connection(uv_stream_t* server, int status);
void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
	//这里只是测试，没有申请这么大内存
	static char store[256] = {'\0'};
	buf->base = store;
	buf->len = 256;
	/*
	buf->base = (char*)malloc(suggested_size);
	memset(buf->base,0,suggested_size);
	buf->len = suggested_size;
	*/
}

void echo_write(uv_write_t *req, int status)
{
	if (status == -1)
	{
		std::cout << "Write error\n";
	}
	std::cout << (char*)req->data << std::endl;
	char *base = (char*)req->data;
	free(base);
	free(req);
}

static void echo_read(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf)
{
	uv_buf_t wrbuf;
	uv_pipe_t *pipe;
	uv_handle_type pending;

	int r;
	uv_write_t* write_req;
	if (nread == UV_EOF || nread == UV_ECONNABORTED)
	{
		return;
	}
	write_req = (uv_write_t*)malloc(sizeof(uv_write_t));
	write_req->data = (void*)buf->base;
	uv_write(write_req, handle, buf, 1, echo_write);
}

uv_loop_t *loop = nullptr;

int main(int argc, char* argv[])
{
	loop = uv_default_loop();
	uv_tcp_t server;
	uv_tcp_init(loop, &server);
	uv_ip4_addr("127.0.0.1", DEFAULT_PORT, &addr);

	uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
	int r = uv_listen((uv_stream_t*)&server, DEFAULT_BACKLOG, on_new_connection);
	if (r)
	{
		fprintf(stderr, "Listen error %s\n", uv_strerror(r));
		return 1;
	}
	return uv_run(loop, UV_RUN_DEFAULT);
}

void on_new_connection(uv_stream_t* server, int status)
{
	if (status < 0) {
		fprintf(stdout, "New connection error %s\n", uv_strerror(status));
		return;
	}
	uv_tcp_t *client = (uv_tcp_t *)malloc(sizeof(uv_tcp_t));
	uv_tcp_init(loop, client);
	if (uv_accept(server, (uv_stream_t*)client) == 0)
	{
		uv_read_start((uv_stream_t*)client, alloc_buffer, echo_read);
	}
}

