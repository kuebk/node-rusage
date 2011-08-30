var rusage = require('./rusage.node'), labels = {}, defaultKey = "default";

exports.start = function(){
	var key, label;

	if(arguments[0]){
		key = arguments[0];
	}else{
		key = defaultKey;
	}

	labels[key] = rusage.cycles();
	return labels[key];
};

exports.stop = function(){
	var key;

	if(arguments[0]){
		key = arguments[0];
	}else{
		key = defaultKey;
	}

	return rusage.cycles() - labels[key];
};

exports.usage = rusage.get;
