ADF.net.HTTP = function() {
	var local = {};
	
	return {
		persistent : {},
		request : function(object) { 
			if (!Ti.Network.online) { 
				return alert('No connectivity'); 
			}
			var httpClient = Ti.Network.createHTTPClient();
	
			if (object.onError) { 
				httpClient.onerror = object.onError; 
			}
			if (object.onLoad) { 
				httpClient.onload = object.onLoad; 
			}
			if (object.timeout) { 
				httpClient.setTimeout(object.timeout); 
			}
			httpClient.open('POST', com.td.ita.configuration.url.request);
			ADF.iterate(this.persistent, function(property, value) {
				if (object.data[property]) { continue; }	
				object.data[property] = value;
			});
			httpClient.send(object.data);
			return;
		}
	};
};