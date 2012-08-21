app.network.connectivityListener = function()
{
	var public =
	{
		init : function()
		{ private.init(); },
		online : function()
		{ return private.online(); },
		registerCallback : function(callback)
		{ private.registerCallback(callback); }
	};

	var private = {};

	private.callback = [];

	private.listener = function()
	{
		for (var index = 0; index < private.callback.length; index++)
		{
			private.callback[index].call
			(
				private.callback[index],
				private.online()
			);
		}
	};

	private.init = function()
	{ Titanium.Network.addConnectivityListener(private.listener); };

	private.online = function()
	{ return Titanium.Network.online; }

	private.registerCallback = function(callback)
	{ private.callback.push(callback); };

	return public;
}();