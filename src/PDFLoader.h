#include <node.h>
#include <goo/GooString.h>
#include <CachedFile.h>
#include <PDFDoc.h>
#include <PDFDocFactory.h>
#include "JsPDFDocument.h"

#ifndef PDFLOADER_H
#define PDFLOADER_H

class PDFLoader : public Job {
	public:
		PDFLoader(JsPDFDocument* document, const v8::Arguments& args);
		~PDFLoader();

		void exec();
		void after();
	private:
		PDFDocFactory factory;
		PDFDoc* tmpDoc;
		JsPDFDocument* document;
		v8::Persistent<v8::Value> source;
		GooString sourceStr;
		v8::Persistent<v8::Function> callback;
		GooString ownerPassword;
		GooString userPassword;

		void loadPages();
		void loadProperties();
		void execCallback();
};

#endif
