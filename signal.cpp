#include <iostream>
#include <uv.h>


uv_loop_t* create_loop()
{
	uv_loop_t *loop = (uv_loop_t*)malloc(sizeof(uv_loop_t));
	if (loop)
	{
		uv_loop_init(loop);
	}
	return loop;
}

void signal_handler(uv_signal_t* handle, int signum)
{
	printf("Signal received: %d\n", signum);
	uv_signal_stop(handle);
}

void thread_worker(void *userp)
{
	uv_loop_t *loop1 = create_loop();
	uv_signal_t sigla, siglb;
	uv_signal_init(loop1, &sigla);
	uv_signal_start(&sigla, signal_handler, SIGINT);

	uv_signal_init(loop1, &siglb);
	uv_signal_start(&siglb, signal_handler, SIGINT);

	uv_run(loop1, UV_RUN_DEFAULT);
}

void thread2_worker(void *userp)
{
	uv_loop_t *loop2 = create_loop();
	auto loop3 = create_loop();
	uv_signal_t sig2;
	uv_signal_init(loop2, &sig2);
	uv_signal_start(&sig2, signal_handler, SIGBREAK);

	uv_signal_t sig3;
	uv_signal_init(loop3, &sig3);
	uv_signal_start(&sig3, signal_handler, SIGBREAK);

	while (uv_run(loop2,UV_RUN_NOWAIT) || uv_run(loop3, UV_RUN_NOWAIT))
	{

	}
}
int main(int argc, char* argv[])
{
	printf("PID %d\n", _getpid());
	uv_thread_t thread1, thread2;
	uv_thread_create(&thread1, thread_worker, 0);
	uv_thread_create(&thread2, thread2_worker, 0);

	uv_thread_join(&thread1);
	uv_thread_join(&thread2);
	return 0;
}