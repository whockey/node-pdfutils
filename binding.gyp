{
	'targets': [{
		'target_name': 'pdfutils',
		"sources": [
			"src/init.cc",
			"src/JsPDFDocument.cc",
			"src/JsPDFPage.cc",
			"src/JobWorker.cc",
			"src/PDFLoader.cc",
			"src/NodePDFDocBuilder.cc"
		],
		'default_configuration': 'Debug',
		'xcode_settings': {
			'OTHER_CFLAGS': [
				'<!@(pkg-config --cflags poppler-cairo)'
			],
		},
		'cflags': [
			'<!@(pkg-config --cflags poppler-cairo)'
		],
		'ldflags': [
			'<!@(pkg-config --libs poppler-cairo)'
		],
		'libraries': [
			'<!@(pkg-config --libs poppler-cairo)'
		]
	}]
}
