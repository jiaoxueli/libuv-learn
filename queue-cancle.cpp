uv_mutex_t mutex;
uv_work_t fib_req[10];
long fib_(int n)
{
	if (n == 0 || n == 1)
		return 1;
	else
	{
		return fib_(n - 2) + fib_(n - 1);
	}
}

void fib(uv_work_t *req)
{
	int n = *(int *)req->data;
	if (rand() % 2)
	{
		Sleep(1000);
	}
	else
		Sleep(3000);
	long fib = fib_(n);
	uv_mutex_lock(&mutex);
	fprintf(stderr, "%dth fibonacci is %lu\n", n, fib);
	uv_mutex_unlock(&mutex);
}

void signal_handler(uv_signal_t *req, int signum)
{
	printf("Signal received!\n");
	int i;
	for (i = 0; i < 10; i++) {
		uv_cancel((uv_req_t*)&fib_req[i]);
	}
	uv_signal_stop(req);
}
int main(int argc, char* argv[])
{
	auto loop = uv_default_loop();
	int data[10];
	uv_mutex_init(&mutex);
	for (int i = 0; i < 10; i++)
	{
		data[i] = i;
		fib_req[i].data = (void*)&data[i];
		uv_queue_work(loop, &fib_req[i], fib, [](uv_work_t* req, int status) {
			if (status == UV_ECANCELED)
				fprintf(stderr, "Calculation of %d cancelled.\n", *(int *)req->data);
			else
				fprintf(stderr, "Done calculating %dth fibonacci\n", *(int *)req->data);
		});
	}
	uv_signal_t sig;
	uv_signal_init(loop, &sig);
	uv_signal_start(&sig, signal_handler, SIGINT);
	uv_run(loop, UV_RUN_DEFAULT);
	uv_mutex_destroy(&mutex);
	return 0;
}