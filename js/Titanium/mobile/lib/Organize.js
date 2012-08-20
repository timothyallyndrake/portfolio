ADF.Organize = {
	sortType : {
	    none : function(s){
	        return s;
	    },
	    stripTagsRE : /<\/?[^>]+>/gi,
	    asText : function(s){
	        return String(s).replace(this.stripTagsRE, "");
	    },
	    asUCText : function(s){
	        return String(s).toUpperCase().replace(this.stripTagsRE, "");
	    },
	    asUCString : function(s) {
	        return String(s).toUpperCase();
	    },
	    asDate : function(s) {
	        if(!s){
	            return 0;
	        }
	        if(ADF.isDate(s)){
	            return s.getTime();
	        }
	        return Date.parse(String(s));
	    },
	    asFloat : function(s) {
	        var val = parseFloat(String(s).replace(/,/g, ""));
	        return isNaN(val) ? 0 : val;
	    },
	    asInt : function(s) {
	        var val = parseInt(String(s).replace(/,/g, ""), 10);
	        return isNaN(val) ? 0 : val;
	    }
	},

	objectArraySort : function(objectArray, direction, sortType) {
		var direction = direction || 'ASC',
			st = sortType ? ADF.Organize.sortType[sortType] : ADF.Organize.sortType['none'],
        	fn = function(r1, r2){
            	var v1 = st(r1.value), v2 = st(r2.value);
	            if (direction == "ASC")
	            	return v1 > v2 ? 1 : (v1 < v2 ? -1 : 0);
	            else
	            	return v1 > v2 ? -1 : (v1 < v2 ? 1 : 0);
        	};

        return objectArray.sort(fn);
	}
};