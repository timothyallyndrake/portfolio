ADF.UI.createPdfView = function(config) {
	var layout = (config.media) ? config.media.layout : 'plan';
	if (config.image.toBlob().width < config.image.toBlob().height) {
		var webView = Ti.UI.createWebView({
			url : config.filePath, 
			width : config.width,
			left : config.left
		});
		return webView;
	}
	else {
		var pdfReader = require('reader.pdf');
		var pdfReaderView = pdfReader.createView(config);
		return pdfReaderView;
	}
};