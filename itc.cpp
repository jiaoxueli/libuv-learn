#include <iostream>
#include <uv.h>

uv_loop_t *loop;
uv_async_t async;
void print_progress(uv_async_t* handle)
{
	double percentage = *((double*)handle->data);
	std::cerr << "Downloaded " << percentage << "\n";
}

void fake_download(uv_work_t *req)
{
	int size = *((int*)req->data);
	int downloaded = 0;
	double percentage;
	while (downloaded < size)
	{
		percentage = downloaded * 100.0 / size;
		async.data = (void*)&percentage;
		uv_async_send(&async);
		Sleep(1);
		downloaded += (200 + rand()) % 1000;
	}
}

void affter(uv_work_t *req, int status)
{
	std::cout << "Download complete\n";
	uv_close((uv_handle_t*)&async, NULL);
}

int main(int argc, char* argv[])
{
	loop = uv_default_loop();
	uv_work_t req;
	int size = 10240;
	req.data = (void*)&size;
	uv_async_init(loop, &async, print_progress);
	uv_queue_work(loop, &req, fake_download, affter);
	return uv_run(loop, UV_RUN_DEFAULT);
	return 0;
}
