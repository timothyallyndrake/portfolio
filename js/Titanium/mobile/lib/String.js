ADF.String = function() {
	return {
		left : function(str, n) {
			if (n <= 0) { return ''; }
			else if (n > String(str).length) { return str; }
			else { return String(str).substring(0,n); }
	    },
	    right : function(str, n) {
		    if (n <= 0) { return ''; }
		    else if (n > String(str).length) { return str; }
		    else {
		       var iLen = String(str).length;
		       return String(str).substring(iLen, iLen - n);
		    }
	    }
	};
}();