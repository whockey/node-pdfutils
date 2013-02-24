#include "NodePDFDocBuilder.h"
#include <CachedFile.h>
#include <node_buffer.h>

class StreamCachedFileLoader : public CachedFileLoader {
	public:
		StreamCachedFileLoader(v8::Persistent<v8::Value> source) {
			puts("Stream");
			this->source = source;
		};
		~StreamCachedFileLoader() {
			this->source.Dispose();
		};
		size_t init(GooString *url, CachedFile* cachedFile) {
			return 0;
		}
		int load(const std::vector<ByteRange> &ranges, CachedFileWriter *writer) {
			return -1;
		}
	private:
		v8::Persistent<v8::Value> source;
};
class BufferCachedFileLoader : public CachedFileLoader {
	public:
		BufferCachedFileLoader(v8::Persistent<v8::Value> source) {
			puts("Buffer");
			this->source = v8::Persistent<v8::Object>::New(v8::Handle<v8::Object>::Cast(source));
		};
		~BufferCachedFileLoader() {
			this->source.Dispose();
		};
		size_t init(GooString *url, CachedFile* cachedFile) {
			int length = node::Buffer::Length(this->source);
			CachedFileWriter writer = CachedFileWriter (cachedFile, NULL);
			writer.write(node::Buffer::Data(this->source), CachedFileChunkSize);
			return length;
		}
		int load(const std::vector<ByteRange> &ranges, CachedFileWriter *writer) {
			return 0;
		}
	private:
		v8::Persistent<v8::Object> source;
};

NodePDFDocBuilder::NodePDFDocBuilder(v8::Persistent<v8::Value> source) {
	this->source = source;
}

NodePDFDocBuilder::~NodePDFDocBuilder() {

}

PDFDoc *NodePDFDocBuilder::buildPDFDoc(const GooString &uri, GooString *ownerPassword,
		GooString *userPassword, void *guiDataA) {
	Object obj;

	CachedFile *cachedFile = new CachedFile(
			new StreamCachedFileLoader(this->source), NULL);
	this->source.Clear();

	obj.initNull();
	BaseStream *str = new CachedFileStream(
			cachedFile, 0, gFalse, cachedFile->getLength(), &obj);

	return new PDFDoc(str, ownerPassword, userPassword, guiDataA);
}

GBool NodePDFDocBuilder::supports(const GooString &uri) {
	return gTrue; // if this Builder is initialized, it is used
}
