ADF.Object = function() {
	return {
		iterate: function(object, fn, scope) {
	        for (var key in object) {
	        	if (object.hasOwnProperty(key)) {
		        	fn.call(scope || this, key, object[key], object);
		        }
	        }
	    },
	    apply : function(object, config, defaults) {
	        if (defaults) {
	            ADF.Object.apply(object, defaults);
	        }
	        if (object && config && typeof config === 'object') {
	            var i, j, k;
	            for (i in config) {
	                object[i] = config[i];
	            }
	            for (j = ADF.ENUMERABLES.length; j--;) {
                    k = ADF.ENUMERABLES[j];
                    if (config.hasOwnProperty(k)) {
                        object[k] = config[k];
                    }
                }
	        }
	        return object;
	    },
	    extend : function(prop) {
			var superclass = this.prototype, prototype = new this();

			for (var name in prop) {
				prototype[name] = typeof prop[name] == "function" && 
				typeof superclass[name] == "function" ?
				(function(name, fn){
					return function() {
						var tmp = this.superclass;
						this.superclass = superclass[name];
						var ret = fn.apply(this, arguments);        
						this.superclass = tmp;
						return ret;
					};
				})(name, prop[name]) :
				prop[name];
			}
			function Class() {
				if (this.init) { this.init.apply(this, arguments); }
			};
			Class.prototype = prototype;
			Class.constructor = Class;
			Class.extend = arguments.callee;
			return Class;
		}
	};
}();