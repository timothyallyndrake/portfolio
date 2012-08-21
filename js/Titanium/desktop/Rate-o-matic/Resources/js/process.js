app.process = function()
{
	var public =
	{
		init : function()
		{ private.init(); },
		reset : function()
		{ private.reset(); },
		resume : function()
		{ private.resume(); },
		start : function()
		{ private.start(); },
		stop : function()
		{ private.stop(); }
	};

	var private = {};

	private.signal = { stop : 0 };
	private.stack = { array : [], count : { complete : 0, total : 0 } };

	private.init = function()
	{ app.network.connectivityListener.registerCallback(private.networkConnectivityListenerCallback); };

	private.networkConnectivityListenerCallback = function(online)
	{
		if (!online)
		{ private.stop(); }
	};

	private.request = {};

	private.request.send = function()
	{
		if (private.returnCheck())
		{ return; }

		app.xml.request.reset();

		var lineObject = private.stack.array[0];

		for (var index = 0; index < lineObject.length; index++)
		{ app.xml.request.add(lineObject[index]); }

		app.network.request.init();
		app.network.request.send(private.request.receive);
	};

	private.request.receive = function(response)
	{
		if (private.returnCheck())
		{ return; }

		var data = app.xml.response.process(response);

		app.file.output.writeLine(Ext.apply(private.stack.array[0][0], data));

		private.stack.count.complete++;
		app.index.updateProgressTextValue(private.stack.count.complete, private.stack.count.total);

		private.stack.array.shift();

		private.request.send();
	};

	private.returnCheck = function()
	{
		var flag = false;

		if (private.signal.stop || !private.stack.array.length)
		{
			flag = true;

			app.process.stop();

			if (!private.stack.array.length)
			{
				if (alert('Process completed successfully!'))
				{ app.process.reset(); }
			}
		}

		return flag;
	};

	private.stack.push = function()
	{
		app.file.input.open();

		var lineArray = app.file.input.readHeader();
		var lineObject = app.file.input.readLine();
		var pointer = [];

		while (lineObject)
		{
			var index = pointer.indexOf(lineObject.referenceNumber);

			if (index == -1)
			{
				private.stack.array.push([]);
				pointer.push(lineObject.referenceNumber);

				index = (private.stack.array.length - 1);
			}

			private.stack.array[index].push(lineObject);
			private.stack.count.total++;

			lineObject = app.file.input.readLine();
		}

		app.file.input.close();
	};

	private.reset = function()
	{
		private.signal.stop = 0;
		private.stack.array = [];
		private.stack.count.complete = 0;
		private.stack.count.total = 0;

		app.index.reset();
	};

	private.resume = function()
	{
		private.signal.stop = 0;

		app.index.resume();	
		app.file.output.open();
		private.request.send();
	};

	private.start = function()
	{
		if (!app.index.validate())
		{ return; }

		private.signal.stop = 0;

		app.index.start();

		private.stack.push();

		app.file.output.create();
		app.file.output.open();
		app.file.output.writeHeader();

		app.index.updateProgressTextValue(private.stack.count.complete, private.stack.count.total);

		private.request.send();
	};

	private.stop = function()
	{
		private.signal.stop = 1;

		app.index.stop();

		app.file.output.close();
	};

	return public;
}();