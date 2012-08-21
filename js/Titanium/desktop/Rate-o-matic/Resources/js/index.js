app.index = function()
{
	var public =
	{
		getValue : function(name)
		{ return private.getValue(name); },
		init : function()
		{ private.init(); },
		inputFileDialog : function()
		{ private.inputFileDialog(); },
		reset : function()
		{ private.reset(); },
		resume : function()
		{ private.resume(); },
		start : function()
		{ private.start(); },
		stop : function()
		{ private.stop(); },
		updateProgressTextValue : function(complete, total)
		{ private.updateProgressTextValue(complete, total); },
		validate : function()
		{ return private.validate(); }
	};

	var private = {};

	private.currentValue = {};
	private.defaultValue = { username : 'uadmincost', password : 'admincost' };

	private.disableButton = function(disabled)
	{
		var disabled = (disabled || true);
		disabled = ((disabled) ? 'disabled' : '');

		Ext.select('#start-button').item(0).dom.disabled = disabled;
		Ext.select('#resume-button').item(0).dom.disabled = disabled;
		Ext.select('#stop-button').item(0).dom.disabled = disabled;
		Ext.select('#reset-button').item(0).dom.disabled = disabled;
	};

	private.disableForm = function(disabled)
	{
		var disabled = ((disabled) ? 'disabled' : '');

		Ext.select('#username-text').item(0).dom.disabled = disabled;
		Ext.select('#password-text').item(0).dom.disabled = disabled;
		Ext.select('#contractId-text').item(0).dom.disabled = disabled;
		Ext.select('#mode-text').item(0).dom.disabled = disabled;
		Ext.select('#useScac-radio-no').item(0).dom.disabled = disabled;
		Ext.select('#useScac-radio-yes').item(0).dom.disabled = disabled;
		Ext.select('#inputFile-button').item(0).dom.disabled = disabled;
	};

	private.getValue = function(name)
	{
		if (private.currentValue[name])
		{ return private.currentValue[name]; }

		var value = '';

		switch (name)
		{
			case 'useScac':
				value = Ext.select('#useScac-radio-yes').item(0).dom.checked;
				break;
			default:
				value = Ext.select('#' + name + '-text').item(0).dom.value.toString().trim();
				break;
		};

		private.currentValue[name] = value;

		return private.currentValue[name];
	};

	private.init = function()
	{
		private.updateNetworkTabValue(app.network.connectivityListener.online());

		app.network.connectivityListener.registerCallback(private.networkConnectivityListenerCallback);

		app.index.reset();

		Ext.select('#primary-tab-top-label').item(0).dom.innerHTML = Titanium.App.getName();
		Ext.select('#primary-tab-bottom-label-left').item(0).dom.innerHTML = '&copy;&nbsp;' + Titanium.App.getCopyright();
		Ext.select('#primary-tab-bottom-label-right').item(0).dom.innerHTML = 'v' + Titanium.App.getVersion();
	};

	private.inputFileDialog = function()
	{ Titanium.UI.openFileChooserDialog(private.inputFileDialogCallback); };

	private.inputFileDialogCallback = function(inputFile)
	{
		var inputFile = (inputFile[0] || '').toString().trim();

		if (!inputFile.length)
		{ return; }

		Ext.select('#inputFile-text').item(0).dom.value = inputFile;
		Ext.select('#inputFile-text').item(0).dom.title = inputFile;

		var outputFile = inputFile.replace(/\.csv/i, '') + '-rates.csv';

		Ext.select('#outputFile-text').item(0).dom.value = outputFile;
		Ext.select('#outputFile-text').item(0).dom.title = outputFile;
	};

	private.networkConnectivityListenerCallback = function(online)
	{
		private.updateNetworkTabValue(online);

		private.disableButton(!online);
	};

	private.reset = function()
	{
		Ext.select('#username-text').item(0).dom.value = private.defaultValue.username;
		Ext.select('#password-text').item(0).dom.value = private.defaultValue.password;
		Ext.select('#contractId-text').item(0).dom.value = '';
		Ext.select('#mode-text').item(0).dom.value = '';
		Ext.select('#useScac-radio-no').item(0).dom.checked = 'checked';
		Ext.select('#inputFile-text').item(0).dom.value = '';
		Ext.select('#inputFile-text').item(0).dom.title = '';
		Ext.select('#outputFile-text').item(0).dom.value = '';
		Ext.select('#outputFile-text').item(0).dom.title = '';
		Ext.select('#progress-text').item(0).dom.value = '';
		Ext.select('#start-button').item(0).dom.style.cssText = 'display: inline;';
		Ext.select('#resume-button').item(0).dom.style.cssText = 'display: none;';
		Ext.select('#stop-button').item(0).dom.style.cssText = 'display: none;';
		Ext.select('#reset-button').item(0).dom.style.cssText = 'display: none;';

		private.currentValue = {};

		private.disableForm(false);
	};

	private.resume = function()
	{
		Ext.select('#start-button').item(0).dom.style.cssText = 'display: none;';
		Ext.select('#resume-button').item(0).dom.style.cssText = 'display: none;';
		Ext.select('#stop-button').item(0).dom.style.cssText = 'display: inline;';
		Ext.select('#reset-button').item(0).dom.style.cssText = 'display: none;';
	};

	private.start = function()
	{
		private.disableForm(true);

		Ext.select('#start-button').item(0).dom.style.cssText = 'display: none;';
		Ext.select('#resume-button').item(0).dom.style.cssText = 'display: none;';
		Ext.select('#stop-button').item(0).dom.style.cssText = 'display: inline;';
		Ext.select('#reset-button').item(0).dom.style.cssText = 'display: none;';
	};

	private.stop = function()
	{
		Ext.select('#start-button').item(0).dom.style.cssText = 'display: none;';
		Ext.select('#resume-button').item(0).dom.style.cssText = 'display: inline;';
		Ext.select('#stop-button').item(0).dom.style.cssText = 'display: none;';
		Ext.select('#reset-button').item(0).dom.style.cssText = 'display: inline;';
	};

	private.updateNetworkTabValue = function(online)
	{
		var color = ((online) ? '00ff00' : 'ff0000');
		var text = ((online) ? 'Online' : 'Offline');

		var innerHTML = '<span style="color: #{0};">{1}</span>';
		innerHTML = String.format(innerHTML, color, text);

		Ext.select('#network-tab-value').item(0).dom.innerHTML = innerHTML;
	};

	private.updateProgressTextValue = function(complete, total)
	{
		var complete = parseInt(complete, 10);
		var total = parseInt(total, 10);

		if (!complete && !total)
		{ return; }

		var percent = parseInt(((complete / total) * 100), 10);

		Ext.select('#progress-text').item(0).dom.value = String.format('{0} / {1} = {2}%', complete, total, percent);
	};

	private.validate = function()
	{
		var array = [];

		if (!private.getValue('username').length)
		{ array.push('Username is required'); }

		if (!private.getValue('password').length)
		{ array.push('Password is required'); }

		if (!private.getValue('inputFile').length)
		{ array.push('Input File is required'); }

		if (array.length)
		{
			alert(array.join('\n'));

			return false;
		}

		return true;
	};

	return public;
}();