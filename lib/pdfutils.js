var Stream = require('stream').Stream;
var util = require('util');
var fs = require('fs');

var bindings
try {
	bindings = require('../build/Debug/pdfutils');
} catch (e) { try {
	bindings = require('../build/Release/pdfutils');
} catch (e) {
	throw e;
}}

Object.defineProperty(bindings.PDFDocument.prototype, "length", {
	enumerable: true,
	configurable: false,
	get: function() {
		var max = -1;
		var n;
		for(var k in this) {
			if(!isNaN(k) && (n = parseInt(k)) > max)
				max = n;
		}
		return n + 1;
	},
	set: function() {
		throw new Error("Cannot set length of PDFDocument");
	}
});

module.exports = function pdfutils(source, cb) {
	return new bindings.PDFDocument(source, cb);
}
module.exports.PDFDocument = bindings.PDFDocument
module.exports.pdfutils = module.exports;
