void on_open(uv_fs_t *req);
void on_read(uv_fs_t *req);
void on_write(uv_fs_t *req);

uv_fs_t open_req;
uv_fs_t read_req;
uv_fs_t write_req;
uv_fs_t close_req;

uv_buf_t buffer;

void on_open(uv_fs_t* req)
{
	if (req->result != -1)
	{
		uv_fs_read(uv_default_loop(), &read_req, req->result, &buffer, 1, -1, on_read);
	}
	else
	{
		std::cerr << "Error opening file!\n";
	}
}

void on_read(uv_fs_t* req)
{
	uv_fs_req_cleanup(req);
	if (req->result < 0)
	{
		std::cerr << "Read error!\n";
	}
	else if (req->result == 0)
	{
		uv_fs_close(uv_default_loop(), &close_req, open_req.result, NULL);
	}
	else
	{
		buffer.len = req->result;
		uv_fs_write(uv_default_loop(), &write_req, 1, &buffer, req->result, -1, on_write);
	}
}

void on_write(uv_fs_t *req)
{
	uv_fs_req_cleanup(req);
	if (req->result < 0)
	{
		std::cerr << "Write error!\n";
	}
	else
	{
		uv_fs_read(uv_default_loop(), &read_req, open_req.result, &buffer, 1, -1, on_read);
	}
}
int main(int argc, char* argv[])
{
	unsigned int len = 1000 * sizeof(char);
	char *base = (char*)malloc(len);
	memset(base, 0, len);
	buffer = uv_buf_init(base, len);
	uv_fs_open(uv_default_loop(), &open_req, argv[1], O_RDONLY, 0, on_open);
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);
	free(buffer.base);
	return 0;
}
