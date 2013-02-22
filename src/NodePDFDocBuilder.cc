#include "NodePDFDocBuilder.h"
#include <CachedFile.h>

class StreamCachedFileLoader : public CachedFileLoader {
	public:
		StreamCachedFileLoader();
		~StreamCachedFileLoader();
		size_t init(GooString *url, CachedFile* cachedFile);
		int load(const std::vector<ByteRange> &ranges, CachedFileWriter *writer);
};

NodePDFDocBuilder::NodePDFDocBuilder(v8::Persistent<v8::Value> source) {
	
}

PDFDoc *NodePDFDocBuilder::buildPDFDoc(const GooString &uri, GooString *ownerPassword,
		GooString *userPassword, void *guiDataA) {
	Object obj;

	CachedFile *cachedFile = new CachedFile(
			new StreamCachedFileLoader(), NULL);

	obj.initNull();
	BaseStream *str = new CachedFileStream(
			cachedFile, 0, gFalse, cachedFile->getLength(), &obj);

	return new PDFDoc(str, ownerPassword, userPassword, guiDataA);
}

GBool NodePDFDocBuilder::supports(const GooString &uri) {
	return gTrue; // if this Builder is initialized, it is used
}
