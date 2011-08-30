var rusage = require('../bin/rusage.js'), xx = [];

console.log('usage data:', rusage.usage());

rusage.start("www");

for(var i = 0; i < 1000000; i++){
	xx.push(Math.sin(Math.sin(i) * Math.cos(i) * Math.tan(i)));
}

console.log('took cycles:', rusage.stop("www"));
