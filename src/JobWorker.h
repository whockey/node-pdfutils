#include <node.h>
#include <queue>

#ifndef JOB_WORKER_H
#define JOB_WORKER_H

typedef enum {
	WORKER_START,
	WORKER_INACTIVE,
	WORKER_RESTART,
	WORKER_STOP,
	WORKER_PROCESS
} WorkerState;

#include "JobWorkerListener.h"
#include "Job.h"

class JobWorker : public JobWorkerListener {
	public:
		JobWorker(JobWorkerListener *listener);
		~JobWorker();
		void enqueue(Job *job);

	private:
		uv_loop_t *loop;
		uv_mutex_t mutex;
		std::queue<Job*> queue;
		std::queue<Job*> finished;
		uv_work_t worker;
		uv_async_t update;
		WorkerState state;
		JobWorkerListener *listener;

		void start();
		void flushFinished();
		static void Worker(uv_work_t *req);
		static void Update(uv_async_t *handle, int status /*UNUSED*/);
		static void AfterWorker(uv_work_t *req, int status);

		void onStateChange(WorkerState state) {};
};

#endif
