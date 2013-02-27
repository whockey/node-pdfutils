#include <node.h>
#include <node_buffer.h>
#include <stdlib.h>
#include <cairo.h>
#include <cairo-svg.h>
#include <sstream>
#include <algorithm>
#include "common.h"
#include "JsPDFPage.h"

using namespace v8;
using namespace node;
using namespace std;


Persistent<Function> JsPDFPage::constructor;

string pageConverter[] = {
	"asPNG",
	"asJPEG",
	"asSVG",
	"asHTML"
};

void JsPDFPage::Init(Handle<v8::Object> target) {
	unsigned int i;
	Local<FunctionTemplate> tpl = FunctionTemplate::New();
	tpl->SetClassName(String::NewSymbol("PDFPage"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	Local< ObjectTemplate > prt = tpl->PrototypeTemplate();
	
	for(i = 0; i < LENGTH(pageConverter); i++) {
		Local<Function> fnc = FunctionTemplate::New(ConvertTo)->GetFunction();
		Local<String> sym = String::NewSymbol(pageConverter[i].c_str());
		fnc->SetName(sym);
		prt->Set(sym, fnc);
	}

	prt->SetAccessor(String::NewSymbol("links"), JsPDFPage::GetLinks, 0 /* setter */, Handle<Value>(), 
			static_cast<v8::AccessControl>(DEFAULT), 
			static_cast<v8::PropertyAttribute>(ReadOnly | DontEnum)
			);

	constructor = Persistent<Function>::New(tpl->GetFunction());
	target->Set(String::NewSymbol("Page"), constructor);
}

JsPDFPage::JsPDFPage(JsPDFDocument *document, int index) {
	this->document = document;
	this->index = index;
	this->pg = document->document->getPage(index);
	this->w = this->pg->getMediaWidth();
	this->h = this->pg->getMediaHeight();
	GooString label;
	document->document->getCatalog()->indexToLabel(index, &label);
	this->label = label.getCString();

}

JsPDFPage::~JsPDFPage() {
	free(this->label);
	if(!this->handle_.IsEmpty())
		this->handle_.Dispose();
};

void JsPDFPage::createObject() {
	HandleScope scope;

	if(!this->handle_.IsEmpty())
		return;
	Handle<Value> argv[] = {
		//this->document->handle_
	};
	Local<v8::Object> instance = (*constructor)->NewInstance(LENGTH(argv), argv);
	instance->Set(String::NewSymbol("width"), Local<Number>::New(Number::New(this->w)), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 
	instance->Set(String::NewSymbol("height"), Local<Number>::New(Number::New(this->h)), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 
	instance->Set(String::NewSymbol("index"), Local<Number>::New(Number::New(this->index)), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 
	instance->Set(String::NewSymbol("label"), Local<String>::New(String::New(this->label)), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 
	this->Wrap(Persistent<v8::Object>::New(instance));
}

Handle<Value> JsPDFPage::GetLinks(Local<String> property, const AccessorInfo &info) {
	HandleScope scope;
	JsPDFPage* self = ObjectWrap::Unwrap<JsPDFPage>(info.This());

	// TODO
	/*if(!self->links.IsEmpty())
		return scope.Close(self->links);

	GList *glinks = poppler_page_get_link_mapping(self->pg), *p;
	int length = g_list_length(glinks), i;
	Local<Array> links = Array::New(length);

	for (i = 0, p = glinks; p; p = p->next, i++) {
		Link *link = new Link(self, (PopplerLinkMapping *) p->data);
		links->Set(i, link->handle_);
	}

	poppler_page_free_link_mapping(glinks);

	self->links = Persistent<Array>::New(links);*/

	return scope.Close(self->links);
}

Handle<Value> JsPDFPage::ConvertTo(const Arguments& args) {
	HandleScope scope;
	JsPDFPage* self = ObjectWrap::Unwrap<JsPDFPage>(args.This());

	// TODO
	/*
	String::AsciiValue callee(args.Callee()->GetName());
	char *name = *callee;
	int len = callee.length();
	Format format = FORMAT_UNKOWN;
	for(i = 0; formatName(i) != NULL && format == FORMAT_UNKOWN; i++) {
		if(strncmp(name, formatName(i), len) == 0)
			format = (Format)i;
	}
	if(format == FORMAT_UNKOWN)
		return ThrowException(Exception::Error(String::New("unkown format to convert to")));

	JsPDFPageJob *pj = new JsPDFPageJob(*self, format);
	if(args.Length() >= 1 && !(args[0]->IsUndefined() || args[0]->IsNull())) {
		if(!args[0]->IsObject())
			return ThrowException(Exception::Error(String::New("first argument must be an Object")));
		pj->calcDimensions(args[0]->ToObject());
	}
	self->document->addJob(pj);
	*/

	return self->handle_;
}
