var assert = require("should")

var pdfutils = require("../lib/pdfutils.js");

describe('PDF Document', function(){
	describe('new PDFDocument', function(){

		it('should be a function', function(){
			pdfutils.should.be.a('function');
			pdfutils.PDFDocument.should.be.a('function');
		});

		it('should create new PDF Document', function(done){
			var result = pdfutils(__dirname + "/simpleText.pdf", function(err, doc) {
				if(err) done(err);
				doc.should.be.a('object');
				assert.equal(1, doc.length);
				doc[0].should.be.a('object');
				done();
			});
		});
	});
});
