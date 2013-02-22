#include <node.h>
#include <PDFDoc.h>
#include "JobWorker.h"
#include "Job.h"
#include "JobWorkerListener.h"

class DocumentController : public node::ObjectWrap, public Job, public JobWorkerListener {
	public:
		static void Init(v8::Handle<v8::Object> target);
		static v8::Handle<v8::Value> New(const v8::Arguments& args);
		DocumentController();
		~DocumentController();
		void open(v8::Handle<v8::Value> &source, v8::Handle<v8::Function> &callback);
	private:
		v8::Persistent<v8::Value> source;
		v8::Persistent<v8::Function> callback;
		JobWorker *worker;
		uv_mutex_t mutex;
		PDFDoc *document;
		v8::Handle<v8::Value> buildObject();

		void exec();
		void after();
		void onStateChange(WorkerState state);
};
