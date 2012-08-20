var ADF = (function() {
	return {
		ID_SEED : 0,
		ENUMERABLES : ['hasOwnProperty', 'valueOf', 'isPrototypeOf', 'propertyIsEnumerable', 'toLocaleString', 'toString', 'constructor'],
		mgr : {},
		net : {},
		sys : {},
		UI : {},
		util : {}
	};
})();

Ti.include('Array.js');
Ti.include('Date.js');
Ti.include('mgr/Component.js');
Ti.include('mgr/Event.js');
Ti.include('mgr/Layout.js');
Ti.include('net/HTTP.js');
Ti.include('Object.js');
Ti.include('Organize.js');
Ti.include('String.js');
Ti.include('sys/Log.js');
Ti.include('UI/PDFViewer.js');
Ti.include('UI/NoteView.js');
Ti.include('util/Collation.js');
Ti.include('util/Format.js');
Ti.include('util/ID.js');

ADF.CmpMgr = new ADF.mgr.Component();
ADF.EventMgr = new ADF.mgr.Event();
ADF.LayoutMgr = new ADF.mgr.Layout();
ADF.ID = ADF.util.ID;
ADF.each = ADF.Array.each;
ADF.iterate = ADF.Object.iterate;
ADF.apply = ADF.Object.apply;
ADF.extend = ADF.Object.extend;
ADF.isDate = ADF.Date.isDate;

ADF.getVersion = function() {
	var local = {
		major : 0,
		minor : 1
	};
	return local.major + '.' + local.minor;
};

ADF.applicationDataPath = function() {
	var pathArr = [Ti.Filesystem.applicationDataDirectory];
	ADF.each(arguments, function (item){
		pathArr.push(item);
	});
	return pathArr.join(Ti.Filesystem.separator);
};