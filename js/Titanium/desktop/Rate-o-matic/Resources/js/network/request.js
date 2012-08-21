app.network.request = function()
{
	var public =
	{
		init : function()
		{ return private.init(); },
		send : function(callback)
		{ return private.send(callback); }
	};

	var private = {};

	private.url = 'http://story-2.mercurygate.net/MercuryGate/inquiry/remoteXMLRating.jsp';

	private.init = function()
	{
		delete private.httpClient;

		private.httpClient = Titanium.Network.createHTTPClient();
		private.httpClient.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
		private.httpClient.setTimeout(-1);
		private.httpClient.onreadystatechange = private.onReadyStateChange;
	};

	private.onReadyStateChange = function()
	{
		if (private.httpClient.readyState != 4)
		{ return; }

		var response =
		{
			data : private.httpClient.responseData,
			status :
			{
				code : parseInt(private.httpClient.status, 10),
				text : private.httpClient.statusText
			},
			text : private.httpClient.responseText,
			xml : private.httpClient.responseXML
		};

		private.receive.call(private.receive, response);
	};

	private.prepare = function(object)
	{
		var array = [];

		for (var property in object)
		{
			array.push
			(
				String.format
				(
					'{0}={1}',
					Titanium.Network.encodeURIComponent(property),
					Titanium.Network.encodeURIComponent(object[property])
				)
			);
		}

		return array.join('&');
	};

	private.receive = Ext.emptyFn;

	private.send = function(callback)
	{
		private.receive = callback;

		private.httpClient.open('POST', private.url);

		private.httpClient.send
		(
			private.prepare
			(
				{
					password : app.index.getValue('password'),
					request : app.xml.request.toString(),
					userid : app.index.getValue('username')
				}
			)
		);
	};

	return public;
}();