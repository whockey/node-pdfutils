#include <node.h>
#include "JobWorker.h"

#define STATE(x,y) x->listener->onStateChange(x->state = y);

JobWorker::JobWorker(JobWorkerListener *listener) {
	this->state = WORKER_INACTIVE;
	this->listener = listener ? listener : this;
	this->worker.data = this->update.data = this;

	this->loop = uv_default_loop();
	uv_mutex_init(&this->mutex);
	uv_async_init(this->loop, &this->update, JobWorker::Update);
}

JobWorker::~JobWorker() {
	uv_mutex_lock(&this->mutex);
	while(!this->finished.empty())
		this->finished.pop();
	while(!this->queue.empty())
		this->queue.pop();
	uv_mutex_unlock(&this->mutex);
	uv_mutex_destroy(&this->mutex);
	uv_close((uv_handle_t*) &this->update, NULL);
}

void JobWorker::enqueue(Job *job) {
	uv_mutex_lock(&this->mutex);
	this->queue.push(job);
	this->start();
	uv_mutex_unlock(&this->mutex);
}

void JobWorker::start() {
	switch(this->state) {
	case WORKER_START:
	case WORKER_PROCESS:
		break;
	case WORKER_RESTART:
	case WORKER_STOP:
		this->state = WORKER_RESTART;
		break;
	case WORKER_INACTIVE:
		STATE(this, WORKER_START)
		uv_queue_work(this->loop, &this->worker, JobWorker::Worker, JobWorker::AfterWorker);
		break;
	}
}

void JobWorker::flushFinished() {
	uv_mutex_lock(&this->mutex);
	while(!this->finished.empty()) {
		Job *job = this->finished.front();
		this->finished.pop();
		uv_mutex_unlock(&this->mutex);
		job->after();
		uv_mutex_lock(&this->mutex);
	}
	uv_mutex_unlock(&this->mutex);
}

void JobWorker::Worker(uv_work_t *req) {
	JobWorker *self = (JobWorker *)req->data;
	if(self == NULL)
		return;
	uv_mutex_lock(&self->mutex);
	while(!self->queue.empty()) {
		Job *job = self->queue.front();
		self->queue.pop();
		uv_mutex_unlock(&self->mutex);

		job->exec();

		uv_mutex_lock(&self->mutex);
		self->finished.push(job);
		uv_async_send(&self->update);
	}
	self->state = WORKER_STOP;
	uv_mutex_unlock(&self->mutex);
}

void JobWorker::Update(uv_async_t *handle, int status /*UNUSED*/) {
	JobWorker *self = (JobWorker *)handle->data;
	if(self == NULL)
		return;
	self->flushFinished();
}

void JobWorker::AfterWorker(uv_work_t *req) {
	JobWorker *self = (JobWorker *)req->data;
	if(self == NULL)
		return;
	self->flushFinished();
	switch(self->state) {
	case WORKER_RESTART:
		self->state = WORKER_INACTIVE;
		self->start();
		break;
	case WORKER_STOP:
		STATE(self, WORKER_INACTIVE);
		break;
	default:
		puts("Invalid state");
	}
}
