#include "DocumentController.h"
#include "common.h"

using namespace v8;

//	{ "author", "author" },
//	{ "creation-date", "creationDate" },
//	{ "creator", "creator" },
//	{ "format", "format" },
//	{ "keywords", "keywords" },
//	{ "linearized", "linearized" },
//	{ "metadata", "metadata" },
//	{ "mod-date", "modDate" },
//	{ "page-layout", "pageLayout" },
//	{ "page-mode", "pageMode" },
//	{ "permissions", "permissions" },
//	{ "producer", "producer" },
//	{ "subject", "subject" },
//	{ "title", "title" },

void DocumentController::Init(v8::Handle<v8::Object> target) {
	Local<FunctionTemplate> tpl = FunctionTemplate::New(DocumentController::New);
	tpl->SetClassName(String::NewSymbol("PDFDocument"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
	target->Set(String::NewSymbol("Document"), constructor);
}

Handle<Value> DocumentController::New(const Arguments& args) {
	HandleScope scope;

	DocumentController *dc = new DocumentController();
	Handle<v8::Object> jsThis = args.This();
	dc->Wrap(jsThis);
	return scope.Close(jsThis);
}

DocumentController::DocumentController() {
	this->worker = new JobWorker(this);
	this->document = NULL;
		this->worker->enqueue(this);
}

DocumentController::~DocumentController() {
	delete this->worker;
	if(this->document != NULL)
		delete this->document;
}


void DocumentController::open(v8::Handle<v8::Value> &source, v8::Handle<v8::Function> &callback) {
	this->source = Persistent<Value>::New(source);
	this->callback = Persistent<Function>::New(callback);
}

void DocumentController::exec() {
	uv_mutex_lock(&this->mutex);

	uv_mutex_unlock(&this->mutex);
}

void DocumentController::after() {
}

void DocumentController::onStateChange(WorkerState state) {
	switch(state) {
		case WORKER_START:
			this->handle_.ClearWeak();
			break;
		case WORKER_INACTIVE:
			this->MakeWeak();
			break;
		default:
			ERROR("ILLEGAL STATE %i", state);
	}
}
