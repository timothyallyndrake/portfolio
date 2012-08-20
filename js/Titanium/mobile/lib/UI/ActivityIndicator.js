ADF.UI.ActivityIndicator = function() {
	var local = {
		object : null,
		hide : function() {
			if (local.object) {
				local.object.hide();
				delete local.object.message;
			}
			return;
		},
		show : function(message) {
			if (!internal.object) { 
				this.init(); 
			}
			local.object.message = message;
			local.object.show();
		}
	};
	
	return {
		init : function() {
			local.object = Ti.UI.createActivityIndicator();
			return this;
		}
	};
}();