#ifndef JOB_WORKER_LISTENER_H
#define JOB_WORKER_LISTENER_H

#include "JobWorker.h"

class JobWorkerListener {
	public:
		virtual void onStateChange(WorkerState state) = 0;
		virtual ~JobWorkerListener() {};
};

#endif
