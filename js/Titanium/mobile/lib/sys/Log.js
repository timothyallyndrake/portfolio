ADF.sys.Log = function() {
	var local = {
		array : []
	};
	
	return {
		add : function(string) {
			return internal.array.push(string);
		},
		alert : function() {
			return alert(JSON.stringify(internal.array));
		}
	};
};