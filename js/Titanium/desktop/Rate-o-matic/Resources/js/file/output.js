app.file.output = function()
{
	var public =
	{
		close : function()
		{ private.close(); },
		create : function()
		{ private.create(); },
		open : function()
		{ private.open(); },
		writeHeader : function()
		{ private.writeHeader(); },
		writeLine : function(lineObject)
		{ private.writeLine(lineObject); }
	};

	var private = {};

	private.fileStream = null;

	private.close = function()
	{
		if (!private.fileStream)
		{ return; }

		if (private.fileStream.isOpen())
		{ private.fileStream.close(); }

		private.fileStream = null;
	};

	private.create = function()
	{
		var file = Titanium.Filesystem.getFile(app.index.getValue('outputFile'));

		if (file.exists())
		{ file.deleteFile(); }

		if (!file.exists())
		{ file.touch(); }
	};

	private.open = function()
	{
		if (!private.fileStream)
		{ private.fileStream = Titanium.Filesystem.getFileStream(app.index.getValue('outputFile')); }

		if (!private.fileStream.isOpen())
		{ private.fileStream.open(Titanium.Filesystem.MODE_APPEND); }
	};

	private.writeHeader = function()
	{
		var lineArray =
		[
			'Reference Number',
			'Origin City',
			'Origin State',
			'Origin Zip',
			'Origin Country',
			'Destination City',
			'Destination State',
			'Destination Zip',
			'Destination Country',
			'Pickup Date',
			'Freight Class',
			'Weight',
			'Hazardous',
			'Scac',
			'Contract ID',
			'Contract Name',
			'Carrier',
			'Transit Time',
			'Carrier Charge',
			'Distance',
			'Error'
		];

		private.fileStream.writeLine(lineArray.join(','));
	};

	private.writeLine = function(lineObject)
	{
		var lineArray = [];

		lineArray.push(lineObject.referenceNumber);
		lineArray.push(lineObject.originCity);
		lineArray.push(lineObject.originState);
		lineArray.push(lineObject.originZip);
		lineArray.push(lineObject.originCountry);
		lineArray.push(lineObject.destinationCity);
		lineArray.push(lineObject.destinationState);
		lineArray.push(lineObject.destinationZip);
		lineArray.push(lineObject.destinationCountry);
		lineArray.push(lineObject.pickUpDate);
		lineArray.push(lineObject.freightClass);
		lineArray.push(lineObject.weight);
		lineArray.push(((lineObject.hazardous) ? 'TRUE' : 'FALSE'));
		lineArray.push(lineObject.scac);
		lineArray.push(lineObject.contractId);
		lineArray.push(lineObject.contractName);
		lineArray.push(lineObject.carrier);
		lineArray.push(lineObject.transitTime);
		lineArray.push(lineObject.carrierCharge);
		lineArray.push(lineObject.distance);
		lineArray.push(lineObject.error);

		private.fileStream.writeLine(lineArray.join(','));
	};

	return public;
}();