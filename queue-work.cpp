#include <iostream>
#include <uv.h>

long fib_(int n)
{
	if (n == 0 || n == 1)
		return 1;
	else
	{
		return fib_(n-2) + fib_(n - 1);
	}
}

void fib(uv_work_t *req)
{
	int n = *(int *)req->data;
	if (rand() % 2)
	{
		Sleep(1);
	}
	else
		Sleep(3);
	long fib = fib_(n);
	fprintf(stderr, "%dth fibonacci is %lu\n", n, fib);
}

int main(int argc, char* argv[])
{
	auto loop = uv_default_loop();
	int data[10];
	uv_work_t req[10];
	for (int i = 0; i < 10; i++)
	{
		data[i] = i;
		req[i].data = (void*)&data[i];
		uv_queue_work(loop, &req[i], fib, [](uv_work_t* req, int status) {
			fprintf(stderr, "Done calculating %dth fibonacci\n", *(int *)req->data);
		});
	}
	uv_run(loop, UV_RUN_DEFAULT);
	return 0;
}
