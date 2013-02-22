#include <node.h>
#include <PDFDoc.h>
#include "JobWorker.h"
#include "Job.h"
#include "JobWorkerListener.h"

#ifndef JSPDFDOCUMENT_H
#define JSPDFDOCUMENT_H

class JsPDFDocument : public node::ObjectWrap, public JobWorkerListener {
	public:
		static void Init(v8::Handle<v8::Object> target);
		static v8::Handle<v8::Value> New(const v8::Arguments& args);
		JsPDFDocument();
		~JsPDFDocument();
		void open(v8::Local<v8::Value> source, v8::Local<v8::Value> callback);

		JobWorker *worker;
		uv_mutex_t mutex;
		PDFDoc *document;
		static v8::Handle<v8::Value> GetPage(v8::Local<v8::String> property, const v8::AccessorInfo &info);
	private:
		v8::Persistent<v8::Value> source;
		v8::Persistent<v8::Function> callback;
		v8::Handle<v8::Value> buildObject();

		void exec();
		void after();
		void onStateChange(WorkerState state);
};

#endif
