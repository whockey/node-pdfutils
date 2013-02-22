#include <node.h>
#include "JsPDFDocument.h"

using namespace v8;

void InitAll(Handle<v8::Object> target) {
	JsPDFDocument::Init(target);
	//Page::Init(target);
	//PageJob::Init(target);
	//Link::Init(target);
}

NODE_MODULE(pdfutils, InitAll)
