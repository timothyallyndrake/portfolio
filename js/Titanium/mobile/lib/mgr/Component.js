ADF.mgr.Component = function() {
	var local = {
		elements : new ADF.util.Collation(),
		salt : ADF.util.ID(),
		getKey : function(key) {
			return key + '-' + local.salt;
		}
	};
	
	return {
		add : function(key, element) {
			local.elements.set(local.getKey(key), element);
			return element;
		},
		remove : function(key) {
			local.elements.remove(local.getKey(key));
		},
		get : function(key) {
			return local.elements.get(local.getKey(key));
		},
		elements : function(){
			return local.elements;
		}
	};
};