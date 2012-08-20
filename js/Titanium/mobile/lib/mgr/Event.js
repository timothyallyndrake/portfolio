ADF.mgr.Event = function(event) {
	var local = {
		events : {}
	};
	
	return {
		add : function(event) {
			var eventArray = (ADF.Array.is(event)) ? event : [event];
			ADF.Array.each(eventArray, function(itm) {
				local.events[itm] = [];
			});
			return this;
		},
		on : function(event, handler) { // handler = { callback : function, options : object }
			if (!local.events[event]) {
				return false;
			}
			if (!this.has(event, handler)) {
				local.events[event].push(handler);
			}
			return true;
		},
		un : function(event, handler) {
			if (!local.events[event]) {
				return false;
			}
			if (this.has(event, handler)) {
				ADF.Array.remove(local.events[event], local.events[event].indexOf(handler));
				return true;
			}
			else { return false; }
		},
		has : function(event, handler) {
			return (local.events[event].indexOf(handler) != -1) ? true : false;
		},
		fire : function(event, data, scope) {
			if (!local.events[event]) {
				return false;
			}
			var eventArray = local.events[event];

			ADF.Array.each(eventArray, function(itm) {
				itm.callback.call(scope || this, data, itm.options);
			});
			return true;
		},
		events : function() {
			return local.events;
		}
	};
};