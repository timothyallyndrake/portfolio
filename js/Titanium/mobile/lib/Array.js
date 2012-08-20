ADF.Array = function() {
	return {
		each: function(array, fn, scope, reverse) {
		    var i = 0,
		        ln = array.length;
		
		    if (reverse !== true) {
		        while (i < ln) {
		            if (fn.call(scope || array[i], array[i], i, array) === false) {
		                return i;
		            }
		            i++;
		        }
		    }
		    else {
		    	i = array.length;
		        while (i > -1) {
		            if (fn.call(scope || array[i], array[i], i, array) === false) {
		                return i;
		            }
		            i--;
		        }
		    }
		    return true;
		},
		is : function(test) {
			return toString.call(test) === '[object Array]';
		},
	    remove: function(array, index) {
	        array.splice(index, 1);
	        return array;
	    }
	};
}();