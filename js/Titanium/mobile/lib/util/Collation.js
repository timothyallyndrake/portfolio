ADF.util.Collation = function() {
	var local = {
		map : {},
		array : []
	};

	return {
		remove : function(key) {
			var value = local.map[key];
			this.removeAt(this.indexOf(value));
			delete value;
		},
		removeAt : function(index) {
			var key = local.array[index];
			local.array.splice(index, 1);
			delete local.map[key];
		},
		get : function(key) {
			return local.map[key];
		},
		getAt : function(index) {
			return local.array[index];
		},
		set : function(key, value) {
			local.map[key] = value;
			if (local.array.indexOf(key) == -1) {
				local.array.push(key);
			}
		},
		setAt : function(index, value) {
			this.set(local.array[index], value);
		},
		add : function(value) {
			var id = ADF.util.ID();
			local.map[id] = value;
			local.array.push(id);
		},
		indexOf : function(value, from) {
			return local.array.indexOf(this.find(value), from);
		},
		findKey : function(value) {
			var foundValue;

			ADF.Object.iterate(local.map, function(key, keyValue) {
				if (value == key) {
					foundValue = keyValue;
					return false;	
				}
			});
			return foundValue;
		},
		queryKey : function(value) {
			var pattern = new RegExp(value, 'ig'),
				results = [];

			ADF.Object.iterate(local.map, function(key, keyValue) {
				if (key.search(pattern) != -1) {
					results.push(key);
				}
			});
			return results;
		},
		organize : function(property, fn, scope) {
			var self = this,
				fn = fn || function(key, object) {
					return {
						key : key,
						value : object[property]
					};
				},
				array = [];
			

			ADF.Object.iterate(local.map, function(key, keyValue) {
				array.push(fn.call(scope || self, key, keyValue));
			});
			return array;
		},
		getCount : function() {
			return local.array.length;
		},
		clear : function() {
			local.map = {};
			local.array = [];
		},
		each : function(fn, scope, reverse) {
			ADF.Array.each(local.array, fn, scope, reverse);
		},
		iterate : function(fn, scope) {
			ADF.Object.iterate(local.map, fn, scope);
		},
		// apply : function(config) {
			// ADF.Object.apply(local.map, config);
			// TODO : needs to append to the array, we may not want to even use this method
			// return this;
		// },
		data : function() {
			return { map : local.map, array : local.array };
		}
	};
};