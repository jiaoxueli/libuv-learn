#include <iostream>
#include <uv.h>

static uv_once_t once_only = UV_ONCE_INIT;
int i = 0;
uv_mutex_t mutex;
void increment()
{
	uv_mutex_lock(&mutex);
	i++;
	uv_mutex_unlock(&mutex);
}
void thread1(void* arg)
{
	uv_once(&once_only, increment);
}
void thread2(void* arg)
{
	uv_once(&once_only, increment);
}

int main(int argc, char* argv[])
{
	uv_mutex_init(&mutex);
	uv_thread_t thread[2];
	uv_thread_create(&thread[0], thread1, NULL);
	uv_thread_create(&thread[1], thread2, NULL);
	uv_thread_join(&thread[0]);
	uv_thread_join(&thread[1]);
	std::cout << "i = " << i << std::endl;
	uv_mutex_destroy(&mutex);
	return 0;
}