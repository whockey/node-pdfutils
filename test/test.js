var assert = require("should")

var pdfutils = require("../lib/pdfutils.js");
var PDFDocument = pdfutils.PDFDocument;
var simpleTextPath = __dirname + "/simpleText.pdf"

describe('PDF Utils', function(){
	it('should be a function', function(){
		pdfutils.should.be.a('function');
		pdfutils.pdfutils.should.equal(pdfutils);
		pdfutils.PDFDocument.should.be.a('function');
	});
	it('should create PDFDocument', function() {
		pdfutils().should.be.instanceof(PDFDocument);
	});
	it('should load PDF synchronous by factory', function() {
		var doc = pdfutils(simpleTextPath);
		doc.should.be.instanceof(PDFDocument);
	});
	it('should load PDF asynchronous by factory', function(done) {
		var doc2 = pdfutils(simpleTextPath, function(err, doc) {
			assert.equal(null, err);
			assert.equal(doc2, doc);
			doc.should.be.instanceof(PDFDocument);
			done();
		});
	});
});

describe('PDFDocument', function(){
	it('synchronous', function() {
		var doc = new PDFDocument(simpleTextPath);
		doc.should.be.instanceof(PDFDocument);
	});
	it('asynchronous', function(done) {
		var doc2 = new PDFDocument(simpleTextPath, function(err, doc) {
			assert.equal(null, err);
			assert.equal(doc2, doc);
			doc.should.be.instanceof(PDFDocument);
			done();
		});
	});
	it('#length', function() {
		var doc = new PDFDocument(simpleTextPath);
		assert.equal(1, doc.length);
		(function(){
			doc.length = 5;
		}).should.throw();
	})
});
