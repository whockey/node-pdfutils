#include <goo/GooList.h>
#include "PDFLoader.h"
#include "JsPDFDocument.h"
#include "NodePDFDocBuilder.h"
#include <sstream>

using namespace v8;
using namespace node;

PDFLoader::PDFLoader(JsPDFDocument* document, const Arguments& args) {
	this->document = document;
	this->source = Persistent<Value>::New(args[0]);

	if(this->source->IsString()) {
		this->sourceStr.Set(*String::Utf8Value(this->source));
	}
	else {
		this->factory.registerPDFDocBuilder(new NodePDFDocBuilder(this->source));
		this->sourceStr.clear();
	}

	if(args.Length() >= 2 && args[1]->IsFunction()) {
		this->callback = Persistent<Function>::New(Handle<Function>::Cast(args[1]));
		this->document->worker->enqueue(this);
	}
	else
		this->execSync();
}

void PDFLoader::exec() {
	this->tmpDoc = this->factory.createPDFDoc(this->sourceStr, &this->ownerPassword, &this->userPassword);
}

void PDFLoader::after() {
	this->loadPages();
	this->loadProperties();
	this->execCallback();
}

void PDFLoader::loadPages() {
	int i = 0;
	Persistent<v8::Object> handle = this->document->handle_;

	int length = this->tmpDoc->getNumPages();
	for(; i < length; i++) {
		std::stringstream istr;
		istr << i;
		handle->SetAccessor(String::NewSymbol(istr.str().c_str()),
				JsPDFDocument::GetPage, 0 /* setter */, Handle<Value>(), 
				static_cast<v8::AccessControl>(DEFAULT));
	}
}

void PDFLoader::loadProperties() {
	// TODO
	//Persistent<v8::Object> handle = this->document->handle_;
}

void PDFLoader::execCallback() {
	if(this->callback.IsEmpty())
		return;

	Local<Value> argv[] = {
		this->tmpDoc == NULL || this->tmpDoc->isOk() == false
			? Exception::Error(String::New("Error loading document"))
			: Local<Value>::New(Null()),
		Local<v8::Object>::New(this->document->handle_)
	};
	TryCatch try_catch;
	(*this->callback)->Call(Context::GetCurrent()->Global(), 2, argv);
	if (try_catch.HasCaught()) {
		FatalException(try_catch);
	}
	this->callback.Dispose();
}

PDFLoader::~PDFLoader() {
}
