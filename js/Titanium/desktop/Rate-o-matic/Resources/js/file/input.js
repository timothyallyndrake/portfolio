app.file.input = function()
{
	var public =
	{
		close : function()
		{ private.close(); },
		open : function()
		{ private.open(); },
		readHeader : function()
		{ return private.readHeader(); },
		readLine : function()
		{ return private.readLine(); }
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

	private.open = function()
	{
		if (!private.fileStream)
		{ private.fileStream = Titanium.Filesystem.getFileStream(app.index.getValue('inputFile')); }

		if (!private.fileStream.isOpen())
		{ private.fileStream.open(Titanium.Filesystem.MODE_READ); }
	};

	private.readHeader = function()
	{
		var lineString = private.fileStream.readLine();

		if (!lineString)
		{ return lineString; }

		lineString = lineString.toString().trim();

		if (!lineString.length)
		{ return lineString; }

		var lineArray = lineString.split(',');

		return lineArray;
	};

	private.readLine = function()
	{
		var lineString = private.fileStream.readLine();

		if (!lineString)
		{ return lineString; }

		lineString = lineString.toString().trim();

		if (!lineString.length)
		{ return private.readLine(); }

		var lineArray = lineString.split(',');

		var lineObject =
		{
			referenceNumber : lineArray[0].toString().trim(),
			originCity : lineArray[1].toString().trim(),
			originState : lineArray[2].toString().trim(),
			originZip : lineArray[3].toString().trim(),
			originCountry : lineArray[4].toString().trim(),
			destinationCity : lineArray[5].toString().trim(),
			destinationState : lineArray[6].toString().trim(),
			destinationZip : lineArray[7].toString().trim(),
			destinationCountry : lineArray[8].toString().trim(),
			pickUpDate : lineArray[9].toString().trim(),
			freightClass : lineArray[10].toString().trim(),
			weight : lineArray[11].toString().trim(),
			hazardous : ((lineArray[12].toString().trim().toLowerCase() == 'true') ? true : false),
			scac : lineArray[13].toString().trim()
		};

		return lineObject;
	};

	return public;
}();