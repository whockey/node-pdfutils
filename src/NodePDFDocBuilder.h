#ifndef STREAMPDFDOCBUILDER_H
#define STREAMPDFDOCBUILDER_H

#include <PDFDocBuilder.h>
#include <node.h>


class NodePDFDocBuilder : public PDFDocBuilder {
	public:
		NodePDFDocBuilder(v8::Persistent<v8::Value> source);
		~NodePDFDocBuilder() {}

		PDFDoc *buildPDFDoc(const GooString &uri, GooString *ownerPassword = NULL,
				GooString *userPassword = NULL, void *guiDataA = NULL);
		GBool supports(const GooString &uri);

};

#endif /* STREAMPDFDOCBUILDER_H */
