#include "JsPDFDocument.h"
#include "PDFLoader.h"
#include "common.h"

#define SYMBOL "PDFDocument"
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

void JsPDFDocument::Init(v8::Handle<v8::Object> target) {
	Local<FunctionTemplate> tpl = FunctionTemplate::New(JsPDFDocument::New);
	tpl->SetClassName(String::NewSymbol(SYMBOL));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
	target->Set(String::NewSymbol(SYMBOL), constructor);
}

Handle<Value> JsPDFDocument::New(const Arguments& args) {
	HandleScope scope;

	JsPDFDocument *dc = new JsPDFDocument();
	Handle<v8::Object> jsThis = args.This();
	dc->Wrap(jsThis);

	if(args.Length() != 0)
		new PDFLoader(dc, args);

	return scope.Close(jsThis);
}

JsPDFDocument::JsPDFDocument() {
	this->worker = new JobWorker(this);
	this->document = NULL;
	uv_mutex_init(&this->mutex);
}

JsPDFDocument::~JsPDFDocument() {
	delete this->worker;
	if(this->document != NULL)
		delete this->document;
	uv_mutex_destroy(&this->mutex);
}



void JsPDFDocument::onStateChange(WorkerState state) {
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

Handle<Value> JsPDFDocument::GetPage(v8::Local<v8::String> property, const v8::AccessorInfo &info) {
	HandleScope scope;
	//JsPDFDocument* self = ObjectWrap::Unwrap<JsPDFDocument>(info.This());

	//int n = atoi(*String::Utf8Value(property));

	return v8::Object::New();
}
